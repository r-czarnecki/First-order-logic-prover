#include "CNF.h"
#include "term.h"

CNFType::CNFType(bool positive)
: positive(positive) {}
void CNFType::getConstants(std::set<std::string> &result) {}
void CNFType::getVariables(std::set<std::string> &result) {}
void CNFType::getFunctionSignatures(std::set<std::pair<std::string, int>> &result) {}
void CNFType::setPositivity(bool positive) {
    this->positive = positive;
}
bool CNFType::isPositive() {
    return positive;
}

void CNFType::renameConsts2(std::map<std::string, std::string> &mapping) {}


CNFConst::CNFConst(std::string name, bool positive)
: CNFType(positive)
, name(name) {}

std::string CNFConst::toString(bool includeNegation) {
    return (std::string)((positive || !includeNegation) ? "" : "~") + "Const_" + name;
}

void CNFConst::getConstants(std::set<std::string> &result) {
    result.insert(name);
} 

std::shared_ptr<CNFType> CNFConst::applyMapping(CNFMapping mapping) {
    return std::make_shared<CNFConst>(name, positive);
}

std::shared_ptr<CNFType> CNFConst::duplicate() {
    return std::make_shared<CNFConst>(name, positive);
}


CNFConst2::CNFConst2(std::string name, bool positive)
: CNFConst(name, positive) {}

std::string CNFConst2::toString(bool includeNegation) {
    return (std::string)((positive || !includeNegation) ? "" : "~") + "Const2_" + name;
}

void CNFConst2::getConstants(std::set<std::string> &result) {} 

std::shared_ptr<CNFType> CNFConst2::applyMapping(CNFMapping mapping) {
    return std::make_shared<CNFConst2>(name, positive);
}

std::shared_ptr<CNFType> CNFConst2::duplicate() {
    return std::make_shared<CNFConst2>(name, positive);
}

void CNFConst2::renameConsts2(std::map<std::string, std::string> &mapping) {
    if (mapping.count(name) != 0) name = mapping[name];
    else {
        auto newName = Term::getNewVarName();
        mapping[name] = newName;
        name = newName;
    }
}


CNFVar::CNFVar(std::string name, bool positive)
: CNFType(positive) 
, name(name) {}

std::string CNFVar::toString(bool includeNegation) {
    return (std::string)((positive || !includeNegation) ? "" : "~") + "Var_" + name;
}

void CNFVar::getVariables(std::set<std::string> &result) {
    result.insert(name);
} 

std::shared_ptr<CNFType> CNFVar::applyMapping(CNFMapping mapping) {
    auto res = mapping[name]->duplicate();
    res->setPositivity(positive);
    return res; 
}

std::shared_ptr<CNFType> CNFVar::duplicate() {
    return std::make_shared<CNFVar>(name, positive);
}


CNFTypeWithArgs::CNFTypeWithArgs(std::string name, CNFArgs args, bool positive)
: CNFType(positive)
, name(name)
, args(args) {}

void CNFTypeWithArgs::getConstants(std::set<std::string> &result) {
    for (auto &l : args) {
        l->getConstants(result);
    }
}

void CNFTypeWithArgs::getVariables(std::set<std::string> &result) {
    for (auto &l : args) {
        l->getVariables(result);
    }
}

void CNFTypeWithArgs::getFunctionSignatures(std::set<std::pair<std::string, int>> &result) {
    for (auto &l : args) {
        l->getFunctionSignatures(result);
    }
}


CNFRel::CNFRel(std::string name, CNFArgs args, bool positive)
: CNFTypeWithArgs(name, args, positive) {}

std::string CNFRel::toString(bool includeNegation) {
    auto res = (std::string)((positive || !includeNegation) ? "" : "~") + "Rel_" + name + " (";
    for (auto &l : args) {
        res += l->toString() + ", ";
    }

    if (res.back() != '(') {
        res.pop_back();
        res.pop_back();
    }

    res += ')';
    return res;
}

