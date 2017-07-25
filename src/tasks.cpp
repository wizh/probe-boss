#include "tasks.h"

std::vector<int> Tasks::virtual_mem_diagnostic()
{
    // sysinfo returns memory in bytes
    struct sysinfo mem_info;
    sysinfo (&mem_info);

    int consumed_mem =
        (mem_info.totalram - mem_info.freeram +
         mem_info.totalswap - mem_info.freeswap) * mem_info.mem_unit /
                                                   1000000;

    int total_virtual_mem =
        (mem_info.totalram + mem_info.totalswap) * mem_info.mem_unit /
                                                   1000000;

    return std::vector<int> { consumed_mem, total_virtual_mem };
}

std::vector<int> Tasks::physical_mem_diagnostic()
{
    // Assume memory usage in file /proc/self/stauts with format:
    // VmRSS: xxxxxxxx kB
    std::string mem_line = parse_mem_file();

    mem_line = mem_line.substr(6, 10);
    mem_line.erase(std::remove(mem_line.begin(), mem_line.end(), ' '),
                   mem_line.end());

    return std::vector<int> { std::stoi(mem_line) };
}

std::string Tasks::parse_mem_file()
{
    FILE* mem_file = fopen("/proc/self/status", "r");
    char line[64];

    while (fgets(line, 64, mem_file) != NULL) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            break;
        }
    }

    fclose(mem_file);
    return (std::string) line;
}
