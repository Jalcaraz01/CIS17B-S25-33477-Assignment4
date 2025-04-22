//Lesson 4, Justin Alcaraz
#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>
using namespace std;

//This exception class defines an exception for duplicate item cases
class DuplicateItemException : public std::runtime_error {
private:
    string duplicate_error;
public:
    DuplicateItemException(const std::string& msg) : std::runtime_error(msg) {}
};

//This exception class defines an exception to be thrown in case of an item not being found in a collection
class ItemNotFoundException : public std::runtime_error {
public:
    ItemNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};

//StoredItem represents an item in storage inventory that has its own ID, Description, and Location
class StoredItem {
private:
    std::string id;
    std::string description;
    std::string location;

public:
    StoredItem(std::string id, std::string desc, std::string loc)
        : id(id), description(desc), location(loc) {}

    std::string getId() const { return id; }
    std::string getDescription() const { return description; }
    std::string getLocation() const { return location; }
};

//StorageManager contains member functions that add, find, remove, and display items in the itemById and itemByDescription collections
class StorageManager {
private:
    std::unordered_map<std::string, std::shared_ptr<StoredItem>> itemById;
    std::map<std::string, std::shared_ptr<StoredItem>> itemByDescription;
    
public:
    void addItem(const std::shared_ptr<StoredItem>& item) {
        //Adds item to both maps and throws exception if ID already exists
        try{
            auto it{itemById.cbegin()};
            while (it != itemById.cend()){
                if(it->first == item->getId()){
                    throw(DuplicateItemException("Item with ID " + item->getId() + " already exists!"));
                }
                ++it;
            }
            itemByDescription.insert(std::make_pair(item->getDescription(), item));
            itemById.insert(std::make_pair(item->getId(), item));
        }
        catch(const DuplicateItemException& Exception){
            cout << "Error: " << Exception.what() << endl;
        }
    }
    
    std::shared_ptr<StoredItem> findById(const std::string& id) const {
        //Returns item if found, if not, throws ItemNotFoundException and returns nullptr
        try{
            auto it{itemById.cbegin()};
            while (it != itemById.cend()){
                if (it->first == id){
                    return it->second;
                }
                ++it;
            }
            throw(ItemNotFoundException("Item with ID " + id + " not found!"));
        }
        catch(ItemNotFoundException& Exception){
            cout << "Error: " << Exception.what() << endl;
            
        }
        return nullptr;
    }
    
    void removeItem(const std::string& id) {
        //Removes item from both maps, throws exception if item ID is not found
        try{
            int counter = 0;
            auto it{itemById.cbegin()};
            while (it != itemById.cend()){
                if (it->first == id){
                    it = itemById.erase(it);
                    counter += 1;
                }
                ++it;
            }
            auto i{itemByDescription.cbegin()};
            while (i != itemByDescription.cend()){
                if ((i->second)->getId() == id){
                    i = itemByDescription.erase(i);
                    counter += 1;
                }
                ++i;
            }
            if (counter == 0){
                throw (ItemNotFoundException("Item with ID " + id + " not found!"));
            }
        }
        catch(ItemNotFoundException& Exception){
            cout << "Error: " << Exception.what() << endl;
        }
    }
    
    void listItemsByDescription() const {
        // Iterates over itemByDescription and displays item info in alphabetical description order
        cout << "Items in Description Order: " << endl;
        auto it{itemByDescription.cbegin()};
        while (it != itemByDescription.cend()){
            cout << "- " << it->first << ": ID: " << (it->second)->getId() << " Location: " << (it->second)->getLocation() << endl;
            ++it;
        }
    }
};

void testDuplicateAddition(StorageManager &storage){
    // Test function used to test the duplicate exception
    auto item1 {std::make_shared<StoredItem>("ITEM001", "LAMP", "Aisle 4, Shelf 2")};
    cout << "Attemping to add ITEM001 again..." << endl;
    storage.addItem(item1);
}

void testItemNotFound(StorageManager &storage){
    // Test function used to test the item not found exception
    auto item4 {std::make_shared<StoredItem>("ITEM000", "NONE", "NO LOCATION")};
    cout << "Removing " << item4->getId() << "..." << endl;
    storage.removeItem(item4->getId());
}

int main() {
    //Main function contains creation of items through use of std::make_shared and several printing statements/function calls displaying the functionality of the program to the user
    //Test functions are also called in order to display the exception handling in cases of duplicate or nonexistant items.
    StorageManager Storage;
    auto item1 {std::make_shared<StoredItem>("ITEM001", "LAMP", "Aisle 4, Shelf 2")};
    auto item2 {std::make_shared<StoredItem>("ITEM002", "LEDS", "Aisle 2, Shelf 4")};
    auto item3 {std::make_shared<StoredItem>("ITEM003", "APPLE", "Aisle 1, Shelf 4")};
    
    cout << "Adding item: " << item1->getId() << " - " << item1->getDescription() << endl;
    Storage.addItem(item1);
    
    cout << "Adding item: " << item2->getId() << " - " << item2->getDescription() << endl;
    Storage.addItem(item2);
    
    cout << "Adding item: " << item3->getId() << " - " << item3->getDescription() << endl;
    Storage.addItem(item3);
    
    cout << "Retrieving " << item2->getId() << "..." << endl;
    auto found_item = Storage.findById(item2->getId());
    cout << "Found: " << found_item->getDescription() << " at " << found_item->getLocation() << endl;
    
    testItemNotFound(Storage);
    testDuplicateAddition(Storage);
    
    Storage.listItemsByDescription();
}

