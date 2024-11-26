#include <crow.h>
#include <sqlite3.h>
#include <iostream>

// Function to establish a database connection
sqlite3* getDatabaseConnection() {
    sqlite3* db;
    int rc = sqlite3_open("reservations.db", &db);
    if (rc) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        exit(EXIT_FAILURE);
    }
    return db;
}

int main() {
    crow::SimpleApp app;

    // Route to retrieve available times for a table
    CROW_ROUTE(app, "/api/available-times")
    .methods("GET"_method)([](const crow::request& req) {
        auto query = crow::query_string(req.url_params);
        int table_id = std::stoi(query.get("table_id"));

        sqlite3* db = getDatabaseConnection();
        sqlite3_stmt* stmt;
        std::string sql = "SELECT time_slot FROM reservations WHERE table_id = ? AND status = 'available'";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, table_id);

        std::vector<std::string> available_times;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            available_times.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);

        crow::json::wvalue response;
        response["available_times"] = available_times;
        return crow::response(response);
    });

    // Route to reserve a time slot
    CROW_ROUTE(app, "/api/reserve")
    .methods("POST"_method)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        int table_id = json["table_id"].i();
        std::string time_slot = json["time_slot"].s();

        sqlite3* db = getDatabaseConnection();
        sqlite3_stmt* stmt;
        std::string sql = "UPDATE reservations SET status = 'reserved' WHERE table_id = ? AND time_slot = ? AND status = 'available'";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, table_id);
        sqlite3_bind_text(stmt, 2, time_slot.c_str(), -1, SQLITE_STATIC);

        crow::json::wvalue response;
        if (sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0) {
            response["success"] = true;
            response["message"] = "Reservation confirmed";
        } else {
            response["success"] = false;
            response["message"] = "Failed to reserve. Slot may already be taken.";
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return crow::response(response);
    });

    // Route to cancel a reservation
    CROW_ROUTE(app, "/api/cancel")
    .methods("POST"_method)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        int table_id = json["table_id"].i();
        std::string time_slot = json["time_slot"].s();

        sqlite3* db = getDatabaseConnection();
        sqlite3_stmt* stmt;
        std::string sql = "UPDATE reservations SET status = 'available' WHERE table_id = ? AND time_slot = ? AND status = 'reserved'";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, table_id);
        sqlite3_bind_text(stmt, 2, time_slot.c_str(), -1, SQLITE_STATIC);

        crow::json::wvalue response;
        if (sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(db) > 0) {
            response["success"] = true;
            response["message"] = "Reservation cancelled";
        } else {
            response["success"] = false;
            response["message"] = "Failed to cancel reservation.";
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return crow::response(response);
    });

    std::cout << "Crow server is running on http://localhost:8080" << std::endl;
    app.port(8080).multithreaded().run();

    return 0;
}
