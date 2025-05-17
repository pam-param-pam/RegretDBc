#pragma once

#include <string>
#include <vector>
#include "TypeHints.h"
#include "DataManager.h"
#include "fmt/core.h"

std::pair<std::string, std::string> splitColumn(const std::string &column);

void printRow(const TypeHints::Row &row);

void printTable(TypeHints::TableData data);

std::string typeToString(Literal::Type type);

void visualizeTableStructure(const std::string &table_name);

void trim(std::string &s);

void rtrim(std::string &s);

void ltrim(std::string &s);

std::string trim_copy(std::string s);