/*
 * I hate this language so much
 */

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


class Parser {
public:
    Parser();
    std::unique_ptr<ASTNode> parse(const std::string& sql_stmt);

private:
    SelectAST parseSelect();
    InsertAST parseInsert();
    UpdateAST parse_update();
    DeleteAST parse_delete();
    CreateAST parseCreate();
    DropAST parse_drop();
//    AlterStmt parse_alter();

    Token peek();
    Token advance();
    Token expect(const std::string& type_or_value);

    Tokenizer tokenizer;
    std::vector<Token> tokens;
    size_t pos = 0;
    std::string sql;
    std::map<std::string, std::string> OPERATOR_MAP;


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

    std::shared_ptr<Operand> parseExpression();

    std::shared_ptr<Operand> parseOr();

    std::shared_ptr<Operand> parseAnd();

    std::shared_ptr<Operand> parseNot();

    std::shared_ptr<Operand> parseComparison();
};
