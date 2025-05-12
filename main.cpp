#include "Token.h"
#include "RegretDB.h"


int main() {
//
    RegretDB engine = RegretDB();
    std::string sql;
    sql = "CREATE TABLE users (id NUMBER, name TEXT, isCool BOOL)";
    engine.executeOrder66(sql);

    sql = "INSERT INTO users (name, id, isCool) VALUES ('Ash', 1, False)";
    engine.executeOrder66(sql);

    sql = "INSERT INTO users (name, id, isCool) VALUES ('Ashley', 2, NULL)";
    engine.executeOrder66(sql);

    sql = "DELETE FROM users where isCool is null";
    engine.executeOrder66(sql);

    sql = "SELECT * FROM users where isCool is not null"; //ORDER BY name asc
    engine.executeOrder66(sql);

}