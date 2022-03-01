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
// Created by cleve on 2/24/2022.
//

#pragma once

#include "scanner/token.h"

#include <fstream>
#include <string>
#include <queue>
#include <vector>

#include <gsl/gsl>

namespace chclang::scanning
{
class scanner
{
public:
	explicit scanner(std::string source);

	std::vector<token> scan();

private:
	void consume_line_comment();

	void consume_block_comment();

	[[nodiscard]] static inline bool is_digit(char c);

	[[nodiscard]] static inline bool is_letter(char c);

	[[nodiscard]] inline bool is_end() const noexcept
	{
		return cur_ >= src_.size();
	}

	void add_token(token_type type, std::optional<literal_value_type> lit = std::nullopt);

	char advance();

	[[nodiscard]] char peek(size_t n = 0);

	[[nodiscard]] bool match(char expect);

	[[nodiscard]] std::string lexeme();

	void scan_next_token();

	std::vector<token> tokens_{};

	std::string src_path_{};

	std::string src_{};

	// TODO: put these in a better "context manager" for better error handling
	gsl::index cur_{ 0 }, start_{ 0 };
	size_t line_{}, col_{};
};
}