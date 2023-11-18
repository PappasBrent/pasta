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

void RegisterElaboratedType(nb::module_ &m) {
  nb::class_<ElaboratedType, TypeWithKeyword>(m, "ElaboratedType")
    .def_prop_ro("desugar", &ElaboratedType::Desugar)
    .def_prop_ro("named_type", &ElaboratedType::NamedType)
    .def_prop_ro("owned_tag_declaration", &ElaboratedType::OwnedTagDeclaration)
    .def_prop_ro("is_sugared", &ElaboratedType::IsSugared);
}
} // namespace pasta
