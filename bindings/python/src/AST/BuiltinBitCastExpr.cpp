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

void RegisterBuiltinBitCastExpr(nb::module_ &m) {
  nb::class_<BuiltinBitCastExpr, ExplicitCastExpr>(m, "BuiltinBitCastExpr")
    .def_prop_ro("begin_token", &BuiltinBitCastExpr::BeginToken)
    .def_prop_ro("end_token", &BuiltinBitCastExpr::EndToken);
}
} // namespace pasta
