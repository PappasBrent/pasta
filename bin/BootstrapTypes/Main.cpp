/*
 * Copyright (c) 2021 Trail of Bits, Inc.
 */

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <llvm/ADT/StringRef.h>

#include "BootstrapConfig.h"

namespace {

static const std::string kDeclNames[] = {
#define PASTA_BEGIN_CLANG_WRAPPER(name, id) #name ,
#include "Generated/Decl.h"
};

static const struct {std::string derived; std::string base; } kDeclExtends[] = {
#define PASTA_PUBLIC_BASE_CLASS(name, id, base_name, base_id) \
    {#name, #base_name},
#include "Generated/Decl.h"
};

static const std::unordered_map<std::string, std::string> kCxxMethodRenames{
    {"SourceRange", "TokenRange"},
    {"Vbases", "VBases"},
    {"vbases", "VirtualBases"},
    {"NumVbases", "NumVBases"},
    {"Ctors", "Constructors"},
    {"ctors", "Constructors"},
    {"Location", "Token"},
};

std::unordered_map<std::string, std::set<std::string>> gBaseClasses;
std::unordered_map<std::string, std::set<std::string>> gDerivedClasses;
std::vector<std::string> gTopologicallyOrderedDecls;


std::unordered_map<std::string, std::set<std::string>> gTransitiveBaseClasses;
std::unordered_map<std::string, std::set<std::string>> gTransitiveDerivedClasses;

static std::string CxxName(llvm::StringRef name) {
  if (name == "getKind" || name == "getDeclKindName") {
    return "";  // We have our own `DeclKind`.

  } else if (name.startswith("get")) {
    return CxxName(name.substr(3));

  // Begin/end iterators.
  } else if (name.endswith("_begin") || name.endswith("_end") ||
             name.endswith("_size") || name.endswith("_empty") ||
             name.endswith("_rbegin") || name.endswith("_rend")) {
    return "";

  // Setters, ignore them.
  } else if (name.startswith("set")) {
    return "";

  } else if (name.endswith("Loc")) {
    return name.substr(0, name.size() - 3).str() + "Token";

  } else if (auto name_it = kCxxMethodRenames.find(name.str());
             name_it != kCxxMethodRenames.end()) {
    return name_it->second;

  } else if (name.empty()) {
    return "";

  } else if (std::islower(name.front())) {
    return name.substr(0, 1).upper() + name.substr(1).str();

  } else {
    return name.str();
  }
}

static void DeclareCppMethods(std::ostream &os, const std::string &class_name) {
#define PASTA_CLASS_METHOD_0(cls, id, meth, rt) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        if (!strcmp(#rt, "(bool)")) { \
          os << "  bool " << meth_name << "(void) const;\n"; \
        } else if (!strcmp(#rt, "(clang::SourceLocation)")) { \
          os << "  std::optional<::pasta::Token> " << meth_name << "(void) const;\n"; \
        } else { \
          os << "  // " << meth_name << "\n"; \
        } \
      } \
    }

#define PASTA_CLASS_METHOD_1(cls, id, meth, rt, p0) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        os << "  // " << meth_name << "\n"; \
      } \
    }

#define PASTA_CLASS_METHOD_2(cls, id, meth, rt, p0, p1) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        os << "  // " << meth_name << "\n"; \
      } \
    }

#define PASTA_CLASS_METHOD_3(cls, id, meth, rt, p0, p1, p2) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        os << "  // " << meth_name << "\n"; \
      } \
    }

#define PASTA_CLASS_METHOD_4(cls, id, meth, rt, p0, p1, p2, p3) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        os << "  // " << meth_name << "\n"; \
      } \
    }

#define PASTA_CLASS_METHOD_5(cls, id, meth, rt, p0, p1, p2, p3, p4) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        os << "  // " << meth_name << "\n"; \
      } \
    }

#define PASTA_CLASS_METHOD_6(cls, id, meth, rt, p0, p1, p2, p3, p4, p5) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        os << "  // " << meth_name << "\n"; \
      } \
    }
#include "Generated/Decl.h"
}

