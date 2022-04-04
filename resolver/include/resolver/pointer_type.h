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
// Created by cleve on 4/4/2022.
//
#pragma once

#include "resolver/type.h"

#include <memory>

#include <gsl/gsl>

namespace chclang::resolving
{
class pointer_type final
	: public type,
	  public std::enable_shared_from_this<pointer_type>
{
 public:
	using size_type = size_t;

	~pointer_type() = default;
	pointer_type(const pointer_type&) = default;
	pointer_type& operator=(const pointer_type&) = default;

	static std::shared_ptr<pointer_type> pointer_to(const std::shared_ptr<type>& base);

	[[nodiscard]] std::shared_ptr<type> base() const
	{
		return base_;
	}

	DEFAULT_DERIVE(pointer_type);

 protected:
	explicit pointer_type(std::shared_ptr<type> base);

 private:
	std::shared_ptr<type> base_{};
};
}