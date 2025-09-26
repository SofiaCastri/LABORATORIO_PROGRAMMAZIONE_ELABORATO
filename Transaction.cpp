//
// Created by Sofia Castri on 25/09/25.
//

#include "Transaction.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

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
void Transaction::setAmount(double amount) {
    Transaction::amount = amount;
}

void Transaction::setDescription(const std::string &description) {
    Transaction::description = description;
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
