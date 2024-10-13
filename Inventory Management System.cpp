#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

//Function to validate numerical inputs
double getValidatedInput(const string& prompt, bool isPrice) {
    string input;
    double value;
    
    while (true) {
        cout << prompt;
        getline(cin, input);

        bool isValid = true;
        int decimalCount = 0;
        
        if (isPrice && input.length() > 1 && input[0] == '0' && input[1] != '.') {
            isValid = false;
        }

        for (char c : input) {
            if (!isdigit(c)) {
                if (isPrice && c == '.' && decimalCount == 0) {
                    decimalCount++;  
                } else {
                    isValid = false;  
                    break;
                }
            }
        }
        
        if (isValid) {
            try {
                if (isPrice) {
    				value = stod(input); 
				} else {
    				value = stoi(input); 
				}

                if (value > 0) { 
                    break;
                } else {
                    cout << "\033[31m   Input should be greater than 0.\033[0m\n";
                }
            } catch (const std::invalid_argument& e) {
                cout << "\033[31m   Invalid input! Please enter a valid number.\033[0m\n";
            } catch (const std::out_of_range& e) {
                cout << "\033[31m   Input is out of range! Please enter a valid number.\033[0m\n";
            }
        } else {
            cout << "\033[31m   Invalid input! Please enter a valid number.\033[0m\n";
        }
    }
    return value;
}

//Function to validate if input ID is in digits
bool isDigitsOnly(const string str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false;  
        }
    }
    return true; 
}
//Function to convert user input in capital letters
string convertToUpper(const string& word) {
    string upperWord; 
    for (char letter : word) {
        upperWord += toupper(letter); 
    }
    return upperWord;
}
//Initialize Inventory class
class Inventory; 

class Item {
private:
    string itemID;
    string itemName;
    int itemQty;
    double itemPrice;
    string itemCat;

public:
    Item(string i, string n, int q, double p, string c) 
        : itemID(i), itemName(n), itemQty(q), itemPrice(p), itemCat(c) {}
	
	//Virtual functions
    virtual string validateID(const Inventory* inventory) = 0;
    virtual void displayItems(const Inventory inventory) = 0; 
    
	//Getters
    string getID() const { return itemID; }
    string getName() const { return itemName; }
    int getQty() const { return itemQty; }
    double getPrice() const { return itemPrice; }
    string getCat() const { return itemCat; }

	//Setters to update the quantity and price
    void setQty(int q) { itemQty = q; }
    void setPrice(double p) { itemPrice = p; }
};

class Inventory {
private:
    vector<Item*> allItems;  

public:
	//Function to add a new item to the inventory
    void addToInventory(Item* newItem) {
        allItems.push_back(newItem);
    }
    // Function to retrieve all items in the inventory
    vector<Item*> getAllItems() const {
    	return allItems;
	}
	//Bool function to check if the inventory is empty
    bool isEmpty() const {
    	if (allItems.size() > 0) {
        	return false; 
    	} else {
        	return true; 
    	}
	}
	// Function to display all items in the inventory
    void displayAllItems() const {
    	cout << "\033[32m";
        cout << setw(10) << "ID" 
             << setw(17) << "Name" 
             << setw(13) << "Quantity" 
             << setw(15) << "Price" 
             << setw(15) << "Category" 
             << "\033[0m"
             << endl;
        
        for (const Item* item : allItems) {
            cout << setw(10) << item->getID()
                 << setw(17) << item->getName()
                 << setw(13) << item->getQty()
                 << setw(15) << fixed << setprecision(2) << item->getPrice()
                 << setw(15) << item->getCat()
                 << endl;
        }
    }
	// Function to search for an item by its ID
    Item* searchItem(const string searchID) const {
        string searchLower = convertToUpper(searchID);
        for (const auto& item : allItems) {
            if (convertToUpper(item->getID()) == searchLower){
                return item;
            }
        }
        return nullptr;
    }
    // Function to perform search and display result
    void performSearch(const string& searchID) const {
        Item* foundItem = searchItem(searchID);
        if (foundItem) {
            cout << "\033[32m  Item found!\033[0m\n\n";
            cout << "\tID: " << foundItem->getID() << "\n"
                 << "\tName: " << foundItem->getName() << "\n"
                 << "\tQuantity: " << foundItem->getQty() << "\n"
                 << "\tPrice: " << foundItem->getPrice() << "\n"
                 << "\tCategory: " << foundItem->getCat() << "\n";
        } else {
            cout << "\033[31m  Item not found.\033[0m \n";
        }
    }
	// Function to update item details
    void updateItem(const string& searchID) {
    Item* item = searchItem(searchID); 
    if (item) {
        string choice;
        cout << "   What would you like to modify?\n";
        cout << "   - Quantity\n";
        cout << "   - Price\n";
        do {
            cout << "   Enter your choice (Quantity or Price): ";
            getline(cin,choice);
            choice = convertToUpper(choice);
            if (choice != "QUANTITY" && choice != "PRICE") {
                cout << "\033[31m   Input Quantity or Price.\033[0m \n";
            }
        } while (choice != "QUANTITY" && choice != "PRICE");

        if (choice == "QUANTITY") { 
            int oldQty = item->getQty();
            int newQty;
            do {
                newQty = getValidatedInput("   Enter new quantity: ", false);
                if (newQty == oldQty) {
                    cout << "\033[31m   The new quantity is the same as the old one. Please enter a different value.\033[0m\n";
                }
            } while (newQty == oldQty);

            item->setQty(newQty);
            cout << "\n\033[32m   Quantity of item '" << item->getName() << "' is updated from " << oldQty << " to " << newQty << ".\033[0m\n";
        } else if (choice == "PRICE") {  
            double oldPrice = item->getPrice();
            double newPrice;
            do {
                newPrice = getValidatedInput("   Enter new price: ", true);

                if (newPrice == oldPrice) {
                    cout << "\033[31m   The new price is the same as the old one. Please enter a different value.\033[0m\n";
                }
            } while (newPrice == oldPrice);

            item->setPrice(newPrice);
            cout << "\n\033[32m   Price of item '" << item->getName() << "' is updated from " << oldPrice << " to " << newPrice << ".\033[0m\n";
        }
    } else {
        cout << "\033[31m   Item not found.\033[0m\n";
    }
}

