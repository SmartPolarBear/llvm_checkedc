// Copyright (c) SmartPolarBear 2022.
//
// Copyright (c) 2021 SmartPolarBear
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//

//
// Created by cleve on 2/26/2022.
//


#pragma once

namespace chclang::scanning
{
enum class token_type
{
	// Single-character tokens.
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET /* [ */, RIGHT_BRACKET, LEFT_BRACE /* { */, RIGHT_BRACE,
	COMMA, DOT,
	SEMICOLON, COLON /* : */, SLASH,
	QMARK /* ? */,

	// One or two character tokens.
	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,
	MINUS, MINUS_MINUS,
	PLUS, PLUS_PLUS,
	STAR, MOD,
	BITWISE_AND, BITWISE_OR, BITWISE_XOR, BITWISE_NOT,

	// Two character tokens
	ARROW, /* -> */
	AND, OR,
	PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, SLASH_EQUAL,
	MOD_EQUAL, BITWISE_AND_EQUAL, BITWISE_OR_EQUAL, BITWISE_XOR_EQUAL,

	// Literals.
	IDENTIFIER, STRING,
	INTEGER, FLOATING,

	// Keywords.
	AUTO, BREAK, CASE, CHAR, CONST, CONTINUE,
	DEFAULT, DO, DOUBLE, ELSE, ENUM, EXTERN,
	FLOAT, FOR, GOTO, IF, INLINE, INT, LONG,
	RETURN, SHORT, SIGNED,
	SIZEOF, STATIC, STRUCT, SWITCH, TYPEDEF, UNION,
	UNSIGNED, VOID, VOLATILE, WHILE,

	// C11 and above

	ALIGNAS, ALIGNOF, ATOMIC, BOOL, COMPLEX, GENERIC,
	IMAGINARY, NORETURN, STATICASSERT, THREADLOCAL,

	// extension
	FALSE, TRUE,

	END_OF_FILE,
};
}