#pragma once
#include <fstream>
#include <iostream>

const short INVALID_ID = -1;

bool fileExists(const char* name) {
    // the stream will be closed with the destruction of the fstream variable (close is not needed)
    return std::ifstream(name).good();
}

template <class T>
class Store
{
    public:
        struct StoreMetadata;

        StoreMetadata getMetadata() const {
            return m_metadata;
        }

    public:
        Store (std::string dbName) {
            m_dbFilename = dbName;
        }

        ~Store() {
            // nothing is needed as once the stream variable is destroyed,
            // the connection to the file will be closed as well.
        }

        /**
         * Opens connection to the db file and loads previously added data from it.
         * In case the provided db file is missing such is created.
         */
        bool init () {
            if (!fileExists(m_dbFilename.c_str())) {
                if (initDbFile()) {
                    std::cout << "Fresh Database file created successfully!" << std::endl;
                } else {
                    std::cout << "Problem creating the db file. There's no chance of opening it as well" << std::endl;
                    return false;
                }
            } else {
                std::cout << "Loading existing database";
            }

            // Now open the existing db file
            db_ios.open(m_dbFilename, std::ios::in | std::ios::out | std::ios::binary);

            if (!db_ios) {
                std::cout << "Problem opening the db file" << std::endl;
                return false;
            }

            if (loadMetadata()) {
                std::cout << "Database metadata loaded successfully!" << std::endl;
            } else {
                std::cout << "Problem loading file metadata. Metadata might be corrupted. Or the file is of different type." << std::endl;
                // we could also simple
                return false;
            }

            return true;
        }

        /**
         * Explicitly closes closes the connection to the file.
         */
        void close () {
            db_ios.close();
        }

        /**
         * Adds an item at the end of the database file in case the item hadn't
         * been saved yet (it is missing an id).
         * In case it is a fresh object a unique id will be generated and set to it.
         */
        void add (T& item) {
            if (item.getId() != INVALID_ID) {
                // the object is already persisted
                return;
            }

            item.setId(getNextId());
            // We always add items to the end of the file
            db_ios.seekp(0, std::ios::end);
            db_ios.clear(); // the previous row triggers eof

            db_ios.write((char*) &item, sizeof(item));

            // added successfully?
            if (db_ios) {
                m_metadata.m_size++;
                writeMetadata();
            } else {
                std::cout << "Problem saving item to file." << std::endl;
            }
        }


        void update (T& item) {
            // not needed for now
        }

        /**
         * Marks an item as removed.
         * @returns Whether the operation was successful
         */
        bool softRemove (int id) {
            T item;
            int itemIdx = findById(id, item),
                itemPtr = getItemFilePtr(itemIdx);

            if (itemIdx != INVALID_ID) {
                item.m_isDeleted = true;

                // write the deleted value to the file
                db_ios.seekp(itemPtr, std::ios::beg);
                db_ios.write((char*) &item, sizeof(item));
            }
        }

        /**
         * Truly remove an item from the store.
         * @returns  whether the delete operation was successful
         */
        bool hardRemove (int id) {
            T deletedItem;
            int deletedItemIdx = findById(id, deletedItem);

            if (deletedItemIdx != INVALID_ID) {
                return hardRemoveItemByIdx(deletedItemIdx);
            }
        }

        /**
         * Find an element by its name.
         * XXX it would be better if this becomes more generic
         */
        T find (char* name) {
            T foundItem;
            for (int i = 0; i < m_metadata.m_size; ++i) {
                getAtIdx(i, foundItem);
                if (!strcmp(foundItem.getName(), name)) {
                    return foundItem;
                }
            }
            return T();
        }

        /**
         * Used to ensure that write operations write to the OS.
         * Useful for tests
         */
        void forcePersist () {
            db_ios << std::flush;
        }


    protected:
        /**
         * Finds an element in the database by its id.
         * @returns The index in the database
         */
        int findById (int id, T& foundItem) {
            for (int i = 0; i < m_metadata.m_size; ++i) {
                getAtIdx(i, foundItem);
                if (foundItem.m_id == id) {
                    return i;
                }
            }
            return INVALID_ID;
        }