static void DeclareCppClasses(void) {
  std::ofstream os(kASTDeclHeader);

  os
      << "/*\n"
      << " * Copyright (c) 2021 Trail of Bits, Inc.\n"
      << " */\n\n"
      << "// This file is auto-generated.\n\n"
      << "#include <memory>\n"
      << "#include <optional>\n\n"
      << "#include \"Token.h\"\n\n"
      << "namespace clang {\n";

  for (const auto &name : kDeclNames) {
    os << "class " << name << ";\n";
  }

  os
      << "}  // namespace clang\n"
      << "namespace pasta {\n"
      << "class AST;\n"
      << "class ASTImpl;\n\n"
      << "enum class DeclKind : unsigned {\n";

  for (const auto &name : kDeclNames) {
    if (name != "DeclContext" && name != "Decl") {
      os << "  k" << name << ",\n";
    }
  }

  os
      << "};\n\n";


  // Forward declare them all.
  for (const auto &name : kDeclNames) {
    os << "class " << name << ";\n";
  }

  // Define them all.
  for (const auto &name : gTopologicallyOrderedDecls) {
    if (name == "DeclContext") {
      continue;
    }

    os
        << "class " << name;

    auto sep = " : ";
    for (const auto &parent_class : gBaseClasses[name]) {
      if (parent_class != "DeclContext") {
        os << sep << "public " << parent_class;
        sep = ", ";
      }
    }

    os
        << " {\n"
        << " public:\n"
        << "  ~" << name << "(void) = default;\n"
        << "  " << name << "(const " << name << " &) = default;\n"
        << "  " << name << "(" << name << " &&) noexcept = default;\n"
        << "  " << name << " &operator=(const " << name << " &) = default;\n"
        << "  " << name << " &operator=(" << name << " &&) noexcept = default;\n\n";

//    // Permits down-casting.
//    for (const auto &base_name : gTransitiveBaseClasses[name]) {
//      if (base_name != "DeclContext") {
//        os << "  static std::optional<" << name << "> From(const "
//           << base_name << " &);\n";
//      }
//    }

    DeclareCppMethods(os, name);

    // The top level `Decl` class has all the content.
    if (name == "Decl") {
      os
          << "  inline DeclKind Kind(void) const {\n"
          << "    return kind;\n"
          << "  }\n\n"
          << "  const char *KindName(void) const;\n\n"
          << " protected:\n"
          << "  std::shared_ptr<ASTImpl> ast;\n"
          << "  union {\n";

      for (const auto &name : kDeclNames) {
        if (name != "DeclContext") {
          os << "    const ::clang::" << name << " *" << name << ";\n";
        }
      }

      os
          << "  } u;\n"
          << "  DeclKind kind;\n\n"
          << "  inline explicit Decl(std::shared_ptr<ASTImpl> ast_,\n"
          << "                       const ::clang::Decl *decl_,\n"
          << "                       DeclKind kind_)\n"
          << "      : ast(std::move(ast_)),\n"
          << "        kind(kind_) {\n"
          << "    u.Decl = decl_;\n"
          << "  }\n\n";
    }

    os
        << " private:\n"
        << "  " << name << "(void) = delete;\n\n"
        << "  friend class AST;\n"
        << "  friend class ASTImpl;\n\n"
        << " protected:\n"
        << "  explicit " << name << "(\n"
        << "      std::shared_ptr<ASTImpl> ast_,\n"
        << "      const ::clang::" << name << " *decl_);\n"
        << "};\n\n";

    // Requiring that all derivations have the same size as the base class
    // will let us do fun sketchy things.
    if (name != "Decl") {
      os << "static_assert(sizeof(Decl) == sizeof(" << name << "));\n\n";
    }
  }

  os
      << "}  // namespace pasta\n";
}

static void DefineCppMethods(std::ostream &os, const std::string &class_name) {
#define PASTA_CLASS_METHOD_0(cls, id, meth, rt) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        if (!strcmp(#rt, "(bool)")) { \
          os << "bool " << class_name << "::" << meth_name << "(void) const {\n" \
             << "  return u." << class_name << "->" << #meth << "();\n" \
             << "}\n\n"; \
        } else if (!strcmp(#rt, "(clang::SourceLocation)")) { \
          os << "std::optional<::pasta::Token> " << class_name << "::" << meth_name \
             << "(void) const {\n" \
             << "  return ast->TokenAt(u." << class_name << "->" << #meth << "());\n" \
             << "}\n\n"; \
        } else { \
          os << "  // " << meth_name << "\n"; \
        } \
      } \
    }

#define PASTA_CLASS_METHOD_1(cls, id, meth, rt, p0) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        os << "  // " << meth_name << "\n"; \
      } \
    }

#define PASTA_CLASS_METHOD_2(cls, id, meth, rt, p0, p1) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        os << "  // " << meth_name << "\n"; \
      } \
    }

#define PASTA_CLASS_METHOD_3(cls, id, meth, rt, p0, p1, p2) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        os << "  // " << meth_name << "\n"; \
      } \
    }

#define PASTA_CLASS_METHOD_4(cls, id, meth, rt, p0, p1, p2, p3) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        os << "  // " << meth_name << "\n"; \
      } \
    }

#define PASTA_CLASS_METHOD_5(cls, id, meth, rt, p0, p1, p2, p3, p4) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        os << "  // " << meth_name << "\n"; \
      } \
    }

#define PASTA_CLASS_METHOD_6(cls, id, meth, rt, p0, p1, p2, p3, p4, p5) \
    if (class_name == #cls) { \
      if (const auto meth_name = CxxName(#meth); !meth_name.empty()) { \
        os << "  // " << meth_name << "\n"; \
      } \
    }
#include "Generated/Decl.h"
}

