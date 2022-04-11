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
// Created by cleve on 3/1/2022.
//

#include "logger/logger.h"

#include "parser/parser.h"
#include "parser/visitor.h"

#include <resolver/type.h>
#include <resolver/integral_type.h>

#include <bitset>

using namespace chclang;
using namespace chclang::scanning;
using namespace chclang::parsing;
using namespace chclang::exceptions;

using namespace std;

chclang::parsing::parser::parser(std::vector<scanning::token> tks)
	: tokens_(std::move(tks))
{
	// TODO: define builtin functions
}

void parser::synchronize()
{
	[[maybe_unused]] auto _ = advance(); // discard it
	while (!is_end())
	{
		if (previous().token_type() == token_type::SEMICOLON)return;

		switch (peek().token_type())
		{
		case token_type::STRUCT:
		case token_type::ENUM:
		case token_type::FOR:
		case token_type::IF:
		case token_type::SWITCH:
		case token_type::DO:
		case token_type::WHILE:
		case token_type::RETURN:
		case token_type::AUTO:
		case token_type::BREAK:
		case token_type::CASE:
		case token_type::CHAR:
		case token_type::CONST:
		case token_type::CONTINUE:
		case token_type::DEFAULT:
		case token_type::DOUBLE:
		case token_type::ELSE:
		case token_type::EXTERN:
		case token_type::FLOAT:
		case token_type::GOTO:
		case token_type::INLINE:
		case token_type::INT:
		case token_type::LONG:
		case token_type::SHORT:
		case token_type::SIGNED:
		case token_type::SIZEOF:
		case token_type::STATIC:
		case token_type::TYPEDEF:
		case token_type::UNION:
		case token_type::UNSIGNED:
		case token_type::VOID:
		case token_type::VOLATILE:
			return;
		default:
			[[maybe_unused]] auto _discard2 = advance();  // discard it
			continue;
		}
	}
}

chclang::scanning::token parser::consume(chclang::scanning::token_type t, const std::string& msg)
{
	if (check(t))
	{
		return advance();
	}
	throw error(peek(), msg);
}

chclang::exceptions::parse_error parser::error(const scanning::token& t, const std::string& msg)
{
	logging::logger::instance().error(t.source_info(), msg);
	return parse_error(t, msg);
}

bool parser::match(std::initializer_list<scanning::token_type> types, gsl::index next)
{
	for (const auto& t : types)
	{
		if (check(t, next))
		{
			[[maybe_unused]]auto _ = advance();
			return true;
		}
	}
	return false;
}

bool parser::check(chclang::scanning::token_type t, gsl::index next)
{
	if (is_end())
	{
		return false;
	}
	return peek(next).token_type() == t;
}

chclang::scanning::token parser::peek(gsl::index offset)
{
	return tokens_.at(current_token_ + offset);
}

chclang::scanning::token parser::advance()
{
	if (!is_end())
	{
		current_token_++;
	}
	return previous();
}

bool parser::is_end() const
{
	return tokens_[current_token_].token_type() == scanning::token_type::END_OF_FILE;
}

chclang::scanning::token parser::previous()
{
	return tokens_.at(current_token_ - 1);
}

std::vector<std::shared_ptr<statement>> parser::parse()
{
	vector<std::shared_ptr<statement>> stmts{};
	while (!is_end())
	{
		auto [base_type, sc, next] = declspec();
	}

	return stmts;
}

scanning::token parser::current()
{
	return tokens_[current_token_];
}

bool parser::is_current_typename()
{
	const auto& tk = current();
	if (tk.lexeme() == "void" || tk.lexeme() == "_Bool" ||
		tk.lexeme() == "bool" || tk.lexeme() == "char" ||
		tk.lexeme() == "short" || tk.lexeme() == "int" ||
		tk.lexeme() == "long" || tk.lexeme() == "struct" ||
		tk.lexeme() == "union" || tk.lexeme() == "typedef" ||
		tk.lexeme() == "enum" || tk.lexeme() == "static" ||
		tk.lexeme() == "extern" || tk.lexeme() == "_Alignas" ||
		tk.lexeme() == "signed" || tk.lexeme() == "unsigned" ||
		tk.lexeme() == "const" || tk.lexeme() == "volatile" ||
		tk.lexeme() == "auto" || tk.lexeme() == "register" ||
		tk.lexeme() == "restrict" || tk.lexeme() == "__restrict" ||
		tk.lexeme() == "__restrict__" || tk.lexeme() == "_Noreturn" ||
		tk.lexeme() == "float" || tk.lexeme() == "double" ||
		tk.lexeme() == "typeof" || tk.lexeme() == "inline" ||
		tk.lexeme() == "_Thread_local" || tk.lexeme() == "__thread" ||
		tk.lexeme() == "_Atomic")
	{
		return true;
	}
	else if (find_typedef(current()))
	{
		return true;
	}

	return false;
}

