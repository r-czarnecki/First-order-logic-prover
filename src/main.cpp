#include <iostream>

#include "parser.h"
#include "SNF.h"
#include "CNF.h"
#include "herbrand.h"
#include "DPLL.h"


int main() {
    Parser p("input");
    std::shared_ptr<Formula> f = p.getFormula(true);
    f = f->negate();
    SNF snf(f);
    CNF cnf;

    snf.toCNF(cnf);

    CNF mainFormula;

    HerbrandUniverse herbrand(cnf);
    auto mapping = herbrand.getNextMapping();

    if (mapping.empty()) {
        DPLLSolver solver(cnf);
        printf("%d\n", !solver.isSatisfiable());
        return 0;
    }

    bool satisfiable = true;
    while (!mapping.empty()) {
        auto cnf2 = cnf.applyMapping(mapping);
        cnf2.renameConsts2();
        mainFormula.addCNF(cnf2);
        DPLLSolver solver(mainFormula);
        auto res = solver.isSatisfiable();
        if (!res) {
            satisfiable = false;
            break;
        }

        if (!mapping.empty()) mapping = herbrand.getNextMapping();
    }
    printf("%d\n", !satisfiable);
}