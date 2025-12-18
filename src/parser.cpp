#include "bolt/parser.h"

PrintNode::PrintNode(std::unique_ptr<ASTNode> e)
    : expr(std::move(e)) {}

Parser::Parser(const std::vector<Token>& toks) : tokens(toks), index(0) {}

std::unique_ptr<ASTNode> Parser::parse() {
    return parseStatement();
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (index >= tokens.size()) return nullptr;
    if (tokens[index].type == Tok_Prnt) {
        ++index;
        if (index >= tokens.size() || tokens[index].type != Tok_LParen) return nullptr;
        ++index;
        std::unique_ptr<ASTNode> expr = nullptr;
        if (index < tokens.size() && tokens[index].type == Tok_Number) {
            int val = std::stoi(tokens[index].text);
            expr = std::make_unique<NumberNode>(val);
            ++index;
        }
        if (index >= tokens.size() || tokens[index].type != Tok_RParen) return nullptr;
        ++index;
        return std::make_unique<PrintNode>(std::move(expr));
    }
    return nullptr;
}

