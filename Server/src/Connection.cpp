#include <Server/Connection.h>

#include <boost/date_time.hpp>
#include <sstream>

#define OTHER_CATEGORY_ID 1

Connection::Connection(tcp::socket &&socket) : socket(std::move(socket)), dbManager() {}

std::shared_ptr<Connection> Connection::create(tcp::socket &&socket) {
    return std::shared_ptr<Connection>(new Connection{std::move(socket)});
}

void Connection::start() {
    asyncRead();
}

void Connection::asyncRead() {
    buffer.clear();
    req.body().clear();
    http::async_read(socket, buffer, req, beast::bind_front_handler(&Connection::onRead, shared_from_this()));
}

void Connection::onRead(const beast::error_code &error, std::size_t bytes_transferred) {
    if (error == http::error::end_of_stream) {
        socket.shutdown(tcp::socket::shutdown_send);
        std::cout << "Connection closed\n";
        return;
    }
    if (error) {
        std::cerr << "Fail on reading: " << error.message() << std::endl;
        return;
    }
    handleRequest();
}

void Connection::asyncWrite(http::message_generator &&msg) {
    bool keep_alive = msg.keep_alive();
    beast::async_write(socket, std::move(msg), [self = shared_from_this(), keep_alive](const beast::error_code &error, std::size_t bytes) {
        self->onWrite(error, bytes, keep_alive);
    });
}

void Connection::onWrite(const beast::error_code &error, std::size_t, bool keep_alive) {
    if (error) {
        std::cerr << "Fail on writing: " << error.message() << std::endl;
        return;
    }

    if (!keep_alive) {
        socket.shutdown(tcp::socket::shutdown_send);
        std::cout << "Connection closed\n";
        return;
    }

    asyncRead();
}

void Connection::handleRequest() {
    std::cout << "\n----------\n" << req.method_string() << std::endl << req.target() << std::endl << req.body()
              << std::endl;

    switch (req.method()) {
        case http::verb::post:
            if (req.target() == "/accounts") {
                addAccount();
            } else if (req.target() == "/expenses") {
                addExpense();
            } else if (req.target() == "/income") {
                addIncome();
            } else if (req.target().starts_with("/categories")) {
                addCategory();
            } else {
                badRequest("Unknown path");
            }
            break;
        case http::verb::put:
            if (req.target() == "/accounts") {
                modifyAccount();
            } else if (req.target() == "/expenses") {
                modifyExpense();
            } else if (req.target() == "/income") {
                modifyIncome();
            } else if (req.target().starts_with("/categories")) {
                modifyCategory();
            } else {
                badRequest("Unknown path");
            }
            break;
        case http::verb::get:
            if (req.target().starts_with("/accounts")) {
                getAccount();
            } else if (req.target().starts_with("/expenses")) {
                getExpense();
            } else if (req.target().starts_with("/income")) {
                getIncome();
            } else if (req.target().starts_with("/categories")) {
                getByCategory();
            } else {
                badRequest("Unknown path");
            }
            break;
        case http::verb::delete_:
            if (req.target().starts_with("/accounts")) {
                deleteAccount();
            } else if (req.target().starts_with("/expenses")) {
                deleteExpense();
            } else if (req.target().starts_with("/income")) {
                deleteIncome();
            } else if (req.target().starts_with("/categories")) {
                deleteCategory();
            } else {
                badRequest("Unknown path");
            }
            break;
        default:badRequest("Unknown HTTP-method");
            break;
    }
}

void Connection::badRequest(beast::string_view why) {
    http::response<http::string_body> res{http::status::bad_request, req.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/plain");
    res.keep_alive(req.keep_alive());
    res.body() = std::string(why);
    res.prepare_payload();

    asyncWrite(std::move(res));
}

void Connection::successResponse(http::status status) {
    http::response<http::string_body> res(status, req.version());
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/plain");
    res.keep_alive(req.keep_alive());
    res.prepare_payload();

    asyncWrite(std::move(res));
}

void Connection::jsonResponse(beast::string_view data) {
    http::response<http::string_body> res(http::status::ok, req.version());
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req.keep_alive());
    res.body() = data;
    res.prepare_payload();

    asyncWrite(std::move(res));
}

