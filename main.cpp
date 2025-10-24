#include <iostream>
#include <fstream>
#include "Account.h"
#include "Transaction.h"

using namespace std;


// Funzione per pulire un file
void clearFile(const string& filename) {
    ofstream file(filename, ios::trunc);
    if (!file) {
        cerr << "Errore nell'aprire il file: " << filename << endl;
    }
}

int main() {
    try {
        const string fileTransazioni = "transazioni.txt";
        const string fileConti = "conti.txt";

        cout << "=== INIZIO PROGRAMMA GESTIONE CONTI CORRENTI ===" << endl;

        // Pulizia dei file all'inizio
        clearFile(fileTransazioni);
        clearFile(fileConti);

        // Creazione dei conti
        Account mario("Mario Rossi", "IT99A0123456789012345678901", 1500.0);
        Account laura("Laura Bianchi", "IT99B9876543210987654321098", 800.0);

        cout << "\nConti creati." << endl;

        // Salvataggio conti iniziali su fileConti
        mario.writeAccountToFile(fileConti);
        laura.writeAccountToFile(fileConti);

        // Creazione transazioni
        Transaction stipendio(1001, 1200.0, TransactionType::Incoming, "Stipendio mensile");
        Transaction affitto(1002, 600.0, TransactionType::Outgoing, "Pagamento affitto");
        Transaction spesa(1003, 85.50, TransactionType::Outgoing, "Spesa supermercato");

        // Aggiunta delle transazioni al conto di Mario
        mario.addTransaction(stipendio);
        mario.addTransaction(affitto);
        mario.addTransaction(spesa);

        // Salvataggio delle transazioni su file
        mario.writeTransactionsToFile(fileTransazioni);
        cout << "\nTransazioni aggiunte al conto di Mario e salvate su file." << endl;

        // Trasferimento tra conti
        mario.transferTo(200.0, laura, "Regalo compleanno");
        cout << "\nTrasferimento di 200€ da Mario a Laura completato!" << endl;

        // Salvataggio conti aggiornati su file
        mario.writeAccountToFile(fileConti);
        laura.writeAccountToFile(fileConti);
        cout << "Conti aggiornati salvati su file." << endl;

        // CREAZIONE DI NUOVE TRANSAZIONI DA AGGIUNGERE AL FILE
        Transaction bonus(1004, 300.0, TransactionType::Incoming, "Bonus produttività");
        Transaction bollette(1005, 150.0, TransactionType::Outgoing, "Pagamento bollette");

        // Salvataggio delle nuove transazioni direttamente sul file
        bonus.writeTransactionToFile(fileTransazioni);
        bollette.writeTransactionToFile(fileTransazioni);

        cout << "\nNuove transazioni aggiunte al file delle transazioni." << endl;

        // Caricamento transazioni dal file Transazioni in un nuovo account
        Account nuovo("Giuseppe Verdi", "IT99C5555555555555555555555", 2000.0);
        nuovo.loadTransactionsFromFile(fileTransazioni);
        cout << "\nNuovo conto creato e transazioni caricate dal file." << endl;

        // Salvataggio nuovo conto su file
        nuovo.writeAccountToFile(fileConti);

        // Stampa riepilogo
        cout << "\n=== RIEPILOGO CONTI ===" << endl;
        cout << mario.accountToString() << endl;
        cout << laura.accountToString() << endl;
        cout << nuovo.accountToString() << endl;

        cout << "\n=== PROGRAMMA COMPLETATO CON SUCCESSO ===" << endl;
        cout << "File generati: " << fileTransazioni << " (transazioni), " << fileConti << " (conti correnti)" << endl;

    } catch (const exception& e) {
        cerr << "\nErrore durante l'esecuzione: " << e.what() << endl;
        return 1;
    }

    return 0;
}