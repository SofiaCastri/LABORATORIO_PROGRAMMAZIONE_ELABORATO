//
// Created by Sofia Castri on 25/09/25.
//

#include "gtest/gtest.h"
#include "../Account.h"
#include <fstream>

// Test costruttore e getter
TEST(AccountTest, ConstructorAndGetters) {
    Account acc("Alice", "IT123", 1000.0);

    EXPECT_EQ(acc.getName(), "Alice");
    EXPECT_EQ(acc.getIban(), "IT123");
    EXPECT_DOUBLE_EQ(acc.getBalance(), 1000.0);
}

TEST(AccountConstructorTest, NegativeInitialBalanceThrows) {
    // Prova a creare un account con saldo negativo
    EXPECT_THROW({
                     Account acc("Mario Rossi", "IT1234567890", -100.0);
                 }, std::invalid_argument);
}

// Test addTransaction con Incoming
TEST(AccountTest, AddTransactionIncoming) {
    Account acc("Bob", "IT456", 500.0);
    Transaction t(1, 200.0, TransactionType::Incoming, "Deposit");

    acc.addTransaction(t);

    EXPECT_DOUBLE_EQ(acc.getBalance(), 700.0); // saldo aggiornato
    auto transactions = acc.searchTransactionByType(TransactionType::Incoming);
    ASSERT_EQ(transactions.size(), 1); // deve esserci una transazione in entrata
    EXPECT_EQ(transactions[0].getDescription(), "Deposit");
}
// Test addTransaction con Outgoing
TEST(AccountTest, AddTransactionOutgoing) {
    Account acc("Charlie", "IT789", 800.0);
    Transaction t(2, 300.0, TransactionType::Outgoing, "Payment");

    acc.addTransaction(t);

    EXPECT_DOUBLE_EQ(acc.getBalance(), 500.0); // saldo aggiornato
    auto transactions = acc.searchTransactionByType(TransactionType::Outgoing);
    ASSERT_EQ(transactions.size(), 1);
    EXPECT_EQ(transactions[0].getDescription(), "Payment");
}

// Test addTransaction con Outgoing che supera il saldo
TEST(AccountTest, AddTransactionOutgoingInsufficientBalanceThrows) {
    Account acc("Diana", "IT999", 100.0);
    Transaction t(3, 200.0, TransactionType::Outgoing, "Big Payment");

    EXPECT_THROW(acc.addTransaction(t), std::runtime_error);

    // Dopo il fallimento, il saldo deve restare invariato
    EXPECT_DOUBLE_EQ(acc.getBalance(), 100.0);

    // E non deve esserci nessuna transazione salvata
    auto transactions = acc.searchTransactionByType(TransactionType::Outgoing);
    EXPECT_TRUE(transactions.empty());
}


// Test transferTo con successo
TEST(AccountTest, TransferToSuccess) {
    Account alice("Alice", "IT111", 1000.0);
    Account bob("Bob", "IT222", 500.0);

    alice.transferTo(300.0, bob, "Rent");

    EXPECT_DOUBLE_EQ(alice.getBalance(), 700.0);
    EXPECT_DOUBLE_EQ(bob.getBalance(), 800.0);

    auto aliceOutgoing = alice.searchTransactionByType(TransactionType::Outgoing);
    auto bobIncoming = bob.searchTransactionByType(TransactionType::Incoming);

    ASSERT_EQ(aliceOutgoing.size(), 1);
    ASSERT_EQ(bobIncoming.size(), 1);

    EXPECT_EQ(aliceOutgoing[0].getDescription(), "Rent");
    EXPECT_EQ(bobIncoming[0].getDescription(), "Rent");
}

// Test transferTo con importo negativo → eccezione
TEST(AccountTest, TransferNegativeAmountThrows) {
    Account alice("Alice", "IT111", 1000.0);
    Account bob("Bob", "IT222", 500.0);

    EXPECT_THROW(alice.transferTo(-100.0, bob, "Invalid"), std::invalid_argument);
}

