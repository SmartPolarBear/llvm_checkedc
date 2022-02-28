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
// Created by cleve on 2/28/2022.
//

#pragma once

#include "base/singleton.h"
#include "scanner/source_information.h"

#include <fmt/format.h>

#include <iostream>
#include <string>
#include <string_view>

namespace chclang::logging
{
enum class message_level
{
	ERROR, WARNING, INFO
};

class logger final
		: public base::singleton<logger>
{
public:
	logger() = default;

	~logger() = default;

	logger(const logger&) = delete;

	logger(logger&&) = delete;

	logger& operator=(const logger&) = delete;

	void set_info_stream(std::ostream& info);

	void set_error_stream(std::ostream& info);

	void write(message_level level, const scanning::source_information& src, const std::string& msg);

private:
	std::ostream* info_{ &std::clog };
	std::ostream* error_{ &std::cerr };
};
}

#include <magic_enum.hpp>

namespace magic_enum
{
template<>
struct customize::enum_range<chclang::logging::message_level>
{
	static constexpr int min = (int)chclang::logging::message_level::ERROR;
	static constexpr int max = (int)chclang::logging::message_level::INFO;
};
}


template<>
struct fmt::formatter<chclang::logging::message_level> : formatter<std::string_view>
{
	// parse is inherited from formatter<string_view>.
	template<typename FormatContext>
	auto format(chclang::logging::message_level l, FormatContext& ctx)
	{
		std::string info{ magic_enum::enum_name(l) };
		for (auto& c: info)
		{
			c = tolower(c);
		}
		return formatter<string_view>::format(info, ctx);
	}
};