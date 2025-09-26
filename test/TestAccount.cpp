//
// Created by Sofia Castri on 25/09/25.
//

#include "gtest/gtest.h"
#include "../Account.h"
#include "../Transaction.h"

// Test costruttore e getter
TEST(AccountTest, ConstructorAndGetters) {
    Account acc("Alice", "IT123", 1000.0);

    EXPECT_EQ(acc.getName(), "Alice");
    EXPECT_EQ(acc.getIban(), "IT123");
    EXPECT_DOUBLE_EQ(acc.getBalance(), 1000.0);
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

// Test transferTo con importo negativo → eccezione
TEST(AccountTest, TransferWithInsufficientBalanceThrows) {
    Account alice("Alice", "IT111", 1000.0);
    Account bob("Bob", "IT222", 500.0);

    EXPECT_THROW(alice.transferTo(-100.0, bob, "Invalid"), std::invalid_argument);
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