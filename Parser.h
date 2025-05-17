#pragma once

#include <vector>
#include <string>
#include <map>
#include "Tokenizer.h"
#include "tokenTypes/Identifier.h"
#include "tokenTypes/Literal.h"
#include "ASTNodes/InsertAST.h"
#include "ASTNodes/UpdateAST.h"
#include "ASTNodes/SelectAST.h"
#include "ASTNodes/DropAST.h"
#include "ASTNodes/DeleteAST.h"
#include "ASTNodes/CreateAST.h"
#include "ASTNodes/AlterAST.h"
#include "ASTNodes/DumpLoadAST.h"


class Parser {
public:
    Parser();
    std::unique_ptr<ASTNode> parse(const std::string& sql_stmt);

private:
    Tokenizer tokenizer;
    std::vector<Token> tokens;
    int pos = 0;
    std::string sql;
    std::map<std::string, std::string> OPERATOR_MAP;

    SelectAST parseSelect();
    InsertAST parseInsert();
    UpdateAST parseUpdate();
    DeleteAST parseDelete();
    CreateAST parseCreate();
    DropAST parseDrop();
    AlterAST parseAlter();
    DumpLoadAST parseDumpLoad();


    Token peek();
    Token advance();
    Token expect(const std::string& typeOrValue);

    std::vector<Literal> parseValueList();

    Literal parseLiteral();

    Identifier parseColumn();

    Identifier parseIdentifier(Identifier::Type type);

    std::vector<Identifier> parseIdentifierList(Identifier::Type type);

    std::vector<Identifier> parseTables();

    Identifier parseTable();

    std::vector<Identifier> parseColumns();

    std::string parseColumnType();

    std::vector<std::pair<Identifier, Literal>> parseAssignments();

    std::vector<std::pair<Identifier, std::string>> parseOrderBy();

    Condition parseExpression();

    Condition parseOr();

    Condition parseAnd();

    Condition parseNot();

    Condition parseComparison();

    Identifier parseQualifiedColumn();

    std::vector<Identifier> parseQualifiedColumns();
};
