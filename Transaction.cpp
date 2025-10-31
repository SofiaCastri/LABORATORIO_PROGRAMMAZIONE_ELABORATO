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

Transaction::Transaction(double amt, TransactionType t, const std::string &desc)
        : transactionId(++nextId), amount(amt), type(t), description(desc)
{
    // Controllo importo
    if (amount <= 0) {
        throw std::invalid_argument("Importo deve essere positivo");
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

void Transaction::resetIdCounter(int startFrom) noexcept {
    nextId = startFrom;
}


//setter
void Transaction::setAmount(double amt) {
    if (amt <= 0) {
        throw std::invalid_argument("L'importo della transazione deve essere positivo");
    }
    amount = amt;
}

void Transaction::setDescription(const std::string &descpt) {
    Transaction::description = descpt;
}

void Transaction::setTransactionId(const int Id) {
    Transaction::transactionId = Id;

    // Mantengo il contatore coerente: se carico da file con ID alti,
    // mi assicuro che i prossimi ID non si sovrappongano
    if (Id > nextId) {
        nextId = Id;
    }
}



void Transaction::setDate(const string& dt) {

    // Controllo validità della nuova data
    std::tm tm = {};
    std::istringstream ss(dt);

    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if (ss.fail()) {
    throw std::invalid_argument("Formato del date non valido");
    }
    std::tm original = tm;

    tm.tm_isdst = -1;  // Lascia che il sistema decida se è ora legale
    if (mktime(&tm) == -1) {
    throw std::invalid_argument("Valori del date non validi");
    }

    if (tm.tm_mday != original.tm_mday ||
        tm.tm_mon  != original.tm_mon  ||
        tm.tm_year != original.tm_year) {
        throw std::invalid_argument("La data inserita non è valida");
    }


    Transaction::date = dt;
}


void Transaction::update(double newAmount, const std::string& newDescription, const std::string& newDate) {
    // Salva i valori attuali (per eventuale rollback)
    double oldAmount = amount;
    std::string oldDescription = description;
    std::string oldDate = date;

    try {

        setAmount(newAmount);
        setDescription(newDescription);
        setDate(newDate);
    }
    catch (const std::exception& e) {

        std::cerr << "Update fallito: " << e.what() << std::endl;
        amount = oldAmount;
        description = oldDescription;
        date = oldDate;

        std::cerr << "Transazione NON modificata. Stato ripristinato ai valori precedenti." << std::endl;

        throw;
    }
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
    std::ofstream file(filename, std::ios::app);
    // apertura in modalità append (aggiunge i dati alla fine del file senza cancellare il contenuto precedente)
    if (file.is_open()) {
        file << transactiontoString() << "\n----------------------\n";
        file.close();
    } else {
        throw std::runtime_error("Impossibile aprire il file " + filename); //se file non si apre manda messaggio di errore
    }
}

