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

#include "base/singleton.h"

#include "resolver/type.h"

namespace chclang::resolving
{
class boolean_type final
	: public type,
	  public std::enable_shared_from_this<boolean_type>,
	  public base::shared_ptr_singleton<boolean_type>
{
 public:
	[[nodiscard]] boolean_type()
		: type(type_kind::BOOL, 1)
	{
	}
};

class char_type final
	: public type,
	  public std::enable_shared_from_this<char_type>,
	  public base::shared_ptr_singleton<char_type>
{
 public:
	[[nodiscard]] char_type()
		: type(type_kind::CHAR, 1)
	{

	}
};

class short_type final
	: public type,
	  public std::enable_shared_from_this<short_type>,
	  public base::shared_ptr_singleton<short_type>
{

 public:
	[[nodiscard]] short_type()
		: type(type_kind::SHORT, 2)
	{

	}
};

class int_type final
	: public type,
	  public std::enable_shared_from_this<int_type>,
	  public base::shared_ptr_singleton<int_type>
{
 public:
	[[nodiscard]] int_type()
		: type(type_kind::INT, 4)
	{

	}
};

class longlong_type final
	: public type,
	  public std::enable_shared_from_this<longlong_type>,
	  public base::shared_ptr_singleton<longlong_type>
{
 public:
	[[nodiscard]] longlong_type()
		: type(type_kind::LONGLONG, 4)
	{

	}
};

class uchar_type final
	: public type,
	  public std::enable_shared_from_this<uchar_type>,
	  public base::shared_ptr_singleton<uchar_type>
{
 public:
	[[nodiscard]] uchar_type()
		: type(type_kind::CHAR, 1, DEFAULT_ALIGNMENT, true)
	{

	}
};

class ushort_type final
	: public type,
	  public std::enable_shared_from_this<ushort_type>,
	  public base::shared_ptr_singleton<ushort_type>
{
 public:
	[[nodiscard]] ushort_type()
		: type(type_kind::SHORT, 2, DEFAULT_ALIGNMENT, true)
	{

	}
};

class uint_type final
	: public type,
	  public std::enable_shared_from_this<uint_type>,
	  public base::shared_ptr_singleton<uint_type>
{
 public:
	[[nodiscard]] uint_type()
		: type(type_kind::INT, 4, DEFAULT_ALIGNMENT, true)
	{

	}
};

class ulonglong_type final
	: public type,
	  public std::enable_shared_from_this<ulonglong_type>,
	  public base::shared_ptr_singleton<ulonglong_type>
{
 public:
	[[nodiscard]] ulonglong_type()
		: type(type_kind::LONGLONG, 8, DEFAULT_ALIGNMENT, true)
	{

	}
};

}