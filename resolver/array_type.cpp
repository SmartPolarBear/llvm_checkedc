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
// Created by cleve on 4/1/2022.
//

#include "resolver/array_type.h"

using namespace chclang::resolving;

using namespace std;

std::shared_ptr<array_type> chclang::resolving::array_type::array_of(const std::shared_ptr<type>& base,
	chclang::resolving::array_type::size_type size)
{
	return shared_ptr<array_type>(new array_type{ base, size });
}

chclang::resolving::array_type::array_type(std::shared_ptr<type> base,
	chclang::resolving::array_type::size_type arr_size)
	: type(type_kind::ARRAY, base->size() * arr_size, base->alignment()), base_(std::move(base)), array_size_(arr_size)
{
}
