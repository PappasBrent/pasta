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

void RegisterCoreturnStmt(nb::module_ &m) {
  nb::class_<CoreturnStmt, Stmt>(m, "CoreturnStmt")
    .def_prop_ro("children", &CoreturnStmt::Children)
    .def_prop_ro("begin_token", &CoreturnStmt::BeginToken)
    .def_prop_ro("end_token", &CoreturnStmt::EndToken)
    .def_prop_ro("keyword_token", &CoreturnStmt::KeywordToken)
    .def_prop_ro("operand", &CoreturnStmt::Operand)
    .def_prop_ro("promise_call", &CoreturnStmt::PromiseCall)
    .def_prop_ro("is_implicit", &CoreturnStmt::IsImplicit);
}
} // namespace pasta
