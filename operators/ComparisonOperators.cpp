#include <memory>
#include "ComparisonOperators.h"
#include "../tokenTypes/Identifier.h"
////holds_alternative is a such a dumb name for checking type of variant...
////goofy ah code duplication


//// ------------ BASE CLASS ------------
std::optional<ComparisonOperator::Value> ComparisonOperator::resolve(const Row &row) const {
    auto it = row.find(columnName);
    if (it == row.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::pair<TypeHints::Value, TypeHints::Value> ComparisonOperator::resolveAndUnpackOrThrow(const Row &row) const {
    auto maybeValue = resolve(row);
    if (!maybeValue.has_value())
        throw IntegrityError("Unexpected: Column '" + columnName + "' not found in row");

    const auto &val = maybeValue.value();
    const auto &lit = literal.getValue();

    if (val.index() != lit.index() && literal.getType() != Literal::Type::NULL_VALUE)
        throw IntegrityError("Unexpected: Type mismatch between column '" + columnName + "' and literal");

    return {val, lit};
}

//// ------------ EQ ------------

EQ::EQ(const std::string &column, const Literal &literal)
        : ComparisonOperator(column, literal) {}

std::optional<bool> EQ::evaluate(const Row &row) const {

    auto values = resolveAndUnpackOrThrow(row);

    const auto &[val, litVal] = values;

    if (std::holds_alternative<int>(val) && std::holds_alternative<int>(litVal)) {
        return std::get<int>(val) == std::get<int>(litVal);
    }
    if (std::holds_alternative<std::string>(val) && std::holds_alternative<std::string>(litVal)) {
        return std::get<std::string>(val) == std::get<std::string>(litVal);
    }
    if (std::holds_alternative<bool>(val) && std::holds_alternative<bool>(litVal)) {
        return std::get<bool>(val) == std::get<bool>(litVal);
    }
    if (std::holds_alternative<std::monostate>(val) || std::holds_alternative<std::monostate>(litVal)) {
        return std::nullopt;
    }
    throw IntegrityError("Unexpected: unable to match values in EQ");

}

std::string EQ::toString() const {
    return "EQ(" + columnName + ", " + literal.toString() + ")";
}

//// ------------ NEQ ------------

NEQ::NEQ(const std::string &column, const Literal &literal)
        : ComparisonOperator(column, literal) {}

std::optional<bool> NEQ::evaluate(const Row &row) const {

    auto values = resolveAndUnpackOrThrow(row);

    const auto &[val, litVal] = values;

    if (std::holds_alternative<int>(val) && std::holds_alternative<int>(litVal)) {
        return std::get<int>(val) != std::get<int>(litVal);
    }
    if (std::holds_alternative<std::string>(val) && std::holds_alternative<std::string>(litVal)) {
        return std::get<std::string>(val) != std::get<std::string>(litVal);
    }
    if (std::holds_alternative<bool>(val) && std::holds_alternative<bool>(litVal)) {
        return std::get<bool>(val) == std::get<bool>(litVal);
    }
    if (std::holds_alternative<std::monostate>(val) || std::holds_alternative<std::monostate>(litVal)) {
        return std::nullopt;
    }

    throw IntegrityError("Unexpected: unable to match values in NEQ");
}

std::string NEQ::toString() const {
    return columnName + " != " + literal.toString();
}

//// ------------ LT ------------

LT::LT(const std::string &column, const Literal &literal)
        : ComparisonOperator(column, literal) {}

std::optional<bool> LT::evaluate(const Row &row) const {

    if (literal.getType() == Literal::Type::BOOLEAN) {
        throw IntegrityError("LT is not allowed for type BOOLEAN");
    }

    auto values = resolveAndUnpackOrThrow(row);

    const auto &[val, litVal] = values;

    if (std::holds_alternative<int>(val) && std::holds_alternative<int>(litVal)) {
        return std::get<int>(val) < std::get<int>(litVal);
    }
    if (std::holds_alternative<std::string>(val) && std::holds_alternative<std::string>(litVal)) {
        return std::get<std::string>(val) < std::get<std::string>(litVal);
    }
    if (std::holds_alternative<std::monostate>(val) || std::holds_alternative<std::monostate>(litVal)) {
        return std::nullopt;
    }

    throw IntegrityError("Unexpected: unable to match values in LT");
}

std::string LT::toString() const {
    return columnName + " < " + literal.toString();
}

////// ------------ LTE ------------

LTE::LTE(const std::string& column, const Literal& literal)
        : ComparisonOperator(column, literal) {}

std::optional<bool> LTE::evaluate(const Row& row) const {
    if (literal.getType() == Literal::Type::BOOLEAN) {
        throw IntegrityError("LTE is not allowed for type BOOLEAN");
    }

    auto values = resolveAndUnpackOrThrow(row);

    const auto &[val, litVal] = values;

    if (std::holds_alternative<int>(val) && std::holds_alternative<int>(litVal)) {
        return std::get<int>(val) <= std::get<int>(litVal);
    }
    if (std::holds_alternative<std::string>(val) && std::holds_alternative<std::string>(litVal)) {
        return std::get<std::string>(val) <= std::get<std::string>(litVal);
    }
    if (std::holds_alternative<std::monostate>(val) || std::holds_alternative<std::monostate>(litVal)) {
        return std::nullopt;
    }

    throw IntegrityError("Unexpected: unable to match values in LTE");
}

std::string LTE::toString() const {
    return columnName + " <= " + literal.toString();
}

//// ------------ GT ------------

GT::GT(const std::string& column, const Literal& literal)
        : ComparisonOperator(column, literal) {}

std::optional<bool> GT::evaluate(const Row& row) const {
    if (literal.getType() == Literal::Type::BOOLEAN) {
        throw IntegrityError("GT is not allowed for type BOOLEAN");
    }

    auto values = resolveAndUnpackOrThrow(row);

    const auto &[val, litVal] = values;

    if (std::holds_alternative<int>(val) && std::holds_alternative<int>(litVal)) {
        return std::get<int>(val) > std::get<int>(litVal);
    }
    if (std::holds_alternative<std::string>(val) && std::holds_alternative<std::string>(litVal)) {
        return std::get<std::string>(val) > std::get<std::string>(litVal);
    }
    if (std::holds_alternative<std::monostate>(val) || std::holds_alternative<std::monostate>(litVal)) {
        return std::nullopt;
    }

    throw IntegrityError("Unexpected: unable to match values in GT");
}

std::string GT::toString() const {
    return columnName + " > " + literal.toString();
}
std::shared_ptr<ComparisonOperator> ComparisonOperator::fromLiteral(const std::string& op, const Identifier& left, const Literal& right) {
    std::string column = left.value;
    if (op == "=") return std::make_shared<EQ>(column, right);
    if (op == "!=") return std::make_shared<NEQ>(column, right);
    if (op == ">") return std::make_shared<GT>(column, right);
    if (op == "<") return std::make_shared<LT>(column, right);
    if (op == ">=") return std::make_shared<GTE>(column, right);
    if (op == "<=") return std::make_shared<LTE>(column, right);

    throw std::invalid_argument("Unknown comparison operator: " + op);
}

//// ------------ GTE ------------

GTE::GTE(const std::string& column, const Literal& literal)
        : ComparisonOperator(column, literal) {}

std::optional<bool> GTE::evaluate(const Row& row) const {
    if (literal.getType() == Literal::Type::BOOLEAN) {
        throw IntegrityError("GTE is not allowed for type BOOLEAN");
    }

    auto values = resolveAndUnpackOrThrow(row);

    const auto &[val, litVal] = values;

    if (std::holds_alternative<int>(val) && std::holds_alternative<int>(litVal)) {
        return std::get<int>(val) >= std::get<int>(litVal);
    }
    if (std::holds_alternative<std::string>(val) && std::holds_alternative<std::string>(litVal)) {
        return std::get<std::string>(val) >= std::get<std::string>(litVal);
    }
    if (std::holds_alternative<std::monostate>(val) || std::holds_alternative<std::monostate>(litVal)) {
        return std::nullopt;
    }

    throw IntegrityError("Unexpected: unable to match values in LT");
}

std::string GTE::toString() const {
    return columnName + " >= " + literal.toString();
}