std::shared_ptr<CNFType> CNFRel::applyMapping(CNFMapping mapping) {
    CNFArgs newArgs;
    for (auto &l : args) {
        newArgs.push_back(l->applyMapping(mapping));
    }

    return std::make_shared<CNFRel>(name, newArgs, positive);
}

std::shared_ptr<CNFType> CNFRel::duplicate() {
    for (auto &l : args) {
        l = l->duplicate();
    }

    return std::make_shared<CNFRel>(name, args, positive);
}


CNFFun::CNFFun(std::string name, CNFArgs args, bool positive)
: CNFTypeWithArgs(name, args, positive) {}

std::string CNFFun::toString(bool includeNegation) {
    auto res = (std::string)((positive || !includeNegation) ? "" : "~") + "Fun_" + name + " (";
    for (auto &l : args) {
        res += l->toString() + ", ";
    }

    if (res.back() != '(') {
        res.pop_back();
        res.pop_back();
    }

    res += ')';
    return res;
}

void CNFFun::getFunctionSignatures(std::set<std::pair<std::string, int>> &result) {
    result.insert(std::make_pair(name, args.size()));

    CNFTypeWithArgs::getFunctionSignatures(result);
}

std::shared_ptr<CNFType> CNFFun::applyMapping(CNFMapping mapping) {
    CNFArgs newArgs;
    for (auto &l : args) {
        newArgs.push_back(l->applyMapping(mapping));
    }

    return std::make_shared<CNFFun>(name, newArgs, positive);
}

std::shared_ptr<CNFType> CNFFun::duplicate() {
    for (auto &l : args) {
        l = l->duplicate();
    }

    return std::make_shared<CNFFun>(name, args, positive);
}


std::string CNF::toString() {
    std::string res;
    for (auto &c : cnfFormula) {
        res += "[";
        for (auto &l : c) {
            res += l->toString() + ", ";
        }

        if (res.back() != '[') {
            res.pop_back();
            res.pop_back();
        }

        res += "]\n";
    }

    return res + "\n";
}

void CNF::addClause(Clause clause) {
    cnfFormula.push_back(clause);
}

std::vector<std::string> CNF::getConstants() {
    std::set<std::string> res;

    for (auto &c : cnfFormula) {
        for (auto &l : c) {
            l->getConstants(res);
        }
    }

    return setToVec(res);
}

std::vector<std::string> CNF::getVariables() {
    std::set<std::string> res;

    for (auto &c : cnfFormula) {
        for (auto &l : c) {
            l->getVariables(res);
        }
    }

    return setToVec(res);
}

std::vector<std::pair<std::string, int>> CNF::getFunctionSignatures() {
    std::set<std::pair<std::string, int>> res;

    for (auto &c : cnfFormula) {
        for (auto &l : c) {
            l->getFunctionSignatures(res);
        }
    }

    return setToVec(res);
}

CNF CNF::applyMapping(CNFMapping mapping) {
    CNF res;
    for (auto &c : cnfFormula) {
        Clause resClause;
        for (auto &l : c) {
            resClause.push_back(l->applyMapping(mapping));
        }
        res.addClause(resClause);
    }

    return res;
}

CNF::CNFFormula CNF::getFormula() {
    return cnfFormula;
}

void CNF::renameConsts2() {
    std::map<std::string, std::string> mapping;
    for (auto &c : cnfFormula) {
        for (auto &l : c) {
            l->renameConsts2(mapping);
        }
    }
}

CNF CNF::duplicate() {
    CNF res;

    for (auto &c : cnfFormula) {
        Clause clause;
        for (auto &l : c) {
            clause.push_back(l->duplicate());
        }
        res.addClause(clause);
    }

    return res;
}

void CNF::addCNF(CNF &cnf) {
    for (auto &c : cnf.cnfFormula) {
        addClause(c);
    }
}