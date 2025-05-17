#include "Exceptions.h"

/// RegretDBError
RegretDBError::RegretDBError(const std::string &msg)
        : std::runtime_error(msg) {
}


/// PreProcessorError
PreProcessorError::PreProcessorError(const std::string &msg)
        : RegretDBError(msg) {}

PreProcessorError::PreProcessorError(const std::string &msg, const std::string &word)
        : RegretDBError(msg) {}

/// IntegrityError
IntegrityError::IntegrityError(const std::string &msg)
        : RegretDBError(msg) {}

/// SQLSyntaxError
SQLSyntaxError::SQLSyntaxError(const std::string &msg)
        : RegretDBError(msg) {
    message = msg;
}


const char *SQLSyntaxError::what() const noexcept {
    return message.c_str();
}

void SQLSyntaxError::setMessage(const std::string &msg) {
    message = msg;
}

std::string SQLSyntaxError::getMessage() {
    return message;
}

std::string SQLSyntaxError::getPrettyError(const std::string &sql, const std::vector<Token> &tokens, int pos, int adjust_pos) {
    int offset = 0;
    int token_length = 1;
    int adjust = 0;

    if (!tokens.empty()) {
        if (pos + adjust_pos < tokens.size()) {
            const Token &token = tokens[pos + adjust_pos];
            offset = token.position;
            token_length = token.value.size();
            if (token.type == "STRING") {
                adjust = 1;
            }
        } else {
            offset = sql.size();
            adjust = 1;
            token_length = 1;
        }

    } else {
        offset = adjust_pos;
    }


    std::string result = sql + "\n";
    result += std::string(offset + adjust, ' ');
    result += std::string(token_length, '^');
    return result;
}
