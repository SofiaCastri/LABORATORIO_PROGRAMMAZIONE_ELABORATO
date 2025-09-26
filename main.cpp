#include <iostream>
#include <fstream>
#include "Account.h"
#include "Transaction.h"

int main() {

    try {
        std::string filename = "transazioni_main.txt";

        // Pulisco il file all’inizio del programma per evitare duplicati
        std::ofstream(filename, std::ios::trunc).close();

        // Creo alcune transazioni
        Transaction t1(1, 200.0, TransactionType::Incoming, "Stipendio");
        Transaction t2(2, 50.0, TransactionType::Outgoing, "Spesa alimentare");

        // Salvo le transazioni su file
        t1.writeTransactionToFile(filename);
        t2.writeTransactionToFile(filename);

        std::cout << "Transazioni salvate su file '" << filename << "'." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Errore: " << e.what() << std::endl;
    }

    return 0;
}