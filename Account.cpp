//
// Created by Sofia Castri on 25/09/25.
//

#include "Account.h"
#include <iostream>
#include <sstream>
#include <fstream>

Account::Account(const string &name, const string &iban, double balance) : name(name), iban(iban), balance(balance){}

// Getter
std::string Account::getName() const {
    return name;
}

std::string Account::getIban() const {
    return iban;
}

double Account::getBalance() const {
    return balance;
}


void Account::addTransaction(const Transaction& t) {
    transactions.push_back(t);
    if (t.getType() == TransactionType::Incoming) {
        balance += t.getAmount();
    } else {
        if (balance < t.getAmount()) {
            transactions.pop_back();//  saldo insufficiente: rimuoviamo la transazione appena aggiunta (annulliamo l'operazione)
            throw std::runtime_error("Saldo insufficiente per registrare la transazione");
        }
        balance -= t.getAmount();
    }
}



// Trasferimento tra conti
void Account::transferTo(double amount, Account& receiver, const std::string& description) {
    if (amount <= 0)
        throw std::invalid_argument("Importo deve essere positivo");
    if (balance < amount)
        throw std::runtime_error("Saldo insufficiente per trasferimento");

    int transactionId = rand() % 90000 + 10000; // ID casuale per la transazione

    Transaction senderT(transactionId, amount, TransactionType::Outgoing, description);
    Transaction receiverT(transactionId, amount, TransactionType::Incoming, description);

    addTransaction(senderT);        // aggiorna saldo mittente
    receiver.addTransaction(receiverT); // aggiorna saldo destinatario
}


vector<Transaction> Account::searchTransactionByType(TransactionType type) const {
    vector<Transaction> filteredTransactions;
    for (const auto& transaction : transactions) {
        if (transaction.getType() == type) {
            filteredTransactions.push_back(transaction);;
        }
    }
    return filteredTransactions;
}

double Account::getTotalIncoming() const {
    double total = 0.0;
    for (const auto& t : transactions) {
        if (t.getType() == TransactionType::Incoming) {
            total += t.getAmount(); // somma solo le entrate
        }
    }
    return total;
}

double Account::getTotalOutgoing() const {
    double total = 0.0;
    for (const auto& t : transactions) {
        if (t.getType() == TransactionType::Outgoing) {
            total += t.getAmount(); // somma solo le uscite
        }
    }
    return total;
}


void Account::writeTransactionsToFile(const std::string& filename) const {
    if (transactions.empty()) {
        std::cout << "Nessuna transazione da scrivere su file.\n";
        return;
    }

    for (const auto& t : transactions) {
        t.writeTransactionToFile(filename); // ogni transazione si salva da sola
    }

    std::cout << "Tutte le transazioni sono state salvate su file: " << filename << std::endl;
}


std::string Account::accountToString() const {
    std::ostringstream oss;
    oss << "Account:\n";
    oss << "Name: " << name << "\n";
    oss << "IBAN: " << iban << "\n";
    oss << "Balance: " << balance << "\n";

    // Stampa tutte le transazioni convertendole in stringa
    if (transactions.empty()) {
        oss << "No transactions.\n";
    } else {
        oss << "Transactions:\n";
        for (const auto& t : transactions) {
            oss << t.transactiontoString() << "\n";
        }
    }

    return oss.str();
}

void Account::writeAccountToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app); // apertura in modalità append
    if (!file.is_open()) {
        throw std::runtime_error("Impossibile aprire il file " + filename);
    }

    // Usa accountToString() per convertire in stringa e scrivere su file
    file << accountToString() << "\n=====================\n";
    file.close();
}

