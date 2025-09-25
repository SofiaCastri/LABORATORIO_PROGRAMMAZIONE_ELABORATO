//
// Created by Sofia Castri on 25/09/25.
//
#include "gtest/gtest.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv); // inizializza GoogleTest con eventuali parametri da riga di comando
    return RUN_ALL_TESTS();                  // esegue tutti i test registrati e ritorna 0 se passano tutti, 1 altrimenti
}
