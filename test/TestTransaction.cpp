//
// Created by Sofia Castri on 25/09/25.
//
#include "gtest/gtest.h"
#include "../Transaction.h"


//Verifica che il costruttore imposti correttamente tutti gli attributi passati.
TEST(TransactionTest, ConstructorValid) {
    Transaction t(1, 100.0, TransactionType::Incoming, "Deposit");

    EXPECT_EQ(t.getTransactionId(), 1);
    EXPECT_DOUBLE_EQ(t.getAmount(), 100.0);
    EXPECT_EQ(t.getType(), TransactionType::Incoming);
    EXPECT_EQ(t.getDescription(), "Deposit");
}

//Verifica che il costruttore lanci un’eccezione se l’importo è negativo
TEST(TransactionTest, ConstructorNegativeAmountThrows) {
    EXPECT_THROW(Transaction t1(2, -50.0, TransactionType::Outgoing, "Withdraw"), std::invalid_argument);
}

//verifica che il costruttore lanci un'eccezione se l'importo è negativo
TEST(TransactionTest, ConstructorZeroAmountThrows) {
    EXPECT_THROW(Transaction t2(3, 0.0, TransactionType::Incoming, "Zero"), std::invalid_argument);
}

//Controlla che i setter funzionino correttamente e che i getter restituiscano i valori corretti.
TEST(TransactionTest, SettersAndGetters) {
    Transaction t(4, 200.0, TransactionType::Outgoing, "Payment");

    EXPECT_EQ(t.getTransactionId(), 4);
    EXPECT_DOUBLE_EQ(t.getAmount(), 200.0);
    EXPECT_EQ(t.getType(), TransactionType::Outgoing);
    EXPECT_EQ(t.getDescription(), "Payment");

    t.setAmount(300.0);
    t.setDescription("Updated Payment");

    EXPECT_DOUBLE_EQ(t.getAmount(), 300.0);
    EXPECT_EQ(t.getDescription(), "Updated Payment");
}
