#include "config.h"

#include "scanner/scanner.h"

#include <iostream>
#include <string>

#include <argparse/argparse.hpp>

#include <gsl/gsl_assert>

using namespace chclang;
using namespace chclang::config;

using namespace argparse;

using namespace std;

void compile_file(const std::string& file)
{
	scanning::scanner scanner{ file };
	auto tokens = scanner.scan();

	// print all tokens
	for (const auto &t: tokens)
	{
		cout << t.lexeme() << " ";
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

	for (const auto& file: files)
	{
		compile_file(file);
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
			.implicit_value(false);

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
