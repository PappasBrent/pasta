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

void RegisterOMPParallelForDirective(nb::module_ &m) {
  nb::class_<OMPParallelForDirective, OMPLoopDirective>(m, "OMPParallelForDirective")
    .def_prop_ro("task_reduction_reference_expression", &OMPParallelForDirective::TaskReductionReferenceExpression)
    .def_prop_ro("has_cancel", &OMPParallelForDirective::HasCancel);
}
} // namespace pasta
