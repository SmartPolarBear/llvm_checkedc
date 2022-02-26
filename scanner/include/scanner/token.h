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

namespace chclang::scanning
{

struct empty_literal_tag
{
};

using integer_literal_type = long long;
using floating_literal_type = long double;
using string_literal_type = std::string;
using boolean_literal_type = bool;

using literal_value_type = std::variant<empty_literal_tag,
		integer_literal_type,
		floating_literal_type,
		string_literal_type,
		boolean_literal_type>;


class token final
{
public:
	static inline constexpr empty_literal_tag empty_literal;

	[[nodiscard]] explicit token(token_type t, std::string lexeme, literal_value_type lit, size_t line)
			: type_(t), lexeme_(std::move(lexeme)), literal_(std::move(lit)), line_(line)
	{
	}

	~token() = default;

	token(const token&) = default;

	token(token&&) = default;

	token& operator=(const token&) = default;

	[[nodiscard]]token_type type() const
	{
		return type_;
	}

	[[nodiscard]]std::string lexeme() const
	{
		return lexeme_;
	}

	[[nodiscard]] literal_value_type literal() const
	{
		return literal_;
	}

	[[nodiscard]] size_t line() const
	{
		return line_;
	}

private:
	token_type type_;
	std::string lexeme_;
	literal_value_type literal_;
	size_t line_;
};
}