// Test transferTo con saldo insufficiente → eccezione
TEST(AccountTest, TransferInsufficientBalanceThrows) {
    Account alice("Alice", "IT111", 100.0);
    Account bob("Bob", "IT222", 500.0);

    EXPECT_THROW(alice.transferTo(200.0, bob, "Big Payment"), std::runtime_error);

    // Verifica che i saldi restino invariati
    EXPECT_DOUBLE_EQ(alice.getBalance(), 100.0);
    EXPECT_DOUBLE_EQ(bob.getBalance(), 500.0);

    // Nessuna transazione dovrebbe essere registrata
    EXPECT_TRUE(alice.searchTransactionByType(TransactionType::Outgoing).empty());
    EXPECT_TRUE(bob.searchTransactionByType(TransactionType::Incoming).empty());
}

TEST(AccountTest, SearchTransactionByType) {
    Account acc("TestUser", "IT123", 1000.0);

    Transaction t1(1, 100.0, TransactionType::Incoming, "Salary");
    Transaction t2(2, 50.0, TransactionType::Outgoing, "Groceries");
    Transaction t3(3, 200.0, TransactionType::Incoming, "Bonus");

    acc.addTransaction(t1);
    acc.addTransaction(t2);
    acc.addTransaction(t3);

    // Verifica Incoming
    auto incoming = acc.searchTransactionByType(TransactionType::Incoming);
    ASSERT_EQ(incoming.size(), 2);
    EXPECT_EQ(incoming[0].getDescription(), "Salary");
    EXPECT_EQ(incoming[1].getDescription(), "Bonus");

    // Verifica Outgoing
    auto outgoing = acc.searchTransactionByType(TransactionType::Outgoing);
    ASSERT_EQ(outgoing.size(), 1);
    EXPECT_EQ(outgoing[0].getDescription(), "Groceries");
}


TEST(AccountDateSearchTest, FindsTransactionsBeforeGivenDate) {
    Account acc("Mario Rossi", "IT1234567890", 1000.0);

    //Aggiungo alcune transazioni con date specifiche
    Transaction t1(1, 100.0, TransactionType::Incoming, "Stipendio");
    t1.setDate("2025-09-25 10:00:00");
    acc.addTransaction(t1);

    Transaction t2(2, 50.0, TransactionType::Outgoing, "Spesa");
    t2.setDate("2025-09-26 12:00:00");
    acc.addTransaction(t2);

    Transaction t3(3, 200.0, TransactionType::Incoming, "Bonus");
    t3.setDate("2025-09-27 15:00:00");
    acc.addTransaction(t3);

    std::vector<Transaction> results = acc.findTransactionsBeforeDate("2025-09-26 00:00:00");

    ASSERT_EQ(results.size(), 1);                  // Deve restituire solo t1
    EXPECT_EQ(results[0].getTransactionId(), 1);
    EXPECT_EQ(results[0].getDescription(), "Stipendio");
}

TEST(AccountDateSearchTest, InvalidDateThrowsException) {
    Account acc("Mario Rossi", "IT1234567890", 1000.0);
    EXPECT_THROW(acc.findTransactionsBeforeDate("2025-99-99 00:00:00"), std::invalid_argument);
}


//test totale entrate
TEST(AccountTest, TotalIncoming) {
    Account acc("Dora", "IT333", 1000.0);
    acc.addTransaction(Transaction(1, 100.0, TransactionType::Incoming, "Salary"));
    acc.addTransaction(Transaction(2, 50.0, TransactionType::Incoming, "Gift"));
    acc.addTransaction(Transaction(3, 30.0, TransactionType::Outgoing, "Groceries"));

    EXPECT_DOUBLE_EQ(acc.getTotalIncoming(), 150.0);
}

