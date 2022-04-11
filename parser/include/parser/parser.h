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

#pragma once

#include "base/iterable_stack.h"
#include "base/exceptions.h"

#include "parser/statement.h"
#include "parser/expression.h"

#include "scanner/token.h"

#include "resolver/type.h"
#include "scope.h"

#include <vector>
#include <tuple>

#include <gsl/gsl>

namespace chclang::parsing
{
class parser
{
 public:
	template<typename... T>
	using skip_ahead = std::tuple<T..., gsl::index>;

	explicit parser(std::vector<scanning::token> tks);

	std::vector<std::shared_ptr<statement>> parse();
 private:

	skip_ahead<std::shared_ptr<resolving::type>, variable_attributes> declspec(bool allow_storage_specifier = false);

	skip_ahead<std::shared_ptr<resolving::type>> type_name();

	skip_ahead<int64_t> const_expr();

	bool is_current_typename();

	std::shared_ptr<resolving::type> find_typedef(const scanning::token& tk);

	// for error recovery
	void synchronize();

	// for error report
	[[nodiscard]] exceptions::parse_error error(const scanning::token& t, const std::string& msg);

	// TODO: warnings

	// for token reading and matching
	[[nodiscard]] scanning::token consume(scanning::token_type t, const std::string& msg);

	[[nodiscard]] bool match(std::initializer_list<scanning::token_type> types, gsl::index next = 0);

	[[nodiscard]] bool check(scanning::token_type t, gsl::index next = 0);

	[[nodiscard]] scanning::token peek(gsl::index offset = 0);

	[[nodiscard]] scanning::token advance();

	[[nodiscard]] scanning::token current();

	[[nodiscard]] bool is_end() const;

	[[nodiscard]] scanning::token previous();

	std::shared_ptr<variable> find_variable(const scanning::token& tk);

	std::vector<scanning::token> tokens_{};

	gsl::index current_token_{};

	base::iterable_stack<scope> scopes_{};
};
}