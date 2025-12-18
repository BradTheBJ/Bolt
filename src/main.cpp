#include "bolt/lexer.h"
#include "bolt/parser.h"
#include "bolt/codegen.h"
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <string>

std::string getOutputName(const std::string& inputFile) {
    std::filesystem::path path(inputFile);
    std::string stem = path.stem().string();
    return stem;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " file.bolt\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputName = getOutputName(inputFile);
    std::string tempLlFile = outputName + ".ll";

    // Generate LLVM IR
    auto tokens = lexFile(inputFile);
    Parser parser(tokens);
    auto astRoot = parser.parse();
    if (!astRoot) {
        std::cerr << "Error: Failed to parse input file\n";
        return 1;
    }
    codeGen(astRoot, tempLlFile);

    // Compile LLVM IR to executable
    std::string compileCmd = "clang " + tempLlFile + " -o " + outputName;
    int result = std::system(compileCmd.c_str());
    if (result != 0) {
        std::cerr << "Error: Failed to compile LLVM IR\n";
        std::filesystem::remove(tempLlFile);
        return 1;
    }

    // Delete temporary .ll file
    std::filesystem::remove(tempLlFile);

    return 0;
}
