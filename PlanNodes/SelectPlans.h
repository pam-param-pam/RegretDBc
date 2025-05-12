//
//#include <string>
//#include <vector>
//#include <memory>
//#include <map>
//#include <any>
//#include "../TypeHints.h"
//
//class PlanNode {
//public:
//    virtual ~PlanNode() = default;
//    [[nodiscard]] virtual std::string toString(int level) const = 0;
//    virtual TypeHints::TableData execute() = 0;
//};
//
//class TableScan : public PlanNode {
//public:
//    explicit TableScan(const std::string& table);
//    TypeHints::TableData execute() override;
//    [[nodiscard]] std::string toString(int level) const override;
//
//private:
//    std::string table;
//};
//
//class CrossJoin : public PlanNode {
//public:
//    CrossJoin(std::shared_ptr<PlanNode> left, std::shared_ptr<PlanNode> right);
//    TypeHints::TableData execute() override;
//    std::string toString(int level = 0) const override;
//
//private:
//    std::shared_ptr<PlanNode> left;
//    std::shared_ptr<PlanNode> right;
//};
//
//class Filter : public PlanNode {
//public:
//    Filter(std::shared_ptr<PlanNode> source, std::shared_ptr<Condition> condition);
//    TypeHints::TableData execute() override;
//    std::string toString(int level = 0) const override;
//
//private:
//    std::shared_ptr<PlanNode> source;
//    std::shared_ptr<Condition> condition;
//};
//
//
//class Project : public PlanNode {
//public:
//    Project(std::shared_ptr<PlanNode> source, const std::vector<std::string>& columns);
//    TypeHints::TableData execute() override;
//    std::string toString(int level = 0) const override;
//
//private:
//    std::shared_ptr<PlanNode> source;
//    std::vector<std::string> columns;
//};
//
//class Sort : public PlanNode {
//public:
//    Sort(std::shared_ptr<PlanNode> source, const std::vector<std::pair<std::string, std::string>>& orderBy);
//    TypeHints::TableData execute() override;
//    std::string toString(int level = 0) const override;
//
//private:
//    std::shared_ptr<PlanNode> source;
//    std::vector<std::pair<std::string, std::string>> orderBy;
//};
//
//class Visualize : public PlanNode {
//public:
//    explicit Visualize(std::shared_ptr<PlanNode> source);
//    TypeHints::TableData execute() override;
//    std::string toString(int level = 0) const override;
//
//private:
//    void visualizeTable();
//    std::shared_ptr<PlanNode> source;
//    std::vector<std::string> headers;
//    TypeHints::TableData data;
//};
//
