#include "herbrand.h"

Counter::Counter()
: Counter(1, 1) {}

Counter::Counter(int n, int m)
: n(n)
, m(m)
, cnt(n, 0) {
    if (n != 0) {
        cnt[0] = -1;
    }
}

std::vector<int> Counter::getNext() {
    for (int i = 0; i < n; i++) {
        if (cnt[i] == m - 1) {
            cnt[i] = 0;
        }
        else {
            cnt[i]++;
            return cnt; 
        }
    }

    return std::vector<int>();
}

HerbrandUniverse::HerbrandUniverse(CNF &cnf)
: universe()
, prevUniverse()
, vars(cnf.getVariables())
, functs(cnf.getFunctionSignatures())
, counter()
, lastUniverseSize(-1) {
    auto consts = cnf.getConstants();
    for (auto &name : consts) {
        universe.push_back(std::make_shared<CNFConst>(name));
    }

    if (universe.empty()) {
        universe.push_back(std::make_shared<CNFConst>("dummy"));
    }

    counter = Counter(vars.size(), universe.size());
    prevUniverse = universe;
}

HerbrandUniverse::Mapping HerbrandUniverse::getNextMapping() {
    auto nextCnt = counter.getNext();

    while (!nextCnt.empty() && isOldMapping(nextCnt)) {
        nextCnt = counter.getNext();
    }

    if (nextCnt.empty()) {
        lastUniverseSize = universe.size();
        if (generateMore()) {
            counter = Counter(vars.size(), universe.size());
            return getNextMapping();
        }
        else {
            return Mapping();
        }
    }
    else {
        Mapping res;
        for (int i = 0; i < (int)vars.size(); i++) {
            auto var = vars[i];
            res[var] = universe[nextCnt[i]];
        }

        return res;
    }
}

bool HerbrandUniverse::isOldMapping(std::vector<int> &cnt) {
    for (int c : cnt) {
        if (c >= lastUniverseSize) {
            return false;
        }
    }

    return true;
}

bool HerbrandUniverse::generateMore() {
    std::vector<std::shared_ptr<CNFType>> newUniverse;

    for (auto &f : functs) {
        auto fName = f.first;
        auto arity = f.second;

        if (arity == 0) continue;

        Counter cnt = Counter(arity, prevUniverse.size());
        auto nextCnt = cnt.getNext();

        while (!nextCnt.empty()) {
            std::vector<std::shared_ptr<CNFType>> args;
            for (int i = 0; i < arity; i++) {
                args.push_back(prevUniverse[nextCnt[i]]);
            }

            newUniverse.push_back(std::make_shared<CNFFun>(fName, args));
            nextCnt = cnt.getNext();
        }
    }

    prevUniverse = newUniverse;
    universe.insert(universe.end(), newUniverse.begin(), newUniverse.end());

    return !newUniverse.empty();
}