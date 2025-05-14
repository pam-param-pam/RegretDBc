#pragma once

#include <string>
#include <vector>
#include "TypeHints.h"
#include "DataManager.h"
#include "fmt/core.h"

void printRow(const TypeHints::Row& row);

void printTable(TypeHints::TableData data);

std::string typeToString(Literal::Type type);

void visualizeTableStructure(const std::string& table_name);

