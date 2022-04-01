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

#include "scanner/token_type.h"
#include "scanner/source_information.h"

#include "resolver/type.h"

#include <optional>

namespace chclang::scanning
{

using integer_literal_type = long long;
using char_literal_type = char;
using floating_literal_type = long double;
using string_literal_type = std::string;
using boolean_literal_type = bool;

using literal_value_type = std::variant<
	integer_literal_type,
	char_literal_type,
	floating_literal_type,
	string_literal_type,
	boolean_literal_type>;

class token final
{
 public:

	[[nodiscard]] explicit token(token_type t,
		std::string lexeme,
		std::optional<literal_value_type> lit,
		source_information src_info,
		std::shared_ptr<resolving::type> tp = nullptr)
		: token_type_(t),
		  lexeme_(std::move(lexeme)),
		  literal_(std::move(lit)),
		  source_info_(std::move(src_info)),
		  type_(std::move(tp))
	{
	}

	~token() = default;

	token(const token&) = default;

	token(token&&) = default;

	token& operator=(const token&) = default;

	[[nodiscard]]chclang::scanning::token_type token_type() const
	{
		return token_type_;
	}

	[[nodiscard]]std::string lexeme() const
	{
		return lexeme_;
	}

	[[nodiscard]] std::optional<literal_value_type> literal() const
	{
		return literal_;
	}

	[[nodiscard]] source_information source_info() const
	{
		return source_info_;
	}

	[[nodiscard]] std::shared_ptr<resolving::type> type() const
	{
		return type_;
	}

 private:
	chclang::scanning::token_type token_type_;
	std::string lexeme_;
	std::optional<literal_value_type> literal_;
	source_information source_info_;

	std::shared_ptr<resolving::type> type_{};
};
}