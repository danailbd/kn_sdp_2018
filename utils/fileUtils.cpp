#include "fileUtils.h"
#include <fstream>
#include <string>
#include <iostream>

namespace futils {
    std::string readFile(char* filename) {
        std::ifstream file_is(filename);
        
        if (!file_is.is_open()) {
            std::cerr << "No such file found: " << filename;
            return "";
        }
        
        // find the size of the file
        file_is.seekg(0, std::ios::end);

        int fileSize = file_is.tellg();
        // create a big enough string
        std::string contents(fileSize, ' ');

        file_is.seekg(0, std::ios::beg);

        file_is.read(&contents[0], contents.capacity());
        
        file_is.close();

        return contents;
    }

    void writeFile(std::string filename, std::string contents) {
        std::ofstream file_os(filename);
        
        if (!file_os.is_open()) {
            std::cerr << "No such file found: " << filename;
            return;
        }
        
        file_os << contents;
        // or use:
        // `file_os.write((char*)&filename[0], filename.size())`
        
        file_os.close();
        
    }
}
