#include "Parser.h"
#include "utils.h"


Parser::Parser() : pos(0) {}

Token Parser::peek() {
    if (pos < tokens.size()) {
        return tokens[pos];
    } else {
        return {"EOF", "", pos};  // Return special EOF token
    }
}

Token Parser::advance() {
    pos++;
    return peek();
}

Token Parser::expect(const std::string &typeOrValue) {
    Token token = peek();

    if (token.type == typeOrValue || token.value == typeOrValue) {
        advance();
        return token;
    } else {
        throw SQLSyntaxError("Expected '" + typeOrValue + "' but found: " + token.toString());
    }
}

std::unique_ptr<ASTNode> Parser::parse(const std::string &sql_stmt) {
    pos = 0;
    try {
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
        } else if (token.type == "DUMP" || token.type == "LOAD") {
            DumpLoadAST stmt = parseDumpLoad();
            ptr = std::make_unique<DumpLoadAST>(std::move(stmt));
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

    if (token.type == "NUMBER_VALUE") {
        advance();
        return {Literal::Type::NUMBER, std::stoi(token.value)};
    }
    if (token.type == "BOOLEAN_VALUE") {
        advance();
        return {Literal::Type::BOOLEAN, parseBoolean(token.value)};
    }
    if (token.type == "TEXT_VALUE") {
        advance();
        return {Literal::Type::TEXT, token.value};
    }
    if (token.type == "NULL") {
        advance();
        return {Literal::Type::NULL_VALUE, std::monostate{}};
    }

    throw SQLSyntaxError("Expected literal value, found " + token.toString());
}

Identifier Parser::parseQualifiedColumn() {
    auto identifier = expect("IDENTIFIER").value;
    if (peek().type == "DOT") {
        advance();
        std::string second_identifier = expect("IDENTIFIER").value;
        return {Identifier::Type::COLUMN, identifier + "." + second_identifier};
    }
    return {Identifier::Type::COLUMN, identifier};

}

std::vector<Identifier> Parser::parseQualifiedColumns() {
    std::vector<Identifier> ids;

    ids.push_back(parseQualifiedColumn());

    while (peek().type == "COMMA") {
        advance();  // Skip comma
        ids.push_back(parseQualifiedColumn());
    }
    return ids;

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

Identifier Parser::parseIdentifier(Identifier::Type type) {
    Token identifier_token = expect("IDENTIFIER");

    std::string identifier = identifier_token.value;

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

    for (const auto &type: {"TEXT", "NUMBER", "BOOLEAN"}) {
        if (peeked.type == type) {
            advance();
            return peeked.type;
        }
    }

    throw SQLSyntaxError("Expected column type (TEXT, NUMBER, BOOLEAN), found " + peeked.value);
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
        Identifier column = parseQualifiedColumn();

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

Condition Parser::parseExpression() {
    return parseOr(); // Lowest precedence
}

Condition Parser::parseOr() {
    Condition left = parseAnd();
    while (peek().type == "OR") {
        advance();
        Condition right = parseAnd();
        left = Condition(std::make_shared<OR>(std::make_shared<Condition>(left), std::make_shared<Condition>(right)));
    }
    return left;
}

Condition Parser::parseAnd() {
    Condition left = parseNot();
    while (peek().type == "AND") {
        advance();
        Condition right = parseNot();
        left = Condition(std::make_shared<AND>(std::make_shared<Condition>(left), std::make_shared<Condition>(right)));
    }
    return left;
}

Condition Parser::parseNot() {
    if (peek().type == "NOT") {
        advance();
        Condition operand = parseNot();
        return Condition(std::make_shared<NOT>(std::make_shared<Condition>(operand)));
    }
    return parseComparison();
}

Condition Parser::parseComparison() {
    if (peek().type == "LPAREN") {
        advance();
        Condition expr = parseExpression();
        expect("RPAREN");
        return expr;
    }

    Token token = peek();

    if (token.type == "BOOLEAN_VALUE") {
        advance();
        bool value = parseBoolean(token.value);
        return Condition(value);
    }

    Identifier left = parseQualifiedColumn();
    Token peeked = peek();

    if (peeked.type == "IS") {
        advance();
        if (peek().type == "NOT") {
            advance();
            expect("NULL");
            return Condition(std::make_shared<ISNOTNULL>(left.value));
        } else {
            expect("NULL");
            return Condition(std::make_shared<ISNULL>(left.value));
        }
    }

    if (peeked.type != "OP") {
        throw SQLSyntaxError("Expected comparison operator, found " + peeked.toString());
    }

    std::string op = peeked.value;
    advance(); // consume operator

    Literal rightLiteral = parseLiteral();
    auto comp = ComparisonOperator::fromLiteral(op, left, rightLiteral);
    return Condition(comp);
}

CreateAST Parser::parseCreate() {
    ///CREATE TABLE <table> (<columnName> <dataType> [, <columnName2> <dataType2> ...])
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
    ///SELECT *|<columns> FROM <table> [WHERE <condition>] [ORDER BY <column> ASC|DESC]
    expect("SELECT");

    std::vector<Identifier> columns;

    if (peek().type == "STAR") {
        advance();
        columns.emplace_back(Identifier::Type::COLUMN, "*");
    } else {
        columns = parseQualifiedColumns();
    }

    expect("FROM");

    std::vector<Identifier> tables = parseTables();

    std::optional<Condition> whereExpr;
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

    std::optional<Condition> whereExpr;
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

    std::optional<Condition> whereExpr;
    if (peek().type == "WHERE") {
        advance();
        whereExpr = parseExpression();
    }

    return {table, assignments, whereExpr};
}


DropAST Parser::parseDrop() {
    ///DROP TABLE <table>
    expect("DROP");
    expect("TABLE");

    Identifier table = parseTable();

    return DropAST(table);
}


AlterAST Parser::parseAlter() {
    ///ALTER TABLE <table>  [ADD COLUMN <columnName> <dataType>]
    ///                    | [DROP COLUMN <column>]
    ///                    | [RENAME COLUMN <oldName> TO <newName>]
    ///                    | [MODIFY COLUMN <column> <newDataType>]

    expect("ALTER");
    expect("TABLE");

    Identifier table = parseTable();


    // Check what operation to perform
    if (peek().type == "ADD") {
        advance();
        expect("COLUMN");

        auto column = parseColumn();

        std::string dataType = parseColumnType();

        return {AlterAST::Action::ADD, table, column, dataType};

    }
    if (peek().type == "DROP") {
        advance();
        expect("COLUMN");
        auto column = parseColumn();

        return {AlterAST::Action::DROP, table, column};

    }
    if (peek().type == "RENAME") {
        advance();
        expect("COLUMN");
        auto column = parseColumn();
        std::string newColumnName = expect("IDENTIFIER").value;

        return {AlterAST::Action::RENAME, table, column, newColumnName};

    }
    if (peek().type == "MODIFY") {
        advance();
        expect("COLUMN");
        auto column = parseColumn();
        std::string newType = parseColumnType();

        return {AlterAST::Action::MODIFY, table, column, newType};
    }

    throw SQLSyntaxError("Expected ADD, DROP, RENAME or MODIFY. Got: " + peek().value);
}

DumpLoadAST Parser::parseDumpLoad() {
    DumpLoadAST::Type type;
    if (peek().type == "LOAD") {
        type = DumpLoadAST::Type::LOAD;
    } else if (peek().type == "DUMP") {
        type = DumpLoadAST::Type::DUMP;
    } else {
        throw SQLSyntaxError("Expected LOAD or DUMP. Instead got: " + peek().value);
    }
    advance();

    // Merge tokens into a full path until we hit something that's not part of a file path
    std::string filePath;
    bool first = true;

    while (peek().type == "IDENTIFIER" || peek().value == "." || peek().value == "/" || peek().value == "\\" || peek().value == "-" || peek().value == "_" ||
           peek().value == ":") {
        if (!first) {
            filePath += "";
        }
        filePath += peek().value;
        advance();
        first = false;
    }

    if (filePath.empty()) {
        throw SQLSyntaxError("Expected file path.");
    }

    return DumpLoadAST(type, filePath);
}
