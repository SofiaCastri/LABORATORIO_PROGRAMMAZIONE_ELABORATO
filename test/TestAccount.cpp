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