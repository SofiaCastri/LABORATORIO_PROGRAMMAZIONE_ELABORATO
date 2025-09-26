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
    oss << "====================\n";
    oss << "Account Info\n";
    oss << "====================\n";
    oss << "Name: " << name << "\n";
    oss << "IBAN: " << iban << "\n";
    oss << "Balance: " << balance << "\n\n";

    if (transactions.empty()) {
        oss << "Nessuna transazione registrata.\n";
    } else {
        oss << "Transactions:\n";
        oss << "--------------------\n";
        for (const auto& t : transactions) {
            // indentiamo ogni transazione
            std::istringstream transStream(t.transactiontoString());
            std::string line;
            while (std::getline(transStream, line)) {
                oss << "  " << line << "\n";  // indentazione
            }
            oss << "--------------------\n";
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


void Account::loadTransactionsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Impossibile aprire il file " + filename);
    }

    std::string line;
    int currentId = 0;
    bool readingTransaction = false;

    while (std::getline(file, line)) {
        if (line.find("Transaction:") != std::string::npos) {
            readingTransaction = true;
            currentId = 0;
            continue;
        }

        if (readingTransaction && line.find("ID: ") != std::string::npos) {
            currentId = std::stoi(line.substr(4));

            // Controlla se esiste già
            bool exists = false;
            for (const auto& t : transactions) {
                if (t.getTransactionId() == currentId) {
                    exists = true;
                    break;
                }
            }

            // Se non esiste, leggila SUBITO
            if (!exists && currentId != 0) {
                try {
                    Transaction reader(0, 1.0, TransactionType::Incoming, "temp");
                    Transaction newTrans = reader.readTransactionFromFile(filename, currentId);
                    addTransaction(newTrans);
                    std::cout << "Aggiunta transazione ID: " << currentId << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Errore transazione " << currentId << ": " << e.what() << std::endl;
                }
            }
        }

        if (line.find("----------------------") != std::string::npos) {
            readingTransaction = false;
        }
    }

    file.close();
}