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
// Created by cleve on 4/10/2022.
//

#include <resolver/type.h>

#include <parser/variable.h>

#include <unordered_map>
#include <string>
#include <memory>

#pragma once

namespace chclang::parsing
{
class scope
{
 public:
	bool contains_variable(const std::string& key) const;

	std::shared_ptr<variable> at_variable(const std::string& key) const;

	bool contains_tag(const std::string& key) const;

	std::shared_ptr<resolving::type> at_tag(const std::string& key) const;

 private:
	std::unordered_map<std::string, std::shared_ptr<variable>> variables_{};
	std::unordered_map<std::string, std::shared_ptr<resolving::type>> tags_{};
};
}