// test Totale Uscite
TEST(AccountTest, TotalOutgoing) {
    Account acc("Eli", "IT444", 1000.0);
    acc.addTransaction(Transaction(1, 100.0, TransactionType::Incoming, "Salary"));
    acc.addTransaction(Transaction(2, 50.0, TransactionType::Outgoing, "Rent"));
    acc.addTransaction(Transaction(3, 30.0, TransactionType::Outgoing, "Utilities"));

    EXPECT_DOUBLE_EQ(acc.getTotalOutgoing(), 80.0);
}

TEST(AccountTest, WriteTransactionsToFile) {
    Account acc("Test User", "IT99999", 1000.0);

    // Creo alcune transazioni
    Transaction t1(1, 200.0, TransactionType::Incoming, "Stipendio");
    Transaction t2(2, 50.0, TransactionType::Outgoing, "Spesa alimentare");

    acc.addTransaction(t1);
    acc.addTransaction(t2);

    std::string filename = "file_transazioni_account_test.txt";
    std::ofstream(filename, std::ios::trunc).close();// Pulisco eventuale file precedente

    acc.writeTransactionsToFile(filename); // Scrivo le transazioni su file usando il metodo della classe Account
    ASSERT_TRUE(std::filesystem::exists(filename));
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Controllo che contenga le informazioni delle due transazioni
    EXPECT_NE(content.find("ID: 1"), std::string::npos);
    EXPECT_NE(content.find("ID: 2"), std::string::npos);
    EXPECT_NE(content.find("Stipendio"), std::string::npos);
    EXPECT_NE(content.find("Spesa alimentare"), std::string::npos);
    EXPECT_NE(content.find("Incoming"), std::string::npos);
    EXPECT_NE(content.find("Outgoing"), std::string::npos);
}


TEST(AccountTest, WriteTransactionsToFile_EmptyVector) {
    Account acc("Empty User", "IT00000", 500.0);

    std::string filename = "file_transazioni_account_vuoto.txt";
    std::ofstream(filename, std::ios::trunc).close();

    acc.writeTransactionsToFile(filename);

    ASSERT_TRUE(std::filesystem::exists(filename));
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Controllo che il file sia vuoto
    EXPECT_TRUE(content.empty());
}

TEST(AccountTest, AccountToStringWithTransactions) {
    Account acc("Alice", "IT12345", 1000.0);

    Transaction t1(1, 200.0, TransactionType::Incoming, "Salary");
    Transaction t2(2, 50.0, TransactionType::Outgoing, "Groceries");

    acc.addTransaction(t1);
    acc.addTransaction(t2);

    std::string str = acc.accountToString();

    // Verifica informazioni generali
    EXPECT_NE(str.find("Alice"), std::string::npos);
    EXPECT_NE(str.find("IT12345"), std::string::npos);
    EXPECT_NE(str.find("1150"), std::string::npos);

    // Verifica che ci siano le transazioni
    EXPECT_NE(str.find("Salary"), std::string::npos);
    EXPECT_NE(str.find("Groceries"), std::string::npos);
    EXPECT_NE(str.find("Incoming"), std::string::npos);
    EXPECT_NE(str.find("Outgoing"), std::string::npos);
}

TEST(AccountTest, AccountToStringNoTransactions) {
    Account acc("Bob", "IT67890", 500.0);

    std::string str = acc.accountToString();

    // Verifica informazioni generali
    EXPECT_NE(str.find("Bob"), std::string::npos);
    EXPECT_NE(str.find("IT67890"), std::string::npos);
    EXPECT_NE(str.find("500"), std::string::npos);

    // Verifica messaggio transazioni vuote
    EXPECT_NE(str.find("Nessuna transazione registrata"), std::string::npos);
}

