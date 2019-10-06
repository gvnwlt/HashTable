//============================================================================
// Name        : HashTable.cpp
// Author      : Gavin Walters
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // FIXME (1): Define structures to hold bids

    unsigned int hash(int key);
    unsigned int convertToInt(string str);
    struct Node {
    		Bid data;
    		int flag; // 1 for used 0 for unused
    		struct Node* lastNodeAddress;
    		struct Node* current;
    		struct Node* next;
    	};
    Node* temp;
    Node* current;
    Node* del;
    // Housekeeping variables that will be used to manage the list.
    vector<Node*> bids;
    int N;
    int count;
    int key;
    int flag;

public:
    HashTable();
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    int Size();
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // FIXME (2): Initialize the structures used to hold bids
	// initializing bid vector here
	count = 0;
	N = 180;
	bids.resize(N);
	key = 0;
	flag = 0;
	temp = NULL;
	current = NULL;
	del = NULL;


	for (int i = 0; i < N; i++) { // initialize vector elements or buckets
		Node* node = new Node();
		node->next = 0;
		node->flag = 0;
		node->lastNodeAddress = node;
		bids.insert(bids.begin() + i, node);
		count++;
		cout << "added at bucket " << i << endl;
	}

	cout << count << " entries" << endl;
	count = 0;
}

/**
 * Destructor
 */
HashTable::~HashTable() {
    // FIXME (3): Implement logic to free storage when class is destroyed
	// deletes all elements from the vector than the vector itself

	delete &bids;
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */

unsigned int HashTable::convertToInt(string key) {
	stringstream convert;
	unsigned int out;

	convert << key;
	convert >> out;

	return out;
}


unsigned int HashTable::hash(int key) {
    // FIXME (4): Implement logic to calculate a hash value
	// making a haskey variable to return that is the result
	// of a modulo hash calculation
	unsigned int hashedKey;

	hashedKey = key % N;

	return hashedKey;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // FIXME (5): Implement logic to insert a bid
	// converting string to int, hashing the bidId (key)
	// and storing it at the hash index

	key = hash(convertToInt(bid.bidId));
	flag = bids.at(key)->flag;
	cout << "1-->line 158: START bid id in bucket " << key << ": id->" << bid.bidId << " flag-> " << flag << endl;

	if (flag == 0) { // if not a dirty bit then just insert the bid
		cout << "2-->line 168: INSERT" << endl;
		bids.at(key)->data = bid;
		bids.at(key)->flag = 1;
	}
	else {
		cout << "line 173: APPEND" << endl; // making sure collisions are indeed happening
		Node* newNode = new Node();
		newNode->data = bid;
		//newNode->next = NULL;

		if (bids.at(key)->lastNodeAddress == bids.at(key)) { // if the first node then store the new node in next
			bids.at(key)->next = newNode;
			bids.at(key)->lastNodeAddress = newNode;
		}
		else { // since this is not a new node just append it on the node recorded in the last node's next
			bids.at(key)->lastNodeAddress->next = newNode; // store new node at last node next
			bids.at(key)->lastNodeAddress = newNode; // update the head node with the new last node address
		}
	}

	flag = bids.at(key)->flag;
	count++;
	cout << "3-->line 181: END bid id in bucket " << key << ": id->" << bids.at(key)->data.bidId << " flag-> " << flag << " current count = " << count << endl;
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // FIXME (6): Implement logic to print all bids
	// printing out all struct fields per vector element of bids

	//version 2
	for (int i = 0; i < N; i++) {
		current = bids.at(i);
		if (current->flag == 1) { // dirty bit when multiple nodes present in bucket, so if multiple look at them all
			bool first_pass =  true;
			while (current != NULL) {
				if (first_pass) { // checks to see if this is the first go--if not then subsequent bids are printed
					cout << "Key " << i << ": " << current->data.bidId << " | " << current->data.title <<
							" | " << current->data.fund << endl;
					current = current->next; // progress through nodes;
					first_pass = false;
				}
				else {
				cout << i << ": " << current->data.bidId << " | " << current->data.title <<
						" | " << current->data.fund << endl;
				current = current->next; // progress through nodes;
				}
			}
		}
	}
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // FIXME (7): Implement logic to remove a bid
	// takes bidId string and converts to integer so it
	// can be used to select an element to erase
	int key = hash(convertToInt(bidId));
	current = bids.at(key);

	//version 2
	if (current->flag == 1) { // if there happens to be more than one
		if (current->data.bidId == bidId ) { // if the first one
			bids.at(key) = current->next;
			bids.at(key)->flag = 1;
			cout << bidId << " removed" << endl;
		}
		else { // if not the first one then go through the routine until found or not found
			while (current != NULL && current->data.bidId != bidId) {
				temp = current;
				current = current->next;
			}
			if (current == NULL) { // if not found let the user know
				cout << "not found for deletion" << endl;
			}
			else { // if found then can it
				del = current;
				current = current->next;
				temp->next = current;

				cout << bidId << " removed" << endl;
			}
		}
	}
	else if (current->data.bidId == bidId) { // if only one then just mod the bid since naturally a vector space always contains a node by default
		//bids.erase(bids.begin() + key);
		Bid bid;
		bid.bidId = "empty";
		bid.amount = 0.00;
		bid.title = "empty";
		bid.fund = "empty";
		bids.at(key)->data = bid;
		cout << "deleted " << bidId << " at key " << key << endl;
	}
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // FIXME (8): Implement logic to search for and return a bid
    // converts id to hashed value and locates it in the vector

    int key = hash(convertToInt(bidId));
    cout << "searching at for key " << key << endl;
    current = bids.at(key);

    if (current->flag == 1) { // if multiple present in a bucket
    	while (current != NULL) {
    		if (current->data.bidId == bidId) {
    			return current->data;
    		}
    		current = current->next;
    	}
    }
    else if (current->data.bidId != bidId) { // if only one is present and it does not check out then disregard
    	return bid;
    }

    return bid;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            cout << "line 309: inserting into hash table now: current count : " <<  i << endl;
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    cout << "done loading" << endl;
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "src/eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bidTable = new HashTable();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
