#include <iostream>
#include "sqlite3.h" // Подключаем SQLite


sqlite3* db; // Указатель на базу данных

// Функция для подключения к базе данных
bool connectToDatabase(const std::string& dbName) {
    int exit = sqlite3_open(dbName.c_str(), &db);
    if (exit) {
        std::cerr << "Ошибка подключения к базе данных: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    std::cout << "Подключение к базе данных успешно!" << std::endl;
    return true;
}

void createTables() {
    const char* createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            user_id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            password TEXT NOT NULL
        );
    )";

    const char* createAccountsTable = R"(
        CREATE TABLE IF NOT EXISTS accounts (
            account_id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            balance REAL NOT NULL DEFAULT 0,
            FOREIGN KEY (user_id) REFERENCES users (user_id)
        );
    )";

    const char* createTransactionsTable = R"(
        CREATE TABLE IF NOT EXISTS transactions (
            transaction_id INTEGER PRIMARY KEY AUTOINCREMENT,
            account_id INTEGER NOT NULL,
            type TEXT NOT NULL, -- deposit, withdraw, transfer
            amount REAL NOT NULL,
            date TEXT NOT NULL,
            FOREIGN KEY (account_id) REFERENCES accounts (account_id)
        );
    )";

    char* errorMessage;
    // Выполняем SQL-команды для создания таблиц
    if (sqlite3_exec(db, createUsersTable, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Ошибка создания таблицы users: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    if (sqlite3_exec(db, createAccountsTable, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Ошибка создания таблицы accounts: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    if (sqlite3_exec(db, createTransactionsTable, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Ошибка создания таблицы transactions: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    std::cout << "Таблицы созданы (если их не было)!" << std::endl;
}

int main() {
    const std::string dbName = "banking_system.db";

    if (!connectToDatabase(dbName)) {
        return -1;
    }

    createTables(); // Создаем таблицы, если их нет

    sqlite3_close(db);
    return 0;
}