	// Function to remove an item from the inventory
    void removeItem(const string& searchID) {
        for (int i = 0; i < allItems.size(); ++i) {
            if (convertToUpper(allItems[i]->getID()) == convertToUpper(searchID)) {
                delete allItems[i];  
                allItems.erase(allItems.begin() + i);  
                cout << "\033[32m  Item removed successfully.\033[0m\n";
                return;
            }
        }
        cout << "\033[31m  Item not found.\033[0m\n"; 
    }
	// Function to sort items based on a criterion and order 
    void sortItems(string criterion, const string sortType) {
        bool isAscending = (sortType == "ASCENDING");
		// Bubble sort algorithm
        for (int i = 0; i < allItems.size(); ++i) {
    		for (int j = 0; j < allItems.size() - 1 - i; ++j) {
        		bool swapNeeded = false;

        		if (criterion == "QUANTITY") {
            		bool isGreater = allItems[j]->getQty() > allItems[j + 1]->getQty();
            		swapNeeded = (isAscending && isGreater) || (!isAscending && !isGreater);
        		} else if (criterion == "PRICE") {
            		bool isGreater = allItems[j]->getPrice() > allItems[j + 1]->getPrice();
            		swapNeeded = (isAscending && isGreater) || (!isAscending && !isGreater);
        		}

        		if (swapNeeded) {
            		Item* temp = allItems[j];
            		allItems[j] = allItems[j + 1];
            		allItems[j + 1] = temp;
        		}
    		}
		}

        cout << endl;
        displayAllItems(); 
    }
	// Function to display items with stock below a specified threshold
    void displayLowStockItems() const {
        bool found = false;
        for (const Item* item : allItems) {
            if (item->getQty() <= 5) {
                cout << setw(10) << item->getID()
                     << setw(17) << item->getName()
                     << setw(13) << item->getQty()
                     << setw(15) << fixed << setprecision(2) << item->getPrice()
                     << setw(15) << item->getCat()
                     << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "\n\t     No low stock items found.\n";
        }
    }
};

class Clothing : public Item {
private:
    vector<Item*> items; 	

public:
    Clothing(string i, string n, int q, double p, string c) 
        : Item(i, n, q, p, c) {}
        
