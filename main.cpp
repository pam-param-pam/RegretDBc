#include <iostream>
#include <string>
#include "RegretDB.h"
#include "utils.h"

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

                   Welcome to RegretDB
         Enter SQL commands or type exit to exit.
                        ~Pam

=============================================================

)";
}

int main() {
    RegretDB engine;
    printWelcomeBanner();
    std::cout << "Enter SQL commands (type 'exit' to quit):\n";

    std::string sql;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, sql);

        // Exit condition
        if (sql == "exit" || sql == "quit") {
            break;
        }

        try {
            engine.executeOrder66(sql);

        } catch (const std::exception &e) {
            std::cerr << "Error executing SQL: " << e.what() << "\n";
        }
    }

    std::cout << "Exiting...\n";
    return 0;
}
