#include <fstream>

#include "Parser.hpp"
#include "Node.hpp"

// Use tutorials in: https://llvm.org/docs/tutorial/

int main()
{
    Parser parser;

    if(!parser.parse()) {
        return 1;
    }

    parser.generate()->print(llvm::outs(), nullptr);

    std::ofstream file{"formatted.mila"};
    parser.format_ast(file);
//    parser.format_ast(std::cout);

    return 0;
}
