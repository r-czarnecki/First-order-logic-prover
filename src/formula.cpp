#include "formula.h"

std::string True::toString() {
    return "T";
}

void True::getFreeVariables(std::set<std::string> &result) {}

void True::renameVariables(std::map<std::string, std::string> &mapping) {}

std::shared_ptr<Formula> True::negate() {
    return std::make_shared<False>();
}

std::shared_ptr<Formula> True::NNF() {
    return std::make_shared<True>();
}

std::shared_ptr<Formula> True::createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) {
    return std::make_shared<True>();
}

std::shared_ptr<Formula> True::removeUniversalQuant() {
    return std::make_shared<True>();
}

std::string True::toCNF(CNF &result) {
    auto newV = Term::getNewVarName();
    result.addClause({std::make_shared<CNFConst2>(newV)});
    return newV;
}


std::string False::toString() {
    return "F";
}

void False::getFreeVariables(std::set<std::string> &result) {}

void False::renameVariables(std::map<std::string, std::string> &mapping) {}

std::shared_ptr<Formula> False::negate() {
    return std::make_shared<True>();
}

std::shared_ptr<Formula> False::NNF() {
    return std::make_shared<False>();
}

std::shared_ptr<Formula> False::createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) {
    return std::make_shared<False>();
}

std::shared_ptr<Formula> False::removeUniversalQuant() {
    return std::make_shared<False>();
}

std::string False::toCNF(CNF &result) {
    auto newV = Term::getNewVarName();
    result.addClause({std::make_shared<CNFConst2>(newV, false)});
    return newV;
}


Rel::Rel(std::string str, std::vector<std::shared_ptr<Term>> args)
: name(str)
, args(args) {}


std::string Rel::toString() {
    std::string res = "Rel \"" + name + "\" [";
    for (auto arg : args) {
        res += arg->toString() + ", ";
    }
    
    if (res.back() != '[') {
        res.pop_back();
        res.pop_back();
    }

    return res + "]";
}

void Rel::getFreeVariables(std::set<std::string> &result) {
    for (auto t : args) {
        t->getFreeVariables(result);
    }
}

void Rel::renameVariables(std::map<std::string, std::string> &mapping) {
    for (auto &t : args) {
        t = t->renameVariables(mapping);
    }
}

std::shared_ptr<Formula> Rel::negate() {
    return std::make_shared<Not>(std::make_shared<Rel>(name, args));
}

std::shared_ptr<Formula> Rel::NNF() {
    return std::make_shared<Rel>(name, args);
}

std::shared_ptr<Formula> Rel::createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) {
    for (auto &t : args) {
        t = t->createSkolemFunctions(scope, mapping);
    }

    return std::make_shared<Rel>(name, args);
}

std::shared_ptr<Formula> Rel::removeUniversalQuant() {
    return std::make_shared<Rel>(name, args);
}

std::string Rel::toCNF(CNF &result) {
    auto newV = Term::getNewVarName();
    CNFArgs cnfArgs;
    for (auto &t : args) {
        cnfArgs.push_back(t->toCNF());
    }

    result.addClause({
        std::make_shared<CNFConst2>(newV, false),
        std::make_shared<CNFRel>(name, cnfArgs)});
    
    result.addClause({
        std::make_shared<CNFConst2>(newV),
        std::make_shared<CNFRel>(name, cnfArgs, false)});

    return newV;
}


Not::Not(std::shared_ptr<Formula> f)
: f(f) {}

std::string Not::toString() {
    return "Not (" + f->toString() + ")";
}

void Not::getFreeVariables(std::set<std::string> &result) {
    f->getFreeVariables(result);
}

void Not::renameVariables(std::map<std::string, std::string> &mapping) {
    f->renameVariables(mapping);
}

std::shared_ptr<Formula> Not::negate() {
    return f->NNF();
}

std::shared_ptr<Formula> Not::NNF() {
    return f->negate();
}

std::shared_ptr<Formula> Not::createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) {
    return std::make_shared<Not>(f->createSkolemFunctions(scope, mapping));
}

std::shared_ptr<Formula> Not::removeUniversalQuant() {
    return std::make_shared<Not>(f->removeUniversalQuant());
}

std::string Not::toCNF(CNF &result) {
    auto newV = Term::getNewVarName();
    auto fV = f->toCNF(result);

    result.addClause({
        std::make_shared<CNFConst2>(newV),
        std::make_shared<CNFConst2>(fV)});

    result.addClause({
        std::make_shared<CNFConst2>(newV, false),
        std::make_shared<CNFConst2>(fV, false)});

    return newV;
}


