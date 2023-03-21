//
// Created by User on 16.03.2023.
//

#include "Server/DatabaseManager.h"

DatabaseManager::DatabaseManager() : conn(connectionString().c_str()) {
    if (!conn.is_open()) {
        std::cerr << "Can't open database\n";
    } else {
        prepare_statements();
    }
}

std::string DatabaseManager::connectionString() const {
    std::string connectionString =
        "host=" + host + " port=" + port + " dbname=" + dbname + " user=" + user + " password=" + password;
    return connectionString;
}

void DatabaseManager::prepare_statements() {
    conn.prepare("findAccount", "SELECT * FROM bank_accounts WHERE id_account=$1");
    conn.prepare("findIncomeCategory", "SELECT * FROM income_categories WHERE id_cat=$1");
    conn.prepare("findExpenseCategory", "SELECT * FROM expense_categories WHERE id_cat=$1");
    conn.prepare("findIncome", "SELECT * FROM income WHERE id_income=$1");
    conn.prepare("findExpense", "SELECT * FROM expenses WHERE id_expense=$1");

    conn.prepare("decreaseAccountAmount", "UPDATE bank_accounts SET amount=amount-$1 WHERE id_account=$2");
    conn.prepare("increaseAccountAmount", "UPDATE bank_accounts SET amount=amount+$1 WHERE id_account=$2");

    conn.prepare("addAccount", "INSERT INTO bank_accounts (name, amount) VALUES($1, $2)");
    conn.prepare("addIncomeCategory", "INSERT INTO income_categories (name) VALUES($1)");
    conn.prepare("addExpenseCategory", "INSERT INTO expense_categories (name) VALUES($1)");
    conn.prepare("addIncome",
                 "INSERT INTO income (id_cat, id_account, amount, date, time, comment) VALUES($1, $2, $3, $4, $5, $6)");
    conn.prepare("addExpense",
                 "INSERT INTO expenses (id_cat, id_account, amount, date, time, comment) VALUES($1, $2, $3, $4, $5, $6)");

    conn.prepare("modifyAccount", "UPDATE bank_accounts SET name=$1, amount=$2 WHERE id_account=$3");
    conn.prepare("modifyIncomeCategory", "UPDATE income_categories SET name=$1 WHERE id_cat=$2");
    conn.prepare("modifyExpenseCategory", "UPDATE expense_categories SET name=$1 WHERE id_cat=$2");
    conn.prepare("modifyIncome",
                 "UPDATE income SET id_cat=$1, id_account=$2, amount=$3, date=$4, time=$5, comment=$6 WHERE id_income=$7");
    conn.prepare("modifyExpense",
                 "UPDATE expenses SET id_cat=$1, id_account=$2, amount=$3, date=$4, time=$5, comment=$6 WHERE id_expense=$7");

    conn.prepare("getByIncomeCategoty", "SELECT * FROM income WHERE id_cat=$1 AND date BETWEEN $2 AND $3 ORDER BY date ASC");
    conn.prepare("getByExpenseCategoty", "SELECT * FROM expenses WHERE id_cat=$1 AND date BETWEEN $2 AND $3 ORDER BY date ASC");
    conn.prepare("getIncome", "SELECT * FROM income WHERE date BETWEEN $1 AND $2 ORDER BY date ASC");
    conn.prepare("getExpense", "SELECT * FROM expenses WHERE date BETWEEN $1 AND $2 ORDER BY date ASC");

    conn.prepare("deleteAccount", "DELETE FROM bank_accounts WHERE id_account=$1");
    conn.prepare("deleteIncomeCategory", "DELETE FROM income_categories WHERE id_cat=$1");
    conn.prepare("deleteExpenseCategory", "DELETE FROM expense_categories WHERE id_cat=$1");
    conn.prepare("changeIncomeCategoryOther", "UPDATE income SET id_cat=1 WHERE id_cat=$1");
    conn.prepare("changeExpenseCategoryOther", "UPDATE expenses SET id_cat=1 WHERE id_cat=$1");
    conn.prepare("deleteIncome", "DELETE FROM income WHERE id_income=$1");
    conn.prepare("deleteExpense", "DELETE FROM expenses WHERE id_expense=$1");
}

pqxx::connection &DatabaseManager::GetConn() {
    return conn;
}