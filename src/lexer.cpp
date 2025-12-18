#include "lexer.h"
#include <fstream>
#include <cctype>
#include <iostream>

std::vector<Token> lexFile(const std::string &filename) {
    std::ifstream file(filename);
    std::vector<Token> tokens;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return tokens;
    }

    std::string line;
    while (std::getline(file, line)) {
        const char* p = line.c_str();

        while (*p) {
            if (isspace(*p)) { 
                p++; 
                continue; 
            }

            if (isdigit(*p)) {
                const char* start = p;
                while (isdigit(*p)) p++;
                tokens.push_back({Tok_Number, std::string(start, p - start)});
                continue;
            }

            if (isalpha(*p)) {
                const char* start = p;
                while (isalnum(*p) || *p == '_') p++;
                std::string word(start, p - start);

                if (word == "var")
                    tokens.push_back({Tok_Var, word});
                else if (word == "print")
                    tokens.push_back({Tok_Prnt, word});
                else
                    tokens.push_back({Tok_Unknown, word});

                continue;
            }

            switch (*p) {
                case '+': tokens.push_back({Tok_Plus, "+"}); break;
                case '*': tokens.push_back({Tok_Mul, "*"}); break;
                case '/': tokens.push_back({Tok_Div, "/"}); break;
                case '=': tokens.push_back({Tok_Eql, "="}); break;
                case ';': tokens.push_back({Tok_EOF, ";"}); break;
                default:
                    tokens.push_back({Tok_Unknown, std::string(1,*p)});
                    break;
            }
            p++;
        }
    }

    tokens.push_back({Tok_EOF, ""});
    return tokens;
}

