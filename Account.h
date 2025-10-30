//
// Created by Sofia Castri on 25/09/25.
//

#ifndef LABORATORIO_PROGRAMMAZIONE_ELABORATO_ACCOUNT_H
#define LABORATORIO_PROGRAMMAZIONE_ELABORATO_ACCOUNT_H

#include "Transaction.h"
#include <string>
#include <vector>

using namespace std;

class Account {
    public: Account(const std::string& name, const std::string& iban, double balance);

    // Getter
    std::string getName() const;
    std::string getIban() const;
    double getBalance() const;

    //metodi principali
    int getTransactionSize() const;
    void addTransaction(const Transaction& t); // aggiunge una transazione
    void transferTo(double amount, Account& receiver, const std::string& description); // trasferisce fondi a un altro conto
    std::vector<Transaction> searchTransactionByType(TransactionType type) const;
    std::vector<Transaction> findTransactionsBeforeDate(const std::string& targetDate) const;
    double getTotalIncoming() const; // Calcola il totale di tutte le transazioni in entrata
    double getTotalOutgoing() const; // Calcola il totale di tutte le transazioni in uscita
    std::vector<Transaction> searchTransactionsByWord(const std::string& keyword) const; // Restituisce tutte le transazioni che contengono la parola chiave specificata nella descrizione
    double simulateCompoundInterest(double monthlySaving, double annualRate) const; // Simula il saldo a fine anno considerando risparmi mensili (interesse composto)

    //metodi per scrivere  e leggere da file
    void writeTransactionsToFile(const std::string& filename) const;
    std::string accountToString() const;
    void writeAccountToFile(const std::string& filename) const;
    bool isDuplicateTransaction(int id) const;
    void readTransactionLine(const std::string& line, int& id, double& amount, TransactionType& type, std::string& date, std::string& description, bool& readingTransaction);
    void loadTransactionsFromFile(const std::string& filename); //legge su file e salva le transazioni sul conto

private:
    string name; // Nome del titolare
    string iban; // IBAN del conto
    double balance; // Saldo corrente
    vector<Transaction> transactions; // Lista delle transazioni

};


#endif //LABORATORIO_PROGRAMMAZIONE_ELABORATO_ACCOUNT_H
