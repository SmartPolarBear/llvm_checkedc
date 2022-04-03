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

#include "resolver/integral_type.h"
#include "resolver/floating_type.h"
#include "resolver/array_type.h"

#include <cctype>
#include <sstream>

using namespace std;

using namespace chclang;
using namespace chclang::exceptions;
using namespace chclang::logging;
using namespace chclang::resolving;

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
	std::ifstream input{ src_path_, std::ios::in };

	if (!input.is_open())
	{
		throw file_operation_error{ src_path_ };
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

bool chclang::scanning::scanner::match(const string& expect)
{
	if (is_end())
	{
		return false;
	}

	if (src_.size() - cur_ + 1 < expect.size())
	{
		return false;
	}

	if (strncmp(src_.data() + cur_, expect.data(), expect.size()) != 0)
	{
		return false;
	}

	for (size_t i = 0; i < expect.size(); i++)
	{
		if (src_.at(cur_) != '\n')
		{
			col_++;
		}
		else
		{
			col_ = 0;
		}
		cur_++;
	}
	return true;
}

std::string chclang::scanning::scanner::lexeme()
{
	return src_.substr(start_, cur_ - start_);
}

void chclang::scanning::scanner::add_token(chclang::scanning::token_type type,
	optional<literal_value_type> lit,
	std::shared_ptr<resolving::type> t)
{
	source_information src_info{ line_, col_ };
	tokens_.emplace_back(type, lexeme(), lit, src_info, t);
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
	case '(':
		add_token(token_type::LEFT_PAREN);
		break;
	case ')':
		add_token(token_type::RIGHT_PAREN);
		break;
	case '[':
		add_token(token_type::LEFT_BRACKET);
		break;
	case ']':
		add_token(token_type::RIGHT_BRACKET);
		break;
	case '{':
		add_token(token_type::LEFT_BRACE);
		break;
	case '}':
		add_token(token_type::RIGHT_BRACE);
		break;
	case ',':
		add_token(token_type::COMMA);
		break;
	case '.':
		add_token(token_type::DOT);
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
	case ';':
		add_token(token_type::SEMICOLON);
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
	case '?':
		add_token(token_type::QMARK);
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
	case '~':
		add_token(token_type::BITWISE_NOT);
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
	case ':':
		add_token(token_type::COLON);
		break;

	case '!':
		add_token(match('=') ? token_type::BANG_EQUAL : token_type::BANG);
		break;
	case '=':
		add_token(match('=') ? token_type::EQUAL_EQUAL : token_type::EQUAL);
		break;
	case '<':
		add_token(match('=') ? token_type::LESS_EQUAL : token_type::LESS);
		break;
	case '>':
		add_token(match('=') ? token_type::GREATER_EQUAL : token_type::GREATER);
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

	case '\n':
		line_++;
		break;

	case '"':
		scan_string();
		break;

	case '\'':
		scan_char();

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
	return source_information{ line_, col_, src_path_ };
}

void chclang::scanning::scanner::scan_string()
{
	string val{};

	while (peek() != '"' && !is_end())
	{
		if (auto c = peek();c == '\n')
		{
			line_++;
		}
		else if (match('\\'))
		{
			val += scan_escaped_character();
		}
		else
		{
			val += advance();
		}
	}

	if (is_end())
	{
		logging::logger::instance().error(current_source_information(), "Unterminated string.");
		return;
	}

	advance(); // eat the closing "

	add_token(token_type::STRING, val,
		array_type::array_of(char_type::instance(),
			val.size() + 1 // for null termination
		));
}

void chclang::scanning::scanner::scan_char()
{
	auto lit = advance();
	if (lit == '\\')
	{
		lit = scan_escaped_character();
	}
	else if (peek() == '\'')
	{
		logging::logger::instance().error(current_source_information(),
			fmt::format("Empty character literal ."));
		advance();
	}

	if (peek() != '\'')
	{
		while (peek() != '\'')
		{
			advance();
		}
		advance();

		auto full = src_.substr(start_ + 1, cur_ - start_ - 2);

		logging::logger::instance().error(current_source_information(),
			fmt::format("Too long character literal {}.", full));
	}
	else
	{
		advance();
		add_token(token_type::CHAR_LITERAL, lit, char_type::instance());
	}
}

void chclang::scanning::scanner::scan_number_literal()
{
	if (match("0b") || match("0B")) // binary literal should be handled mannually
	{
		if (!is_number_literal_component(peek()))
		{
			logging::logger::instance().error(current_source_information(),
				fmt::format("Invalid number literal {}.", lexeme()));
		}

		while (is_number_literal_component(peek()))
		{
			advance();
		}

		uint64_t value{ 0 };

		auto num = lexeme();
		for (const auto& n : num)
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

		if (auto type = scan_integral_postfix();type)
		{
			add_token(token_type::INTEGER, static_cast<integer_literal_type>(value), type);
		}
		else
		{
			type = infer_integral_type(static_cast<integer_literal_type>(value));
			add_token(token_type::INTEGER, static_cast<integer_literal_type>(value), type);
		}

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

	bool floating{ false };
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
		auto type = scan_float_postfix();
		add_token(token_type::FLOATING, std::stold(string{ lexeme() }), type);
	}
	else if (integral)
	{
		auto val = std::stoll(string{ lexeme() });
		if (auto type = scan_integral_postfix();type)
		{
			add_token(token_type::INTEGER, val, type);
		}
		else
		{
			type = infer_integral_type(val);
			add_token(token_type::INTEGER, val, type);
		}
	}
	else
	{
		assert(false);
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

char chclang::scanning::scanner::scan_escaped_character()
{
	if (auto c = peek();c >= '0' && c <= '7')
	{
		int32_t val = c - '0';
		if (auto c1 = peek(1);c1 >= '0' && c1 <= '7')
		{
			val = (val << 3) + (c1 - '0');
			if (auto c2 = peek(2);c2 >= '0' && c2 <= '7')
			{
				val = (val << 3) + (c2 - '0');
				advance();
			}
			advance();
		}
		advance();

		return static_cast<char>(val);
	}
	else if (c == 'x')
	{
		advance();
		if (!isdigit(peek()))
		{
			logging::logger::instance().error(current_source_information(),
				fmt::format("Invalid hex escape sequence."));
		}

		int32_t val = 0;
		while (isdigit(peek()))
		{
			val = (val << 4) + from_hex_character(advance());
		}

		return static_cast<char>(val);
	}
	else
	{
		advance();
		switch (c)
		{
		case 'a':
			return '\a';
		case 'b':
			return '\b';
		case 't':
			return '\t';
		case 'n':
			return '\n';
		case 'v':
			return '\v';
		case 'f':
			return '\f';
		case 'r':
			return '\r';
			// [GNU] \e for the ASCII escape character is a GNU C extension.
		case 'e':
			return 27;
		default:
			return c;
		}
	}
}

std::shared_ptr<resolving::type> chclang::scanning::scanner::scan_float_postfix()
{
	if (match('f') || match('F'))
	{
		return float_type::instance();
	}
	else if (match('l') || match('L'))
	{
		return longdouble_type::instance();
	}
	else
	{
		return double_type::instance();
	}
}

shared_ptr<chclang::resolving::type> chclang::scanning::scanner::scan_integral_postfix()
{
	if (match("llu") || match("llU") ||
		match("LLU") || match("LLu") ||
		match("ull") || match("uLL") ||
		match("ULL") || match("Ull"))
	{
		return ulonglong_type::instance();
	}
	else if (match("ul") || match("lu"))
	{
		return uint_type::instance();
	}
	else if (match("ll") || match("LL"))
	{
		return longlong_type::instance();
	}
	else if (match('l') || match('L'))
	{
		return int_type::instance();
	}
	else if (match('u') || match('U'))
	{
		return uint_type::instance();
	}

	return nullptr;
}

shared_ptr<resolving::type> chclang::scanning::scanner::infer_integral_type(chclang::scanning::integer_literal_type val)
{
	if (val >> 63)
	{
		return ulonglong_type::instance();
	}
	else if (val >> 32)
	{
		return longlong_type::instance();
	}
	else if (val >> 31)
	{
		return uint_type::instance();
	}
	else
	{
		return int_type::instance();
	}
}