TwoArgFormula::TwoArgFormula(std::shared_ptr<Formula> f1, std::shared_ptr<Formula> f2)
: f1(f1)
, f2(f2) {}

std::string TwoArgFormula::toString() {
    return "(" + f1->toString() + ") (" + f2->toString() + ")";
}

void TwoArgFormula::getFreeVariables(std::set<std::string> &result) {
    f1->getFreeVariables(result);
    f2->getFreeVariables(result);
}

void TwoArgFormula::renameVariables(std::map<std::string, std::string> &mapping) {
    f1->renameVariables(mapping);
    f2->renameVariables(mapping);
}

std::string TwoArgFormula::toCNF(CNF &result) { return ""; }


And::And(std::shared_ptr<Formula> f1, std::shared_ptr<Formula> f2)
: TwoArgFormula(f1, f2) {}

std::string And::toString() {
    return "And " + TwoArgFormula::toString();
}

std::shared_ptr<Formula> And::negate() {
    return std::make_shared<Or>(f1->negate(), f2->negate());
}

std::shared_ptr<Formula> And::NNF() {
    return std::make_shared<And>(f1->NNF(), f2->NNF());
}

std::shared_ptr<Formula> And::createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) {
    auto newF1 = f1->createSkolemFunctions(scope, mapping);
    auto newF2 = f2->createSkolemFunctions(scope, mapping);
    return std::make_shared<And>(newF1, newF2);   
}

std::shared_ptr<Formula> And::removeUniversalQuant() {
    return std::make_shared<And>(f1->removeUniversalQuant(), f2->removeUniversalQuant());
}

std::string And::toCNF(CNF &result) {
    auto newV = Term::getNewVarName();
    auto f1V = f1->toCNF(result);
    auto f2V = f2->toCNF(result);

    result.addClause({
        std::make_shared<CNFConst2>(newV, false),
        std::make_shared<CNFConst2>(f1V)});
        
    result.addClause({
        std::make_shared<CNFConst2>(newV, false),
        std::make_shared<CNFConst2>(f2V)});
        
    result.addClause({
        std::make_shared<CNFConst2>(newV),
        std::make_shared<CNFConst2>(f1V, false),
        std::make_shared<CNFConst2>(f2V, false)});

    return newV;
}


Or::Or(std::shared_ptr<Formula> f1, std::shared_ptr<Formula> f2)
: TwoArgFormula(f1, f2) {}

std::string Or::toString() {
    return "Or " + TwoArgFormula::toString();
}

std::shared_ptr<Formula> Or::negate() {
    return std::make_shared<And>(f1->negate(), f2->negate());
}

std::shared_ptr<Formula> Or::NNF() {
    return std::make_shared<Or>(f1->NNF(), f2->NNF());
}

std::shared_ptr<Formula> Or::createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) {
    auto newF1 = f1->createSkolemFunctions(scope, mapping);
    auto newF2 = f2->createSkolemFunctions(scope, mapping);
    return std::make_shared<Or>(newF1, newF2);     
}

std::shared_ptr<Formula> Or::removeUniversalQuant() {
    return std::make_shared<Or>(f1->removeUniversalQuant(), f2->removeUniversalQuant());
}

std::string Or::toCNF(CNF &result) {
    auto newV = Term::getNewVarName();
    auto f1V = f1->toCNF(result);
    auto f2V = f2->toCNF(result);

    result.addClause({
        std::make_shared<CNFConst2>(newV),
        std::make_shared<CNFConst2>(f1V, false)});
        
    result.addClause({
        std::make_shared<CNFConst2>(newV),
        std::make_shared<CNFConst2>(f2V, false)});
        
    result.addClause({
        std::make_shared<CNFConst2>(newV, false),
        std::make_shared<CNFConst2>(f1V),
        std::make_shared<CNFConst2>(f2V)});

    return newV;
}


Implies::Implies(std::shared_ptr<Formula> f1, std::shared_ptr<Formula> f2)
: TwoArgFormula(f1, f2) {}

std::string Implies::toString() {
    return "Implies " + TwoArgFormula::toString();
}

std::shared_ptr<Formula> Implies::negate() {
    return std::make_shared<And>(f1->NNF(), f2->negate());
}

std::shared_ptr<Formula> Implies::NNF() {
    return std::make_shared<Or>(f1->negate(), f2->NNF());
}

