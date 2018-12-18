#pragma once
#include <string>
#include "Store.h"

class Product
{
    // give access to default constructor
    friend class Store<Product>;

    
    public:
        Product (char* name, double price) {
            std::strcpy(m_name, name);
            m_price = price;
            m_id = INVALID_ID;
        }
        
        // should be used for operator= as well
        Product (const Product& other) {
            copy(other);
        }

        int getId () const {
            return m_id;
        }

        const char* getName () const {
            return m_name;
        }

        double getPrice () const {
            return m_price;
        }

        void getPrice (double& price) {
            m_price = price;
        }

        bool isSaved () {
            return m_id > 0;
        }

    protected:
        // create a placeholder object
        Product() {
            m_id = INVALID_ID;
        }

        // Only the Store should be able assign ids
        void setId (int id) {
            m_id = id;
        }

    private:
        void copy (const Product& product) {
            m_id = product.m_id; 
            strcpy(m_name, product.m_name);
            m_price = product.m_price;
        }

        /*
         * Data
         */
        int m_id;
        char m_name[200];
        double m_price;

        // Service field for deleted items
        bool m_isDeleted;
};
