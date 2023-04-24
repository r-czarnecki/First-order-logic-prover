#pragma once
#include <map>
#include <memory>

#include "formula.h"

class Parser {
public:
    Parser(std::string filename);

    std::shared_ptr<Formula> getFormula(bool renameVars = false);
private:
    std::string input;
    int ptr;

    std::string readUntil(std::string str);

    std::shared_ptr<Formula> parseFormula(std::string name);
    std::shared_ptr<Term> parseTerm(std::string name);

    template<typename T>
    std::shared_ptr<Formula> parseFormula();
    
    template<typename T>
    std::shared_ptr<Term> parseTerm();
};

#include "parser.inl"