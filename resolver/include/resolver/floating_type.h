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
// Created by cleve on 3/31/2022.
//

#pragma once
#include "base/factory.h"

#include "resolver/type.h"

namespace chclang::resolving
{
class float_type final
	: public type,
	  public std::enable_shared_from_this<float_type>,
	  public base::factory<float_type>
{
 public:
	[[nodiscard]] float_type()
		: type(type_kind::FLOAT, 4)
	{
	}

	DEFAULT_DERIVE(float_type);
};

class double_type final
	: public type,
	  public std::enable_shared_from_this<double_type>,
	  public base::factory<double_type>
{
 public:
	[[nodiscard]] double_type()
		: type(type_kind::DOUBLE, 8)
	{

	}

	DEFAULT_DERIVE(double_type);
};

class longdouble_type final
	: public type,
	  public std::enable_shared_from_this<longdouble_type>,
	  public base::factory<longdouble_type>
{
 public:
	[[nodiscard]] longdouble_type()
		: type(type_kind::LONG_DOUBLE, 16)
	{

	}

	DEFAULT_DERIVE(longdouble_type);
};

}