static void DefineCppClasses(void) {
  std::ofstream os(kASTDeclCpp);

  os
      << "/*\n"
      << " * Copyright (c) 2021 Trail of Bits, Inc.\n"
      << " */\n\n"
      << "// This file is auto-generated.\n\n"
      << "#include <pasta/AST/Decl.h>\n\n"
      << "#pragma clang diagnostic push\n"
      << "#pragma clang diagnostic ignored \"-Wimplicit-int-conversion\"\n"
      << "#pragma clang diagnostic ignored \"-Wsign-conversion\"\n"
      << "#pragma clang diagnostic ignored \"-Wshorten-64-to-32\"\n"
      << "#include <clang/AST/Decl.h>\n"
      << "#include <clang/AST/DeclCXX.h>\n"
      << "#include <clang/AST/DeclFriend.h>\n"
      << "#include <clang/AST/DeclObjC.h>\n"
      << "#include <clang/AST/DeclOpenMP.h>\n"
      << "#include <clang/AST/DeclTemplate.h>\n"
      << "#pragma clang diagnostic pop\n\n"
      << "#include \"AST.h\"\n\n"
      << "namespace pasta {\n"
      << "namespace {\n"
      << "// Return the PASTA `DeclKind` for a Clang `Decl`.\n"
      << "static DeclKind KindOfDecl(const clang::Decl *decl) {\n"
      << "  switch (decl->getKind()) {\n"
      << "#define ABSTRACT_DECL(DECL)\n"
      << "#define DECL(DERIVED, BASE) \\\n"
      << "    case clang::Decl::DERIVED: \\\n"
      << "      return DeclKind::k ## DERIVED ## Decl;\n"
      << "#include <clang/AST/DeclNodes.inc>\n"
      << "  }\n"
      << "  __builtin_unreachable();\n"
      << "}\n\n"
      << "}  // namespace\n\n"
      << "const char *Decl::KindName(void) const {\n"
      << "  switch (kind) {\n";

  for (const auto &name : kDeclNames) {
    if (name != "Decl" && name != "DeclContext") {
      os << "    case DeclKind::k" << name << ": return \"" << name << "\";\n";
    }
  }

  os
      << "  }\n"
      << "}\n\n";


  // Define them all.
  for (const auto &name : gTopologicallyOrderedDecls) {
    if (name == "DeclContext") {
      continue;
    }

    os
        << name << "::" << name << "(\n"
        << "    std::shared_ptr<ASTImpl> ast_,\n"
        << "    const ::clang::" << name << " *decl_)";

    // Dispatch to our hand-written constructor that takes the `DeclKind`.
    if (name == "Decl") {
      os << "\n    : Decl(std::move(ast_), decl_, KindOfDecl(decl_)) {}\n";

    // Dispatch to the base class constructor(s).
    } else {
      auto sep = "\n    : ";
      const auto &base_classes = gBaseClasses[name];
      auto prefix = base_classes.size() == 1u ? "std::move(" : "";
      auto suffix = base_classes.size() == 1u ? ")" : "";
      for (const auto &parent_class : base_classes) {
        if (parent_class != "DeclContext") {
          os << sep << parent_class << "(" << prefix << "ast_" << suffix
             << ", decl_)";
          sep = ",\n      ";
        }
      }
      os << " {}\n\n";
    }

    DefineCppMethods(os, name);
  }

  os
      << "}  // namespace pasta\n";
}

}  // namespace

int main(void) {

  std::unordered_set<std::string> seen;
  //gBaseClasses["Decl"].insert("DeclBase");
  //gBaseClasses["DeclContext"].insert("DeclBase");

  // Build up an adjacency list of parent/child relations.
  for (const auto &[name, base_name] : kDeclExtends) {
    gBaseClasses[name].insert(base_name);
    gDerivedClasses[base_name].insert(name);
  }

  // Topologically order the classes by the parent/child relations.
  for (auto changed = true; changed; ) {
    changed = false;
    for (const auto &name : kDeclNames) {
      if (seen.count(name)) {
        goto skip;
      }

      for (const auto &parent_name : gBaseClasses[name]) {
        if (!seen.count(parent_name)) {
          goto skip;
        }
      }

      gTopologicallyOrderedDecls.push_back(name);
      seen.insert(name);
      changed = true;

    skip:
      continue;
    }
  }

  // Go find the transitive base classes. Rely on the topological order
  // so that we can do it in a single pass.
  for (const auto &base_name : gTopologicallyOrderedDecls) {
    const auto &base_classes = gTransitiveBaseClasses[base_name];

    for (auto &derived_name : gDerivedClasses[base_name]) {
      auto &derived_base_classes = gTransitiveBaseClasses[derived_name];
      derived_base_classes.insert(base_name);
      derived_base_classes.insert(base_classes.begin(), base_classes.end());
    }
  }

  // Go find the transitive derived classes. Rely on the reverse topological
  // order so that we can do it in a single pass.
  for (auto it = gTopologicallyOrderedDecls.rbegin();
       it != gTopologicallyOrderedDecls.rend(); ++it) {
    const auto &derived_name = *it;
    const auto &derived_classes = gTransitiveDerivedClasses[derived_name];

    for (auto &base_name : gBaseClasses[derived_name]) {
      auto &base_derived_classes = gTransitiveDerivedClasses[base_name];
      base_derived_classes.insert(derived_name);
      base_derived_classes.insert(derived_classes.begin(),
                                  derived_classes.end());
    }
  }

  DeclareCppClasses();
  DefineCppClasses();

  return EXIT_SUCCESS;
}