// Test: scrittura account con transazioni
TEST(AccountTest, WriteAccountToFileWithTransactions) {
    Account acc("Alice", "IT12345", 1000.0);
    Transaction t1(1, 200.0, TransactionType::Incoming, "Stipendio");
    Transaction t2(2, 50.0, TransactionType::Outgoing, "Spesa alimentare");

    acc.addTransaction(t1);
    acc.addTransaction(t2);

    std::string filename = "test_account_with_transactions.txt";
    std::ofstream(filename, std::ios::trunc).close(); // reset file

    // Scrivo su file
    acc.writeAccountToFile(filename);

    // Verifico che il file esista
    ASSERT_TRUE(std::filesystem::exists(filename));

    // Leggo il contenuto
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Controlli sul contenuto
    EXPECT_NE(content.find("Alice"), std::string::npos);
    EXPECT_NE(content.find("IT12345"), std::string::npos);
    EXPECT_NE(content.find("1150"), std::string::npos);
    EXPECT_NE(content.find("Stipendio"), std::string::npos);
    EXPECT_NE(content.find("Spesa alimentare"), std::string::npos);
}

// Test: scrittura account senza transazioni
TEST(AccountTest, WriteAccountToFileWithoutTransactions) {
    Account acc("Bob", "IT67890", 500.0);

    std::string filename = "test_account_without_transactions.txt";
    std::ofstream(filename, std::ios::trunc).close(); // reset file

    acc.writeAccountToFile(filename);

    ASSERT_TRUE(std::filesystem::exists(filename));

    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    EXPECT_NE(content.find("Bob"), std::string::npos);
    EXPECT_NE(content.find("IT67890"), std::string::npos);
    EXPECT_NE(content.find("500"), std::string::npos);

    // Verifica che NON ci siano transazioni scritte
    EXPECT_NE(content.find("Nessuna transazione registrata"), std::string::npos);
}

//caso transazioni duplicata
TEST(AccountTest, LoadTransactionsFromFileAvoidDuplicates) {
    std::string filename = "test_duplicates.txt";
    std::ofstream(filename, std::ios::trunc).close();

    Transaction t1(400, 100.0, TransactionType::Incoming, "Bonus");
    Transaction t2(500, 25.0, TransactionType::Outgoing, "Caffè");

    t1.writeTransactionToFile(filename);
    t2.writeTransactionToFile(filename);

    Account account("Luigi", "IT456", 500.0);

    // Prima caricamento
    account.loadTransactionsFromFile(filename);
    EXPECT_EQ(account.getTransactionSize(), 2); // Ora puoi usare getTransactionSize()
    EXPECT_DOUBLE_EQ(account.getBalance(), 575.0); // 500 + 100 - 25

    // Secondo caricamento - le transazioni dovrebbero essere saltate
    account.loadTransactionsFromFile(filename);
    EXPECT_EQ(account.getTransactionSize(), 2); // Stesso numero, no duplicati
    EXPECT_DOUBLE_EQ(account.getBalance(), 575.0); // Saldo invariato

    std::remove(filename.c_str());
}

// 2. File vuoto → nessuna transazione caricata, saldo invariato
TEST(AccountLoadTest, EmptyFile) {
    std::string fname = "test_empty.txt";
    std::ofstream(fname, std::ios::trunc).close();  // crea/azzerra
    Account acc("Test", "IT000", 500.0);

    acc.loadTransactionsFromFile(fname);

    EXPECT_EQ(acc.getTransactionSize(), 0);
    EXPECT_DOUBLE_EQ(acc.getBalance(), 500.0);

    std::remove(fname.c_str());
}

TEST(AccountLoadTest, OutgoingExceedingBalance) {
    std::string fname = "test_overout.txt";

    // Creo una transazione outgoing che supera il saldo iniziale
    Transaction t1(501, 200.0, TransactionType::Outgoing, "Big outgoing");
    t1.writeTransactionToFile(fname);

    // Creo un nuovo account con saldo troppo basso per coprire la transazione
    Account acc("Test", "IT000", 100.0);

    // Mi aspetto che durante il caricamento venga lanciata un'eccezione per saldo insufficiente
    EXPECT_THROW(acc.loadTransactionsFromFile(fname), std::runtime_error);
    std::remove(fname.c_str());
}