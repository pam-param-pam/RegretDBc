#include "Parser.h"
#include "tokenTypes/Identifier.h"
#include "fmt/base.h"
#include "tokenTypes/Literal.h"
#include "operators/Operand.h"
#include "ASTNodes/AlterAST.h"
#include <stdexcept>


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
Token Parser::expect(const std::string &type_or_value) {
    Token token = peek();

    if (token.type == type_or_value || token.value == type_or_value) {
        advance();
        return token;
    } else {
        throw SQLSyntaxError("Expected '" + type_or_value + "' but found '" + token.value + "'");
    }
}

// Main function to parse the SQL statement
std::unique_ptr<ASTNode> Parser::parse(const std::string &sql_stmt) {
    pos = 0;
    try {
        // Tokenize the SQL statement
        tokens = tokenizer.tokenize(sql_stmt);
        std::unique_ptr<ASTNode> ptr;

        Token token = peek();
        if (token.type == "CREATE") {
            CreateAST stmt = parseCreate();
            ptr = std::make_unique<CreateAST>(std::move(stmt));
        } else if (token.type == "INSERT") {
            InsertAST stmt = parseInsert();
            ptr = std::make_unique<InsertAST>(std::move(stmt));
        } else if (token.type == "SELECT") {
            SelectAST stmt = parseSelect();
            ptr = std::make_unique<SelectAST>(std::move(stmt));
        } else if (token.type == "DELETE") {
            DeleteAST stmt = parseDelete();
            ptr = std::make_unique<DeleteAST>(std::move(stmt));
        } else if (token.type == "UPDATE") {
            UpdateAST stmt = parseUpdate();
            ptr = std::make_unique<UpdateAST>(std::move(stmt));
        } else if (token.type == "DROP") {
            DropAST stmt = parseDrop();
            ptr = std::make_unique<DropAST>(std::move(stmt));
        } else if (token.type == "ALTER") {
            AlterAST stmt = parseAlter();
            ptr = std::make_unique<AlterAST>(std::move(stmt));

        } else {
            throw SQLSyntaxError("Unknown statement start: " + token.value);
        }

        // Check for leftover tokens
        if (peek().type != "EOF") {
            throw SQLSyntaxError("Unexpected token after end of statement: " + peek().value);
        }

        return ptr;
    } catch (SQLSyntaxError &e) {
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
    } else if (token.type == "NULL") {
        advance();
        return {Literal::Type::NULL_VALUE, std::monostate{}};
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
        if (peeked.type == type) {
            advance();
            return peeked.type;
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

std::vector<std::pair<Identifier, std::string>> Parser::parseOrderBy() {
    expect("BY");

    std::vector<std::pair<Identifier, std::string>> orderings;

    while (true) {
        Identifier column = parseColumn();

        Token directionToken = peek();
        if (directionToken.type != "ASC" && directionToken.type != "DESC") {
            throw SQLSyntaxError("Expected 'ASC' or 'DESC', found '" + directionToken.type + "'");
        }

        std::string direction = directionToken.type;
        advance();

        orderings.emplace_back(column, direction);

        if (peek().type != "COMMA") {
            break;
        }
        advance();
    }
    return orderings;
}

Operand Parser::parseExpression() {
    return parseOr(); // Lowest precedence
}

Operand Parser::parseOr() {
    Operand left = parseAnd();
    while (peek().type == "OR") {
        advance();
        Operand right = parseAnd();
        left = Operand(std::make_shared<OR>(std::make_shared<Operand>(left), std::make_shared<Operand>(right)));
    }
    return left;
}

Operand Parser::parseAnd() {
    Operand left = parseNot();
    while (peek().type == "AND") {
        advance();
        Operand right = parseNot();
        left = Operand(std::make_shared<AND>(std::make_shared<Operand>(left), std::make_shared<Operand>(right)));
    }
    return left;
}

Operand Parser::parseNot() {
    if (peek().type == "NOT") {
        advance();
        Operand operand = parseNot();
        return Operand(std::make_shared<NOT>(std::make_shared<Operand>(operand)));
    }
    return parseComparison();
}

Operand Parser::parseComparison() {
    if (peek().type == "LPAREN") {
        advance();
        Operand expr = parseExpression();
        expect("RPAREN");
        return expr;
    }

    Token token = peek();

    if (token.type == "BOOLEAN") {
        advance();
        bool value = parse_boolean(token.value);
        return Operand(value);
    }

    Identifier left = parseColumn();
    Token peeked = peek();

    if (peeked.type == "IS") {
        advance();
        if (peek().type == "NOT") {
            advance();
            expect("NULL");
            return Operand(std::make_shared<ISNOTNULL>(left.value));
        } else {
            expect("NULL");
            return Operand(std::make_shared<ISNULL>(left.value));
        }
    }

    if (peeked.type != "OP") {
        throw SQLSyntaxError("Expected comparison operator, found " + peeked.toString());
    }

    std::string op = peeked.type;
    advance(); // consume operator

    Literal rightLiteral = parseLiteral();
    auto comp = ComparisonOperator::fromLiteral(op, left, rightLiteral);
    return Operand(comp);
}

CreateAST Parser::parseCreate() {
    ///CREATE TABLE <table_name> (<column_name> <data_type>, [, <column_name2> <data_type2> ...])
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


InsertAST Parser::parseInsert() {
    ///INSERT INTO <table> (<columns>) VALUES (<values>)
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

SelectAST Parser::parseSelect() {
    ///SELECT <columns> FROM <table> [WHERE <expr>] [ORDER BY <column> ASC|DESC]

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

    std::optional<Operand> whereExpr;
    if (peek().type == "WHERE") {
        advance();
        whereExpr = parseExpression();
    }

    std::vector<std::pair<Identifier, std::string>> orderBy;
    if (peek().type == "ORDER") {
        advance();
        orderBy = parseOrderBy();
    }

    return {columns, tables, whereExpr, orderBy};
}

DeleteAST Parser::parseDelete() {
    ///DELETE FROM <table> [WHERE <condition>]
    expect("DELETE");
    expect("FROM");

    Identifier table = parseTable();

    std::optional<Operand> whereExpr;
    if (peek().type == "WHERE") {
        advance();
        whereExpr = parseExpression();
    }

    return {table, whereExpr};
}

UpdateAST Parser::parseUpdate() {
    ///UPDATE <table> SET <column>=<value> [, <column>=<value> ...] [WHERE <condition>]
    expect("UPDATE");

    Identifier table = parseTable();

    expect("SET");

    std::vector<std::pair<Identifier, Literal>> assignments = parseAssignments();

    std::optional<Operand> whereExpr;
    if (peek().type == "WHERE") {
        advance();
        whereExpr = parseExpression();
    }

    return {table, assignments, whereExpr};
}


DropAST Parser::parseDrop() {
    ///DROP TABLE <table_name>
    expect("DROP");
    expect("TABLE");

    Identifier table = parseTable();

    return DropAST(table);
}


AlterAST Parser::parseAlter() {
    ///ALTER TABLE <table_name>  [ADD COLUMN <column_name> <data_type>]
    ///                        | [DROP COLUMN <column_name>]
    ///                        | [RENAME COLUMN <old_name> TO <new_name>]
    ///                        | [MODIFY COLUMN <column_name> <new_data_type>

    expect("ALTER");
    expect("TABLE");

    Identifier table = parseTable();


    // Check what operation to perform
    if (peek().type == "ADD") {
        advance();
        expect("COLUMN");

        auto columnName = expect("IDENTIFIER").value;
        auto column = Identifier(Identifier::Type::COLUMN, columnName);

        std::string dataType = parseColumnType();

        return {AlterAST::Action::ADD, table, column, dataType};

    } else if (peek().type == "DROP") {
        advance();
        expect("COLUMN");

        auto columnName = expect("IDENTIFIER").value;
        auto column = Identifier(Identifier::Type::COLUMN, columnName);

        return {AlterAST::Action::DROP, table, column};

    } else if (peek().type == "RENAME") {
        advance();
        expect("COLUMN");

        auto columnName = expect("IDENTIFIER").value;
        auto column = Identifier(Identifier::Type::COLUMN, columnName);

        std::string newColumnName = expect("IDENTIFIER").value;

        return {AlterAST::Action::RENAME, table, column, newColumnName};

    } else if (peek().type == "MODIFY") {
        advance();
        expect("COLUMN");

        auto columnName = expect("IDENTIFIER").value;
        auto column = Identifier(Identifier::Type::COLUMN, columnName);

        std::string newType = parseColumnType();

        return {AlterAST::Action::MODIFY, table, column, newType};
    }

}