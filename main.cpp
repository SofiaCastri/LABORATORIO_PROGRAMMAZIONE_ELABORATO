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
    } else {
        cout << "File '" << filename << "' pulito con successo." << endl;
    }
}

int main() {
    try {
        // Nomi dei file di testo
        string fileTransazioni = "transazioni.txt";
        string fileConti = "conti.txt";

        cout << "=== INIZIO PROGRAMMA GESTIONE CONTI CORRENTI ===" << endl;

        // Pulisco i file all'inizio per iniziare da zero
        clearFile(fileTransazioni);
        clearFile(fileConti);

        // CREAZIONE DEI CONTI CORRENTI
        cout << "\n1. CREAZIONE CONTI CORRENTI" << endl;

        Account contoMario("Mario Rossi", "IT99A0123456789012345678901", 1500.0);
        Account contoLaura("Laura Bianchi", "IT99B9876543210987654321098", 800.0);

        cout << "Creato conto di Mario Rossi - Saldo iniziale: 1500€" << endl;
        cout << "Creato conto di Laura Bianchi - Saldo iniziale: 800€" << endl;
        cout << "---------------------------" << endl;

        // SCRITTURA DEI CONTI SU FILE
        cout << "\n2. SALVATAGGIO CONTI SU FILE" << endl;

        contoMario.writeAccountToFile(fileConti);
        contoLaura.writeAccountToFile(fileConti);
        cout << "Conti salvati nel file: " << fileConti << endl;
        cout << "---------------------------" << endl;

        // CREAZIONE E GESTIONE TRANSAZIONI
        cout << "\n3. GESTIONE TRANSAZIONI" << endl;

        // Creo alcune transazioni di esempio
        Transaction stipendio(1001, 1200.0, TransactionType::Incoming, "Stipendio mensile");
        Transaction affitto(1002, 600.0, TransactionType::Outgoing, "Pagamento affitto");
        Transaction spesa(1003, 85.50, TransactionType::Outgoing, "Spesa supermercato");

        // Salvo le transazioni su file
        stipendio.writeTransactionToFile(fileTransazioni);
        affitto.writeTransactionToFile(fileTransazioni);
        spesa.writeTransactionToFile(fileTransazioni);
        cout << "Transazioni salvate nel file: " << fileTransazioni << endl;
        cout << "----------------------" << endl;


        // Aggiungo le transazioni al conto di Mario
        contoMario.addTransaction(stipendio);
        contoMario.addTransaction(affitto);
        contoMario.addTransaction(spesa);
        cout << "Transazioni aggiunte al conto di Mario Rossi" << endl;
        cout << "Nuovo saldo Mario: " << contoMario.getBalance() << "€" << endl;
        cout << "-------------------------" << endl;

        // TRASFERIMENTO TRA CONTI
        cout << "\n4. TRASFERIMENTO TRA CONTI" << endl;
        cout << "Saldo prima del trasferimento:" << endl;
        cout << "Mario: " << contoMario.getBalance() << "€" << endl;
        cout << "Laura: " << contoLaura.getBalance() << "€" << endl;

        // Mario trasferisce 200€ a Laura
        contoMario.transferTo(200.0, contoLaura, "Regalo compleanno");
        cout << "\nTrasferimento di 200€ da Mario a Laura completato!" << endl;

        cout << "Saldo dopo il trasferimento:" << endl;
        cout << "Mario: " << contoMario.getBalance() << "€" << endl;
        cout << "Laura: " << contoLaura.getBalance() << "€" << endl;

        // Aggiorno i conti sul file dopo il trasferimento
        contoMario.writeAccountToFile(fileConti);
        contoLaura.writeAccountToFile(fileConti);
        cout << "Conti aggiornati sul file." << endl;
        cout << "-------------------------" << endl;


        // LETTURA DA FILE
        cout << "\n6. LETTURA TRANSAZIONE DA FILE" << endl;

        try {
            cout << "Caricamento delle transazioni dal file '" << fileTransazioni << "'..." << endl;

            // Carica tutte le transazioni dal file nel conto di Mario
            contoMario.loadTransactionsFromFile(fileTransazioni);

            cout << "Transazioni caricate con successo nel conto di Mario Rossi." << endl;
            cout << "Numero totale transazioni: " << contoMario.getTransactionSize() << endl;

            // Stampa le transazioni per verificarne il caricamento
            cout << "\nTransazioni attualmente registrate nel conto di Mario:" << endl;
            cout << contoMario.accountToString() << endl;

        } catch (const exception& e) {
            cerr << "Errore durante il caricamento delle transazioni: " << e.what() << endl;
        }

        cout << "----------------------------------------" << endl;
        // CREAZIONE NUOVO CONTO CON CARICAMENTO DA FILE
        cout << "\n7. NUOVO CONTO CON CARICAMENTO TRANSAZIONI" << endl;
        Account contoNuovo("Giuseppe Verdi", "IT99C5555555555555555555555", 2000.0);
        cout << "Creato nuovo conto per Giuseppe Verdi" << endl;
        cout << "Transazioni prima del caricamento: " << contoNuovo.getTransactionSize() << endl;

        // Carico le transazioni dal file
        contoNuovo.loadTransactionsFromFile(fileTransazioni);
        cout << "Transazioni dopo il caricamento: " << contoNuovo.getTransactionSize() << endl;

        // Salvo il nuovo conto sul file
        contoNuovo.writeAccountToFile(fileConti);
        cout << "Nuovo conto salvato sul file." << endl;

        // MESSAGGIO FINALE
        cout << "\n=== PROGRAMMA COMPLETATO CON SUCCESSO ===" << endl;
        cout << "File generati:" << endl;
        cout << "- " << fileTransazioni << " (transazioni)" << endl;
        cout << "- " << fileConti << " (conti correnti)" << endl;
        cout << "Controlla i file per vedere i risultati." << endl;

    } catch (const exception& e) {
        // Gestione degli errori
        cerr << "\n!!! ERRORE DURANTE L'ESECUZIONE !!!" << endl;
        cerr << "Motivo: " << e.what() << endl;
        return 1;
    }

    return 0;
}