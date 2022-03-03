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
// Created by cleve on 3/1/2022.
//

#pragma once
#include "parser/statement.h"
#include "parser/expression.h"
#include "scanner/token.h"

#include <vector>

namespace chclang::parsing
{
class parser
{
public:
	explicit parser(std::vector<scanning::token> tks);
private:
	/// translate_unit -> external_declaration*
	void translate_unit();

	/// external_declaration -> function_definition | declaration
	void external_declaration();

	/// function_definition -> declaration_specifiers declarator declaration_list compound_statement
	void function_definition();

	/// declaration_specifiers -> (storage_class_specifier|type_specifier|type_qualifier|function_specifier)*
	void declaration_specifiers();

	std::vector<scanning::token> tokens_{};
};
}