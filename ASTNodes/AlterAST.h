#pragma once

#include <string>
#include <vector>
#include "ASTNodeBase.h"

class AlterAST : public ASTNode {
public:
    enum class Action {
        ADD,
        DROP,
        RENAME,
        MODIFY,
    };

    AlterAST(Action action, Identifier table, Identifier column, std::string newValue);

    AlterAST(Action action, Identifier table, Identifier column);

    void performChecks() override;

    [[nodiscard]] const std::string &getQualifiedColumn() const;

    [[nodiscard]] const std::string &getNewValue() const;

    [[nodiscard]] const std::string &getTableName() const;

    [[nodiscard]] Action getAction() const;

    [[nodiscard]] std::string repr() const override;

    static std::string actionToString(Action action1);

private:
    std::string tableName;
    std::string qualifiedColumn;
    Action action;
    Identifier table;
    Identifier column;
    std::string newValue;

};
