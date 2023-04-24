#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>

#include "term.h"
#include "CNF.h"

class Formula {
public:
    virtual ~Formula() = default; 
    virtual std::string toString() = 0;
    virtual void getFreeVariables(std::set<std::string> &result) = 0;
    virtual void renameVariables(std::map<std::string, std::string> &mapping) = 0;
    virtual std::shared_ptr<Formula> negate() = 0;
    virtual std::shared_ptr<Formula> NNF() = 0;
    virtual std::shared_ptr<Formula> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) = 0;
    virtual std::shared_ptr<Formula> removeUniversalQuant() = 0;
    virtual std::string toCNF(CNF &result) = 0;
};

class True : public Formula {
public:
    std::string toString();
    void getFreeVariables(std::set<std::string> &result);
    void renameVariables(std::map<std::string, std::string> &mapping);
    std::shared_ptr<Formula> negate();
    std::shared_ptr<Formula> NNF();
    std::shared_ptr<Formula> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping);
    std::shared_ptr<Formula> removeUniversalQuant();
    std::string toCNF(CNF &result);
};

class False : public Formula {
public: 
    std::string toString();
    void getFreeVariables(std::set<std::string> &result);
    void renameVariables(std::map<std::string, std::string> &mapping);
    std::shared_ptr<Formula> negate();
    std::shared_ptr<Formula> NNF();
    std::shared_ptr<Formula> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping);
    std::shared_ptr<Formula> removeUniversalQuant();
    std::string toCNF(CNF &result);
};

class Rel : public Formula {
public:
    Rel(std::string str, std::vector<std::shared_ptr<Term>> args);
    
    std::string toString();
    void getFreeVariables(std::set<std::string> &result);
    void renameVariables(std::map<std::string, std::string> &mapping);
    std::shared_ptr<Formula> negate();
    std::shared_ptr<Formula> NNF();
    std::shared_ptr<Formula> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping);
    std::shared_ptr<Formula> removeUniversalQuant();
    std::string toCNF(CNF &result);
private:
    std::string name;
    std::vector<std::shared_ptr<Term>> args;
};

class Not : public Formula {
public:
    Not(std::shared_ptr<Formula> f);
    
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
};

class TwoArgFormula : public Formula {
public:
    TwoArgFormula(std::shared_ptr<Formula> f1, std::shared_ptr<Formula> f2);
    virtual ~TwoArgFormula() = default;

    virtual std::shared_ptr<Formula> negate() = 0;
    virtual std::shared_ptr<Formula> NNF() = 0;
    virtual std::shared_ptr<Formula> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping) = 0;
    virtual std::shared_ptr<Formula> removeUniversalQuant() = 0;
    
    virtual std::string toString();
    virtual void getFreeVariables(std::set<std::string> &result);
    virtual void renameVariables(std::map<std::string, std::string> &mapping);
    virtual std::string toCNF(CNF &result);
protected:
    std::shared_ptr<Formula> f1, f2;
};

class And : public TwoArgFormula {
public:
    And(std::shared_ptr<Formula> f1, std::shared_ptr<Formula> f2);
    
    std::string toString();
    std::shared_ptr<Formula> negate();
    std::shared_ptr<Formula> NNF();
    std::shared_ptr<Formula> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping);
    std::shared_ptr<Formula> removeUniversalQuant();
    std::string toCNF(CNF &result);
};

class Or : public TwoArgFormula {
public:
    Or(std::shared_ptr<Formula> f1, std::shared_ptr<Formula> f2);
    
    std::string toString();
    std::shared_ptr<Formula> negate();
    std::shared_ptr<Formula> NNF();
    std::shared_ptr<Formula> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping);
    std::shared_ptr<Formula> removeUniversalQuant();
    std::string toCNF(CNF &result);
};

class Implies : public TwoArgFormula {
public:
    Implies(std::shared_ptr<Formula> f1, std::shared_ptr<Formula> f2);
    
    std::string toString();
    std::shared_ptr<Formula> negate();
    std::shared_ptr<Formula> NNF();
    std::shared_ptr<Formula> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping);
    std::shared_ptr<Formula> removeUniversalQuant();
};

class Iff : public TwoArgFormula {
public:
    Iff(std::shared_ptr<Formula> f1, std::shared_ptr<Formula> f2);
    
    std::string toString();
    std::shared_ptr<Formula> negate();
    std::shared_ptr<Formula> NNF();
    std::shared_ptr<Formula> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping);
    std::shared_ptr<Formula> removeUniversalQuant();
};

class Exists : public Formula {
public:
    Exists(std::string str, std::shared_ptr<Formula> f);
    
    std::string toString();
    void getFreeVariables(std::set<std::string> &result);
    void renameVariables(std::map<std::string, std::string> &mapping);
    std::shared_ptr<Formula> negate();
    std::shared_ptr<Formula> NNF();
    std::shared_ptr<Formula> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping);
    std::shared_ptr<Formula> removeUniversalQuant();
    std::string toCNF(CNF &result);
private:
    std::string name;
    std::shared_ptr<Formula> f;
};

class Forall : public Formula {
public:
    Forall(std::string str, std::shared_ptr<Formula> f);
    
    std::string toString();
    void getFreeVariables(std::set<std::string> &result);
    void renameVariables(std::map<std::string, std::string> &mapping);
    std::shared_ptr<Formula> negate();
    std::shared_ptr<Formula> NNF();
    std::shared_ptr<Formula> createSkolemFunctions(std::vector<std::shared_ptr<Term>> &scope, std::map<std::string, std::shared_ptr<Term>> &mapping);
    std::shared_ptr<Formula> removeUniversalQuant();
    std::string toCNF(CNF &result);
private:
    std::string name;
    std::shared_ptr<Formula> f;
};
