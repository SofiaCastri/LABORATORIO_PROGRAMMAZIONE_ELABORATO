//
// Created by Sofia Castri on 25/09/25.
//

#ifndef LABORATORIO_PROGRAMMAZIONE_ELABORATO_TRANSACTION_H
#define LABORATORIO_PROGRAMMAZIONE_ELABORATO_TRANSACTION_H

#include <string>

using namespace std;

enum class TransactionType { Outgoing, Incoming };

class Transaction {
public:
    Transaction(double amount, TransactionType type, const std::string& description);

    ~Transaction() = default;

    //getter
    int getTransactionId() const;
    double getAmount() const;
    TransactionType getType() const;
    std::string getDate() const;
    std::string getDescription() const;

    // Metodo statico per resettare o leggere il contatore
    static void resetIdCounter(int startFrom = 0) noexcept;
    static int getNextId() noexcept { return nextId; }

    // setter
    void setAmount(double amt);
    void setDescription(const std::string& descpt) ;
    void setTransactionId(const int Id);
    void setType(const TransactionType tp);
    void setDate(const string& dt);


    // Aggiornamento completo della transazione
    void update(double newAmount, const std::string& newDescription, const std::string& newDate);


    //metodi per I/O
    std::string transactiontoString() const; // Metodo per rappresentare una transazione come stringa leggibile
    void writeTransactionToFile(const std::string& filename) const; //metodo scrittura di una transazione su file di testo

private:
    int transactionId;
    double amount;
    TransactionType type;
    string date;
    string description;

    //Contatore statico condiviso da tutte le transazioni
    inline static int nextId = 0;
};


#endif //LABORATORIO_PROGRAMMAZIONE_ELABORATO_TRANSACTION_H
