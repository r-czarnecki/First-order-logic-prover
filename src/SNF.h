#pragma once
#include <memory>
#include "formula.h"

// Skolem Normal Form
class SNF : public Formula {
public:
    SNF(std::shared_ptr<Formula> formula);

    std::string toString();
    void getFreeVariables(std::set<std::string> &result);
    void renameVariables(std::map<std::string, std::string> &mapping);
    std::shared_ptr<Formula> negate();
    std::shared_ptr<Formula> NNF();
    std::shared_ptr<Formula> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping);
    std::shared_ptr<Formula> removeUniversalQuant();
    std::string toCNF(CNF &result);
private:
    std::shared_ptr<Formula> f;

    std::shared_ptr<Formula> quantifyExistentially();
    std::shared_ptr<Formula> createSkolemFunctions();
};