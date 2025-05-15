#include "DropAST.h"
#include "DumpLoadAST.h"
#include <filesystem>

DumpLoadAST::DumpLoadAST(DumpLoadAST::Type type, std::string filePath)
    : type(type), filePath(std::move(filePath)) {}

void DumpLoadAST::performChecks() {
    if (type == Type::LOAD) {
        if (!std::filesystem::exists(filePath)) {
            throw IntegrityError("LOAD failed: File does not exist: " + filePath);
        }
    } else if (type == Type::DUMP) {
        if (std::filesystem::exists(filePath)) {
            throw IntegrityError("DUMP failed: File already exists: " + filePath);
        }
    } else {
        throw IntegrityError("Unable to match type in DumpLoadAST");
    }
}

const DumpLoadAST::Type& DumpLoadAST::getType() const {
    return type;
}
const std::string& DumpLoadAST::getFilePath() const {
    return filePath;
}

std::string DumpLoadAST::typeToString(DumpLoadAST::Type type1) {
    switch (type1) {
        case Type::DUMP:
            return "DUMP";
        case Type::LOAD:
            return "LOAD";
        default:
            throw IntegrityError("Unable to match type in DumpLoadAST");
    }
}

std::string DumpLoadAST::repr() const {
    return "DumpLoadAST(type=" + typeToString(type) + ", filePath=" + filePath + ")";
}
