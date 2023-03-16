//
// Created by User on 16.03.2023.
//

#include "Networking/DatabaseManager.h"

DatabaseManager::DatabaseManager() : conn(connectionString()) {
    if(!conn.is_open()) {
        std::cerr << "Can't open database\n";
    }
};

std::string DatabaseManager::connectionString() const {
    std::string connectionString =
        "host=" + host + " port=" + port + " dbname=" + dbname + " user=" + user + " password=" + password;
    return connectionString;
}


