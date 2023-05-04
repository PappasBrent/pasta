/*
 * Copyright (c) 2023 Trail of Bits, Inc.
 */

// This file is auto-generated.

#include <pasta/AST/AST.h>
#include <pasta/AST/Attr.h>
#include <pasta/AST/Decl.h>
#include <pasta/AST/Stmt.h>
#include <pasta/AST/Type.h>

#include <pybind11/pybind11.h>

namespace pasta {
namespace py = pybind11;

void RegisterFunctionDecl(py::module_ &m) {
  py::class_<FunctionDecl, Decl, DeclaratorDecl, NamedDecl, ValueDecl>(m, "FunctionDecl")
    .def_property_readonly("FriendConstraintRefersToEnclosingTemplate", &FunctionDecl::FriendConstraintRefersToEnclosingTemplate)
    .def_property_readonly("UsesFPIntrin", &FunctionDecl::UsesFPIntrin)
    .def_property_readonly("DoesDeclarationForceExternallyVisibleDefinition", &FunctionDecl::DoesDeclarationForceExternallyVisibleDefinition)
    .def_property_readonly("DoesThisDeclarationHaveABody", &FunctionDecl::DoesThisDeclarationHaveABody)
    .def_property_readonly("BuiltinID", &FunctionDecl::BuiltinID)
    .def_property_readonly("CallResultType", &FunctionDecl::CallResultType)
    .def_property_readonly("CanonicalDeclaration", &FunctionDecl::CanonicalDeclaration)
    .def_property_readonly("ConstexprKind", &FunctionDecl::ConstexprKind)
    .def_property_readonly("DeclaredReturnType", &FunctionDecl::DeclaredReturnType)
    .def_property_readonly("DefaultToken", &FunctionDecl::DefaultToken)
    .def_property_readonly("Definition", &FunctionDecl::Definition)
    .def_property_readonly("DescribedFunctionTemplate", &FunctionDecl::DescribedFunctionTemplate)
    .def_property_readonly("EllipsisToken", &FunctionDecl::EllipsisToken)
    .def_property_readonly("ExceptionSpecSourceRange", &FunctionDecl::ExceptionSpecSourceRange)
    .def_property_readonly("ExceptionSpecType", &FunctionDecl::ExceptionSpecType)
    .def_property_readonly("InstantiatedFromDeclaration", &FunctionDecl::InstantiatedFromDeclaration)
    .def_property_readonly("InstantiatedFromMemberFunction", &FunctionDecl::InstantiatedFromMemberFunction)
    .def_property_readonly("LanguageLinkage", &FunctionDecl::LanguageLinkage)
    .def_property_readonly("MemoryFunctionKind", &FunctionDecl::MemoryFunctionKind)
    .def_property_readonly("MinRequiredArguments", &FunctionDecl::MinRequiredArguments)
    .def_property_readonly("MultiVersionKind", &FunctionDecl::MultiVersionKind)
    .def_property_readonly("NumParameters", &FunctionDecl::NumParameters)
    .def_property_readonly("ODRHash", &FunctionDecl::ODRHash)
    .def_property_readonly("OverloadedOperator", &FunctionDecl::OverloadedOperator)
    .def_property_readonly("ParametersSourceRange", &FunctionDecl::ParametersSourceRange)
    .def_property_readonly("PointOfInstantiation", &FunctionDecl::PointOfInstantiation)
    .def_property_readonly("PrimaryTemplate", &FunctionDecl::PrimaryTemplate)
    .def_property_readonly("ReturnType", &FunctionDecl::ReturnType)
    .def_property_readonly("ReturnTypeSourceRange", &FunctionDecl::ReturnTypeSourceRange)
    .def_property_readonly("StorageClass", &FunctionDecl::StorageClass)
    .def_property_readonly("TemplateInstantiationPattern", &FunctionDecl::TemplateInstantiationPattern)
    .def_property_readonly("TemplateSpecializationKind", &FunctionDecl::TemplateSpecializationKind)
    .def_property_readonly("TemplateSpecializationKindForInstantiation", &FunctionDecl::TemplateSpecializationKindForInstantiation)
    .def_property_readonly("TemplatedKind", &FunctionDecl::TemplatedKind)
    .def_property_readonly("HasImplicitReturnZero", &FunctionDecl::HasImplicitReturnZero)
    .def_property_readonly("HasInheritedPrototype", &FunctionDecl::HasInheritedPrototype)
    .def_property_readonly("HasOneParameterOrDefaultArguments", &FunctionDecl::HasOneParameterOrDefaultArguments)
    .def_property_readonly("HasPrototype", &FunctionDecl::HasPrototype)
    .def_property_readonly("HasSkippedBody", &FunctionDecl::HasSkippedBody)
    .def_property_readonly("HasTrivialBody", &FunctionDecl::HasTrivialBody)
    .def_property_readonly("HasWrittenPrototype", &FunctionDecl::HasWrittenPrototype)
    .def_property_readonly("InstantiationIsPending", &FunctionDecl::InstantiationIsPending)
    .def_property_readonly("IsCPUDispatchMultiVersion", &FunctionDecl::IsCPUDispatchMultiVersion)
    .def_property_readonly("IsCPUSpecificMultiVersion", &FunctionDecl::IsCPUSpecificMultiVersion)
    .def_property_readonly("IsConsteval", &FunctionDecl::IsConsteval)
    .def_property_readonly("IsConstexpr", &FunctionDecl::IsConstexpr)
    .def_property_readonly("IsConstexprSpecified", &FunctionDecl::IsConstexprSpecified)
    .def_property_readonly("IsDefaulted", &FunctionDecl::IsDefaulted)
    .def_property_readonly("IsDeleted", &FunctionDecl::IsDeleted)
    .def_property_readonly("IsDeletedAsWritten", &FunctionDecl::IsDeletedAsWritten)
    .def_property_readonly("IsDestroyingOperatorDelete", &FunctionDecl::IsDestroyingOperatorDelete)
    .def_property_readonly("IsExplicitlyDefaulted", &FunctionDecl::IsExplicitlyDefaulted)
    .def_property_readonly("IsExternC", &FunctionDecl::IsExternC)
    .def_property_readonly("IsFunctionTemplateSpecialization", &FunctionDecl::IsFunctionTemplateSpecialization)
    .def_property_readonly("IsGlobal", &FunctionDecl::IsGlobal)
    .def_property_readonly("IsImplicitlyInstantiable", &FunctionDecl::IsImplicitlyInstantiable)
    .def_property_readonly("IsInExternCContext", &FunctionDecl::IsInExternCContext)
    .def_property_readonly("IsInExternCXXContext", &FunctionDecl::IsInExternCXXContext)
    .def_property_readonly("IsIneligibleOrNotSelected", &FunctionDecl::IsIneligibleOrNotSelected)
    .def_property_readonly("IsInlineBuiltinDeclaration", &FunctionDecl::IsInlineBuiltinDeclaration)
    .def_property_readonly("IsInlineDefinitionExternallyVisible", &FunctionDecl::IsInlineDefinitionExternallyVisible)
    .def_property_readonly("IsInlineSpecified", &FunctionDecl::IsInlineSpecified)
    .def_property_readonly("IsInlined", &FunctionDecl::IsInlined)
    .def_property_readonly("IsLateTemplateParsed", &FunctionDecl::IsLateTemplateParsed)
    .def_property_readonly("IsMSExternInline", &FunctionDecl::IsMSExternInline)
    .def_property_readonly("IsMSVCRTEntryPoint", &FunctionDecl::IsMSVCRTEntryPoint)
    .def_property_readonly("IsMain", &FunctionDecl::IsMain)
    .def_property_readonly("IsMultiVersion", &FunctionDecl::IsMultiVersion)
    .def_property_readonly("IsNoReturn", &FunctionDecl::IsNoReturn)
    .def_property_readonly("IsOverloadedOperator", &FunctionDecl::IsOverloadedOperator)
    .def_property_readonly("IsPure", &FunctionDecl::IsPure)
    .def_property_readonly("IsReplaceableGlobalAllocationFunction", &FunctionDecl::IsReplaceableGlobalAllocationFunction)
    .def_property_readonly("IsReservedGlobalPlacementOperator", &FunctionDecl::IsReservedGlobalPlacementOperator)
    .def_property_readonly("IsStatic", &FunctionDecl::IsStatic)
    .def_property_readonly("IsTargetClonesMultiVersion", &FunctionDecl::IsTargetClonesMultiVersion)
    .def_property_readonly("IsTargetMultiVersion", &FunctionDecl::IsTargetMultiVersion)
    .def_property_readonly("IsTemplateInstantiation", &FunctionDecl::IsTemplateInstantiation)
    .def_property_readonly("IsThisDeclarationADefinition", &FunctionDecl::IsThisDeclarationADefinition)
    .def_property_readonly("IsThisDeclarationInstantiatedFromAFriendDefinition", &FunctionDecl::IsThisDeclarationInstantiatedFromAFriendDefinition)
    .def_property_readonly("IsTrivial", &FunctionDecl::IsTrivial)
    .def_property_readonly("IsTrivialForCall", &FunctionDecl::IsTrivialForCall)
    .def_property_readonly("IsUserProvided", &FunctionDecl::IsUserProvided)
    .def_property_readonly("IsVariadic", &FunctionDecl::IsVariadic)
    .def_property_readonly("IsVirtualAsWritten", &FunctionDecl::IsVirtualAsWritten)
    .def_property_readonly("Parameters", &FunctionDecl::Parameters)
    .def_property_readonly("UsesSEHTry", &FunctionDecl::UsesSEHTry)
    .def_property_readonly("WillHaveBody", &FunctionDecl::WillHaveBody)
    .def_property_readonly("Body", &FunctionDecl::Body);
}
} // namespace pasta
