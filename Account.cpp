//
// Created by Sofia Castri on 25/09/25.
//

#include "Account.h"

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

    if (t.getType() == TransactionType::Incoming)
        balance += t.getAmount();
    else
        balance -= t.getAmount();
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