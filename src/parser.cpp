#include <iostream>
#include <fstream>
#include <string>
#include <any>
#include <memory>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "No input file provided\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Could not open file\n";
        return 1;
    }

    std::ofstream transpile("transpile.cpp");
    if (!transpile.is_open()) {
        std::cerr << "Failed to create output file\n";
        return 1;
    }

    enum class import_libs {
        none,
        iostream,
        memory
    };
    import_libs lib = import_libs::none;

    const size_t PRINT_PREFIX_LEN = 6;
    const size_t VAR_PREFIX_LEN   = 4;
    const size_t LET_PREFIX_LEN   = 4;

    std::string line;
    std::string output;

    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line == "entry") {
            output += "int main() {\n\n";
        } else if (line == "exit") {
            output += "}\n\n";
        } else if (line.substr(0, PRINT_PREFIX_LEN) == "print(") {
            lib = import_libs::iostream;

            size_t start = line.find('(') + 1;
            size_t end   = line.find(')');
            std::string content = line.substr(start, end - start);

            output += "std::cout << " + content + " << std::endl;\n";

        } else if (line.substr(0, VAR_PREFIX_LEN) == "var ") {
            size_t start = line.find(' ') + 1;
            std::string content = line.substr(start);

            output += "auto " + content + ";\n";

        } else if (line.substr(0, LET_PREFIX_LEN) == "let ") {
            size_t first_space = line.find(' ') + 1;
            size_t equals_sign = line.find('=');

            std::string var_name = line.substr(first_space, equals_sign - first_space - 1);
            std::string value    = line.substr(equals_sign + 1);
            if (!value.empty() && value.back() == ';') value.pop_back();

            output += "auto " + var_name + " = std::make_unique<std::any>(" + value + ");\n";
            lib = import_libs::memory;

        } else {
            output += line + ";\n";
        }
    }

    switch (lib) {
        case import_libs::iostream:
            transpile << "#include <iostream>\n";
            break;
        case import_libs::memory:
            transpile << "#include <memory>\n#include <any>\n";
            break;
        default:
            break;
    }

    transpile << output;

    return 0;
}

