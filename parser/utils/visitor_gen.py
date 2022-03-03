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


def file_date_time(path: str) -> datetime.datetime:
    fname = pathlib.Path(path)
    return datetime.datetime.fromtimestamp(fname.stat().st_mtime)


def generate_includes(types: list[str], middle: str) -> list[str]:
    return list("#include <parser/{}/{}.h>".format(middle, t) for t in types)


def generate_operators(types: list[str], arg_name: str) -> list[str]:
    return list("virtual R operator()(const std::shared_ptr<{0}> &{1}) = 0;".format(t, arg_name) for t in types)


def write_back(inc: str, lines: list[str]):
    logging.info("Writing result to {}.".format(inc))

    with open(inc, "w") as f:
        for l in lines:
            f.write("{}\n".format(l))


def generate_content(inc: str, stmts, exprs):
    lines: list[str] = list()

    with open(inc, "r") as incl:
        line = incl.readline()
        while len(line) != 0:
            lines.append(line.strip())
            line = incl.readline()

    newlines: list = list()

    begin_include_index = 0
    try:
        begin_include_index = lines.index('//#BEGININC')
    except ValueError:
        logging.error("Missing //#BEGININC tag in visitor.h")
        exit(1)

    for l in lines[:begin_include_index + 1]:
        newlines.append(l)
        if "#BEGININC" in l:
            break

    for i in generate_includes(exprs, "expression"):
        newlines.append(i)

    for i in generate_includes(stmts, "statement"):
        newlines.append(i)

    end_include_index = 0
    try:
        end_include_index = lines.index('//#ENDINC')
    except ValueError:
        logging.error("Missing //#ENDINC tag in visitor.h")
        exit(1)

    begin_op_index = 0
    try:
        begin_op_index = lines.index('//#BEGINOP')
    except ValueError:
        logging.error("Missing //#BEGINOP tag in visitor.h")
        exit(1)

    for l in lines[end_include_index:begin_op_index + 1]:
        newlines.append(l)
        if "#BEGINOP" in l:
            break

    for i in generate_operators(exprs, "expr"):
        newlines.append(i)

    for i in generate_operators(stmts, "stmt"):
        newlines.append(i)

    end_op_index = 0
    try:
        end_op_index = lines.index("//#ENDOP")
    except ValueError:
        logging.error("Missing //#ENDOP tag in visitor.h")
        exit(1)

    for l in lines[end_op_index:]:
        newlines.append(l)

    write_back(inc, newlines)

    logging.info("{} expressions and {} statements is added.".format(len(exprs), len(stmts)))


def generate(args: argparse):
    logging.info("Configuration file is {}".format(args.config[0]))
    logging.info("Include file is {}".format(args.include[0]))

    config: str = args.config[0]
    including: str = args.include[0]

    if file_date_time(config) < file_date_time(including):
        logging.info("Everything to update.")
        exit(0)

    exprs: list = []
    stmts: list = []

    with open(str(args.config[0]), "r") as conf:
        conf_dict: dict = json.load(conf)

        expressions: dict = conf_dict["expressions"]
        exprs.extend(str(b) for b in expressions)

        statements: dict = conf_dict["statements"]
        stmts.extend(str(s) for s in statements)

    generate_content(including, stmts, exprs)


def file_pathname(arg: str) -> str:
    path: pathlib.Path = pathlib.Path(arg)

    if not path.is_file():
        raise argparse.ArgumentError(message="FATAL:{} should be a file".format(arg))

    if path.exists():
        return str(path.absolute().as_posix())
    else:
        raise argparse.ArgumentError(message="FATAL: path {} not exists".format(arg))


if __name__ == "__main__":
    # Logging to screen
    formatter = logging.Formatter('%(message)s')
    logging.getLogger('').setLevel(logging.DEBUG)

    parser: argparse = argparse.ArgumentParser(prog="visitor_gen", description="AST visitor generator")

    parser.add_argument('-c', '--config', type=lambda x: file_pathname(x), nargs=1,
                        help='configuration file', required=True)

    parser.add_argument('-i', '--include', type=lambda x: file_pathname(x), nargs=1,
                        help='configuration file', required=True)

    parser = parser.parse_args()

    generate(parser)
