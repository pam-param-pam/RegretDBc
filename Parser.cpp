#include "Parser.h"
#include "tokenTypes/Identifier.h"
#include "fmt/base.h"
#include "tokenTypes/Literal.h"
#include <stdexcept>

using std::string;

bool parse_boolean(const std::string &token) {
    std::string lower_token = token;
    std::transform(lower_token.begin(), lower_token.end(), lower_token.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    if (lower_token == "true") {
        return true;
    }
    if (lower_token == "false") {
        return false;
    }

    throw std::invalid_argument("Invalid boolean string: " + token);
}

// Constructor
Parser::Parser() : pos(0) {
    OPERATOR_MAP = {
            {"=",  "EG"},
            {"!=", "NE"},
            {">",  "GT"},
            {"<",  "LT"},
            {">=", "GE"},
            {"<=", "LE"}
    };
}

// Helper function to peek at the next token
Token Parser::peek() {
    if (pos < tokens.size()) {
//        fmt::print("111111111111\n");
        return tokens[pos];
    } else {
        return Token("EOF", "", pos);  // Return special EOF token
    }
}

// Helper function to advance to the next token
Token Parser::advance() {
    pos++;
    return peek();
}

// Helper function to expect a specific token (by type or value)
Token Parser::expect(const string &type_or_value) {
    Token token = peek();

    if (token.type == type_or_value || token.value == type_or_value) {
        advance();
        return token;
    } else {
        throw SQLSyntaxError("Expected '" + type_or_value + "' but found '" + token.value + "'");
    }
}

// Main function to parse the SQL statement
std::unique_ptr<ASTNode> Parser::parse(const string &sql_stmt) {
    pos = 0;
    sql = sql_stmt;
    try {
    // Tokenize the SQL statement
    tokens = tokenizer.tokenize(sql_stmt);

    // Check for SQL statement type
    Token token = peek();
    if (token.type == "CREATE") {
        CreateAST stmt = parseCreate();
        auto ptr = std::make_unique<CreateAST>(std::move(stmt));
        return ptr;
    } else if (token.type == "INSERT") {
        InsertAST stmt = parseInsert();
        auto ptr = std::make_unique<InsertAST>(std::move(stmt));
        return ptr;
    } else if (token.type == "SELECT") {
        SelectAST stmt = parseSelect();
//    } else if (token.type == "UPDATE") {
//        UpdateStmt stmt = parse_update();
//        fmt::print(stmt.repr());
//    } else if (token.type == "DELETE") {
//        DeleteStmt stmt = parse_delete();
//        fmt::print(stmt.repr());
//    } else if (token.type == "DROP") {
//        DropStmt stmt = parse_drop();
        fmt::print(stmt.repr());
//    } else if (token.type == "ALTER") {
//        return parse_alter();
    } else {
        throw SQLSyntaxError("Unknown statement start: " + token.value);
    }

    // Check for leftover tokens
    if (peek().type != "EOF") {
        throw SQLSyntaxError("Unexpected token after end of statement: " + peek().value);
    }
    } catch (SQLSyntaxError& e) {
        e.setMessage(e.getMessage() + "\n" + e.getPrettyError(sql_stmt, tokens, pos));
        throw e;
    }
}

Literal Parser::parseLiteral() {
    Token token = peek();

    if (token.type == "NUMBER") {
        advance();
        return {Literal::Type::INTEGER, std::stoi(token.value)};
    } else if (token.type == "BOOLEAN") {
        advance();
        return {Literal::Type::BOOLEAN, parse_boolean(token.value)};
    } else if (token.type == "TEXT") {
        advance();
        return {Literal::Type::TEXT, token.value};
    } else {
        throw SQLSyntaxError("Expected literal value, found " + token.toString());
    }

}

Identifier Parser::parseColumn() {
    return parseIdentifier(Identifier::Type::COLUMN);
}

std::vector<Identifier> Parser::parseColumns() {
    return parseIdentifierList(Identifier::Type::COLUMN);
}

Identifier Parser::parseTable() {
    return parseIdentifier(Identifier::Type::TABLE);
}

std::vector<Identifier> Parser::parseTables() {
    return parseIdentifierList(Identifier::Type::TABLE);
}

// Helper method to parse identifiers (columns or tables)
Identifier Parser::parseIdentifier(Identifier::Type type) {
    Token identifier_token = expect("IDENTIFIER");

    std::string identifier = identifier_token.value;

    if (peek().type == "DOT") {
        advance();
        std::string second_identifier = expect("IDENTIFIER").value;
        return {type, identifier + "." + second_identifier};
    }
    return {type, identifier};
}

std::vector<Identifier> Parser::parseIdentifierList(Identifier::Type type) {
    std::vector<Identifier> ids;

    ids.push_back(parseIdentifier(type));

    while (peek().type == "COMMA") {
        advance();  // Skip comma
        ids.push_back(parseIdentifier(type));
    }
    return ids;
}

std::vector<Literal> Parser::parseValueList() {
    std::vector<Literal> values;

    Literal literal = parseLiteral();
    values.push_back(literal);

    while (peek().type == "COMMA") {
        advance();
        literal = parseLiteral();
        values.push_back(literal);
    }

    return values;
}

std::string Parser::parseColumnType() {
    Token peeked = peek();

    for (const auto &type: {"TEXT", "NUMBER", "BOOL"}) {
        if (peeked.value == type) {
            advance();
            return peeked.value;
        }
    }

    throw SQLSyntaxError("Expected column type (TEXT, NUMBER, BOOL), found " + peeked.value);
}

std::vector<std::pair<Identifier, Literal>> Parser::parseAssignments() {
    std::vector<std::pair<Identifier, Literal>> assignments;

    while (true) {
        Identifier column = parseColumn();
        expect("=");
        Literal literal = parseLiteral();

        assignments.emplace_back(column, literal);

        if (peek().type != "COMMA") {
            break;
        }

        advance(); // skip comma
    }

    return assignments;
}

CreateAST Parser::parseCreate() {
    /*CREATE TABLE <table_name> (<column_name> <data_type>, [, <column_name2> <data_type2> ...])*/
    expect("CREATE");
    expect("TABLE");
    Identifier table = parseTable();
    expect("(");
    std::vector<Identifier> columns;
    std::vector<Literal::Type> columnTypes;

    while (true) {
        columns.emplace_back(parseColumn());
        columnTypes.emplace_back(Literal::getTypeFromValue(parseColumnType()));

        if (peek().type != "COMMA") {
            break;
        }
        advance();
    }
    expect(")");

    return {table.value, columns, columnTypes};
}

SelectAST Parser::parseSelect() {
    /*SELECT <columns> FROM <table> [WHERE <expr>] [ORDER BY <column> ASC|DESC]*/

    expect("SELECT");

    std::vector<Identifier> columns;

    if (peek().type == "STAR") {
        advance();
        columns.emplace_back(Identifier::Type::COLUMN, "*");
    } else {
        columns = parseColumns();
    }

    expect("FROM");

    std::vector<Identifier> tables = parseTables();

    std::string where_expr;
//    if (peek().type == "WHERE") {
//        advance();
//        where_expr = parse_expression();
//    }

    std::string order_by;
//    if (peek().type == "ORDER") {
//    advance();
//        order_by = parse_order_by();
//    }

    return {columns, tables, where_expr, order_by};
}

InsertAST Parser::parseInsert() {
    /*INSERT INTO <table> (<columns>) VALUES (<values>)*/
    expect("INSERT");
    expect("INTO");

    Identifier table = parseTable();

    expect("(");
    std::vector<Identifier> columns = parseColumns();
    expect(")");

    expect("VALUES");

    expect("(");
    std::vector<Literal> values = parseValueList();
    expect(")");

    return {table, columns, values};

}

UpdateAST Parser::parse_update() {
    /*UPDATE <table> SET <column>=<value> [, <column>=<value> ...] [WHERE <condition>]*/
    expect("UPDATE");

    Identifier table = parseTable();

    expect("SET");

    std::vector<std::pair<Identifier, Literal>> assignments = parseAssignments();
    std::string where_expr;
//    if (peek().type == "WHERE") {
//        advance();  // Skip the "WHERE"
//        where_expr = parseExpression();  // You can leave this as a placeholder for expression parsing
//    }

    return {table, assignments, where_expr};
}

//DeleteStmt Parser::parse_delete() {
//    /*DELETE FROM <table> [WHERE <condition>]*/
//    return "Parsed DELETE";
//}
//
//
//
//DropStmt Parser::parse_drop() {
//    /*DROP TABLE <table_name> [, <table_name2> ...]*/
//    return "Parsed DROP";
//}

//string Parser::parse_alter() {
//    /*ALTER TABLE <table_name> [ADD COLUMN <column_name> <data_type> [<constraints>]]
//          | [DROP COLUMN <column_name>]
//          | [RENAME COLUMN <old_name> TO <new_name>]
//          | [MODIFY COLUMN <column_name> <new_data_type> [<constraints>]]*/
//    return "Parsed ALTER";
//}


