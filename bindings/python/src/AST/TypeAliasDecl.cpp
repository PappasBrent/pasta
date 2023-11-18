/*
 * Copyright (c) 2023 Trail of Bits, Inc.
 */

// This file is auto-generated.

#include <pasta/AST/AST.h>
#include <pasta/AST/Attr.h>
#include <pasta/AST/Decl.h>
#include <pasta/AST/Printer.h>
#include <pasta/AST/Stmt.h>
#include <pasta/AST/Token.h>
#include <pasta/AST/Type.h>

#include "../Bindings.h"

namespace pasta {
namespace nb = nanobind;

void RegisterTypeAliasDecl(nb::module_ &m) {
  nb::class_<TypeAliasDecl, TypedefNameDecl>(m, "TypeAliasDecl")
    .def_prop_ro("described_alias_template", &TypeAliasDecl::DescribedAliasTemplate);
}
} // namespace pasta
