#pragma once
#include <string>
#include "Store.h"

class Product : public StoreItem
{ 
    // give access to default constructor
    friend class Store<Product>;

    public:
        Product (char* name, double price) {
            std::strcpy(m_name, name);
            m_price = price;
        }
        
        // should be used for operator= as well
        Product (const Product& other): StoreItem(other) {
            copy(other);
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

    protected:
        Product() {} // make our life easier

    private:
        void copy (const Product& product) {
            strcpy(m_name, product.m_name);
            m_price = product.m_price;
        }

        /*
         * Data
         */
        char m_name[200];
        double m_price;
};
