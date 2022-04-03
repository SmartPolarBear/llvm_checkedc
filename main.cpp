#include "config.h"

#include "scanner/scanner.h"

#include <iostream>
#include <string>

#include <argparse/argparse.hpp>

#include <gsl/assert>


using namespace chclang;
using namespace chclang::config;

using namespace argparse;

using namespace std;

void compile_file(const ArgumentParser& program, const std::string& file)
{
	const auto verbose = program.get<bool>("--verbose");

	scanning::scanner scanner{ file };
	auto tokens = scanner.scan();

	// print all tokens
	if (verbose)
	{
		for (uint64_t counter = 1; const auto& tk : tokens)
		{
			clog << tk.lexeme() << (((counter++) % 10 != 0) ? " " : "\n");
		}
	}
}

void compile(const ArgumentParser& program)
{
	vector<string> files;
	try
	{
		files = program.get<std::vector<std::string>>("files");
	}
	catch (std::logic_error& e)
	{
		std::cout << "No files provided" << std::endl;
	}

	Expects(!files.empty());

	for (const auto& file : files)
	{
		compile_file(program, file);
	}
}

int main(int argc, char* argv[])
{
	ArgumentParser program{ string{ predefined_configurations::PROGRAM_NAME },
							string{ predefined_configurations::PROGRAM_VERSION }};

	program.add_argument("files")
		.help("The files to be compiled")
		.remaining()
		.required();

	program.add_argument("--dump-ir")
		.help("Print llvm IR")
		.default_value(false)
		.implicit_value(true);

	program.add_argument("-v", "--verbose")
		.help("Print verbose compiling information")
		.default_value(false)
		.implicit_value(true);

	program.add_argument("-o", "--output")
		.nargs(1)
		.required()
		.help("Specify the output file.");

	try
	{
		program.parse_args(argc, argv);
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
		std::cerr << program;
		std::exit(1);
	}

	compile(program);

	return 0;
}
