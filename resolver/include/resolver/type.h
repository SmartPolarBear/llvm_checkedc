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
// Created by cleve on 3/30/2022.
//

#pragma once

#include "resolver/type_kind.h"

#include <cstdint>
#include <cstddef>

#include <memory>

namespace chclang::resolving
{

static inline constexpr size_t DEFAULT_ALIGNMENT = 8;

class type
{
 public:
	[[nodiscard]] constexpr type() = default;
	[[nodiscard]] constexpr type(type_kind kind,
		size_t size,
		size_t alignment = DEFAULT_ALIGNMENT,
		bool us = false,
		bool atom = false)
		: kind_(kind), size_(size), alignment_(alignment), unsigned_(us), atomic_(atom)
	{
	}

	type(const type&) = default;
	type& operator=(const type&) = default;

	[[nodiscard]] type_kind kind() const
	{
		return kind_;
	}

	[[nodiscard]] size_t size() const
	{
		return size_;
	}

	[[nodiscard]] size_t alignment() const
	{
		return alignment_;
	}

	[[nodiscard]] bool is_unsigned() const
	{
		return unsigned_;
	}

	[[nodiscard]] bool is_atomic() const
	{
		return atomic_;
	}

 private:
	type_kind kind_{};
	size_t size_{};
	size_t alignment_{};

	bool unsigned_{};
	bool atomic_{};

	std::weak_ptr<type> origin_{};
	std::weak_ptr<type> base_{};
};

}