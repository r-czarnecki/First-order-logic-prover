#include "SNF.h"


SNF::SNF(std::shared_ptr<Formula> formula) 
: f(formula) {
    std::map<std::string, std::string> mapping;
    f->renameVariables(mapping);
    f = quantifyExistentially();
    f = f->NNF();
    f = createSkolemFunctions();
    f = removeUniversalQuant();
}

std::string SNF::toString() {
    return f->toString();
}

void SNF::getFreeVariables(std::set<std::string> &result) {
    f->getFreeVariables(result);
}

void SNF::renameVariables(std::map<std::string, std::string> &mapping) {
    f->renameVariables(mapping);
}

std::shared_ptr<Formula> SNF::negate() {
    return f->negate();
}

std::shared_ptr<Formula> SNF::NNF() {
    return f->NNF();
}

std::shared_ptr<Formula> SNF::createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) {
    return f->createSkolemFunctions(scope, mapping);
}

std::shared_ptr<Formula> SNF::removeUniversalQuant() {
    return f->removeUniversalQuant();
}

std::shared_ptr<Formula> SNF::quantifyExistentially() {
    std::shared_ptr<Formula> res = f;
    std::set<std::string> freeVariables;
    f->getFreeVariables(freeVariables);

    for (auto &var : freeVariables) {
        res = std::make_shared<Exists>(var, res);
    }

    return res;
}

std::shared_ptr<Formula> SNF::createSkolemFunctions() {
    std::vector<std::shared_ptr<Term>> scope;
    std::map<std::string, std::shared_ptr<Term>> mapping;
    return f->createSkolemFunctions(scope, mapping);
}

std::string SNF::toCNF(CNF &result) {
    auto newV = f->toCNF(result);
    result.addClause({std::make_shared<CNFConst2>(newV)});
    return newV;
}