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

#include <exception>
#include <stdexcept>
#include <string>

#include <fmt/format.h>

namespace chclang::exceptions
{
class file_operation_error
	: public std::runtime_error
{
public:
	explicit file_operation_error(const std::string &filename)
		: filename_(filename), std::runtime_error(fmt::format("{} file error", filename))
	{
	}

private:
	std::string filename_{};
};

class parse_error
	: public std::runtime_error
{
public:

	explicit parse_error(scanning::token tk, const std::string &msg)
		: tk_(std::move(tk)), std::runtime_error(msg)
	{
	}

	~parse_error() override = default;

	const scanning::token &token() const
	{
		return tk_;
	}
private:
	const scanning::token tk_;
};
}