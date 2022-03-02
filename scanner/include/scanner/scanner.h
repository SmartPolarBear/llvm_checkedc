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
#include <unordered_map>

#include <gsl/gsl>

namespace chclang::scanning
{
class scanner
{
public:
	explicit scanner(std::string source);

	std::vector<token> scan();

private:
	static inline std::unordered_map<std::string, token_type> keywords_to_type_{
		{"struct", token_type::STRUCT},
		{"enum", token_type::ENUM},
		{"else", token_type::ELSE},
		{"false", token_type::FALSE},
		{"for", token_type::FOR},
		{"if", token_type::IF},
		{"return", token_type::RETURN},
		{"void", token_type::VOID},
		{"this", token_type::THIS},
		{"true", token_type::TRUE},
		{"switch", token_type::SWITCH},
		{"case", token_type::CASE},
		{"const", token_type::CONST},
		{"while", token_type::WHILE},
		{"sizeof", token_type::SIZEOF},
		{"do", token_type::DO},
		{"short", token_type::SHORT},
		{"float", token_type::FLOAT},
		{"int", token_type::INT},
		{"long", token_type::LONG},
		{"signed", token_type::SIGNED},
		{"unsigned", token_type::UNSIGNED},
		{"double", token_type::DOUBLE},
		{"goto", token_type::GOTO},
		{"default", token_type::DEFAULT},
		{"typedef", token_type::TYPEDEF},
		{"union", token_type::UNION},

	};

	void consume_line_comment();

	void consume_block_comment();

	void scan_string();

	void scan_number_literal();

	void scan_identifier();

	[[nodiscard]] static inline bool is_digit(char c);

	[[nodiscard]] static inline bool is_letter(char c);

	[[nodiscard]] static inline bool is_number_literal_component(char c);

	[[nodiscard]] inline bool is_end() const noexcept
	{
		return cur_ >= src_.size();
	}

	[[nodiscard]] source_information current_source_information() const;

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
	gsl::index cur_{0}, start_{0};
	size_t line_{}, col_{};
};
}