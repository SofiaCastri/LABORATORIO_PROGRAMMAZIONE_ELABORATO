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

    //avranno id diverso
    Transaction senderT( amount, TransactionType::Outgoing, description);
    Transaction receiverT( amount, TransactionType::Incoming, description);

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
    // Controlla e valida la data passata come parametro
    std::tm target_tm = {};
    std::istringstream ss(targetDate);
    ss >> std::get_time(&target_tm, "%Y-%m-%d %H:%M:%S");

    if (ss.fail()) {
        throw std::invalid_argument("Formato della data non valido. Usa il formato: YYYY-MM-DD HH:MM:SS");
    }

    std::tm original = target_tm;
    target_tm.tm_isdst = -1;

    if (mktime(&target_tm) == -1 ||
        target_tm.tm_mday != original.tm_mday ||
        target_tm.tm_mon  != original.tm_mon  ||
        target_tm.tm_year != original.tm_year) {
        throw std::invalid_argument("La data inserita non è valida (es. 30 febbraio)");
    }

    time_t target_time = mktime(&target_tm); //converto in numero
    std::vector<Transaction> results;

    for (const auto& t : transactions) {
        std::tm trans_tm = {};
        std::istringstream ts(t.getDate());
        ts >> std::get_time(&trans_tm, "%Y-%m-%d %H:%M:%S");

        time_t trans_time = mktime(&trans_tm);
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


std::vector<Transaction> Account::searchTransactionsByWord(const std::string& word) const {
    std::vector<Transaction> results;

    if (word.empty()) return results; // Se la parola chiave è vuota, ritorna direttamente un vettore vuoto

    std::string keywordLower = word;
    // Conversione temporanea della parola chiave in minuscolo
    std::transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);

    for (const auto& t : transactions) {
        std::string descriptionLower = t.getDescription();
        std::transform(descriptionLower.begin(), descriptionLower.end(), descriptionLower.begin(), ::tolower);

        if (descriptionLower.find(keywordLower) != std::string::npos) {
            results.push_back(t);
        }
    }

    return results;
}


double Account::simulateCompoundInterest(double monthlySaving, double annualRate) const {
        double saldoSimulato = balance;                  // saldo corrente
        double monthlyRate = annualRate / 12.0 / 100.0; // tasso mensile

        for (int i = 0; i < 12; ++i) {
            saldoSimulato += monthlySaving;                     // aggiungi risparmio mensile
            double interesseMensile = saldoSimulato * monthlyRate; // calcola interesse del mese
            saldoSimulato += interesseMensile;
        }

        return saldoSimulato;
}



void Account::writeTransactionsToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app); // apre il file in modalità append
    if (!file.is_open()) {
        std::cerr << "Impossibile aprire il file: " << filename << std::endl;
        return;
    }

    if (transactions.empty()) {
        std::cout << "Nessuna transazione da scrivere su file.\n";
    } else {
        for (const auto& t : transactions) {
            file << t.transactiontoString() << "\n----------------------\n";
        }
        std::cout << "Tutte le transazioni sono state salvate su file: " << filename << std::endl;
    }

    file.close();
}



std::string Account::accountToString() const {
    std::ostringstream oss;
    oss << "Account Info\n";
    oss << "Name: " << name << "\n";
    oss << "IBAN: " << iban << "\n";
    oss << "Balance: " << balance << "\n";

    if (!transactions.empty()) {
        oss << "Transactions:\n\n";
        for (const auto& t : transactions) {
            oss << t.transactiontoString() << "\n----------------------\n";
        }
    } else {
        oss << "Nessuna transazione registrata.\n";
    }

    return oss.str();
}

void Account::writeAccountToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) throw std::runtime_error("Impossibile aprire il file " + filename);

    file << accountToString() << "\n\n";
    file.close();
}


bool Account::isDuplicateTransaction(int id) const {
    for (const auto& t : transactions) {
        if (t.getTransactionId() == id) {
            return true; // trovato duplicato
        }
    }
    return false;
}


void Account::readTransactionLine(const std::string& line, int& id, double& amount, TransactionType& type, std::string& date, std::string& description, bool& readingTransaction) {

    if (line.find("ID: ") != std::string::npos) {
        id = std::stoi(line.substr(4)); // estrapolo ID
        //controllo duplicati
        if (isDuplicateTransaction(id)) {
            readingTransaction = false;
            return;
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
            Transaction t(amount, type, description);
            t.setDate(date);
            t.setTransactionId(id); //imposto l'ID letto da file
            try {
                addTransaction(t);
                std::cout << "Aggiunta transazione ID: " << id << std::endl;
            } catch (const std::runtime_error& e) {
                std::cerr << "Transazione ID " << id << " saltata: " << e.what() << std::endl;
            }
        }
        readingTransaction = false;
    }
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
            date="2000-01-01 00:00:00";
            description.clear();
            continue; //vado al ciclo while successivo (riga successiva)
        }

        if (readingTransaction) {
            readTransactionLine(line, id, amount, type, date, description, readingTransaction);
        }
    }

    file.close();
}
