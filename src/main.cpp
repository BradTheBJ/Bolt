#include "lexer.h"
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " file.bolt\n";
        return 1;
    }

    auto tokens = lexFile(argv[1]);

    for (auto &t : tokens) {
        if (t.type == Tok_EOF) {
            std::cout << t.text << " \n";
        } else {
            std::cout << t.text << " ";
        }
    }
    std::cout << "\n";

    return 0;
}
