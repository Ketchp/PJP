#include "Parser.hpp"
#include "Node.hpp"

// Use tutorials in: https://llvm.org/docs/tutorial/

int main (int argc, char *argv[])
{
    Parser parser;

    if(!parser.parse()) {
        return 1;
    }

    parser.generate()->print(llvm::outs(), nullptr);

    return 0;
}
