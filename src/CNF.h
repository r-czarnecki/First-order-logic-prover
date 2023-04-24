#pragma once
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <map>


class CNFType {
public:
    CNFType(bool positive=true);
    virtual ~CNFType() = default;
    virtual std::string toString(bool includeNegation=true) = 0;
    virtual std::shared_ptr<CNFType> applyMapping(std::map<std::string, std::shared_ptr<CNFType>> mapping) = 0;
    virtual std::shared_ptr<CNFType> duplicate() = 0;

    virtual void getConstants(std::set<std::string> &result);
    virtual void getVariables(std::set<std::string> &result);
    virtual void getFunctionSignatures(std::set<std::pair<std::string, int>> &result); 
    virtual void setPositivity(bool positive); 
    virtual bool isPositive();
    virtual void renameConsts2(std::map<std::string, std::string> &mapping);
protected:
    bool positive;
};

using CNFArgs = std::vector<std::shared_ptr<CNFType>>;
using CNFMapping = std::map<std::string, std::shared_ptr<CNFType>>;

class CNFConst : public CNFType {
public:
    CNFConst(std::string name, bool positive=true);
    virtual ~CNFConst() = default;

    virtual std::string toString(bool includeNegation=true);
    virtual void getConstants(std::set<std::string> &result);
    virtual std::shared_ptr<CNFType> applyMapping(CNFMapping mapping);
    virtual std::shared_ptr<CNFType> duplicate();
protected:
    std::string name;
};

class CNFConst2 : public CNFConst {
public:
    CNFConst2(std::string name, bool positive=true);

    std::string toString(bool includeNegation=true);
    void getConstants(std::set<std::string> &result);
    std::shared_ptr<CNFType> applyMapping(CNFMapping mapping);
    std::shared_ptr<CNFType> duplicate();
    void renameConsts2(std::map<std::string, std::string> &mapping);
};

class CNFVar : public CNFType {
public:
    CNFVar(std::string name, bool positive=true);

    std::string toString(bool includeNegation=true);
    void getVariables(std::set<std::string> &result);
    std::shared_ptr<CNFType> applyMapping(CNFMapping mapping);
    std::shared_ptr<CNFType> duplicate();
private:
    std::string name;
};

class CNFTypeWithArgs : public CNFType {
public:
    CNFTypeWithArgs(std::string name, CNFArgs args, bool positive=true);
    virtual ~CNFTypeWithArgs() = default;
    virtual std::string toString(bool includeNegation=true) = 0;
    virtual std::shared_ptr<CNFType> duplicate() = 0;
    virtual std::shared_ptr<CNFType> applyMapping(CNFMapping mapping) = 0;

    virtual void getConstants(std::set<std::string> &result);
    virtual void getVariables(std::set<std::string> &result);
    virtual void getFunctionSignatures(std::set<std::pair<std::string, int>> &result); 
protected:
    std::string name;
    CNFArgs args;
};

class CNFRel : public CNFTypeWithArgs {
public:
    CNFRel(std::string name, CNFArgs args, bool positive=true);

    std::string toString(bool includeNegation=true);
    std::shared_ptr<CNFType> applyMapping(CNFMapping mapping);
    std::shared_ptr<CNFType> duplicate();
};

class CNFFun : public CNFTypeWithArgs {
public:
    CNFFun(std::string name, CNFArgs args, bool positive=true);

    std::string toString(bool includeNegation=true);
    void getFunctionSignatures(std::set<std::pair<std::string, int>> &result); 
    std::shared_ptr<CNFType> applyMapping(CNFMapping mapping);
    std::shared_ptr<CNFType> duplicate();
};


class CNF {
public:
    using Clause = std::vector<std::shared_ptr<CNFType>>;
    using CNFFormula = std::vector<Clause>;
    
    std::string toString();
    void addClause(Clause clause);
    std::vector<std::string> getConstants();
    std::vector<std::string> getVariables();
    std::vector<std::pair<std::string, int>> getFunctionSignatures(); 

    CNF applyMapping(CNFMapping mapping);
    CNFFormula getFormula();

    void renameConsts2();
    CNF duplicate();
    void addCNF(CNF &cnf);
private:
    CNFFormula cnfFormula;

    template<typename T>
    std::vector<T> setToVec(std::set<T> s) {
        std::vector<T> res;
        for (auto &elem : s) {
            res.push_back(elem);
        }

        return res;
    }
};