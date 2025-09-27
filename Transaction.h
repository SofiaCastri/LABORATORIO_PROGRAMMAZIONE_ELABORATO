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
    Transaction(int id, double amount, TransactionType type, const std::string& description);

    ~Transaction() = default;

    //getter
    int getTransactionId() const;
    double getAmount() const;
    TransactionType getType() const;
    std::string getDate() const;
    std::string getDescription() const;

    // setter
    void setAmount(double amt);
    void setDescription(const std::string& descpt) ;
    void setTransactionId(const int Id);
    void setType(const TransactionType tp);
    void setDate(const string& dt);


    //metodi per I/O
    std::string transactiontoString() const; // Metodo per rappresentare una transazione come stringa leggibile
    void writeTransactionToFile(const std::string& filename) const; //metodo scrittura di una transazione su file di testo
    Transaction readTransactionFromFile(const std::string& filename, int transactionId); //cerca una transazione nel file di testo


private:
    int transactionId;
    double amount;
    TransactionType type;
    string date;
    string description;

};


#endif //LABORATORIO_PROGRAMMAZIONE_ELABORATO_TRANSACTION_H
