#include "DPLL.h"


DPLLSolver::DPLLSolver(CNF &cnf) {
    auto f = cnf.getFormula();

    state.numberOfClauses = f.size();
    for (int i = 0; i < state.numberOfClauses; i++) {
        auto &c = f[i];
        state.literalsInClauses.push_back(0);
        state.clauseLiterals.push_back(std::set<std::string>());
        for (auto &l : c) {
            Literal lit = std::make_pair(l->toString(false), l->isPositive());
            
            if (state.positiveClauses.count(lit.first) == 0) {
                state.positiveClauses[lit.first] = std::set<int>();
                state.negativeClauses[lit.first] = std::set<int>();
            }

            if (lit.second) state.positiveClauses[lit.first].insert(i);
            else state.negativeClauses[lit.first].insert(i);

            state.clauseLiterals[i].insert(lit.first);
        }
    }

    for (auto &v : state.positiveClauses) {
        for (auto c : v.second) {
            state.literalsInClauses[c]++;
        }
    }

    for (auto &v : state.negativeClauses) {
        for (auto c : v.second) {
            state.literalsInClauses[c]++;
        }
    }
}

DPLLSolver::DPLLSolver(DPLLSolver::CNFState state)
: state(state) {}

bool DPLLSolver::isSatisfiable() {
    bool shouldContinue = true;
    while (shouldContinue) {
        shouldContinue = false;

        auto unit = getUnitClauses();
        while (!unit.empty()) {          
            for (auto &literal : unit) {
                propagateUnit(literal);
            }

            unit = getUnitClauses();
        }

        auto pure = getPureLiterals();
        while (!pure.empty()) {
            for (auto &literal : pure) {
                propagateUnit(literal);
            }

            shouldContinue = true;

            pure = getPureLiterals();
        }

        if (isEmpty()) return true;
        if (containsEmpty()) return false;
    }

    auto literal = getMostFrequentLiteral();
    DPLLSolver branch1(assign(literal.first, literal.second));
    if (branch1.isSatisfiable()) return true;
    else {
        DPLLSolver branch2(assign(literal.first, !literal.second));
        if (branch2.isSatisfiable()) return true;
        else return false;
    }
}

std::vector<DPLLSolver::Literal> DPLLSolver::getUnitClauses() {
    std::vector<Literal> res;

    for (int i = 0; i < (int)state.literalsInClauses.size(); i++) {
        if (state.literalsInClauses[i] == 1) {
            res.push_back(findLiteralInClause(i));
        }
    }

    return res;
}

DPLLSolver::Literal DPLLSolver::findLiteralInClause(int clause) {
    for (auto &v : state.positiveClauses) {
        if (v.second.count(clause) != 0) {
            return std::make_pair(v.first, true);
        }
    }

    for (auto &v : state.negativeClauses) {
        if (v.second.count(clause) != 0) {
            return std::make_pair(v.first, false);
        }
    }

    fprintf(stderr, "ERROR: No literal found\n");
    exit(1);
}

void DPLLSolver::propagateUnit(Literal literal) {
    auto &deleteClauses = (literal.second) ? state.positiveClauses : state.negativeClauses;
    auto &reduceClauses = (literal.second) ? state.negativeClauses : state.positiveClauses;

    for (auto clause : deleteClauses[literal.first]) {
        state.literalsInClauses[clause] = -2;
        state.numberOfClauses--;
        for (auto &l : state.clauseLiterals[clause]) {
            if (l == literal.first) continue;
            state.positiveClauses[l].erase(clause);
            state.negativeClauses[l].erase(clause);
        }
    }

    for (auto clause : reduceClauses[literal.first]) {
        state.literalsInClauses[clause]--;
        state.clauseLiterals[clause].erase(literal.first);
    }

    state.positiveClauses.erase(literal.first);
    state.negativeClauses.erase(literal.first);
}

void DPLLSolver::cleanRemovedClauses() {
    for (int i = 0; i < (int)state.literalsInClauses.size(); i++) {
        if (state.literalsInClauses[i] == -1) {
            state.literalsInClauses[i] = -2;
            for (auto &v : state.positiveClauses) {
                v.second.erase(i);
            }

            for (auto &v : state.negativeClauses) {
                v.second.erase(i);
            }
        }
    }
}

std::vector<DPLLSolver::Literal> DPLLSolver::getPureLiterals() {
    std::vector<Literal> res;

    for (auto &v : state.positiveClauses) {
        auto literal = v.first;
        if (state.positiveClauses[literal].empty() && !state.negativeClauses[literal].empty()) {
            res.push_back(std::make_pair(literal, false));
        }
        else if (!state.positiveClauses[literal].empty() && state.negativeClauses[literal].empty()) {
            res.push_back(std::make_pair(literal, true));
        }
    }

    return res;
}

bool DPLLSolver::isEmpty() {
    return state.numberOfClauses == 0;
}

bool DPLLSolver::containsEmpty() {
    for (int i = 0; i < (int)state.literalsInClauses.size(); i++) {
        if (state.literalsInClauses[i] == 0) {
            return true;
        }
    }

    return false;
}

DPLLSolver::Literal DPLLSolver::getMostFrequentLiteral() {
    Literal currentBest;
    int occurences = -1;

    for (auto &v : state.positiveClauses) {
        auto literal = v.first;
        int posOcc = state.positiveClauses[literal].size();
        int negOcc = state.negativeClauses[literal].size();
        int occ = posOcc + negOcc;
        if (occ > occurences) {
            currentBest = std::make_pair(literal, posOcc >= negOcc);
        }
    }

    return currentBest;
}

DPLLSolver::CNFState DPLLSolver::assign(std::string literal, bool val) {
    CNFState currentState = state;
    propagateUnit(std::make_pair(literal, val));
    CNFState res = state;
    state = currentState;
    return res;
}

std::string DPLLSolver::toString() {
    std::string res;
    std::vector<std::vector<std::string>> cnf(state.literalsInClauses.size(), std::vector<std::string>());
    for (auto &v : state.positiveClauses) {
        for (auto &c : v.second) {
            cnf[c].push_back(v.first);
        }
    }

    for (auto &v : state.negativeClauses) {
        for (auto &c : v.second) {
            cnf[c].push_back("~" + v.first);
        }
    }

    for (auto &c : cnf) {
        res += "[";
        for (auto &l : c) {
            res += l + ", ";
        }

        if (res.back() != '[') {
            res.pop_back();
            res.pop_back();
        }

        res += "]\n";
    }

    res += "Number of clauses: " + std::to_string(state.numberOfClauses) + "\n";
    for (int i = 0; i < (int)state.literalsInClauses.size(); i++) {
        res += std::to_string(state.literalsInClauses[i]) + " ";
    }
    res += "\n";

    return res;
}