#  Copyright (c) SmartPolarBear 2022.
#
#  Copyright (c) 2021 SmartPolarBear
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
#

import argparse
import logging
import pathlib
import datetime
import json

from typing import Final


def file_date_time(path: str) -> datetime.datetime:
    fname = pathlib.Path(path)
    return datetime.datetime.fromtimestamp(fname.stat().st_mtime)


def file_pathname(arg: str) -> str:
    path: pathlib.Path = pathlib.Path(arg)

    if not path.is_file():
        raise argparse.ArgumentError(message="FATAL:{} should be a file".format(arg))

    if path.exists():
        return str(path.absolute().as_posix())
    else:
        raise argparse.ArgumentError(message="FATAL: path {} not exists".format(arg))


def generate_includes(types: list[int], middle: str) -> list[str]:
    return list("#include <parser/{}/{}>".format(middle, t) for t in types)


def generate_operators(types: list[int], arg_name: str) -> list[str]:
    return list("virtual R operator()(const std::shared_ptr<{0}> &{1}) = 0;".format(t, arg_name) for t in types)


def write_back(inc: str, lines: list[str]):
    logging.info("Writing result to {}.".format(inc))

    with open(inc, "w") as f:
        f.writelines(lines)


def generate_content(inc: str, stmts, exprs):
    lines: list[str] = list()

    with open(inc, "r") as incl:
        line = incl.readline()
        while len(line) != 0:
            lines.append(line.strip())
            line = incl.readline()

    newlines: list = list()
    for l in lines:
        newlines.append(l)
        if "#BEGININC" in l:
            break

    for i in generate_includes(exprs, "expression"):
        newlines.append(i)

    for i in generate_includes(exprs, "statement"):
        newlines.append(i)

    end_include_index = newlines.index("//#ENDINC")

    for l in lines[end_include_index:]:
        newlines.append(l)
        if "#BEGINOP" in l:
            break

    for i in generate_operators(exprs, "expr"):
        newlines.append(i)

    for i in generate_operators(stmts, "stmt"):
        newlines.append(i)

    end_op_index = newlines.index("//#ENDOP")

    for l in lines[end_op_index:]:
        newlines.append(l)

    write_back(inc, lines)


def generate(args: argparse):
    logging.info("Configuration file is {}".format(args.config[0]))
    logging.info("Include file is {}".format(args.include[0]))

    config: str = args.config[0]
    including: str = args.include[0]

    if file_date_time(config) < file_date_time(including):
        logging.info("Everything updated.")
        exit(0)

    exprs: list = []
    stmts: list = []

    with open(str(args.config[0]), "r") as conf:
        conf_dict: dict = json.load(conf)

        expressions: dict = conf_dict["expressions"]
        exprs.append(str(b) for b in expressions)

        statements: dict = conf_dict["statements"]
        stmts.append(str(s) for s in statements)

    generate_content(including, stmts, exprs)


if __name__ == "main":
    parser: argparse = argparse.ArgumentParser(prog="visitor_gen", description="AST visitor generator")

    parser.add_argument('-c', '--config', type=lambda x: file_pathname(x), nargs=1,
                        help='configuration file', required=True)

    parser.add_argument('-i', '--include', type=lambda x: file_pathname(x), nargs=1,
                        help='configuration file', required=True)

    parser = parser.parse_args()

    generate(parser)