std::shared_ptr<resolving::type> parser::find_typedef(const token& tk)
{
	if (tk.token_type() == scanning::token_type::IDENTIFIER)
	{
		auto var = find_variable(tk);
		if (var && var->var_type() == variable_type::TYPEDEF)
		{
			return static_pointer_cast<typedef_variable>(var)->type();
		}
	}
	return nullptr;
}

parser::skip_ahead<shared_ptr<resolving::type>, variable_attributes> parser::declspec(bool allow_storage_specifier)
{
	// Use for counter for various combos of internal types
	enum : uint32_t
	{
		VOID = 1 << 0,
		BOOL = 1 << 2,
		CHAR = 1 << 4,
		SHORT = 1 << 6,
		INT = 1 << 8,
		LONG = 1 << 10,
		FLOAT = 1 << 12,
		DOUBLE = 1 << 14,
		OTHER = 1 << 16,
		SIGNED = 1 << 17,
		UNSIGNED = 1 << 18,
	};

	chclang::parsing::variable_attributes attr{ 0 };
	std::shared_ptr<resolving::type> type{ resolving::int_type::make() }; // default int rule

	while (is_current_typename())
	{

		if (match({ token_type::TYPEDEF,
					token_type::EXTERN,
					token_type::STATIC,
					token_type::AUTO,
					token_type::INLINE,
					token_type::THREADLOCAL,
					token_type::CONST,
					token_type::VOLATILE,
					token_type::NORETURN }))
		{
			if (!allow_storage_specifier)
			{
				throw error(current(), "Storage specifier is not allowed in this context.");
			}

			auto sc = previous();
			switch (sc.token_type())
			{
			case token_type::TYPEDEF:
				attr.storage_class |= (1 << STORAGE_CLASS_TYPEDEF_OFF);
				break;
			case token_type::EXTERN:
				attr.storage_class |= (1 << STORAGE_CLASS_EXTERN_OFF);
				break;
			case token_type::STATIC:
				attr.storage_class |= (1 << STORAGE_CLASS_STATIC_OFF);
				break;
			case token_type::AUTO:
				attr.storage_class |= (1 << STORAGE_CLASS_AUTO_OFF);
				break;
			case token_type::INLINE:
				attr.storage_class |= (1 << STORAGE_CLASS_INLINE_OFF);
				break;
			case token_type::THREADLOCAL:
				attr.storage_class |= (1 << STORAGE_CLASS_THREAD_LOCAL_OFF);
				break;
			case token_type::CONST:
				attr.is_const = true;
				break;
			case token_type::VOLATILE:
				attr.is_volatile = true;
				break;
			case token_type::NORETURN:
				attr.no_return = true;
				break;
			default:
				assert(false);
			}

			if ((attr.storage_class & (1 << STORAGE_CLASS_TYPEDEF_OFF)) && (attr.storage_class & 0b11110))
			{
				throw error(previous(), "Redundant typedef specifier.");
			}
			continue;
		}

		if (match({ token_type::ATOMIC }))
		{
			if (match({ token_type::LEFT_PAREN }))
			{
				auto [t, next] = type_name();
				type = t;
				current_token_ = next;
				consume(scanning::token_type::RIGHT_PAREN, "Unmatched '(' and ')'.");
			}
			else
			{
				auto [t, next] = type_name();
				if (t)
				{
					type = t;
					current_token_ = next;
				}
				else
				{
					throw error(previous(), fmt::format("Invalid type {} for _Atomic.", current().lexeme()));
				}
			}
			continue;
		}

		if (match({ token_type::ALIGNAS }))
		{
			if (!allow_storage_specifier)
			{
				throw error(current(), "_Alignas is not allowed in this context.");
			}

			consume(scanning::token_type::LEFT_PAREN, "Alignment specifier is required.");

			if (is_current_typename())
			{
				auto [t, next] = type_name();
				attr.alignment = t->alignment();
				current_token_ = next;
			}
			else
			{
				auto [align, next] = const_expr();
				attr.alignment = align;
				current_token_ = next;
			}
			continue;

		}
	}

}

std::shared_ptr<variable> parser::find_variable(const token& tk)
{
	for (const auto& scope : scopes_)
	{
		if (scope.contains_variable(tk.lexeme()))
		{
			return scope.at_variable(tk.lexeme());
		}
	}

	return nullptr;
}
parser::skip_ahead<shared_ptr<resolving::type>> parser::type_name()
{
	return chclang::parsing::parser::skip_ahead<shared_ptr<resolving::type>>();
}

parser::skip_ahead<int64_t> parser::const_expr()
{
	return chclang::parsing::parser::skip_ahead<int64_t>();
}





