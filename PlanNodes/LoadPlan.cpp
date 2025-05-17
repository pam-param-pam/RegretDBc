#include <fstream>
#include "../DataManager.h"
#include "LoadPlan.h"
#include "../utils.h"
#include "../RegretDB.h"


LoadPlan::LoadPlan(std::string filePath)
        : filePath(std::move(filePath)) {}


void LoadPlan::execute() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw IntegrityError("Failed to open file for reading: " + filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string contents = buffer.str();
    auto &engine = RegretDB::getInstance();
    auto start = 0;
    while (true) {
        auto semicolonPos = contents.find(';', start);
        if (semicolonPos == std::string::npos) {

            std::string statement = trim_copy(contents.substr(start));
            if (!statement.empty()) {
                engine.executeOrder66(statement);

            }
            break;
        }

        std::string statement = trim_copy(contents.substr(start, semicolonPos - start));
        if (!statement.empty()) {
            engine.executeOrder66(statement);
        }

        start = semicolonPos + 1;
    }
}

TypeHints::TableData LoadPlan::getResult() const {
    throw IntegrityError("Do not call getResult() on LoadPlan");
}

std::string LoadPlan::toString(int level) const {
    std::string result = "LoadPlan(\n";
    result += std::string(level + 1, ' ') + "filePath='" + filePath + "',\n";
    result += "]\n" + std::string(level, ' ') + ")";
    return result;
}
