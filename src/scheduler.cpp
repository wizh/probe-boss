#include "scheduler.h"

Scheduler::Scheduler() { }

void Scheduler::add_task(std::vector<int> (*task)(), unsigned int interval,
                         std::string table_name,
                         std::vector<std::string> column_names)
{
    std::thread([this, task, interval, table_name, column_names]() {
        sqlite3 *db;
        sqlite3_open("scheduler.db", &db);

        create_table(db, table_name, column_names);

        while (true)
        {
            std::vector<int> res = task();
            update_database(db, table_name, column_names, res);

            std::this_thread::sleep_for(std::chrono::seconds(interval));
        }
    }).detach();
}

void Scheduler::create_table(sqlite3 *db, std::string table_name,
                             std::vector<std::string> column_names)
{
    int rc;
    std::string sql;

    sql = "CREATE TABLE IF NOT EXISTS " + table_name + "(";

    // Create column for each attribute and its associated aggregate values
    for (auto column_name : column_names) {
        sql += "\n" + column_name + " INT NOT NULL,";
        sql += "\n" + column_name + "_min INT NOT NULL,";
        sql += "\n" + column_name + "_max INT NOT NULL,";
        sql += "\n" + column_name + "_avg INT NOT NULL,";
    }

    sql.erase(sql.size()-1);
    sql += " );";

    bool try_exec = true;
    while (try_exec)
    {
        rc = sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);
        if (rc == SQLITE_BUSY) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } else {
            try_exec = false;
        }
    }
}

void Scheduler::update_database(sqlite3 *db, std::string table_name,
                                std::vector<std::string> column_names,
                                std::vector<int> values)
{
    int rc;
    std::string sql;

    sql = "INSERT INTO " + table_name + " (";

    for (auto column_name : column_names) {
        sql += column_name + ", ";
        sql += column_name + "_min" + ", ";
        sql += column_name + "_max" + ", ";
        sql += column_name + "_avg" + ", ";
    }

    sql.erase(sql.size()-2);
    sql += ") VALUES (";

    for (std::vector<int>::size_type i = 0; i != column_names.size(); i++) {
        std::string column_name = column_names[i];
        sql += std::to_string(values[i]) + ", ";
        sql += get_aggregate(db, "min", table_name, column_name) + ", ";
        sql += get_aggregate(db, "max", table_name, column_name) + ", ";
        sql += get_aggregate(db, "avg", table_name, column_name) + ", ";
    }

    sql.erase(sql.size()-2);
    sql += ");";

    bool try_exec = true;
    while (try_exec)
    {
        rc = sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);
        if (rc == SQLITE_BUSY) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } else {
            try_exec = false;
        }
    }
}

std::string Scheduler::get_aggregate(sqlite3 *db, std::string type,
                                     std::string table_name,
                                     std::string column_name)
{
    int rc;
    sqlite3_stmt *stmt;
	std::string sql = "select " + type + "(" + column_name + ") from " +
					  table_name + ";";

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    bool try_step = true;
    while (try_step)
    {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_BUSY) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } else {
            try_step = false;
        }
    }

    int aggregate = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

	return std::to_string(aggregate);
}