    //display items in the Clothing category
    void displayItems(const Inventory inventory) override {
    	cout << "   _________________________________________________________\n\n  ";
    	cout << "\t\t\033[38;5;214m    C L O T H I N G  \033[0m\n\n";
    	
        cout << "\033[32m";
        cout << setw(10) << "ID" 
             << setw(17) << "Name" 
             << setw(13) << "Quantity" 
             << setw(15) << "Price" 
             << "\033[0m"
             << endl;

        bool found = false;
        for (const Item* item : inventory.getAllItems()) {
            if (item->getCat() == "CLOTHING") {
                cout << setw(10) << item->getID() 
                     << setw(17) << item->getName()  
                     << setw(13) << item->getQty() 
                     << setw(15) << fixed << setprecision(2) << item->getPrice() 
                     << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "     No items in Clothing.\n";
        }
    }
	//Validate the item ID if it adheres to Clothing ID format
    string validateID(const Inventory* inventory) override {
        string id;
        do {
            cout << "   Enter a valid Item ID (starting with 1 and 4 digits long): ";
            getline(cin,id);
            if (id.length() != 4 || id[0] != '1' || !isDigitsOnly(id)) {
                cout << "\033[31m   Invalid Item ID. Ensure it starts with 1 and is 4 digits long.\033[0m\n";
            } else if (inventory->searchItem(id)) {
                cout << "\033[31m   ID already exists. Enter a unique ID.\033[0m \n";
            }
        } while (id.length() != 4 || id[0] != '1' || !isDigitsOnly(id) || inventory->searchItem(id));
        return id;
    }
};

class Electronics : public Item {
private:
    vector<Item*> items; 	

public:
    // Constructor
    Electronics(string i, string n, int q, double p, string c) 
        : Item(i, n, q, p, c) {}
    //display items in the Electronics category
   void displayItems(const Inventory inventory) override {
   		cout << "   _________________________________________________________\n\n  ";
   		cout << "\t\033[38;5;214m        E L E C T R O N I C S  \033[0m\n\n";
        cout << "\033[32m";
        cout << setw(10) << "ID" 
             << setw(17) << "Name" 
             << setw(13) << "Quantity" 
             << setw(15) << "Price" 
             << "\033[0m"
             << endl;

        bool found = false;
        for (const Item* item : inventory.getAllItems()) {
            if (item->getCat() == "ELECTRONICS") {
                cout << setw(10) << item->getID() 
                     << setw(17) << item->getName()  
                     << setw(13) << item->getQty() 
                     << setw(15) << fixed << setprecision(2) << item->getPrice() 
                     << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "     No items in Electronics.\n";
        }
    }
	//Validate the item ID if it adheres to Electronics ID format
    string validateID(const Inventory* inventory) override {
        string id;
        do {
            cout << "   Enter a valid Item ID (starting with 2 and 4 digits long): ";
            getline(cin,id);
            if (id.length() != 4 || id[0] != '2' || !isDigitsOnly(id)) {
                cout << "\033[31m   Invalid Item ID. Ensure it starts with 2 and is 4 digits long.\033[0m \n";
            } else if (inventory->searchItem(id)) {
                cout << "\033[31m   ID already exists. Enter a unique ID.\033[0m \n";
            }
        } while (id.length() != 4 || id[0] != '2' || !isDigitsOnly(id) || inventory->searchItem(id));
        return id;
    }
};

class Entertainment : public Item {
private:
    vector<Item*> items; 

public:
    Entertainment(string i, string n, int q, double p, string c) 
        : Item(i, n, q, p, c) {}
	//display items in the Entertainment category
    void displayItems(const Inventory inventory) override {
    	cout << "   _________________________________________________________\n\n  ";
    	cout << "\t\033[38;5;214m       E N T E R T A I N M E N T  \033[0m\n\n";
    	cout << "\033[32m";
        cout << setw(10) << "ID" 
             << setw(17) << "Name" 
             << setw(13) << "Quantity" 
             << setw(15) << "Price" 
             << "\033[0m"
             << endl;

        bool found = false;
        for (const Item* item : inventory.getAllItems()) {
            if (item->getCat() == "ENTERTAINMENT") {
                cout << setw(10) << item->getID() 
                     << setw(17) << item->getName()  
                     << setw(13) << item->getQty() 
                     << setw(15) << fixed << setprecision(2) << item->getPrice() 
                     << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "     No items in Entertainment.\n";
        }
    }
	//Validate the item ID if it adheres to Entertainment ID format
    string validateID(const Inventory* inventory) override {
        string id;
        do {
            cout << "   Enter a valid Item ID (starting with 3 and 4 digits long): ";
            getline(cin,id);
            if (id.length() != 4 || id[0] != '3' || !isDigitsOnly(id)) {
                cout << "\033[31m   Invalid Item ID. Ensure it starts with 3 and is 4 digits long.\033[0m \n";
            } else if (inventory->searchItem(id)) {
                cout << "\033[31m   ID already exists. Enter a unique ID.\033[0m \n";
            }
        } while (id.length() != 4 || id[0] != '3' || !isDigitsOnly(id) || inventory->searchItem(id));
        return id;
    }
};


int main() {
    Inventory inventory;
    Clothing* clothes = new Clothing("", "", 0, 0.0, "");
    Electronics* electronics = new Electronics("", "", 0, 0.0, "");
    Entertainment* entertainment = new Entertainment("", "", 0, 0.0, "");
    
    int choice;
    string category, id, name, sortType;
    int qty;
    double price;

    do {
        system("CLS");
        cout << "\033[38;5;214m  I N V E N T O R Y   M A N A G E M E N T\033[0m\n\n";
        cout << "\033[1;33m   1. Add Item \n";
        cout << "   2. Update Item \n";
        cout << "   3. Remove Item \n";
        cout << "   4. Display Items by Category \n";
        cout << "   5. Display All Items \n";
        cout << "   6. Search Item \n";
        cout << "   7. Sort Items \n";
        cout << "   8. Display Low Stock Items \n";
        cout << "   9. Exit \033[0m\n";
        choice = getValidatedInput("\033[38;5;214m   Enter your choice [1-9]: \033[0m", false); 

        if (choice != 9 && choice < 10 && choice > 0 ) { system("CLS"); }
        switch (choice) {
        	//Add Item
            case 1: {
                cout << "\033[38;5;214m      A D D   I T E M\033[0m\n\n";
                cout << "\tCATEGORIES: \n";
                cout << "       - Clothing\n";
                cout << "       - Electronics\n";
                cout << "       - Entertainment\n";
                do {
                    cout << "\n   Enter category: ";
                    getline(cin,category);
                    category = convertToUpper(category);
                    
                    if (category != "CLOTHING" && category != "ELECTRONICS" && category != "ENTERTAINMENT") {
                        cout << "\033[31m   Category " << category << " does not exist!\033[0m";
                    }
                } while (category != "CLOTHING" && category != "ELECTRONICS" && category != "ENTERTAINMENT");
                
                if (category == "CLOTHING") {
                    id = Clothing("", "", 0, 0.0, "").validateID(&inventory);
                } else if (category == "ELECTRONICS") {
                    id = Electronics("", "", 0, 0.0, "").validateID(&inventory);
                } else if (category == "ENTERTAINMENT") {
                    id = Entertainment("", "", 0, 0.0, "").validateID(&inventory);
                }
                
                do{
                cout << "   Item name: ";
                getline(cin, name); 
                if(name.length() == 0) { cout << "\033[31m   Input a name for the item.\033[0m\n";}
                }while(name.length() == 0);
                
                qty = getValidatedInput("   Item quantity: ", false); 
                price = getValidatedInput("   Item price: ", true); 

                if (category == "CLOTHING") {
                    Clothing* newClothing = new Clothing(id, name, qty, price, category);
                    inventory.addToInventory(newClothing);
                    cout << "\n\033[32m   Item added successfully!\033[0m\n";
                } else if (category == "ELECTRONICS") {
                    Electronics* newElectronics = new Electronics(id, name, qty, price, category);
                    inventory.addToInventory(newElectronics);
                    cout << "\n\033[32m   Item added successfully!\033[0m\n";
                } else if (category == "ENTERTAINMENT") {
                    Entertainment* newEntertainment = new Entertainment(id, name, qty, price, category);
                    inventory.addToInventory(newEntertainment);
                    cout << "\n\033[32m   Item added successfully!\033[0m\n";
                }
                break;
            }
            //Update an Item
            case 2: { 
            	cout << "\t\t\033[38;5;214m       U P D A T E    I T E M\033[0m\n";
                if (inventory.isEmpty()) {
                    cout << "\n  No items in inventory to update.\n";
                } else {
                	cout << endl; 
                	inventory.displayAllItems();
                	cout << "\n   ___________________________________________________________________\n  ";
                    cout << "\n   Enter the item ID to update: ";
                    getline(cin, id);
                    inventory.updateItem(id);
                    
                }
                break;
            }
			//Remove an Item
            case 3: { 
            	cout << "\t\t\033[38;5;214m         R E M O V E   I T E M \033[0m\n";
                if (inventory.isEmpty()) {
                    cout << "\n  No items in inventory to remove.\n";
                } else {
                	cout << endl; 
                	inventory.displayAllItems();
                	cout << "\n  ____________________________________________________________________\n  ";
                    cout << "\n  Enter the item ID to remove: ";
                    getline(cin,id);
                    inventory.removeItem(id);
                }
                break;
            }
            //Display Items by Category
            case 4: {
    			cout << "\033[38;5;214m     D I S P L A Y   I T E M S   B Y   C A T E G O R Y \033[0m\n\n";

    				if (inventory.isEmpty()) {
        				cout << "  No items in inventory to display.\n";
        				break; }
    				cout << "\t   CATEGORIES: \n";
    				cout << "          - Clothing\n";
    				cout << "          - Electronics\n";
    				cout << "          - Entertainment\n";

    				do {
        				cout << "\n     Enter category to display: ";
        				getline(cin,category);
        				category = convertToUpper(category);
        			if (category != "CLOTHING" && category != "ELECTRONICS" && category != "ENTERTAINMENT") {
            			cout << "\033[31m     Category " << category << " does not exist!\033[0m";
        			}
    				} while (category != "CLOTHING" && category != "ELECTRONICS" && category != "ENTERTAINMENT");
					
					cout << endl;
    
   					if (category == "CLOTHING") {
        				clothes->displayItems(inventory);
    				} else if (category == "ELECTRONICS") {
        				electronics->displayItems(inventory);
    				} else if (category == "ENTERTAINMENT") {
        				entertainment->displayItems(inventory);
    				}
    				break;
				}
            //Display All Items
            case 5: { 
            	cout << "\t\t\033[38;5;214m     D I S P L A Y   A L L   I T E M S\033[0m\n\n";
                if (inventory.isEmpty()) {
                    cout << "  No items in inventory to display.\n";
                } else {
                    inventory.displayAllItems();
                }
                break;
            }
			//Search an Item
            case 6: { 
            	cout << "\033[38;5;214m       S E A R C H   I T E M \033[0m\n";
    			if (inventory.isEmpty()) {
        			cout << "\n  No items in inventory to search.\n";
    			} else {
        			cout << "\n  Enter the item ID to search: ";
        			getline(cin,id);
        			inventory.performSearch(id);
    			}
    		break;
            }
			//Sort Items
            case 7: { 
            	cout << "\033[38;5;214m          S O R T   I T E M S\033[0m\n";
                if (inventory.isEmpty()) {
                    cout << "\n  No items in inventory to sort.\n";
                } else {
                    string criterion;
                    do {
                        cout << "\n  Sort by: Quantity or Price: ";
                        getline(cin,criterion);
                        criterion = convertToUpper(criterion);
                        if(criterion != "QUANTITY" && criterion != "PRICE"){
                        	cout << "\033[31m  Input Quantity or Price.\033[0m ";
						}
                    } while (criterion != "QUANTITY" && criterion != "PRICE");

                    do {
                        cout << "  Sort Ascending or Descending: ";
                        getline(cin,sortType);
                        sortType = convertToUpper(sortType);
                        if(sortType != "ASCENDING" && sortType != "DESCENDING"){
                        	cout << "\033[31m  Input Ascending or Descending.\033[0m\n";
						}
                    } while (sortType != "ASCENDING" && sortType != "DESCENDING");
                    
            		cout << "\n\033[32m  Items by " << criterion << " have been successfully sorted in " << sortType << " order.\033[0m\n";
                    inventory.sortItems(criterion, sortType);
                }
                break;
            }
            //Display Low Stock Items
            case 8: { 
            	cout << "\033[38;5;214m            D I S P L A Y   L O W   S T O C K   I T E M S\033[0m\n";
                if (inventory.isEmpty()) {
                    cout << "\n  No items in inventory to display low stock items.\n";
                } else {
                	cout << "\n\tLow stock items (quantity equal or less than 5):\n\n";
                	cout << "\033[32m";
                	cout << setw(10) << "ID" 
             			 << setw(17) << "Name" 
            			 << setw(13) << "Quantity" 
            			 << setw(15) << "Price" 
            			 << setw(15) << "Category"
            			 << "\033[0m"
            			 << endl;
                    inventory.displayLowStockItems();
                }
                break;
            }
            //Exit
            case 9: {
                cout << "\n  Exiting program...\n";
                return 0;
            }
            
            default: {
            	cout << "\033[31m   Invalid input. Input a number from 1-9.\033[0m\n";
				break;
			}
        }
        cout << endl;
        system("PAUSE");
    } while (choice != 9);
    
    return 0;
}