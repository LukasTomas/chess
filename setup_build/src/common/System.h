#pragma once

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

namespace systemStats {

    inline unsigned long long GetAvailableMemory() {
        long pages = sysconf(_SC_PHYS_PAGES);
        long page_size = sysconf(_SC_PAGE_SIZE);
        return pages * page_size;
    }


// Function to get free memory size from /proc/meminfo
    inline long GetFreeMemorySize() {
        std::ifstream meminfo("/proc/meminfo");
        if (!meminfo.is_open()) {
            std::cerr << "Failed to open /proc/meminfo" << std::endl;
            return -1;
        }

        std::string line;
        while (std::getline(meminfo, line)) {
            if (line.find("MemFree:") != std::string::npos) {
                // Extracting the value of MemFree from the line
                long freeMemorySize;
                if (std::sscanf(line.c_str(), "MemFree: %ld kB", &freeMemorySize) == 1) {
                    // Convert from kilobytes to bytes
                    freeMemorySize *= 1024;
                    return freeMemorySize;
                } else {
                    std::cerr << "Failed to parse MemFree value" << std::endl;
                    return -1;
                }
            }
        }

        std::cerr << "MemFree not found in /proc/meminfo" << std::endl;
        return -1;
    }

};
