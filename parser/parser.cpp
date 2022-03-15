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

using namespace chclang::scanning;
using namespace chclang::parsing;
using namespace chclang::exceptions;

chclang::parsing::parser::parser(std::vector<scanning::token> tks)
		: tokens_(std::move(tks))
{
}

chclang::parsing::storage_class chclang::parsing::parser::storage_class_specifier()
{
	chclang::parsing::storage_class ret{ 0 };
	while (match({ token_type::TYPEDEF, token_type::EXTERN, token_type::STATIC, token_type::AUTO }))
	{
		auto sc = previous();
		switch (sc.type())
		{
		case token_type::TYPEDEF:
			ret |= (1 << STORAGE_CLASS_TYPEDEF_OFF);
			break;
		case token_type::EXTERN:
			ret |= (1 << STORAGE_CLASS_EXTERN_OFF);
			break;
		case token_type::STATIC:
			ret |= (1 << STORAGE_CLASS_STATIC_OFF);
			break;
		case token_type::AUTO:
			ret |= (1 << STORAGE_CLASS_AUTO_OFF);
			break;

			[[unlikely]]default:
			// NOT POSSIBLE TO REACH HERE
			assert(false);
			break;
		}
	}
	return ret;
}

void parser::synchronize()
{
	[[maybe_unused]] auto _ = advance(); // discard it
	while (!is_end())
	{
		if (previous().type() == token_type::SEMICOLON)return;

		switch (peek().type())
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
	for (const auto& t: types)
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
	return peek(next).type() == t;
}

chclang::scanning::token parser::peek(gsl::index offset)
{
	return tokens_.at(current_.current_token + offset);
}

chclang::scanning::token parser::advance()
{
	if (!is_end())
	{
		current_.current_token++;
	}
	return previous();
}

bool parser::is_end() const
{
	return tokens_[current_.current_token].type() == scanning::token_type::END_OF_FILE;
}

chclang::scanning::token parser::previous()
{
	return tokens_.at(current_.current_token - 1);
}

void parser::push_state()
{
	states_.push(current_);
}

parser::parser_state parser::pop_state()
{
	auto ret = states_.top();
	return ret;
}

void parser::restore_state()
{
	current_ = pop_state();
}

std::vector<std::shared_ptr<statement>> parser::parse()
{
	return std::vector<std::shared_ptr<statement>>();
}

std::vector<std::shared_ptr<statement>> parser::translate_unit()
{
	return std::vector<std::shared_ptr<statement>>();
}

std::shared_ptr<statement> parser::external_declaration()
{
	return std::shared_ptr<statement>();
}

std::shared_ptr<statement> parser::function_definition()
{
	return std::shared_ptr<statement>();
}

std::shared_ptr<statement> parser::declaration_specifiers()
{
	return std::shared_ptr<statement>();
}

std::shared_ptr<statement> parser::declaration()
{
	return std::shared_ptr<statement>();
}

void parser::accept_state()
{

}
