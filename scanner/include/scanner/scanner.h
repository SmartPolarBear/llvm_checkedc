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

#include "resolver/type.h"

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
			{ "auto", token_type::AUTO },
			{ "break", token_type::BREAK },
			{ "case", token_type::CASE },
			{ "char", token_type::CHAR },
			{ "const", token_type::CONST },
			{ "continue", token_type::CONTINUE },
			{ "default", token_type::DEFAULT },
			{ "do", token_type::DO },
			{ "double", token_type::DOUBLE },
			{ "else", token_type::ELSE },
			{ "enum", token_type::ENUM },
			{ "extern", token_type::EXTERN },
			{ "float", token_type::FLOAT },
			{ "for", token_type::FOR },
			{ "goto", token_type::GOTO },
			{ "if", token_type::IF },
			{ "inline", token_type::INLINE },
			{ "int", token_type::INT },
			{ "long", token_type::LONG },
			{ "return", token_type::RETURN },
			{ "short", token_type::SHORT },
			{ "signed", token_type::SIGNED },
			{ "sizeof", token_type::SIZEOF },
			{ "static", token_type::STATIC },
			{ "struct", token_type::STRUCT },
			{ "switch", token_type::SWITCH },
			{ "typedef", token_type::TYPEDEF },
			{ "union", token_type::UNION },
			{ "unsigned", token_type::UNSIGNED },
			{ "void", token_type::VOID },
			{ "volatile", token_type::VOLATILE },
			{ "while", token_type::WHILE },

			// C11 and above
			{ "_Alignas", token_type::ALIGNAS },
			{ "_Alignof", token_type::ALIGNOF },
			{ "_Atomic", token_type::ATOMIC },
			{ "_Bool", token_type::BOOL },
			{ "_Complex", token_type::COMPLEX },
			{ "_Generic", token_type::GENERIC },
			{ "_Imaginary", token_type::IMAGINARY },
			{ "_Noreturn", token_type::NORETURN },
			{ "_Static_assert", token_type::STATICASSERT },
			{ "_Thread_local", token_type::THREADLOCAL },


			// extension
			{ "false", token_type::FALSE },
			{ "true", token_type::TRUE },
			{ "bool", token_type::BOOL },
		};

		static inline constexpr int32_t from_hex_character(char c)
		{
			switch (c)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				return c - '0';
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
				return 10 + c - 'A';
			default:
				return -1;
			}
		}

		void consume_line_comment();

		void consume_block_comment();

		void scan_string();

		char scan_escaped_character();

		void scan_char();

		std::shared_ptr<resolving::type> scan_integral_postfix();

		std::shared_ptr<resolving::type> infer_integral_type(integer_literal_type val);


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

		[[nodiscard]] bool match(const std::string& expect);

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