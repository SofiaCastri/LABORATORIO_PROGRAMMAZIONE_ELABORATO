//
// Created by Sofia Castri on 25/09/25.
//

#include "Account.h"
#include <iostream>
#include <sstream>
#include <fstream>

Account::Account(const string &name, const string &iban, double balance) : name(name), iban(iban), balance(balance){
    if (balance < 0.0) {
        throw std::invalid_argument("Il saldo iniziale non può essere negativo.");
    }
}

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

int Account::getTransactionSize() const {
    return static_cast<int>(transactions.size());
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
            filteredTransactions.push_back(transaction);
        }
    }
    return filteredTransactions;
}

std::vector<Transaction> Account::findTransactionsBeforeDate(const std::string& targetDate) const {
    //Controllo che il formato della data sia corretto (es. "2025-09-26 00:00:00")
    std::tm target_tm = {};
    std::istringstream ss(targetDate);
    ss >> std::get_time(&target_tm, "%Y-%m-%d %H:%M:%S");

    if (ss.fail() || mktime(&target_tm) == -1) {
        throw std::invalid_argument("Formato della data non valido. Usa il formato: YYYY-MM-DD HH:MM:SS");
    }

    time_t target_time = mktime(&target_tm); // converte stringa data in un numero per poterlo confrontare

    std::vector<Transaction> results;

    for (const auto& t : transactions) {
        std::tm trans_tm = {};
        std::istringstream ts(t.getDate());
        ts >> std::get_time(&trans_tm, "%Y-%m-%d %H:%M:%S");

        time_t trans_time = mktime(&trans_tm); // converte stringa data in un numero per poterlo confrontare

        if (trans_time < target_time) {
            results.push_back(t);
        }

    }

    return results;
}

double Account::getTotalIncoming() const {
    double total = 0.0;
    auto incomingTransactions = searchTransactionByType(TransactionType::Incoming);
    for (const auto& t : incomingTransactions) {
        total += t.getAmount();
    }
    return total;
}

double Account::getTotalOutgoing() const {
    double total = 0.0;
    auto outgoingTransactions = searchTransactionByType(TransactionType::Outgoing);
    for (const auto& t : outgoingTransactions) {
        total += t.getAmount();
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
                oss << "  " << line << "\n";  // spazi di indentazione per rendere il contenuto più leggibile
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
    //apertura file di testo. se l'apertura dovesse fallire lancia un eccezione
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Impossibile aprire il file " + filename);
    }

    std::string line;
    //creo variabili temporanee per i parametri della transazione
    int id = 0;
    double amount = 0.0;
    TransactionType type = TransactionType::Outgoing;
    std::string date, description;

    bool readingTransaction = false;

    while (std::getline(file, line)) {
        if (line.find("Transaction:") != std::string::npos) {
            readingTransaction = true; //trovata transazione
            id = 0;
            amount = 0.0;
            date.clear();
            description.clear();
            continue;
        }

        if (readingTransaction) {
            if (line.find("ID: ") != std::string::npos) {
                id = std::stoi(line.substr(4)); //estrapolo ID

                // controlla duplicati
                bool exists = false;
                for (const auto& t : transactions) {
                    if (t.getTransactionId() == id) {
                        exists = true;
                        break;
                    }
                }

                if (exists) {
                    readingTransaction = false; // skip
                    continue;
                }
            }
            else if (line.find("Amount: ") != std::string::npos) {
                amount = std::stod(line.substr(8));
            }
            else if (line.find("Type: ") != std::string::npos) {
                std::string typeStr = line.substr(6);
                type = (typeStr == "Incoming" ? TransactionType::Incoming : TransactionType::Outgoing);
            }
            else if (line.find("Date: ") != std::string::npos) {
                date = line.substr(6);
            }
            else if (line.find("Description: ") != std::string::npos) {
                description = line.substr(13);
            }
            else if (line.find("----------------------") != std::string::npos) {
                // fine di una transazione completa
                if (id != 0 && amount > 0.0) {
                    Transaction t(id, amount, type, description);
                    t.setDate(date);
                    addTransaction(t);
                    std::cout << "Aggiunta transazione ID: " << id << std::endl;
                }
                readingTransaction = false;
            }
        }
    }

    file.close();
}
