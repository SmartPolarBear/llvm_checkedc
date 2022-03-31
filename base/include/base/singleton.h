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

#include <concepts>
#include <type_traits>
#include <memory>

namespace chclang::base
{
template<typename T>
class singleton
{
 public:
	static T& instance();

	singleton(const singleton&) = delete;

	singleton& operator=(const singleton&) = delete;

 protected:
	singleton()
	{
	}
};

template<typename T>
// requires std::derived_from<T, std::enable_shared_from_this<T>>
class shared_ptr_singleton
{
 public:
	static T& instance();

	shared_ptr_singleton(const shared_ptr_singleton&) = delete;

	shared_ptr_singleton& operator=(const shared_ptr_singleton&) = delete;

 protected:
	shared_ptr_singleton()
	{
	}
};

template<typename T>
// requires std::derived_from<T, std::enable_shared_from_this<T>>
T& shared_ptr_singleton<T>::instance()
{
	static T inst{};
	return inst.shared_from_this();
}

template<typename T>
T& singleton<T>::instance()
{
	static T inst{};
	return inst;
}
}