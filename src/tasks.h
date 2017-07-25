#ifndef TASKS_H
#define TASKS_H

#include "sys/types.h"
#include "sys/sysinfo.h"
#include "string.h"
#include <algorithm>

// Collection of tasks representing diagnostic tools
class Tasks {
 public:
    // Returns current virtual memory usage and maximum capacity
    static std::vector<int> virtual_mem_diagnostic();
    // Returns current physical memory usage
    static std::vector<int> physical_mem_disagnostic();
 private:
    static std::string parse_mem_file();
};

#endif
