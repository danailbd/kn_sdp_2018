#pragma once

#include <string>

namespace futils {
    std::string readFile(char* filename);
    void writeFile(std::string filename, std::string contents);
}
