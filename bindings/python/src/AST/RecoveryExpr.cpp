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

void RegisterRecoveryExpr(nb::module_ &m) {
  nb::class_<RecoveryExpr, Expr>(m, "RecoveryExpr")
    .def_prop_ro("begin_token", &RecoveryExpr::BeginToken)
    .def_prop_ro("end_token", &RecoveryExpr::EndToken)
    .def_prop_ro("sub_expressions", &RecoveryExpr::SubExpressions);
}
} // namespace pasta
