# probe-boss
Periodic task scheduler for monitoring internal/external systems with the puporse of performing statistical analysis on aggregated and correlated data. Currently implements only system probing, however the scheduler is easily extendable to also include network probing. All aggregates are stored in an SQLite database.

## Usage
First instantiate a scheduler
```cpp
Scheduler scheduler;
```
Then simply add your needed probes
```cpp
scheduler.add_task(task, interval, table_name, column_names);
```
For a concrete implementation see `src/example.cpp`.

## Building
Compile with
```
g++ *.cpp -std=c++11 -lsqlite3 -pthread -DTHREADSAFE=2
```
For instructions on compiling SQLite3 please see [this.](https://www.sqlite.org/howtocompile.html)

## Todo
+ Generalize API to allow arbitrary return types using std::bind
+ Handle potential SQLite errors
+ More diagnostics/probes like...
    + Time to connect to a TCP server
    + ICMP ping time to a server
    + Total packets sent and received on a specific network interface
