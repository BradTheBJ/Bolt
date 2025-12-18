#pragma once
#include <string>
#include <vector>

enum TokenType {
    Tok_Number,
    Tok_Plus,
    Tok_Mul,
    Tok_Eql,
    Tok_Div,
    Tok_Var,
    Tok_Prnt,
    Tok_EOF,
    Tok_Unknown
};

struct Token {
    TokenType type;
    std::string text;
};

extern std::vector<Token> lexFile(const std::string &filename);
