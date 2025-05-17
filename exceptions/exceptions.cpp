#include "Exceptions.h"

/// RegretDBError
RegretDBError::RegretDBError(const std::string &msg)
        : std::runtime_error(msg) {
    message = msg;

}
const char *RegretDBError::what() const noexcept {
    return message.c_str();
}

void RegretDBError::setMessage(const std::string &msg) {
    message = msg;
}

std::string RegretDBError::getMessage() {
    return message;
}

/// PreProcessorError
PreProcessorError::PreProcessorError(const std::string &msg)
        : RegretDBError(msg) {}

PreProcessorError::PreProcessorError(const std::string &msg, std::string word)
        : RegretDBError(msg), word(std::move(word)) {}

std::string PreProcessorError::getPrettyError(const std::string &sqlStmt) const {

    if (word.empty() || sqlStmt.empty()) {
        return message;
    }

    std::string underline = std::string(sqlStmt.size(), ' ');
    size_t wordLen = word.size();
    size_t idx = 0;

    while ((idx = sqlStmt.find(word, idx)) != std::string::npos) {
        char before = (idx > 0) ? sqlStmt[idx - 1] : '\0';
        char after = (idx + wordLen < sqlStmt.size()) ? sqlStmt[idx + wordLen] : '\0';

        // Skip if surrounded by single quotes (string literal)
        if (before == '\'' && after == '\'') {
            idx += wordLen;
            continue;
        }

        // Skip if part of a longer identifier or dotted identifier
        if ((isalnum(before) || before == '.') || (isalnum(after) || after == '.')) {
            idx += wordLen;
            continue;
        }

        for (auto i = 0; i < wordLen && (idx + i < underline.size()); ++i) {
            underline[idx + i] = '^';
        }

        idx += wordLen;
    }

    return message + "\n" + sqlStmt + "\n" + underline;
}

/// IntegrityError
IntegrityError::IntegrityError(const std::string &msg)
        : RegretDBError(msg) {}

/// SQLSyntaxError
SQLSyntaxError::SQLSyntaxError(const std::string &msg)
        : RegretDBError(msg) {}


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