        /**
         * Generate the exact position in the file for a specific index.
         */
        int getItemFilePtr (int idx) {
            return sizeof(m_metadata) + sizeof(T) * idx;
        }
        /**
         * Retrieves the element at a specified index from the database.
         * @return The position of the item
         */
        int getAtIdx (int idx, T& item) {
            // Skip the part reserved for the metadata
            int itemPointer = sizeof(m_metadata) + sizeof(T) * idx;
            db_ios.seekg(itemPointer, std::ios::beg);
            db_ios.read((char*) &item, sizeof(item));

            // We might reach the end of the file or provide invalid index
            if (!db_ios) {
                db_ios.clear();
            }
        }

        /**
         * Fully removes an element from the store by moving all the db data
         * to a new file, omitting the one that is to be removed.
         */
        bool hardRemoveItemByIdx(int excludedItemIdx) {
            // exclude item from db
            // quite inefficient
            std::string tmpFileName = m_dbFilename + ".tmp";
            std::ofstream tmpDb_os(tmpFileName);

            if (!tmpDb_os) {
                std::cout << "Problem creating temporary db file." << std::endl;
                return false;
            }

            tmpDb_os.write((char*)& m_metadata, sizeof(m_metadata));
            T tmpItem;
            bool success = true;
            for (int i = 0; i < m_metadata.m_size && success; ++i) {
                if (i == excludedItemIdx) continue;

                getAtIdx(i, tmpItem); // item should exist
                // TODO ensure all write operations pass
                success = !!tmpDb_os.write((char*)& tmpItem, sizeof(tmpItem));
            }

            if (success) {
                remove(m_dbFilename.c_str());
                success = rename(tmpFileName.c_str(), m_dbFilename.c_str());
            } else { // bailout in case something has gone wrong
                remove(tmpFileName.c_str());
            }

            // The rename operation might have failed
            // it succeeds with a 0 - http://www.cplusplus.com/reference/cstdio/rename/
            if (!success) {
                m_metadata.m_size--;
                writeMetadata();
            } else {
                std::cout << "Unsuccessful renaming for the updated db file. Code: " << success << std::endl;
            }
        }

        /**
         * Ensures the db file exists.
         * In case it is missing a fresh one is created
         */
        bool initDbFile () {
            // create the file
            db_ios.open(m_dbFilename, std::ios::out);

            // simply init the metadata block
            // in case there was a problem with file creation, write would also
            // result in an error
            bool success = writeMetadata();
            // close it as we'll need to open it with more permissions
            db_ios.close();

            return success;
        }

        /**
         * Simply writes the current state of the metadata to the file.
         */
        bool writeMetadata () {
            db_ios.seekg(0, std::ios::beg);
            db_ios.write((char*) &m_metadata, sizeof(m_metadata));

            return !!db_ios;
        }

        /**
         * We're keeping some metadata in the file such as the number of elements there.
         */
        bool loadMetadata () {
            // go to the metadata part
            db_ios.seekg(0, std::ios::beg);

            // ... and read it
            db_ios.read((char*) &m_metadata, sizeof(m_metadata));
            return !!db_ios;

            ///  ... another way to achieve the same (if we don't keep any metadata):
            ///  db_ios.seekg(0, std::ios::end);
            ///  m_metadata.m_size = ( (int) db_ios.tellg() ) / sizeof(T);
        }

        int getNextId () {
            return m_metadata.m_currentId++;
        }

    protected:
        /* data */
        // we could use separate variables for in & out streams
        std::fstream db_ios;
        std::string m_dbFilename;

        StoreMetadata m_metadata;
};

// The metadata for the store
template <class T>
struct Store<T>::StoreMetadata {
    // It is more of an example as these fields can be easily computed
    int m_size;
    int m_currentId;

    // any other useful info...
    StoreMetadata () {
        m_size = 0;
        // helps for keeping ids in the Store unique
        m_currentId = 1;
    }
};


/**
 * Enables a class to be saved to the store.
 */
class StoreItem {
    // give access to default constructor
    friend class Store<StoreItem>;

    public:
        int getId () const {
            return m_id;
        }

        bool isSaved () {
            return m_id > 0;
        }


    protected:
        // Only the Store should be able assign ids
        void setId (int id) {
            m_id = id;
        }

        // create a placeholder object
        StoreItem() {
            m_id = INVALID_ID;
        }

        StoreItem(const StoreItem& other) {
            m_id = other.m_id; 
        }


    protected: 
        // Service field for deleted items
        int m_id;
        bool m_isDeleted;
};

