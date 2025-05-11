#pragma once

#include <any>
#include <memory>
#include <string>
#include <unordered_map>
#include <optional>
#include <type_traits>
#include <variant>

using Row = std::unordered_map<std::string, std::any>;
using Value = std::variant<std::monostate, bool, int, double, std::string>;

class Operator {
public:
    //alias
    using Ptr = std::shared_ptr<Operator>;

    virtual ~Operator() = default;

    explicit Operator(Ptr left = nullptr, Ptr right = nullptr)
            : left(std::move(left)), right(std::move(right)) {}

    [[nodiscard]] virtual std::optional<bool> execute(const Row& row) const = 0;

protected:
    Ptr left;
    Ptr right;

    template<typename T>
    std::optional<T> resolve(const std::any& operand) const {
        if (operand.type() == typeid(T))
            return std::any_cast<T>(operand);
        return std::nullopt;
    }

    std::any resolveOperand(const std::any& operand, const Row& row) const {
        if (operand.type() == typeid(std::string)) {
            std::string key = std::any_cast<std::string>(operand);
            auto it = row.find(key);
            if (it != row.end()) return it->second;
        }
        return operand;
    }

    template<typename T>
    std::optional<T> resolveTyped(const std::any& operand, const Row& row) const {
        std::any resolved = resolveOperand(operand, row);
        if (resolved.has_value() && resolved.type() == typeid(T)) {
            return std::any_cast<T>(resolved);
        }
        return std::nullopt;
    }
};

// BOOL
class BOOL : public Operator {
public:
    explicit BOOL(std::any value) : value(std::move(value)) {}

    std::optional<bool> execute(const Row& row) const override {
        auto strOpt = resolveTyped<std::string>(value, row);
        if (!strOpt.has_value()) return std::nullopt;
        std::string val = strOpt.value();
        for (auto& c : val) c = std::toupper(c);
        return val == "TRUE";
    }

private:
    std::any value;
};

// NOT
class NOT : public Operator {
public:
    explicit NOT(Ptr operand) : Operator(operand) {}

    std::optional<bool> execute(const Row& row) const override {
        auto result = left->execute(row);
        if (!result.has_value()) return std::nullopt;
        return !result.value();
    }
};

// AND
class AND : public Operator {
public:
    AND(Ptr left, Ptr right) : Operator(left, right) {}

    std::optional<bool> execute(const Row& row) const override {
        auto l = left->execute(row);
        auto r = right->execute(row);
        if (l == std::optional<bool>{false} || r == std::optional<bool>{false}) return false;
        if (!l.has_value() || !r.has_value()) return std::nullopt;
        return true;
    }
};

// OR
class OR : public Operator {
public:
    OR(Ptr left, Ptr right) : Operator(left, right) {}

    std::optional<bool> execute(const Row& row) const override {
        auto l = left->execute(row);
        auto r = right->execute(row);
        if (l == std::optional<bool>{true} || r == std::optional<bool>{true}) return true;
        if (!l.has_value() || !r.has_value()) return std::nullopt;
        return false;
    }
};

// Comparison Base
class Comparison : public Operator {
public:
    Comparison(std::any leftVal, std::any rightVal)
            : valLeft(std::move(leftVal)), valRight(std::move(rightVal)) {}

protected:
    std::any valLeft, valRight;
};

// GT
class GT : public Comparison {
public:
    using Comparison::Comparison;

    std::optional<bool> execute(const Row& row) const override {
        auto l = resolveTyped<double>(valLeft, row);
        auto r = resolveTyped<double>(valRight, row);
        if (!l.has_value() || !r.has_value()) return std::nullopt;
        return l.value() > r.value();
    }
};

// LT
class LT : public Comparison {
public:
    using Comparison::Comparison;

    std::optional<bool> execute(const Row& row) const override {
        auto l = resolveTyped<double>(valLeft, row);
        auto r = resolveTyped<double>(valRight, row);
        if (!l.has_value() || !r.has_value()) return std::nullopt;
        return l.value() < r.value();
    }
};

// EG (Equal)
class EG : public Comparison {
public:
    using Comparison::Comparison;

    [[nodiscard]] std::optional<bool> execute(const Row& row) const override {
        auto l = resolveOperand(valLeft, row);
        auto r = resolveOperand(valRight, row);

        if (l.type() != r.type()) return std::nullopt;

        // Handle comparison based on type
        if (l.type() == typeid(int)) {
            return std::any_cast<int>(l) == std::any_cast<int>(r);
        } else if (l.type() == typeid(double)) {
            return std::any_cast<double>(l) == std::any_cast<double>(r);
        } else if (l.type() == typeid(std::string)) {
            return std::any_cast<std::string>(l) == std::any_cast<std::string>(r);
        } else {
            return std::nullopt;  // Return nullopt for unsupported types todo
        }
    }
};

// NE (Not Equal)
class NE : public Comparison {
public:
    using Comparison::Comparison;

    [[nodiscard]] std::optional<bool> execute(const Row& row) const override {
        auto l = resolveOperand(valLeft, row);
        auto r = resolveOperand(valRight, row);
        if (l.type() != r.type()) return std::nullopt;
        return l != r;
    }
};

// IS_NULL
class IS_NULL : public Operator {
public:
    explicit IS_NULL(std::any val) : value(std::move(val)) {}

    [[nodiscard]] std::optional<bool> execute(const Row& row) const override {
        auto v = resolveOperand(value, row);
        return !v.has_value();
    }

private:
    std::any value;
};

// IS_NOT_NULL
class IS_NOT_NULL : public Operator {
public:
    explicit IS_NOT_NULL(std::any val) : value(std::move(val)) {}

    [[nodiscard]] std::optional<bool> execute(const Row& row) const override {
        auto v = resolveOperand(value, row);
        return v.has_value();
    }

private:
    std::any value;
};

