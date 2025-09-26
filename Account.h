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
    void addTransaction(const Transaction& t); // aggiunge una transazione
    void transferTo(double amount, Account& receiver, const std::string& description); // trasferisce fondi a un altro conto
    std::vector<Transaction> searchTransactionByType(TransactionType type) const;
    double getTotalIncoming() const; // Calcola il totale di tutte le transazioni in entrata
    double getTotalOutgoing() const; // Calcola il totale di tutte le transazioni in uscita
    std::vector<Transaction> getTransactionsByDateRange(const std::string& fromDate, const std::string& toDate) const; //metodo per filtrare le transazioni per data

    //metodi per scrivere su file
    void writeTransactionsToFile(const std::string& filename) const;
    std::string accountToString() const;
    void writeAccountToFile(const std::string& filename) const;

private:
    string name; // Nome del titolare
    string iban; // IBAN del conto
    double balance; // Saldo corrente
    vector<Transaction> transactions; // Lista delle transazioni

    // Helper per conversione data
    static time_t stringToTime(const std::string& datetimeStr);

};


#endif //LABORATORIO_PROGRAMMAZIONE_ELABORATO_ACCOUNT_H
