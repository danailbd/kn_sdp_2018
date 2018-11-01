#include "FileManager.h"
#include <algorithm>


FileManager::FileManager(std::fstream* file) {
    m_file_stream = file;
}


const char* FileManager::getCurrentChunk() const {
    return chunkBuffer;
}

/*
 * This is only for exercise
 */
void FileManager::printNextChunkWithGetline(int chunkSize) {
    int totalSymbolsRead = 0;
    const int maxRowLength = 30;
    char smallBuffer[maxRowLength];

    while( m_file_stream && totalSymbolsRead < chunkSize ) {
        // we can use .read  to avoid this substitution madness
        int symbolsToCollect = std::min(
                int(maxRowLength), // make it an rvalue
                chunkSize - totalSymbolsRead + 1 // atleast 2
                );
        m_file_stream->getline(smallBuffer, symbolsToCollect);
        totalSymbolsRead += m_file_stream->gcount();
        // or use `strlen(smalBuffer)`

        // as `getline` gets rid of the '\n'
        std::cout << "  " << smallBuffer << std::endl;
    }
    std::cout << "Note the stream state: " << m_file_stream->rdstate() << std::endl;
}

void FileManager::getNextChunk(int chunkSize) {
    m_file_stream->read(chunkBuffer, chunkSize);
    m_currentChunk++;
    m_currentChunkSize = m_file_stream->gcount();
    // we could also use: 
    // m_currentChunkSize = strlen(chunkBuffer)
}

void FileManager::itrativeShow() {
    bool shouldExit = false;
    while ( !shouldExit && !m_file_stream->eof()) {

        // Show next chunk of the file
        getNextChunk(MAX_CHUNK_SIZE);
        // TODO show line number
        std::cout << chunkBuffer;

        if (m_file_stream->eof()) {
            std::cout << "--- File read ---" << std::endl;
        } else {
            // use string to simplify message clear
            std::string promptMsg = "---------- Continue ? --------";
            std::cout << promptMsg;
            char choice;
            std::cin.get(choice);
            /*
             * .get will start work only if we hit enter, which means that
             * Enter will always be hit. The only thing we have to check is
             * what was entered. In case it was nothing, good for us we may continue
             */

            if (choice == 'q') {
                shouldExit = true;
            }
        }
    }
}
 
bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void getNextWord (char* str, char* word) {
    int i;
    for (i = 0; str[i] && isLetter(str[i]); ++i) {
        word[i] = str[i];
    }
    word[i] = '\0';
}


// Searches for a word in the current chunk and returns its position.
// -1 if word not found
int FileManager::findWordIdx(char* word) {
    const int MAX_WORD_SIZE = 50;
    char wordBuffer[MAX_WORD_SIZE];  

    int nextWordBeg = 0,
        chunkSize = strlen(chunkBuffer);
    // get next word
    while (chunkSize > nextWordBeg) {
        getNextWord(chunkBuffer + nextWordBeg, wordBuffer);

        if (strcmp(wordBuffer, word) == 0) {
            return nextWordBeg; // return the current word begining
        }
        
        nextWordBeg += strlen(wordBuffer) + 1 /* we have a ' ' to avoid*/;
    }
    return -1;
}


void FileManager::censorWord(char* word) {
    int wordIdx = findWordIdx(word); // find the word in the current chunk

    int filePos = MAX_CHUNK_SIZE * (m_currentChunk-1) + m_currentChunkSize;
    int putPos = filePos - m_currentChunkSize + wordIdx;
    // place it where it should
    m_file_stream->clear(); // in case we've reached eof
    m_file_stream->seekp(putPos);

    std::string censor(strlen(word), censorSymbol);
    // Override existing word
    *m_file_stream << censor;
}


void FileManager::getFirtChunk() {
    m_file_stream->seekg(0, std::ios::beg);

    getNextChunk(MAX_CHUNK_SIZE);
}

void FileManager::getLastChunk() {
    m_file_stream->seekg(0, std::ios::beg);
    
    int lastChunkPosition = int(m_file_stream->tellg()) - MAX_CHUNK_SIZE;
    // in case the file is really short
    lastChunkPosition = lastChunkPosition > 0 ? lastChunkPosition : 0;
    m_file_stream->seekg(lastChunkPosition);
}

void FileManager::deleteWord(char* word) {
//TODO
} // first occurrence
void FileManager::deleteLine(int& lineNumber) {
//TODO
}
void FileManager::replaceWord(char* work, char* replaceWord) {
//TODO
}

