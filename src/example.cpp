#include "scheduler.h"
#include "tasks.h"

int main(int arc, char* argv[])
{
    Scheduler scheduler;
    scheduler.add_task(Tasks::virtual_mem_diagnostic, 1, "virtual_memory",
                       std::vector<std::string> { "consumed_memory",
                                                  "total_memory" });
    scheduler.add_task(Tasks::physical_mem_diagnostic, 1, "physical_memory",
                       std::vector<std::string> { "consumed_memory" });

    while(true);
}
