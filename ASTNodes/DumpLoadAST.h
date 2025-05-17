#pragma once

#include "ASTNodeBase.h"
#include "../tokenTypes/Identifier.h"

class DumpLoadAST : public ASTNode {
public:
    enum class Type {
        LOAD,
        DUMP,
    };

    explicit DumpLoadAST(Type type, std::string filePath);

    void performChecks() override;

    [[nodiscard]] std::string repr() const override;

    [[nodiscard]] const std::string &getFilePath() const;

    [[nodiscard]] const Type &getType() const;

    [[nodiscard]] static std::string typeToString(Type type1);

private:
    std::string filePath;
    Type type;
};
