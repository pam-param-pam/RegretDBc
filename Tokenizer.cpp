#include "Tokenizer.h"
#include <stdexcept>

Tokenizer::Tokenizer() {
    token_specification = {
            {"BOOLEAN", R"(\b[Tt][Rr][Uu][Ee]\b|\b[Ff][Aa][Ll][Ss][Ee]\b)"}, // This must be before identifiers
            {"IDENTIFIER", "[A-Za-z_][A-Za-z_0-9]*"},
            {"OP", "<=|>=|!=|=|<|>"},
            {"STAR", "\\*"},
            {"COMMA", ","},
            {"LPAREN", "\\("},
            {"RPAREN", "\\)"},
            {"SEMI", ";"},
            {"SKIP", R"([ \t\n\r]+)"}, // Skip whitespace
            {"DOT", "\\."},
            {"NUMBER", R"([-]?\b\d+(?:\.\d*)?)"},
            {"TEXT", "'([^']*)'"}, // Single-quoted string
//            {"BLOB", "b'([0-9A-Fa-f]+)'|x'([0-9A-Fa-f]+)'"}, // BLOB
            {"MISMATCH", "."} // Any other character
    };

    // Build the combined regex pattern
    std::string combined_pattern;
    for (const auto& spec : token_specification) {
        if (!combined_pattern.empty()) combined_pattern += "|";
        combined_pattern += "(" + spec.second + ")";
    }
    tok_regex = std::regex(combined_pattern);

    // SQL keywords and types
    keywords = {
            "SELECT", "FROM", "WHERE", "ORDER", "BY", "ASC", "DESC",
            "INSERT", "INTO", "VALUES",
            "UPDATE", "SET",
            "DELETE",
            "CREATE", "TABLE",
            "DROP",
            "ALTER", "ADD", "RENAME", "MODIFY", "CASCADE", "RESTRICT",
            "AND", "OR", "IS", "NOT", "NULL", "FALSE", "TRUE",
            "PRIMARY", "FOREIGN", "KEY", "UNIQUE", "DEFAULT",
            "TEXT", "NUMBER", "BLOB", "BOOL"
    };
}

std::vector<Token> Tokenizer::tokenize(const std::string& sql) {
    std::vector<Token> tokens;
    size_t pos = 0;

    // Loop over the input SQL string
    while (pos < sql.length()) {
        std::smatch match;
        if (!std::regex_search(sql.begin() + pos, sql.end(), match, tok_regex)) {
            throw SQLSyntaxError("Illegal character at position " + std::to_string(pos));
        }

        // Determine which matched token
        for (size_t i = 0; i < token_specification.size(); ++i) {
            if (match[i + 1].matched) {
                std::string token_value = match[i + 1].str();
                std::string token_type = token_specification[i].first;

                if (token_type == "TEXT") {
                    token_value = token_value.substr(1, token_value.length() - 2); // Remove quotes
                } else if (token_type == "IDENTIFIER") {
                    // Check if it's a keyword
                    std::string upper_val = token_value;
                    for (auto& ch : upper_val) ch = std::toupper(ch);
                    if (std::find(keywords.begin(), keywords.end(), upper_val) != keywords.end()) {
                        token_type = upper_val;
                    }
                }

                // Skip whitespace
                if (token_type == "SKIP") {
                    pos += match.position(0) + match.length(0);
                    continue;
                }

                tokens.emplace_back(token_type, token_value, pos + match.position(0));
                pos += match.position(0) + match.length(0);
                break;
            }
        }
    }
    return tokens;
}