std::shared_ptr<Formula> Implies::createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) {
    auto newF1 = f1->createSkolemFunctions(scope, mapping);
    auto newF2 = f2->createSkolemFunctions(scope, mapping);
    return std::make_shared<Implies>(newF1, newF2);     
}

std::shared_ptr<Formula> Implies::removeUniversalQuant() {
    return std::make_shared<Implies>(f1->removeUniversalQuant(), f2->removeUniversalQuant());
}


Iff::Iff(std::shared_ptr<Formula> f1, std::shared_ptr<Formula> f2)
: TwoArgFormula(f1, f2) {}

std::string Iff::toString() {
    return "Iff " + TwoArgFormula::toString();
}

std::shared_ptr<Formula> Iff::negate() {
    auto newF1 = std::make_shared<Implies>(f1, f2);
    auto newF2 = std::make_shared<Implies>(f2, f1);
    return std::make_shared<And>(newF1, newF2)->negate();
}

std::shared_ptr<Formula> Iff::NNF() {
    auto newF1 = std::make_shared<Implies>(f1, f2);
    auto newF2 = std::make_shared<Implies>(f2, f1);
    return std::make_shared<And>(newF1, newF2)->NNF();
}

std::shared_ptr<Formula> Iff::createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) {
    auto newF1 = f1->createSkolemFunctions(scope, mapping);
    auto newF2 = f2->createSkolemFunctions(scope, mapping);
    return std::make_shared<Iff>(newF1, newF2);   
}

std::shared_ptr<Formula> Iff::removeUniversalQuant() {
    return std::make_shared<Iff>(f1->removeUniversalQuant(), f2->removeUniversalQuant());
}


Exists::Exists(std::string str, std::shared_ptr<Formula> f)
: name(str)
, f(f) {}

std::string Exists::toString() {
    return "Exists \"" + name + "\" (" + f->toString() + ")";
}

void Exists::getFreeVariables(std::set<std::string> &result) {
    f->getFreeVariables(result);
    result.erase(name);
}

void Exists::renameVariables(std::map<std::string, std::string> &mapping) {
    if (mapping.count(name) != 0) {
        auto prevMapping = mapping[name];
        auto newName = Term::getNewVarName();
        mapping[name] = newName;
        name = newName;
        f->renameVariables(mapping);
        mapping[name] = prevMapping;
    }
    else {
        auto newName = Term::getNewVarName();
        mapping[name] = newName;
        name = newName;
        f->renameVariables(mapping);
        mapping.erase(name);
    }
}

std::shared_ptr<Formula> Exists::negate() {
    return std::make_shared<Forall>(name, f->negate());
}

std::shared_ptr<Formula> Exists::NNF() {
    return std::make_shared<Exists>(name, f->NNF());
}

std::shared_ptr<Formula> Exists::createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) {
    mapping[name] = std::make_shared<Fun>(name, scope);
    return f->createSkolemFunctions(scope, mapping);
}

std::shared_ptr<Formula> Exists::removeUniversalQuant() {
    return std::make_shared<Exists>(name, f->removeUniversalQuant());
}

std::string Exists::toCNF(CNF &result) { return ""; }


Forall::Forall(std::string str, std::shared_ptr<Formula> f)
: name(str)
, f(f) {}

std::string Forall::toString() {
    return "Forall \"" + name + "\" (" + f->toString() + ")";
}

void Forall::getFreeVariables(std::set<std::string> &result) {
    f->getFreeVariables(result);
    result.erase(name);
}

void Forall::renameVariables(std::map<std::string, std::string> &mapping) {
    if (mapping.count(name) != 0) {
        auto prevMapping = mapping[name];
        auto newName = Term::getNewVarName();
        mapping[name] = newName;
        name = newName;
        f->renameVariables(mapping);
        mapping[name] = prevMapping;
    }
    else {
        auto newName = Term::getNewVarName();
        mapping[name] = newName;
        name = newName;
        f->renameVariables(mapping);
        mapping.erase(name);
    }
}

std::shared_ptr<Formula> Forall::negate() {
    return std::make_shared<Exists>(name, f->negate());
}

std::shared_ptr<Formula> Forall::NNF() {
    return std::make_shared<Forall>(name, f->NNF());
}

std::shared_ptr<Formula> Forall::createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) {
    scope.push_back(std::make_shared<Var>(name));
    auto res = f->createSkolemFunctions(scope, mapping);
    scope.pop_back();
    return std::make_shared<Forall>(name, res);
}

std::shared_ptr<Formula> Forall::removeUniversalQuant() {
    return f->removeUniversalQuant();
}

std::string Forall::toCNF(CNF &result) { return ""; }
