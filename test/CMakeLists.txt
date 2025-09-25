cmake_minimum_required(VERSION 3.10)


add_subdirectory(./lib/googletest)
set(gtest_SOURCE_DIR ./lib/googletest/)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_SOURCE_FILES runAllTests.cpp TestTransaction.cpp TestAccount.cpp)
add_executable(runAccountTransactionTests ${TEST_SOURCE_FILES})
target_link_libraries(runAccountTransactionTests gtest gtest_main core)

