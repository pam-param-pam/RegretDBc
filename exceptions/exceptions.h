#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include "../Token.h"

class RegretDBError : public std::runtime_error {
public:
    explicit RegretDBError(const std::string &msg);

    void setMessage(const std::string &msg);

    std::string getMessage();

    const char* what() const noexcept override;

protected:
    std::string message;
};

class SQLSyntaxError : public RegretDBError {

public:
    explicit SQLSyntaxError(const std::string &msg);

    std::string getPrettyError(const std::string &sql, const std::vector<Token> &tokens, int pos, int adjust_pos = 0);

};


class PreProcessorError : public RegretDBError {
public:
    explicit PreProcessorError(const std::string &msg);

    PreProcessorError(const std::string &msg, std::string word);

    std::string getPrettyError(const std::string &stmt) const;

private:
    std::string word;

};

class IntegrityError : public RegretDBError {
public:
    explicit IntegrityError(const std::string &msg);
};