void Connection::addAccount() {
    try {
        if (req.body().empty()) {
            throw std::exception("Request's body is empty");
        }

        std::stringstream jsonEncoded(req.body());
        boost::property_tree::ptree root;
        boost::property_tree::read_json(jsonEncoded, root);

        pqxx::work worker(dbManager.GetConn());
        worker.exec_prepared("addAccount", root.get<std::string>("name"), root.get<int>("amount"));
        worker.commit();
        successResponse(http::status::created);
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::addExpense() {
    try {
        if (req.body().empty()) {
            throw std::exception("Request's body is empty");
        }

        std::stringstream jsonEncoded(req.body());
        boost::property_tree::ptree root;
        boost::property_tree::read_json(jsonEncoded, root);

        boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
        std::string curDate = to_simple_string(timeLocal.date());
        std::string curTime = to_simple_string(timeLocal.time_of_day());

        if (!recordExists(root.get<int>("id_account"), "bank_accounts")) {
            throw std::exception("Account doesn't exist");
        }
        if (!recordExists(root.get<int>("id_cat"), "expense_categories")) {
            throw std::exception("Category doesn't exist");
        }

        pqxx::work worker(dbManager.GetConn());
        worker.exec_prepared("addExpense",
                             root.get<int>("id_cat"),
                             root.get<int>("id_account"),
                             root.get<int>("amount"),
                             root.get<std::string>("date", curDate),
                             root.get<std::string>("time", curTime),
                             root.get<std::string>("comment", ""));

        worker.exec_prepared("decreaseAccountAmount",
                             root.get<int>("amount"),
                             root.get<int>("id_account"));
        worker.commit();
        successResponse(http::status::created);
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::addIncome() {
    try {
        if (req.body().empty()) {
            throw std::exception("Request's body is empty");
        }

        std::stringstream jsonEncoded(req.body());
        boost::property_tree::ptree root;
        boost::property_tree::read_json(jsonEncoded, root);

        if (!recordExists(root.get<int>("id_account"), "bank_accounts")) {
            throw std::exception("Account doesn't exist");
        }
        if (!recordExists(root.get<int>("id_cat"), "income_categories")) {
            throw std::exception("Category doesn't exist");
        }

        boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
        std::string curDate = to_simple_string(timeLocal.date());
        std::string curTime = to_simple_string(timeLocal.time_of_day());

        pqxx::work worker(dbManager.GetConn());
        worker.exec_prepared("addIncome",
                             root.get<int>("id_income_cat"),
                             root.get<int>("id_account"),
                             root.get<int>("amount"),
                             root.get<std::string>("date", curDate),
                             root.get<std::string>("time", curTime),
                             root.get<std::string>("comment", ""));

        worker.exec_prepared("increaseAccountAmount",
                             root.get<int>("amount"),
                             root.get<int>("id_account"));
        worker.commit();
        successResponse(http::status::created);
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::addCategory() {
    try {
        if (req.body().empty()) {
            throw std::exception("Request's body is empty");
        }

        std::stringstream jsonEncoded(req.body());
        boost::property_tree::ptree root;
        boost::property_tree::read_json(jsonEncoded, root);

        pqxx::work worker(dbManager.GetConn());
        if (req.target() == "/categories/income") {
            worker.exec_prepared("addIncomeCategory", root.get<std::string>("name"));
        } else if (req.target() == "/categories/expenses") {
            worker.exec_prepared("addExpenseCategory", root.get<std::string>("name"));
        } else {
            throw std::exception("Unknown type of categories");
        }
        worker.commit();
        successResponse(http::status::created);
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::modifyAccount() {
    try {
        if (req.body().empty()) {
            throw std::exception("Request's body is empty");
        }

        std::stringstream jsonEncoded(req.body());
        boost::property_tree::ptree root;
        boost::property_tree::read_json(jsonEncoded, root);

        if (root.find("id_account") == root.not_found()) {
            pqxx::work worker(dbManager.GetConn());
            worker.exec_prepared("addAccount", root.get<std::string>("name"), root.get<int>("amount"));
            worker.commit();
            successResponse(http::status::created);
        } else if (!recordExists(root.get<int>("id_account"), "bank_accounts")) {
            throw std::exception("Account doesn't exist");
        } else {
            pqxx::work worker(dbManager.GetConn());
            pqxx::result res = worker.exec_prepared("findAccount", root.get<int>("id_account"));
            worker.exec_prepared("modifyAccount",
                                 root.get<std::string>("name", res[0]["name"].as<std::string>()),
                                 root.get<int>("amount", res[0]["amount"].as<int>()),
                                 root.get<int>("id_account")
            );
            worker.commit();
            successResponse(http::status::ok);
        }
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::modifyExpense() {
    try {
        if (req.body().empty()) {
            throw std::exception("Request's body is empty");
        }

        std::stringstream jsonEncoded(req.body());
        boost::property_tree::ptree root;
        boost::property_tree::read_json(jsonEncoded, root);

        if (root.find("id_expense") == root.not_found()) {
            boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
            std::string curDate = to_simple_string(timeLocal.date());
            std::string curTime = to_simple_string(timeLocal.time_of_day());

            pqxx::work worker(dbManager.GetConn());
            worker.exec_prepared("addExpense",
                                 root.get<int>("id_cat"),
                                 root.get<int>("id_account"),
                                 root.get<int>("amount"),
                                 root.get<std::string>("date", curDate),
                                 root.get<std::string>("time", curTime),
                                 root.get<std::string>("comment", ""));

            worker.exec_prepared("decreaseAccountAmount",
                                 root.get<int>("amount"),
                                 root.get<int>("id_account"));
            worker.commit();
            successResponse(http::status::created);
        } else if (!recordExists(root.get<int>("id_expense"), "expenses")) {
            throw std::exception("Expense doesn't exist");
        } else {
            if (root.find("id_account") != root.not_found()
                && !recordExists(root.get<int>("id_account"), "bank_accounts")) {
                throw std::exception("Account doesn't exist");
            }
            if (root.find("id_cat") != root.not_found()
                && !recordExists(root.get<int>("id_cat"), "expense_categories")) {
                throw std::exception("Category doesn't exist");
            }
            pqxx::work worker(dbManager.GetConn());
            pqxx::result res = worker.exec_prepared("findExpense", root.get<int>("id_expense"));
            worker.exec_prepared("modifyExpense",
                                 root.get<int>("id_cat", res[0]["id_cat"].as<int>()),
                                 root.get<int>("id_account", res[0]["id_account"].as<int>()),
                                 root.get<int>("amount", res[0]["amount"].as<int>()),
                                 root.get<std::string>("date", res[0]["date"].as<std::string>()),
                                 root.get<std::string>("time", res[0]["time"].as<std::string>()),
                                 root.get<std::string>("comment", res[0]["comment"].as<std::string>()),
                                 root.get<int>("id_expense")
            );
            worker.exec_prepared("increaseAccountAmount",
                                 res[0]["amount"].as<int>(),
                                 res[0]["id_account"].as<int>());
            worker.exec_prepared("decreaseAccountAmount",
                                 root.get<int>("amount", res[0]["amount"].as<int>()),
                                 root.get<int>("id_account", res[0]["id_account"].as<int>()));
            worker.commit();
            successResponse(http::status::ok);
        }
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::modifyIncome() {
    try {
        if (req.body().empty()) {
            throw std::exception("Request's body is empty");
        }

        std::stringstream jsonEncoded(req.body());
        boost::property_tree::ptree root;
        boost::property_tree::read_json(jsonEncoded, root);

        if (root.find("id_income") == root.not_found()) {
            boost::posix_time::ptime timeLocal = boost::posix_time::second_clock::local_time();
            std::string curDate = to_simple_string(timeLocal.date());
            std::string curTime = to_simple_string(timeLocal.time_of_day());

            pqxx::work worker(dbManager.GetConn());
            worker.exec_prepared("addIncome",
                                 root.get<int>("id_cat"),
                                 root.get<int>("id_account"),
                                 root.get<int>("amount"),
                                 root.get<std::string>("date", curDate),
                                 root.get<std::string>("time", curTime),
                                 root.get<std::string>("comment", ""));

            worker.exec_prepared("increaseAccountAmount",
                                 root.get<int>("amount"),
                                 root.get<int>("id_account"));
            worker.commit();
            successResponse(http::status::created);
            worker.commit();
        } else if (!recordExists(root.get<int>("id_income"), "income")) {
            throw std::exception("Income doesn't exist");
        } else {
            if (root.find("id_account") != root.not_found()
                && !recordExists(root.get<int>("id_account"), "bank_accounts")) {
                throw std::exception("Account doesn't exist");
            }
            if (root.find("id_cat") != root.not_found()
                && !recordExists(root.get<int>("id_cat"), "income_categories")) {
                throw std::exception("Category doesn't exist");
            }
            pqxx::work worker(dbManager.GetConn());
            pqxx::result res = worker.exec_prepared("findIncome", root.get<int>("id_income"));
            worker.exec_prepared("modifyIncome",
                                 root.get<int>("id_cat", res[0]["id_cat"].as<int>()),
                                 root.get<int>("id_account", res[0]["id_account"].as<int>()),
                                 root.get<int>("amount", res[0]["amount"].as<int>()),
                                 root.get<std::string>("date", res[0]["date"].as<std::string>()),
                                 root.get<std::string>("time", res[0]["time"].as<std::string>()),
                                 root.get<std::string>("comment", res[0]["comment"].as<std::string>()),
                                 root.get<int>("id_income")
            );
            worker.exec_prepared("increaseAccountAmount",
                                 root.get<int>("amount", res[0]["amount"].as<int>()),
                                 root.get<int>("id_account", res[0]["id_account"].as<int>()));
            worker.exec_prepared("decreaseAccountAmount",
                                 res[0]["amount"].as<int>(),
                                 res[0]["id_account"].as<int>());
            worker.commit();
            successResponse(http::status::ok);
        }
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::modifyCategory() {
    try {
        if (req.body().empty()) {
            throw std::exception("Request's body is empty");
        }

        std::stringstream jsonEncoded(req.body());
        boost::property_tree::ptree root;
        boost::property_tree::read_json(jsonEncoded, root);

        if (req.target() == "/categories/income") {
            if (root.find("id_cat") == root.not_found()) {
                pqxx::work worker(dbManager.GetConn());
                worker.exec_prepared("addIncomeCategory", root.get<std::string>("name"));
                worker.commit();
                successResponse(http::status::created);
            } else if (recordExists(root.get<int>("id_cat"), "income_categories")) {
                if (root.get<int>("id_cat") == OTHER_CATEGORY_ID) {
                    throw std::exception("This is a service category, it can't be edited");
                }
                pqxx::work worker(dbManager.GetConn());
                worker.exec_prepared("modifyIncomeCategory", root.get<std::string>("name"), root.get<int>("id_cat"));
                worker.commit();
                successResponse(http::status::ok);
            } else {
                throw std::exception("Category doesn't exist");
            }
        } else if (req.target() == "/categories/expenses") {
            if (root.find("id_cat") == root.not_found()) {
                pqxx::work worker(dbManager.GetConn());
                worker.exec_prepared("addExpenseCategory", root.get<std::string>("name"));
                worker.commit();
                successResponse(http::status::created);
            } else if (recordExists(root.get<int>("id_cat"), "expense_categories")) {
                if (root.get<int>("id_cat") == OTHER_CATEGORY_ID) {
                    throw std::exception("This is a service category, it can't be edited");
                }
                pqxx::work worker(dbManager.GetConn());
                worker.exec_prepared("modifyExpenseCategory", root.get<std::string>("name"), root.get<int>("id_cat"));
                worker.commit();
                successResponse(http::status::ok);
            } else {
                throw std::exception("Category doesn't exist");
            }
        } else {
            throw std::exception("Unknown type of categories");
        }
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::getAccount() {
    try {
        if (!req.target().starts_with("/accounts?")) {
            throw std::exception("Incorrect query");
        }

        auto query = parseQuery();
        if (!query.contains("id")) {
            throw std::exception("Incorrect query");
        }
        int id = boost::lexical_cast<int>(query["id"]);

        if (!recordExists(id, "bank_accounts")) {
            throw std::exception("Account doesn't exist");
        }

        pqxx::work worker(dbManager.GetConn());
        pqxx::result res = worker.exec_prepared("findAccount", id);
        worker.commit();

        boost::property_tree::ptree root;
        root.add_child("account", toJson(res));
        std::stringstream data;
        boost::property_tree::write_json(data, root);
        jsonResponse(data.str());
    } catch (boost::bad_lexical_cast &e) {
        badRequest("ID must be an integer");
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::getExpense() {
    try {
        if (!req.target().starts_with("/expenses?")) {
            throw std::exception("Incorrect query");
        }

        pqxx::result res;
        boost::property_tree::ptree root;
        auto query = parseQuery();
        if (!query.contains("id")) {
            if (!query.contains("begin") && !query.contains("end")) {
                throw std::exception("Incorrect query");
            }
            root.put("begin", query["begin"]);
            root.put("end", query["end"]);
            pqxx::work worker(dbManager.GetConn());
            res = worker.exec_prepared("getExpense", query["begin"], query["end"]);
            worker.commit();
        } else {
            int id = boost::lexical_cast<int>(query["id"]);

            if (!recordExists(id, "expenses")) {
                throw std::exception("Expense doesn't exist");
            }
            pqxx::work worker(dbManager.GetConn());
            res = worker.exec_prepared("findExpense", id);
            worker.commit();
        }

        root.add_child("expenses", toJson(res));
        std::stringstream data;
        boost::property_tree::write_json(data, root);
        jsonResponse(data.str());
    } catch (boost::bad_lexical_cast &e) {
        badRequest("ID must be an integer");
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::getIncome() {
    // возвращает инф-ю о доходе ЛИБО инф-ю о всех доходах за период
    try {
        if (!req.target().starts_with("/income?")) {
            throw std::exception("Incorrect query");
        }

        pqxx::result res;
        boost::property_tree::ptree root;
        auto query = parseQuery();
        if (!query.contains("id")) {
            if (!query.contains("begin") && !query.contains("end")) {
                throw std::exception("Incorrect query");
            }
            root.put("begin", query["begin"]);
            root.put("end", query["end"]);
            pqxx::work worker(dbManager.GetConn());
            res = worker.exec_prepared("getIncome", query["begin"], query["end"]);
            worker.commit();
        } else {
            int id = boost::lexical_cast<int>(query["id"]);

            if (!recordExists(id, "income")) {
                throw std::exception("Income doesn't exist");
            }
            pqxx::work worker(dbManager.GetConn());
            res = worker.exec_prepared("findIncome", id);
            worker.commit();
        }

        root.add_child("income", toJson(res));
        std::stringstream data;
        boost::property_tree::write_json(data, root);
        jsonResponse(data.str());
    } catch (boost::bad_lexical_cast &e) {
        badRequest("ID must be an integer");
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::getByCategory() {
    // возвращает инф-ю о тратах/доходах в категории за период
    try {
        if (!req.target().starts_with("/categories/expenses?") && !req.target().starts_with("/categories/income?")) {
            throw std::exception("Unknown type of categories");
        }

        pqxx::result res;
        boost::property_tree::ptree root;
        auto query = parseQuery();
        if (query.contains("id")) {
            int id = boost::lexical_cast<int>(query["id"]);
            if (query.contains("begin") && query.contains("end")) {
                root.put("id_cat", query["id"]);
                root.put("begin", query["begin"]);
                root.put("end", query["end"]);
                if (req.target().starts_with("/categories/expenses?")) {
                    if (!recordExists(id, "expense_categories")) {
                        throw std::exception("Category doesn't exist");
                    }
                    pqxx::work worker(dbManager.GetConn());
                    res = worker.exec_prepared("getByExpenseCategory", id, query["begin"], query["end"]);
                    worker.commit();
                    root.add_child("expenses", toJson(res));
                } else {
                    if (!recordExists(id, "income_categories")) {
                        throw std::exception("Category doesn't exist");
                    }
                    pqxx::work worker(dbManager.GetConn());
                    res = worker.exec_prepared("getByIncomeCategory", id, query["begin"], query["end"]);
                    worker.commit();
                    root.add_child("income", toJson(res));
                }
            } else {
                throw std::exception("Incorrect query");
            }
        } else {
            throw std::exception("Incorrect query");
        }

        std::stringstream data;
        boost::property_tree::write_json(data, root);
        jsonResponse(data.str());
    } catch (boost::bad_lexical_cast &e) {
        badRequest("ID must be an integer");
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::deleteAccount() {
    try {
        if (!req.target().starts_with("/accounts?")) {
            throw std::exception("Incorrect query");
        }

        auto query = parseQuery();

        if (!query.contains("id")) {
            throw std::exception("Incorrect query");
        }
        int id = boost::lexical_cast<int>(query["id"]);

        if (!recordExists(id, "bank_accounts")) {
            throw std::exception("Account doesn't exist");
        }

        pqxx::work worker(dbManager.GetConn());
        worker.exec_prepared("deleteAccount", id);
        worker.commit();
        successResponse(http::status::ok);
    } catch (boost::bad_lexical_cast &e) {
        badRequest("ID must be an integer");
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::deleteExpense() {
    try {
        if (!req.target().starts_with("/expenses?")) {
            throw std::exception("Incorrect query");
        }

        auto query = parseQuery();

        if (!query.contains("id")) {
            throw std::exception("Incorrect query");
        }
        int id = boost::lexical_cast<int>(query["id"]);

        if (!recordExists(id, "expenses")) {
            throw std::exception("Expense doesn't exist");
        }

        pqxx::work worker(dbManager.GetConn());
        worker.exec_prepared("deleteExpense", id);
        worker.commit();
        successResponse(http::status::ok);
    } catch (boost::bad_lexical_cast &e) {
        badRequest("ID must be an integer");
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::deleteIncome() {
    try {
        if (!req.target().starts_with("/income?")) {
            throw std::exception("Incorrect query");
        }

        auto query = parseQuery();

        if (!query.contains("id")) {
            throw std::exception("Incorrect query");
        }
        int id = boost::lexical_cast<int>(query["id"]);

        if (!recordExists(id, "income")) {
            throw std::exception("Income doesn't exist");
        }

        pqxx::work worker(dbManager.GetConn());
        worker.exec_prepared("deleteIncome", id);
        worker.commit();
        successResponse(http::status::ok);
    } catch (boost::bad_lexical_cast &e) {
        badRequest("ID must be an integer");
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

void Connection::deleteCategory() {
    try {
        if (!req.target().starts_with("/categories/expenses?") && !req.target().starts_with("/categories/income?")) {
            throw std::exception("Unknown type of categories");
        }

        auto query = parseQuery();

        if (!query.contains("id")) {
            throw std::exception("Incorrect query");
        }
        int id = boost::lexical_cast<int>(query["id"]);

        if (req.target().starts_with("/categories/expenses?")) {
            if (!recordExists(id, "expense_categories")) {
                throw std::exception("Category doesn't exist");
            }
            if (id == OTHER_CATEGORY_ID) {
                throw std::exception("This is a service category, it can't be edited");
            }
            pqxx::work worker(dbManager.GetConn());
            worker.exec_prepared("changeExpenseCategoryOther", id);
            worker.exec_prepared("deleteExpenseCategory", id);
            worker.commit();
        } else {
            if (!recordExists(id, "income_categories")) {
                throw std::exception("Category doesn't exist");
            }
            if (id == OTHER_CATEGORY_ID) {
                throw std::exception("This is a service category, it can't be edited");
            }
            pqxx::work worker(dbManager.GetConn());
            worker.exec_prepared("changeIncomeCategoryOther", id);
            worker.exec_prepared("deleteIncomeCategory", id);
            worker.commit();
        }
        successResponse(http::status::ok);
    } catch (boost::bad_lexical_cast &e) {
        badRequest("ID must be an integer");
    } catch (std::exception &e) {
        badRequest(e.what());
    }
}

std::unordered_map<std::string, std::string> Connection::parseQuery() {
    std::unordered_map<std::string, std::string> query;
    auto start = req.target().find("?");
    while (start != std::string::npos) {
        start++;
        auto end = req.target().find("&", start);
        auto pos = req.target().find("=", start);
        std::string key = req.target().substr(start, pos - start);
        std::string value = req.target().substr(pos + 1, end - pos - 1);
        query[key] = value;
        start = end;
    }
    if (query.empty()) {
        throw std::exception("Empty query");
    }
    return query;
}

bool Connection::recordExists(int id, const std::string& tableName) {

    try {
        pqxx::work worker(dbManager.GetConn());
        pqxx::result result;
        if (tableName == "income_categories") {
            result = worker.exec_prepared("findIncomeCategory", id);
        } else if (tableName == "expense_categories") {
            result = worker.exec_prepared("findExpenseCategory", id);
        } else if (tableName == "expenses") {
            result = worker.exec_prepared("findExpense", id);
        } else if (tableName == "income") {
            result = worker.exec_prepared("findIncome", id);
        } else {
            result = worker.exec_prepared("findAccount", id);
        }

        worker.commit();

        if (result.size() == 1) {
            return true;
        } else {
            return false;
        }
    } catch (std::exception &e) {
        std::cerr << e.what();
        return false;
    }
}

boost::property_tree::ptree Connection::toJson(pqxx::result &res) {
    boost::property_tree::ptree ptree;
    for (pqxx::result::size_type i = 0; i < res.size(); ++i) {
        boost::property_tree::ptree child;
        for (pqxx::row::size_type j = 0; j < res[0].size(); ++j) {
            child.put(res.column_name(j), res[i][j]);
        }
        ptree.push_back(std::make_pair("", child));
    }
    return ptree;
}

