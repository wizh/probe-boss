#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "string.h"
#include <sqlite3.h>
#include <vector>
#include <thread>
#include <chrono>

class Scheduler {
public:
    Scheduler();
    // Add task to scheduler performing specified function each time the
    // specified time interval has passed. Results will be stored in a database
    // with specified table name and column names for each returned value. Each
    // entry will contain returned values for an iteration of the task as well
    // as aggregate values (min, max, average) leading up to that iteration.
    void add_task(std::vector<int> (*task)(), unsigned int interval,
                  std::string table_name,
                  std::vector<std::string> column_names);
private:
    void create_table(sqlite3 *db, std::string table_name,
                      std::vector<std::string> column_names);
    void update_database(sqlite3 *db, std::string table_name,
                         std::vector<std::string> column_names,
                         std::vector<int> values);
    std::string get_aggregate(sqlite3 *db, std::string type,
                              std::string table_name,
                              std::string column_name);
};

#endif
