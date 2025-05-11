#pragma once

#include <string>
#include <vector>
#include <regex>
#include "Token.h"
#include "exceptions/exceptions.h"



// Tokenizer class that splits SQL strings into tokens
class Tokenizer {
private:
    std::vector<std::pair<std::string, std::string>> token_specification;
    std::vector<std::string> keywords;
    std::regex tok_regex;

public:
    Tokenizer();
    std::vector<Token> tokenize(const std::string& sql);
};

