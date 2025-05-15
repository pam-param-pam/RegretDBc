#include "Tokenizer.h"
#include "fmt/base.h"
#include <stdexcept>
#include <iostream>

Tokenizer::Tokenizer() {
    token_specification = {
            {"BOOLEAN_VALUE", R"(\b[Tt][Rr][Uu][Ee]\b|\b[Ff][Aa][Ll][Ss][Ee]\b)"}, // This must be before identifiers
            {"IDENTIFIER", "[A-Za-z_][A-Za-z_0-9]*"},
            {"OP", "<=|>=|!=|=|<|>"},
            {"STAR", "\\*"},
            {"COMMA", ","},
            {"LPAREN", "\\("},
            {"RPAREN", "\\)"},
            {"SEMI", ";"},
            {"SKIP", R"([ \t\n\r]+)"}, // Skip whitespace
            {"DOT", "\\."},
            {"NUMBER_VALUE", R"([-]?\b\d+(?:\.\d*)?)"},
            {"TEXT_VALUE", R"('(?:[^']|'')*')"},
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
            "ALTER", "ADD", "RENAME", "MODIFY", "COLUMN",
            "AND", "OR", "IS", "NOT", "FALSE", "TRUE",
            "TEXT", "NUMBER", "BOOLEAN", "NULL",
            "LOAD", "DUMP",
            // "PRIMARY", "FOREIGN", "KEY", "UNIQUE", "DEFAULT", "CASCADE", "RESTRICT",     //no support for constraints atp

    };
}

std::vector<Token> Tokenizer::tokenize(const std::string& sql) {
    std::vector<Token> tokens;
    int pos = 0;

    // Loop over the input SQL string
    while (pos < sql.length()) {

        std::smatch match;

        if (!std::regex_search(sql.begin() + pos, sql.end(), match, tok_regex, std::regex_constants::match_continuous)) {
            throw SQLSyntaxError("Illegal character at position " + std::to_string(pos));
        }

        // Determine which matched token
        for (auto i = 0; i < token_specification.size(); ++i) {
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
