#include <sstream>
#include "Exceptions.h"
#include "vector"
#include "fmt/base.h"

/// RegretDBError
RegretDBError::RegretDBError(const std::string &msg)
        : std::runtime_error(msg) {
}


//// PreProcessorError
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

void SQLSyntaxError::attachContext(const std::string &sql_stmt, const std::vector<Token> &toks, size_t position) {
    sql = sql_stmt;
    tokens = toks;
    pos = position;
}

std::string SQLSyntaxError::getPrettyError(const std::string &sql, const std::vector<Token> &tokens, size_t pos, int adjust_pos) {
    size_t offset = 0;
    size_t token_length = 1;
    int adjust = 0;
//    fmt::println("pos {}", pos);
//    fmt::println("adjust_pos {}", adjust_pos);

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


//    fmt::println("offset {}", offset);
//    fmt::println("adjust {}", adjust);
//    fmt::println("token_length {}", token_length);

    std::ostringstream oss;
    oss << sql << "\n";
    oss << std::string(offset + adjust, ' ');
    oss << std::string(token_length, '^');
    return oss.str();
}