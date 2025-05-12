#include "../DataManager.h"
#include "DeletePlan.h"

DeletePlan::DeletePlan(std::string tableName )
        : tableName(std::move(tableName)) {}

void DeletePlan::execute() {

}



