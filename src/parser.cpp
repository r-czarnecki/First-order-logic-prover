#include <iostream>
#include <fstream>

#include "formula.h"
#include "parser.h"

Parser::Parser(std::string filename) {
    std::ifstream file(filename);
    std::getline(file, input);

    ptr = 0;
}

std::shared_ptr<Formula> Parser::getFormula(bool renameVars) {
    auto f = parseFormula(readUntil(" "));
    std::map<std::string, std::string> mapping;

    if (renameVars) {
        f->renameVariables(mapping);
    }

    return f;
}

std::string Parser::readUntil(std::string str) {
    int begin = ptr;

    for (; ptr < (int)input.length(); ptr++) {
        char ch = input[ptr];
        if (str.find(ch) != std::string::npos) {
            break;
        }
    }

    auto res = input.substr(begin, ptr - begin);
    ptr++;

    return res;
}

std::shared_ptr<Formula> Parser::parseFormula(std::string name) {
    if (name == "T") return parseFormula<True>();
    if (name == "F") return parseFormula<False>();
    if (name == "Rel") return parseFormula<Rel>();
    if (name == "Not") return parseFormula<Not>();
    if (name == "And") return parseFormula<And>();
    if (name == "Or") return parseFormula<Or>();
    if (name == "Implies") return parseFormula<Implies>();
    if (name == "Iff") return parseFormula<Iff>();
    if (name == "Exists") return parseFormula<Exists>();
    if (name == "Forall") return parseFormula<Forall>();
    return nullptr;
}

std::shared_ptr<Term> Parser::parseTerm(std::string name) {
    if (name == "Var") return parseTerm<Var>();
    if (name == "Fun") return parseTerm<Fun>();
    return nullptr;
}