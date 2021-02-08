/*
 * Copyright (c) 2021 Trail of Bits, Inc.
 */

#include "Token.h"

#include <cassert>

#include <clang/Basic/IdentifierTable.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>
#include <clang/Lex/Token.h>

namespace pasta {
namespace {

// Try to use the kind of the token to get its representation.
static bool ReadRawTokenByKind(clang::SourceManager &source_manager,
                               clang::Token tok, std::string *out) {
  llvm::StringRef backup;
  switch (const auto tok_kind = tok.getKind()) {
    case clang::tok::eof:
    case clang::tok::code_completion:
      return false;

    case clang::tok::identifier: {
      if (const auto ident_info = tok.getIdentifierInfo()) {
        backup = ident_info->getName();
      }
      break;
    }

    case clang::tok::raw_identifier:
      backup = tok.getRawIdentifier();
      break;

    case clang::tok::numeric_constant:
    case clang::tok::char_constant:
    case clang::tok::wide_char_constant:
    case clang::tok::utf8_char_constant:
    case clang::tok::utf16_char_constant:
    case clang::tok::utf32_char_constant:
    case clang::tok::string_literal:
    case clang::tok::wide_string_literal:
    case clang::tok::header_name:
    case clang::tok::utf8_string_literal:
    case clang::tok::utf16_string_literal:
    case clang::tok::utf32_string_literal:
      assert(tok.isLiteral());
      backup = llvm::StringRef(tok.getLiteralData(), tok.getLength());
      break;

#define PUNCTUATOR(case_label, rep) \
    case clang::tok::case_label: \
      backup = clang::tok::getPunctuatorSpelling(tok_kind); \
      break;

#define KEYWORD(case_label, feature) \
    case clang::tok::kw_ ## case_label: \
      backup = clang::tok::getKeywordSpelling(tok_kind); \
      break;

// TODO(pag): Deal with Objective-C @ keywords.

#include <clang/Basic/TokenKinds.def>

    case clang::tok::comment:
    case clang::tok::unknown:
    default:
      break;
  }

  if (!backup.empty()) {
    out->assign(backup.data(), backup.size());
    return true;
  }

  return false;
}

// Read the data of the token into the passed in string pointer. This tries
// to find the backing character data for the token, and fill it in that way.
static bool ReadRawTokenData(clang::SourceManager &source_manager,
                             clang::LangOptions &lang_opts,
                             const clang::Token &tok,
                             const clang::SourceLocation begin_loc,
                             std::string *out) {

  const auto begin = source_manager.getDecomposedLoc(begin_loc);
  if (begin.first.isInvalid()) {
    return false;
  }

  auto invalid = false;
  const auto data = source_manager.getCharacterData(begin_loc, &invalid);
  if (invalid) {
    return false;
  }

  unsigned len = 0;
  if (tok.is(clang::tok::unknown)) {
    len = tok.getLength();
    out->reserve(len);

    for (auto i = 0U; i < len; ++i) {
      switch (data[i]) {
        case '\t':
        case ' ':
        case '\n':
        case '\\':
          out->push_back(data[i]);
          break;
        case '\r':
          break;

        // TODO(pag): This is kind of an error condition.
        default:
          len = i;
          break;
      }
    }

    return !out->empty();

  } else {
    len = clang::Lexer::MeasureTokenLength(begin_loc, source_manager,
                                           lang_opts);
  }

  if (!len) {
    return false;
  }

  // We'll try to get only valid UTF-8 characters, and printable ASCII
  // characters.
  //
  // TODO(pag): This may be overkill, but the lifetimes of the backing buffers
  //            for things like macro expansions is not clear to me, so this
  //            is a reasonable way to go about detecting unusual token data
  //            that may have been corrupted/reused.
  auto can_be_ident = true;

  // We can't allow `NUL` characters into our tokens as we'll be using them
  // to split tokens.
  for (auto i = 0U; i < len; ++i) {
    if (!data[i]) {
      len = i;
      break;
    }
  }

  if (!len) {
    return false;
  }

  out->assign(data, len);

  // Also try to catch errors when reading out identifiers or keywords.
  //
  // TODO(pag): We can't apply this to keywords as it very frequently triggers
  //            in macro definitions, where keyword tokens can contain line
  //            continuations and whitespace.
  if (!can_be_ident && tok.isAnyIdentifier()) {
    // ...
  }

  return true;
}

} // namespace

bool ReadRawToken(clang::SourceManager &source_manager,
                  clang::LangOptions &lang_opts,
                  const clang::Token &tok, std::string *out) {
  out->clear();

  // This could be our sentinel EOF that we add at the end of all tokens, or
  // it could be one of our special macro-expansion EOFs.
  if (tok.is(clang::tok::eof)) {
    return true;

  // Annotations are things like `#pragma`s.
  } else if (tok.isAnnotation()) {

  // A token that has trigraphs or digraphs is one that needs to be cleaned up.
  // The identifier info or literal data info of a token is the post-cleaning
  // representation. If the token therefore never needed cleaning, then we can
  // get its representation via some internal lookup.
  } else if (!tok.needsCleaning()) {
    clang::IdentifierInfo *ident_info = nullptr;

    if (tok.is(clang::tok::raw_identifier)) {
      const auto raw_ident = tok.getRawIdentifier();
      out->assign(raw_ident.data(), raw_ident.size());
      return true;

    } else if (tok.is(clang::tok::identifier) &&
               nullptr != (ident_info = tok.getIdentifierInfo())) {

      out->assign(ident_info->getNameStart(), ident_info->getLength());
      return true;

    } else if (tok.isLiteral() && tok.getLiteralData()) {
      out->assign(tok.getLiteralData(), tok.getLength());
      return true;
    }
  }

  const auto orig_tok_begin = tok.getLocation();
  const auto tok_begin = source_manager.getSpellingLoc(orig_tok_begin);

  // Try to find the token's representation using its location.
  if (tok_begin.isValid()) {
    if (ReadRawTokenData(source_manager, lang_opts, tok, tok_begin, out)) {
      return true;
    }

    bool is_invalid = false;
    const clang::SourceRange token_range(tok_begin);

    auto spelling = clang::Lexer::getSourceText(
        clang::CharSourceRange::getTokenRange(token_range),
        source_manager, lang_opts, &is_invalid);

    if (!is_invalid && !spelling.empty()) {
      out->assign(spelling.data(), spelling.size());
      return true;
    }
  }

  // If all else fails, try to get the representation of the token using
  // its kind.
  return ReadRawTokenByKind(source_manager, tok, out);
}
} // namespace pasta
