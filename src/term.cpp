#include "term.h"

int Term::varNum = 0;

std::string Term::getNewVarName() {
    auto res = "V" + std::to_string(varNum);
    varNum++;
    return res;
}


Var::Var(std::string str)
: name(str) {};

std::string Var::toString() {
    return "Var \"" + name + "\"";
}

void Var::getFreeVariables(std::set<std::string> &result) {
    result.insert(name);
}

std::shared_ptr<Term> Var::renameVariables(std::map<std::string, std::string> &mapping) {
    if (mapping.count(name) != 0) {
        return std::make_shared<Var>(mapping[name]);
    }
    else {
        auto newName = getNewVarName();
        mapping[name] = newName; 
        return std::make_shared<Var>(newName);
    }
}

std::shared_ptr<Term> Var::createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) {
    if (mapping.count(name) == 0) {
        return std::make_shared<Var>(name);
    } 
    else {
        return mapping[name];
    }
}

std::shared_ptr<CNFType> Var::toCNF() {
    return std::make_shared<CNFVar>(name);
}


Fun::Fun(std::string str, std::vector<std::shared_ptr<Term>> args)
: name(str)
, args(args) {}

std::string Fun::toString() {
    std::string res = "Fun \"" + name + "\" [";
    for (auto arg : args) {
        res += arg->toString() + ", ";
    }
    
    if (res.back() != '[') {
        res.pop_back();
        res.pop_back();
    }

    return res + "]";
}

void Fun::getFreeVariables(std::set<std::string> &result) {
    for (auto t : args) {
        t->getFreeVariables(result);
    }
}

std::shared_ptr<Term> Fun::renameVariables(std::map<std::string, std::string> &mapping) {
    std::vector<std::shared_ptr<Term>> newArgs;
    for (auto &t : args) {
        newArgs.push_back(t->renameVariables(mapping));
    }

    return std::make_shared<Fun>(name, newArgs);
}

std::shared_ptr<Term> Fun::createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) {
    for (auto &t : args) {
        t = t->createSkolemFunctions(scope, mapping);
    }

    return std::make_shared<Fun>(name, args);
}

std::shared_ptr<CNFType> Fun::toCNF() {
    if (args.size() == 0) return std::make_shared<CNFConst>(name);
    else {
        CNFArgs cnfArgs;
        for (auto &t : args) {
            cnfArgs.push_back(t->toCNF());
        }

        return std::make_shared<CNFFun>(name, cnfArgs);
    }
}