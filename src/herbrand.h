#pragma once
#include <map>

#include "CNF.h"

class Counter {
public:
    Counter();
    Counter(int n, int m);

    std::vector<int> getNext();
private:
    int n, m;
    std::vector<int> cnt;
};

class HerbrandUniverse {
public: 
    using Mapping = std::map<std::string, std::shared_ptr<CNFType>>;

    HerbrandUniverse(CNF &cnf);

    Mapping getNextMapping();
private:
    std::vector<std::shared_ptr<CNFType>> universe;
    std::vector<std::shared_ptr<CNFType>> prevUniverse;
    std::vector<std::string> vars;
    std::vector<std::pair<std::string, int>> functs; 
    Counter counter;
    int lastUniverseSize;

    bool isOldMapping(std::vector<int> &cnt);
    bool generateMore();
};