#pragma once

#include "CNF.h"

class DPLLSolver {
public:
    DPLLSolver(CNF &cnf);

    bool isSatisfiable();
private:  
    using Literal = std::pair<std::string, bool>;
    struct CNFState {
        int numberOfClauses;
        std::vector<int> literalsInClauses;
        std::vector<std::set<std::string>> clauseLiterals;
        std::map<std::string, std::set<int>> positiveClauses;
        std::map<std::string, std::set<int>> negativeClauses;
    };

    CNFState state;

    DPLLSolver(CNFState state);

    std::vector<Literal> getUnitClauses();
    Literal findLiteralInClause(int clause);
    void propagateUnit(Literal literal);
    void cleanRemovedClauses();

    std::vector<Literal> getPureLiterals();

    bool isEmpty();
    bool containsEmpty();

    Literal getMostFrequentLiteral();

    CNFState assign(std::string literal, bool val);

    std::string toString();
};