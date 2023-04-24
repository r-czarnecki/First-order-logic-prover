template<typename T>
inline std::shared_ptr<Formula> Parser::parseFormula() {
    return nullptr;
}

template<>
inline std::shared_ptr<Formula> Parser::parseFormula<True>() {
    return std::make_shared<True>();
}

template<>
inline std::shared_ptr<Formula> Parser::parseFormula<False>() {
    return std::make_shared<False>();
}

template<>
inline std::shared_ptr<Formula> Parser::parseFormula<Rel>() {
    readUntil("\"");
    std::string name = readUntil("\"");
    readUntil("[");

    std::vector<std::shared_ptr<Term>> terms;

    if (input[ptr] == ']') {
        ptr++;
        return std::make_shared<Rel>(name, terms);
    }
    
    while (input[ptr - 1] != ']') {
        std::string term = readUntil(" ");
        terms.push_back(parseTerm(term));

        readUntil(",]");
    }

    return std::make_shared<Rel>(name, terms);
}

template<>
inline std::shared_ptr<Formula> Parser::parseFormula<Not>() {
    readUntil("(");
    std::shared_ptr<Formula> f = getFormula();
    readUntil(")");
    return std::make_shared<Not>(f);
}

template<>
inline std::shared_ptr<Formula> Parser::parseFormula<And>() {
    readUntil("(");
    std::shared_ptr<Formula> f1 = getFormula();
    readUntil(")");

    readUntil("(");
    std::shared_ptr<Formula> f2 = getFormula();
    readUntil(")");

    return std::make_shared<And>(f1, f2);
}

template<>
inline std::shared_ptr<Formula> Parser::parseFormula<Or>() {
    readUntil("(");
    std::shared_ptr<Formula> f1 = getFormula();
    readUntil(")");

    readUntil("(");
    std::shared_ptr<Formula> f2 = getFormula();
    readUntil(")");

    return std::make_shared<Or>(f1, f2);
}

template<>
inline std::shared_ptr<Formula> Parser::parseFormula<Implies>() {
    readUntil("(");
    std::shared_ptr<Formula> f1 = getFormula();
    readUntil(")");

    readUntil("(");
    std::shared_ptr<Formula> f2 = getFormula();
    readUntil(")");

    return std::make_shared<Implies>(f1, f2);
}

template<>
inline std::shared_ptr<Formula> Parser::parseFormula<Iff>() {
    readUntil("(");
    std::shared_ptr<Formula> f1 = getFormula();
    readUntil(")");

    readUntil("(");
    std::shared_ptr<Formula> f2 = getFormula();
    readUntil(")");

    return std::make_shared<Iff>(f1, f2);
}

template<>
inline std::shared_ptr<Formula> Parser::parseFormula<Exists>() {
    readUntil("\"");
    std::string name = readUntil("\"");

    readUntil("(");
    std::shared_ptr<Formula> f = getFormula();
    readUntil(")");

    return std::make_shared<Exists>(name, f);
}

template<>
inline std::shared_ptr<Formula> Parser::parseFormula<Forall>() {
    readUntil("\"");
    std::string name = readUntil("\"");

    readUntil("(");
    std::shared_ptr<Formula> f = getFormula();
    readUntil(")");

    return std::make_shared<Forall>(name, f);
}


template<typename T>
inline std::shared_ptr<Term> Parser::parseTerm() {
    return nullptr;
}

template<>
inline std::shared_ptr<Term> Parser::parseTerm<Var>() {
    readUntil("\"");
    std::string name = readUntil("\"");

    return std::make_shared<Var>(name);
}


template<>
inline std::shared_ptr<Term> Parser::parseTerm<Fun>() {
    readUntil("\"");
    std::string name = readUntil("\"");
    readUntil("[");

    std::vector<std::shared_ptr<Term>> terms;

    if (input[ptr] == ']') {
        ptr++;
        return std::make_shared<Fun>(name, terms);
    }
    
    while (input[ptr - 1] != ']') {
        std::string term = readUntil(" ");
        terms.push_back(parseTerm(term));

        readUntil(",]");
    }

    return std::make_shared<Fun>(name, terms);
}
