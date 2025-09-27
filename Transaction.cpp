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

