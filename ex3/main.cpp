#include"FileManager.h"
#include "../utils/fileUtils.h"
#include<string>
#include<fstream>


void test_censorWord() {
    // create test file
    char testFilename[] = "censorWord.txt";
    char testText[] = "some more text and we will be alright";
    char resultTestText[] = "some more **** and we will be alright";

    // Prepare file
    futils::writeFile(testFilename, testText);


    // Test cases
    std::fstream os_tstFile(testFilename, std::fstream::in | std::fstream::out);

    FileManager previewer(&os_tstFile);

    previewer.getNextChunk(200);
    previewer.censorWord("text");

    os_tstFile.close();

    std::string censoredContent = futils::readFile(testFilename);

    std::cout << "'censorWord' should change the file properly: " << (strcmp(censoredContent.c_str(), resultTestText) == 0) << std::endl;


    // cleanup (tear down)
    remove(testFilename);
}

void test_findWord() {
    char filename[] = "findWordTest.txt";
    char testData[] = "I am some simple\nmulti-line message.";

    // Prepare file
    futils::writeFile(filename, testData);


    // Test cases
    std::fstream chunkFile_s(filename, std::ios::binary | std::ios::in | std::ios::out);
    // we know that the file is there
    FileManager fpreview(&chunkFile_s);

    // NOTE it needs a chunk to be loaded
    fpreview.getNextChunk(100);
    int missingIndex = fpreview.findWordIdx("missing");
    std::cout << "'findWordIdx' should return -1 when item is missing: " << (missingIndex == -1) << std::endl;

    int fileindex = fpreview.findWordIdx("message");
    std::cout << "'findWordIdx' should return proper index: " << (fileindex == 28) << std::endl;

    chunkFile_s.close();

    // cleanup
    remove(filename);
}

void test_getChunk() {
    char filename[] = "chunkTest.txt";
    char testData[] = "a bc defg.\nnew line.gdsg";
    // Prepare file (set up)
    futils::writeFile(filename, testData);


    // Test cases
    std::fstream chunkFile_s(filename, std::ios::binary | std::ios::in | std::ios::out);
    // we know that the file is there
    FileManager fpreview(&chunkFile_s);

    testData[20] = '\0'; // shorten the string

    fpreview.getNextChunk(20);
    std::cout << "'getChunk' should get only the given amount of symbols: " << (strcmp(fpreview.getCurrentChunk(), testData) == 0) << std::endl;

    chunkFile_s.close();

    // cleanup (tear down)
    remove(filename);
}

int main() {
    test_getChunk();
    test_findWord();
    test_censorWord();

    return 0;
}
