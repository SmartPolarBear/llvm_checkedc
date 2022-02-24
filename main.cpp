#include "config.h"

#include <iostream>
#include <string>

#include <argparse/argparse.hpp>

using namespace chclang::config;

using namespace argparse;

using namespace std;

int main(int argc, char* argv[])
{
	ArgumentParser program{ string{ predefined_configurations::PROGRAM_NAME },
							string{ predefined_configurations::PROGRAM_VERSION }};

	program.add_argument("files")
			.help("The files to be compiled")
			.nargs(predefined_configurations::MAX_FILES)
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

	return 0;
}
