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

#include "logger/logger.h"

using namespace chclang::logging;

void chclang::logging::logger::set_info_stream(std::ostream& info)
{
	info_ = &info;
}

void chclang::logging::logger::set_error_stream(std::ostream& err)
{
	error_ = &err;
}

void
chclang::logging::logger::write(chclang::logging::message_level level, const chclang::scanning::source_information& src,
		const std::string& msg)
{
	auto stream = (level == message_level::ERROR ? error_ : info_);

	*stream << fmt::format("{0:t}: {1} {2}\n{0:c}", src, level, msg);
}
