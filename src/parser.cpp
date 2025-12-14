#include <iostream>
#include <fstream>
#include <string>

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
        iostream
    };
    import_libs lib = import_libs::none;

    std::string line;
    std::string output;

    while (std::getline(file, line)) {
        if (line == "entry") {
            output += "int main() {\n\n";
        } else if (line == "exit") {
            output += "}\n\n";
        } else if (line.substr(0, 6) == "print(") {
            lib = import_libs::iostream;

            size_t start = line.find('(') + 1;
            size_t end = line.find(')');
            std::string content = line.substr(start, end - start);

            output += "std::cout << " + content + " << std::endl;\n";
        } else if (line.substr(0, 4) == "var ") {
            size_t start = line.find(' ') + 1;
            std::string content = line.substr(start); // take everything after "var "
            output += "auto " + content + ";\n";
        }
    }

    switch (lib) {
        case import_libs::none:
            break;
        case import_libs::iostream:
            transpile << "#include <iostream>\n";
            break;
    }

    transpile << output;

    // std::system("clang++ transpile.cpp -o app.exe"); // placeholder, not for release

    return 0;
}
