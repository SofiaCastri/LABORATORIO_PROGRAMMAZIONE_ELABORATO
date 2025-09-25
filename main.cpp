#include <iostream>
#include "Account.h"
#include "Transaction.h"
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {

    try {
        // Creo due conti
        Account acc1("Alice", "IT12345", 1000.0);
        Account acc2("Bob", "IT67890", 500.0);

        std::cout << "Saldo iniziale Alice: " << acc1.getBalance() << std::endl;
        std::cout << "Saldo iniziale Bob: " << acc2.getBalance() << std::endl;

        // Aggiungo una transazione manuale su Alice
        Transaction t1(1, 200.0, TransactionType::Incoming, "Stipendio");
        acc1.addTransaction(t1);

        std::cout << "Saldo Alice dopo transazione: " << acc1.getBalance() << std::endl;

        // Trasferimento da Alice a Bob
        acc1.transferTo(150.0, acc2, "Pagamento affitto");

        std::cout << "Saldo Alice dopo trasferimento: " << acc1.getBalance() << std::endl;
        std::cout << "Saldo Bob dopo trasferimento: " << acc2.getBalance() << std::endl;

        // Cerco le transazioni in entrata di Alice
        auto incomingAlice = acc1.searchTransactionByType(TransactionType::Incoming);
        std::cout << "Transazioni in entrata di Alice: " << incomingAlice.size() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Errore: " << e.what() << std::endl;
    }

    return 0;
}