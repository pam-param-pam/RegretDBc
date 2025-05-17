#include "RegretDB.h"

void printWelcomeBanner() {
    std::cout << R"(
=============================================================
           ______                   _  ____________
          | ___ \                  | | |  _  \ ___ \
          | |_/ /___  __ _ _ __ ___| |_| | | | |_/ /
          |    // _ \/ _` | '__/ _ \ __| | | | ___ \
          | |\ \  __/ (_| | | |  __/ |_| |/ /| |_/ /
          \_| \_\___|\__, |_|  \___|\__|___/ \____/
                      __/ |
                     |___/

                   Welcome to RegretDB!
                          ~Pam
=============================================================

)";
}
///Baza danych przechowuje dane za pomocą singletona: DataManager. Są tam 2 kontenery:
///1) tableData: mapa gdzie klucz to nazwa tabeli a wartość to lista wierszy. Każdy wiersz jest mapą z nazwami kolumn jako kluczami
///2) columnTypes: mapa gdzie klucz to nazwa kolumny a wartość to typ kolumny

///Backup działa w bardzo prosty sposób. Wszystkie dane z bazy są zapisywane w formie zapytań sql w pliku. Który jest potem czytany by załadować dane z pliku.

///Wspierany język:
///CREATE TABLE <table> (<columnName> <dataType> [, <columnName2> <dataType2> ...])
///INSERT INTO <table> (<columns>) VALUES (<values>)
///SELECT *|<columns> FROM <table> [WHERE <condition>] [ORDER BY <column> ASC|DESC]
///DELETE FROM <table> [WHERE <condition>]
///UPDATE <table> SET <column>=<value> [, <column>=<value> ...] [WHERE <condition>]
///DROP TABLE <table>
///ALTER TABLE <table>  [ADD COLUMN <columnName> <dataType>]
///                    | [DROP COLUMN <column>]
///                    | [RENAME COLUMN <oldName> TO <newName>]
///                    | [MODIFY COLUMN <column> <newDataType>]
///LOAD filePath
///DUMP filePath

int main() {
    RegretDB engine;
    printWelcomeBanner();
    std::cout << "Enter SQL commands (type 'exit' to quit):\n";

    std::string sql;
    while (true) {
        std::cout << "RegretDB> ";
        std::getline(std::cin, sql);

        if (sql == "exit" || sql == "quit") {
            break;
        }

        try {
            engine.executeOrder66(sql);

        } catch (const RegretDBError &e) {
            std::cerr << "Error executing SQL: " << e.what() << "\n";
        }
    }

    std::cout << "Exiting...\n";
    return 0;
}
