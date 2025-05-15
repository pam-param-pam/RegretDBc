cmake_minimum_required(VERSION 3.25)
project(RegretDB)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         11.0.2
)

FetchContent_MakeAvailable(fmt)

add_executable(RegretDB
        main.cpp
        Parser.cpp
        Parser.h
        Tokenizer.h
        Tokenizer.cpp
        TokenTypes/Identifier.cpp
        TokenTypes/Identifier.h
        TokenTypes/Literal.cpp
        TokenTypes/Literal.h
        TokenTypes/Constraint.cpp
        TokenTypes/Constraint.h
        Token.cpp Token.h
        ASTNodes/ASTNodeBase.cpp ASTNodes/ASTNodeBase.h exceptions/exceptions.cpp exceptions/exceptions.h ASTNodes/SelectAST.cpp ASTNodes/SelectAST.h ASTNodes/InsertAST.cpp ASTNodes/InsertAST.h ASTNodes/DeleteAST.cpp ASTNodes/DeleteAST.h ASTNodes/DropAST.cpp ASTNodes/DropAST.h ASTNodes/UpdateAST.cpp ASTNodes/UpdateAST.h ASTNodes/CreateAST.cpp ASTNodes/CreateAST.h ExecutionPlanner.cpp ExecutionPlanner.h PlanNodes/PlanNodeBase.cpp PlanNodes/PlanNodeBase.h DataManager.cpp DataManager.h PlanNodes/CreateTablePlan.cpp PlanNodes/CreateTablePlan.h PlanNodes/InsertPlan.cpp PlanNodes/InsertPlan.h PlanNodes/SelectPlans.cpp PlanNodes/SelectPlans.h RegretDB.cpp RegretDB.h TypeHints.h PlanNodes/DeletePlan.cpp PlanNodes/DeletePlan.h operators/ComparisonOperators.cpp operators/ComparisonOperators.h operators/LogicalOperators.cpp operators/LogicalOperators.h operators/NullOperators.cpp operators/NullOperators.h operators/Operand.cpp operators/Operand.h ASTNodes/AlterAST.cpp ASTNodes/AlterAST.h PlanNodes/UpdatePlan.cpp PlanNodes/UpdatePlan.h utils.cpp PlanNodes/DropTablePlan.cpp PlanNodes/DropTablePlan.h utils.h PlanNodes/AlterTablePlan.cpp PlanNodes/AlterTablePlan.h)

target_link_libraries(RegretDB fmt)

