#ifndef FILEPREVIEWER_H
#define FILEPREVIEWER_H

#include<iostream>
#include<string>
#include<fstream>
#include<array>


class FileManager
{
private:

    // inline init as it is of `const` type
    static const char censorSymbol = '*';
    static const unsigned MAX_CHUNK_SIZE = 2000; // do not show more than 2000 symbols


    std::fstream* m_file_stream;

    char chunkBuffer[MAX_CHUNK_SIZE]; // 
    int m_currentChunk = 0; // how much lines of the files have we read?
    int m_currentChunkSize = 0;

public:
    FileManager(std::fstream* file);
    virtual ~FileManager() {};
    
    /*
     * Commands
     */
    const char* getCurrentChunk() const;
    void printNextChunkWithGetline(int chunkSize); // some naive function showing the usage of getline
    void getNextChunk(int maxShownSymbols);    // gives sequent part of the file

    // TODO
    void getFirtChunk(); // used for go to begining
    void getLastChunk(); // used for go to end

    void itrativeShow();
    
    /*
     * All of these functions work on the chunkBuffer
     */

    int findWordIdx(char* word); // find word position

    void censorWord(char* word); // first occurrence
    void deleteWord(char* word); // first occurrence
    void deleteLine(int& lineNumber);
    void replaceWord(char* work, char* replaceWord);

    void quit();
};

#endif /* FILEPREVIEWER_H */
