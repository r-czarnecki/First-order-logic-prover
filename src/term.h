#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>

#include "CNF.h"

class Term {
public:
    virtual ~Term() = default;
    virtual std::string toString() = 0;
    virtual void getFreeVariables(std::set<std::string> &result) = 0;
    virtual std::shared_ptr<Term> renameVariables(std::map<std::string, std::string> &mapping) = 0;
    virtual std::shared_ptr<Term> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) = 0;
    virtual std::shared_ptr<CNFType> toCNF() = 0;

    static std::string getNewVarName();
private:
    static int varNum;
};

class Var : public Term {
public:
    Var(std::string str);

    std::string toString();
    void getFreeVariables(std::set<std::string> &result);
    std::shared_ptr<Term> renameVariables(std::map<std::string, std::string> &mapping);
    std::shared_ptr<Term> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping);
    std::shared_ptr<CNFType> toCNF();
private:
    std::string name;
};


class Fun : public Term {
public:
    Fun(std::string str, std::vector<std::shared_ptr<Term>> args);

    std::string toString();
    void getFreeVariables(std::set<std::string> &result);
    std::shared_ptr<Term> renameVariables(std::map<std::string, std::string> &mapping);
    std::shared_ptr<Term> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping);
    std::shared_ptr<CNFType> toCNF();
private:
    std::string name;
    std::vector<std::shared_ptr<Term>> args;
};