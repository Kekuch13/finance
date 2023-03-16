//
// Created by User on 16.03.2023.
//

#pragma once

#include <iostream>
#include <pqxx/pqxx>
#include <string>

class DatabaseManager {
private:
    std::string host = "localhost";
    std::string port = "5432";
    std::string dbname = "finance";
    std::string user = "postgres";
    std::string password = "Happy2022";

    std::string connectionString() const;
private:
    pqxx::connection conn;
public:
    DatabaseManager();

    pqxx::connection &GetConn();
};
