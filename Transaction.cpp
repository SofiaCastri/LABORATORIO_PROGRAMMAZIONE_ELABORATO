//
// Created by Sofia Castri on 25/09/25.
//

#include "Transaction.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <iostream>

Transaction::Transaction(int id, double amt, TransactionType t, const std::string &desc)
        : transactionId(id), amount(amt), type(t), description(desc)
{
    // Controllo importo
    if (amount <= 0) {
        throw std::invalid_argument("Importo deve essere positivo"); // Non ha senso creare una transazione con importo zero o negativo
    }

    // Imposta la data corrente come stringa leggibile
    auto now = std::chrono::system_clock::now(); // Prende il tempo corrente dal sistema
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;// Creo uno stream per formattare la data
    ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
    date = ss.str(); // Memorizza la data formattata nella variabile membro
}


// Getter
int Transaction::getTransactionId() const {
    return transactionId;
}

double Transaction::getAmount() const {
    return amount;
}

TransactionType Transaction::getType() const {
    return type;
}

std::string Transaction::getDate() const {
    return date;
}

std::string Transaction::getDescription() const {
    return description;
}

//setter
void Transaction::setAmount(double amt) {
    Transaction::amount = amt;
}

void Transaction::setDescription(const std::string &descpt) {
    Transaction::description = descpt;
}

void Transaction::setTransactionId(const int Id) {
    Transaction::transactionId = Id;
}

void Transaction::setType(const TransactionType tp) {
    Transaction::type = tp;
}

void Transaction::setDate(const string& dt) {

    // Controllo validità della nuova data
    std::tm tm = {};
    std::istringstream ss(dt);

    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if (ss.fail()) {
    throw std::invalid_argument("Formato del date non valido");
    }

    tm.tm_isdst = -1;  // Lascia che il sistema decida se è ora legale
    if (mktime(&tm) == -1) {
    throw std::invalid_argument("Valori del date non validi");
    }

    // setter
    Transaction::date = dt;
}


std::string Transaction::transactiontoString() const {
    std::ostringstream oss;
    oss << "Transaction:\n"
        << "ID: " << transactionId << "\n"
        << "Amount: " << amount << "\n"
        << "Type: " << (type == TransactionType::Incoming ? "Incoming" : "Outgoing") << "\n"
        << "Date: " << date << "\n"
        << "Description: " << description << "\n";
    return oss.str();
}

void Transaction::writeTransactionToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app); // apertura in modalità append (aggiunge i dati alla fine del file senza cancellare il contenuto precedente)
    if (file.is_open()) {
        file << transactiontoString() << "\n----------------------\n";
        file.close(); // chiude il file per salvare correttamente i dati
    } else {
        throw std::runtime_error("Impossibile aprire il file " + filename); //se file non si apre manda messaggio di errore
    }
}



Transaction Transaction::readTransactionFromFile(const std::string& filename, int targetId) {

    //Prova ad aprire il file in lettura. Se fallisce, lancia un'eccezione
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Impossibile aprire il file " + filename);
    }

    std::string line;
    // Crea variabili temporanee per memorizzare i dati della transazione
    int id = 0;
    double amt = 0.0;
    TransactionType tp = TransactionType::Outgoing;
    std::string dt;
    std::string descpt;

    bool found = false;
    bool readingTransaction = false;

    while (std::getline(file, line)) { //Legge il file una riga alla volta fino alla fine.
        if (line.find("Transaction:") != std::string::npos) {
            readingTransaction = true; //trovato inizio di una transazione
            continue;
        }

        if (readingTransaction) {
            if (line.find("ID: ") != std::string::npos) {
                id = std::stoi(line.substr(4));
                if (id == targetId) {
                    found = true;
                }
            }
            else if (found && line.find("Amount: ") != std::string::npos) {
                amt = std::stod(line.substr(8));
            }
            else if (found && line.find("Type: ") != std::string::npos) {
                std::string typeStr = line.substr(6);
                tp = (typeStr == "Incoming" ? TransactionType::Incoming : TransactionType::Outgoing);
            }
            else if (found && line.find("Date: ") != std::string::npos) {
                dt = line.substr(6);
            }
            else if (found && line.find("Description: ") != std::string::npos) {
                descpt = line.substr(13);
            }
            else if (line.find("----------------------") != std::string::npos && found) {
                break; // Fine della transazione
            }
        }
    }

    file.close();

    if (!found) {
        throw std::runtime_error("Transazione con ID " + std::to_string(targetId) + " non trovata");
    }

    // ORA creiamo la transazione con tutti i dati raccolti
    Transaction transaction(id, amt, tp, descpt);
    transaction.setDate(dt); // Impostiamo la data separatamente

    return transaction;
}