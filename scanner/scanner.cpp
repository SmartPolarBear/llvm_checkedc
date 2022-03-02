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

#include "base/exceptions.h"
#include "logger/logger.h"
#include "scanner/scanner.h"

#include <cctype>
#include <sstream>

using namespace std;
using namespace chclang::exceptions;
using namespace chclang::logging;

bool chclang::scanning::scanner::is_digit(char c)
{
	return c >= '0' && c <= '9';
}

bool chclang::scanning::scanner::is_letter(char c)
{
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

bool chclang::scanning::scanner::is_number_literal_component(char c)
{
	return is_digit(c) || c == 'x' || c == 'X' || c == 'b' || c == 'B';
}

chclang::scanning::scanner::scanner(std::string source)
	: src_path_(std::move(source))
{
	std::ifstream input{src_path_, std::ios::in};

	if (!input.is_open())
	{
		throw file_operation_error{src_path_};
	}

	stringstream ss{};
	ss << input.rdbuf();

	src_ = ss.str();
}

char chclang::scanning::scanner::advance()
{
	auto ret = src_.at(cur_++);
	if (ret != '\n')
	{
		col_++;
	}
	else
	{
		col_ = 0;
	}
	return ret;
}

char chclang::scanning::scanner::peek(size_t n)
{
	if (is_end())
	{
		return 0;
	}
	if (cur_ + n >= src_.size())
	{
		return 0;
	}

	return src_.at(cur_ + n);
}

bool chclang::scanning::scanner::match(char expect)
{
	if (is_end())
	{
		return false;
	}
	if (src_.at(cur_) != expect)
	{
		return false;
	}

	if (src_.at(cur_) != '\n')
	{
		col_++;
	}
	else
	{
		col_ = 0;
	}
	cur_++;
	return true;
}

std::string chclang::scanning::scanner::lexeme()
{
	return src_.substr(start_, cur_ - start_);
}

void chclang::scanning::scanner::add_token(chclang::scanning::token_type type, optional<literal_value_type> lit)
{
	source_information src_info{line_, col_};
	tokens_.emplace_back(type, lexeme(), lit, src_info);
}

vector<chclang::scanning::token> chclang::scanning::scanner::scan()
{
	while (!is_end())
	{
		start_ = cur_;
		scan_next_token();
	}

	add_token(token_type::END_OF_FILE);
	return tokens_;
}

void chclang::scanning::scanner::scan_next_token()
{
	char c = advance();
	switch (c)
	{
	case '(': add_token(token_type::LEFT_PAREN);
		break;
	case ')': add_token(token_type::RIGHT_PAREN);
		break;
	case '[': add_token(token_type::LEFT_BRACKET);
		break;
	case ']': add_token(token_type::RIGHT_BRACKET);
		break;
	case '{': add_token(token_type::LEFT_BRACE);
		break;
	case '}': add_token(token_type::RIGHT_BRACE);
		break;
	case ',': add_token(token_type::COMMA);
		break;
	case '.': add_token(token_type::DOT);
		break;
	case '-':
		if (match('-'))
		{
			add_token(token_type::MINUS_MINUS);
		}
		else if (match('>'))
		{
			add_token(token_type::ARROW);
		}
		else if (match('='))
		{
			add_token(token_type::MINUS_EQUAL);
		}
		else
		{
			add_token(token_type::MINUS);
		}
		break;
	case '%':
		if (match('='))
		{
			add_token(token_type::MOD);
		}
		else
		{
			add_token(token_type::MOD_EQUAL);
		}
		break;
	case '+':
		if (match('+'))
		{
			add_token(token_type::PLUS_PLUS);
		}
		else if (match('='))
		{
			add_token(token_type::PLUS_EQUAL);
		}
		else
		{
			add_token(token_type::PLUS);
		}
		break;
	case ';': add_token(token_type::SEMICOLON);
		break;
	case '*':
		if (match('='))
		{
			add_token(token_type::STAR_EQUAL);
		}
		else
		{
			add_token(token_type::STAR);
		}
		break;
	case '?': add_token(token_type::QMARK);
		break;
	case '^':
		if (match('='))
		{
			add_token(token_type::BITWISE_XOR_EQUAL);
		}
		else
		{
			add_token(token_type::BITWISE_XOR);
		}
		break;
	case '~': add_token(token_type::BITWISE_NOT);
		break;
	case '|':
		if (match('|'))
		{
			add_token(token_type::OR);
		}
		else if (match('='))
		{
			add_token(token_type::BITWISE_OR_EQUAL);
		}
		else
		{
			add_token(token_type::BITWISE_OR);
		}
		break;
	case '&':
		if (match('&'))
		{
			add_token(token_type::AND);
		}
		else if (match('='))
		{
			add_token(token_type::BITWISE_AND_EQUAL);
		}
		else
		{
			add_token(token_type::BITWISE_AND);
		}
		break;
	case ':': add_token(token_type::COLON);
		break;

	case '!': add_token(match('=') ? token_type::BANG_EQUAL : token_type::BANG);
		break;
	case '=': add_token(match('=') ? token_type::EQUAL_EQUAL : token_type::EQUAL);
		break;
	case '<': add_token(match('=') ? token_type::LESS_EQUAL : token_type::LESS);
		break;
	case '>': add_token(match('=') ? token_type::GREATER_EQUAL : token_type::GREATER);
		break;

	case '/':
		if (match('/')) // this is a line comment
		{
			consume_line_comment();
		}
		else if (match('*')) // this is a block comment
		{
			consume_block_comment();
		}
		else if (match('='))
		{
			add_token(token_type::SLASH_EQUAL);
		}
		else
		{
			add_token(token_type::SLASH);
		}
		break;

	case ' ':
	case '\r':
	case '\t':
		// Do nothing to ignore whitespaces.
		break;

	case '\n': line_++;
		break;

	case '"': scan_string();
		break;

	default:
		if (is_digit(c)) // a number literal begins with a digit
		{
			scan_number_literal();
		}
		else if (is_letter(c)) // an identifier begins with a letter
		{
			scan_identifier();
		}
		else
		{
			logging::logger::instance().error(current_source_information(), fmt::format("Unexpected character {}.", c));
		}

		break;
	}
}

void chclang::scanning::scanner::consume_line_comment()
{
	while (peek() != '\n' && !is_end())
	{
		[[maybe_unused]] auto discard = advance();
	}
}

void chclang::scanning::scanner::consume_block_comment()
{
	while (!is_end())
	{
		auto c = advance();
		if (c == '\n')
		{
			line_++;
		}
		else if (c == '*' && peek() == '/')
		{
			[[maybe_unused]]auto _ = advance(); // eat "/"
			return;
		}
		else if (c == '/' && peek() == '*')
		{
			[[maybe_unused]]auto _ = advance(); // eat "*"
			consume_block_comment(); // nested block comment
		}
	}

	if (!is_end())
	{
		logger::instance().error(current_source_information(), "Unclosed block comment found.");
	}
}

chclang::scanning::source_information chclang::scanning::scanner::current_source_information() const
{
	return source_information{line_, col_, src_path_};
}

void chclang::scanning::scanner::scan_string()
{
	while (peek() != '"' && !is_end())
	{
		if (peek() == '\n')
		{
			line_++;
		}
		advance();
	}

	if (is_end())
	{
		logging::logger::instance().error(current_source_information(), "Unterminated string.");
		return;
	}

	advance(); // eat the closing "

	string val = src_.substr(start_ + 1, cur_ - start_ - 2);

	add_token(token_type::STRING, val);
}

void chclang::scanning::scanner::scan_number_literal()
{
	if (peek() == '0' && (peek(1) == 'b' || peek(1) == 'B')) // binary literal should be handled mannually
	{
		// eat 0b/B
		advance();
		advance();

		if (!is_number_literal_component(peek()))
		{
			logging::logger::instance().error(current_source_information(),
											  fmt::format("Invalid number literal {}.", lexeme()));
		}

		while (is_number_literal_component(peek()))
		{
			advance();
		}

		uint64_t value{0};

		auto num = lexeme();
		for (const auto &n: num)
		{
			if (n != '0' && n != '1')
			{
				logging::logger::instance().error(current_source_information(),
												  fmt::format("Invalid binary number literal {}.", num));
				break;
			}

			value <<= 1;
			value |= (n == '1');
		}

		add_token(token_type::INTEGER, static_cast<integer_literal_type>(value));

		return;
	}

	bool integral = false;
	while (is_number_literal_component(peek()))
	{
		char c = advance();
		if (c == 'x' || c == 'X')
		{
			integral = true;
		}
	}

	bool floating{false};
	if (peek() == '.' && is_number_literal_component(peek(1)))
	{
		floating = true;
		advance();
		while (is_number_literal_component(peek()))
			advance();
	}

	if (integral && floating)
	{
		logging::logger::instance().error(current_source_information(),
										  fmt::format("Invalid number literal {}.", lexeme()));

	}
	else if (floating)
	{
		add_token(token_type::FLOATING, std::stold(string{lexeme()}));
	}
	else
	{
		add_token(token_type::INTEGER, std::stoll(string{lexeme()}));
	}
}

void chclang::scanning::scanner::scan_identifier()
{
	while (is_digit(peek()) || is_letter(peek()))
		advance();

	auto identifier = lexeme();

	if (keywords_to_type_.contains(identifier))
	{
		add_token(keywords_to_type_.at(identifier));
	}
	else
	{
		add_token(token_type::IDENTIFIER);
	}
}

