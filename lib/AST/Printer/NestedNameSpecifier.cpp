//===- NestedNameSpecifier.cpp - C++ nested name specifiers ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file defines the NestedNameSpecifier class, which represents
//  a C++ nested-name-specifier.
//
//===----------------------------------------------------------------------===//

#include <clang/AST/NestedNameSpecifier.h>

#include "DeclStmtPrinter.h"

namespace pasta {

class PrintedTokenRangeImpl;

/// Print this nested name specifier to the given output
/// stream.
void NestedNameSpecifier_print(
    clang::NestedNameSpecifier *Spec, Printer &printer,
    const clang::PrintingPolicy &Policy,
    bool ResolveTemplateArguments) {

  if (Spec->getPrefix())
    NestedNameSpecifier_print(Spec->getPrefix(), printer, Policy);

  auto &OS = printer.OS;
  auto &tokens = printer.tokens;

  switch (Spec->getKind()) {
    case clang::NestedNameSpecifier::Identifier:
      OS << Spec->getAsIdentifier()->getName();
      break;

    case clang::NestedNameSpecifier::Namespace:
      if (Spec->getAsNamespace()->isAnonymousNamespace())
        return;

      OS << Spec->getAsNamespace()->getName();
      break;

    case clang::NestedNameSpecifier::NamespaceAlias:
      OS << Spec->getAsNamespaceAlias()->getName();
      break;

    case clang::NestedNameSpecifier::Global:
      break;

    case clang::NestedNameSpecifier::Super:
      OS << "__super";
      break;

    case clang::NestedNameSpecifier::TypeSpecWithTemplate:
      OS << "template ";
      // Fall through to print the type.
      [[fallthrough]];

    case clang::NestedNameSpecifier::TypeSpec: {
      const auto *Record =
          clang::dyn_cast_or_null<clang::ClassTemplateSpecializationDecl>(
              Spec->getAsRecordDecl());
      if (ResolveTemplateArguments && Record) {
          // Print the type trait with resolved template parameters.
          Record->printName(OS, Policy);
          printTemplateArgumentList(
              printer, Record->getTemplateArgs().asArray(), Policy,
              Record->getSpecializedTemplate()->getTemplateParameters());
          break;
      }

      const clang::Type *T = Spec->getAsType();

      clang::PrintingPolicy InnerPolicy(Policy);
      InnerPolicy.SuppressScope = true;

      // Nested-name-specifiers are intended to contain minimally-qualified
      // types. An actual ElaboratedType will not occur, since we'll store
      // just the type that is referred to in the nested-name-specifier (e.g.,
      // a TypedefType, TagType, etc.). However, when we are dealing with
      // dependent template-id types (e.g., Outer<T>::template Inner<U>),
      // the type requires its own nested-name-specifier for uniqueness, so we
      // suppress that nested-name-specifier during printing.
      assert(!clang::isa<clang::ElaboratedType>(T) &&
             "Elaborated type in nested-name-specifier");
      if (const clang::TemplateSpecializationType *SpecType
            = clang::dyn_cast<clang::TemplateSpecializationType>(T)) {
        // Print the template name without its corresponding
        // nested-name-specifier.
        SpecType->getTemplateName().print(OS, InnerPolicy,
                                          clang::TemplateName::Qualified::None);

        // Print the template argument list.
        printTemplateArgumentList(printer, SpecType->template_arguments(),
                                  InnerPolicy);

      } else if (const auto *DepSpecType =
                     clang::dyn_cast<clang::DependentTemplateSpecializationType>(T)) {
        // Print the template name without its corresponding
        // nested-name-specifier.
        OS << DepSpecType->getIdentifier()->getName();
        // Print the template argument list.
        printTemplateArgumentList(printer, DepSpecType->template_arguments(),
                                  InnerPolicy);
      } else {
        // Print the type normally
        TypePrinter(OS, Policy, tokens).print(
            clang::QualType(T, 0), clang::StringRef());
      }
      break;
    }
  }

  OS << "::";
}

}  // namespace pasta
