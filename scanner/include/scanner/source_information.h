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

#include <fmt/format.h>

#include <string>
#include <string_view>

namespace chclang::scanning
{
class source_information final
{
public:
	explicit source_information(size_t line, size_t col, std::string filename = "", std::string context = "")
			: line_(line), column_(col), context_(std::move(context)), file_name_(std::move(filename))
	{
	}

	~source_information() = default;

	source_information(const source_information&) = default;

	source_information(source_information&&) = default;

	source_information& operator=(const source_information&) = default;

	[[nodiscard]] size_t line() const
	{
		return line_;
	}

	[[nodiscard]] size_t column() const
	{
		return column_;
	}


	[[nodiscard]] std::string context() const
	{
		return context_;
	}

	[[nodiscard]] std::string file() const
	{
		return file_name_;
	}

private:
	size_t line_{};
	size_t column_{};

	std::string context_{};
	std::string file_name_{};
};
}

template<>
struct fmt::formatter<chclang::scanning::source_information>
{
	bool title{ false };

	constexpr auto parse(fmt::format_parse_context& context)
	{
		auto it = context.begin(), end = context.end();
		if (it != end && (*it == 't' || *it == 'c')) title = (*it++) == 't';

		if (it != end && *it != '}') throw fmt::format_error("Invalid format");

		return it;
	}

	template<class FormatContext>
	auto format(
			const chclang::scanning::source_information& src,
			FormatContext& context)
	{
		if (title)
		{
			return format_to(context.out(), "{}:{}:{}", src.file(), src.line(), src.column());
		}
		else
		{
			return format_to(context.out(), "  ", src.context());
		}
	}
};