#include<iostream>
#include<fstream>
#include<string>


// a far slower version of read file
void readLine(std::fstream* stream) {
    char symbol;
    while (stream && symbol != '\n') {
        std::cout << (bool) stream->get(symbol);
    }
}


void testSimpleTestFile() {
    // char*
    std::string filename = "simpleText.txt";

    std::ifstream file_is(filename, std::ios::binary | std::ios::in);
    
    if (!file_is.is_open()) { // of simply !s_txtFile
        std::cerr << "No such file found: " << filename;
        return;
    }

    char buffer[300];

    while ( !file_is.eof() ) {
        file_is.getline(buffer, 300);
        std::cout << buffer << std::endl;
    }

    // get char
    char ch;
    file_is.get(ch);

    std::cout << "Single char read: " << ch;

    // get word
    char wordBuffer[60];
    // words are separated by space
    file_is.get(wordBuffer, 60, ' ');

    std::cout << "Word: " << wordBuffer << std::endl;

    // which is the same as
    file_is >> wordBuffer;

    std::cout << "Word: " << wordBuffer << std::endl;

    char lineBuffer[300];
    // get line
    file_is.get(lineBuffer, 300, '\n'); // here \n is the default delimiter

    std::cout << "Line with get: " << lineBuffer << std::endl;

    // which is similar to
    file_is.getline(lineBuffer, 300);

    std::cout << "Line with getline: " << lineBuffer << std::endl;
    

    file_is.close();
}


void testSimleBinaryFileArr() {
    std::string filename = "test.bin";
    std::fstream s_binFile(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
    
    if (!s_binFile.is_open()) {
        std::cerr << "No such file found: " << filename;
        return;
    }

    int a[5] = { 2, 5, 1, 7 };
    int b[5];
    
    std::cout << sizeof(a) << std::endl;
    s_binFile.write((char*)&a, sizeof(a));


    s_binFile.seekg(0);
    s_binFile.read((char*)&b, sizeof(a));

    for (int i = 0; i < 5; ++i) {
        std::cout << a[i] << ", " << std::endl;
    }
    
    s_binFile.close();
}


void testSimpleBinaryFile() {
    // create simple binary file
    // write int
    // read int

    std::string filename = "some.bin";
    std::fstream s_binFile(filename,  std::ios::in | std::ios::out);

    if (!s_binFile.is_open()) {
        std::cerr << "No such file found: " << filename;
        return;
    }

    int x = 5;
    int y = 2;

    std::cout << s_binFile.tellg() << " " << s_binFile.tellp() << std::endl;
    std::cout << "Writing state: " << (bool) s_binFile.write((char*)&x, sizeof(x));

    std::cout << "After write: " << !!s_binFile << std::endl;


    std::cout << s_binFile.tellg() << " " << s_binFile.tellp() << std::endl;

    /* s_binFile.seekg(0, std::ios::beg); */
    /* s_binFile.seekg(s_binFile.tellg(), std::ios::beg); */
    std::cout << s_binFile.tellg() << " " << s_binFile.tellp() << std::endl;
    s_binFile.read((char*)&y, sizeof(x));
    std::cout << s_binFile.tellg() << " " << s_binFile.tellp() << std::endl;

    std::cout << " " << y << " " << sizeof(x) << std::endl;

    s_binFile.close();
}

int main()
{
    /* testSimpleTestFile(); */
    testSimpleBinaryFile();
    /* testSimleBinaryFileArr(); */

    return 0;
}
