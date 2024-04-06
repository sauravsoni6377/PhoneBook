#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <set>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>

using namespace std;

class PhoneRecord
{
private:
    string name;
    string organisation;
    vector<string> phoneNumbers;

public:
    // Constructor
    PhoneRecord(const string &n, const string &org, const vector<string> &numbers)
        : name(n), organisation(org), phoneNumbers(numbers) {}

    // Getter methods
    string getName() const
    {
        return name;
    }

    string getOrganisation() const
    {
        return organisation;
    }

    vector<string> getPhoneNumbers() const
    {
        return phoneNumbers;
    }
};

class HashTableRecord
{
private:
    int key;
    PhoneRecord *element; // Pointer to PhoneRecord
    HashTableRecord *next;

public:
    // Constructor
    HashTableRecord(int k, PhoneRecord *rec)
        : key(k), element(rec), next(nullptr) {}

    // Getter methods
    int getKey() const
    {
        return key;
    }

    PhoneRecord *getRecord() const
    {
        return element;
    }

    HashTableRecord *getNext() const
    {
        return next;
    }

    void setNext(HashTableRecord *nxt)
    {
        next = nxt;
    }
};

class PhoneBook
{
private:
    static const int HASH_TABLE_SIZE = 263;
    HashTableRecord *hashTable[HASH_TABLE_SIZE];

public:
    // Constructor
    PhoneBook()
    {
        for (int i = 0; i < HASH_TABLE_SIZE; ++i)
        {
            hashTable[i] = nullptr;
        }
    }

    // Add your own implementation for hashing
    long long computeHash(const string &str)
    {
        const long long p = 1000000007;
        long long h_value = 0;
        int x = 263;

        for (int i = 0; i < str.size(); i++)
        {
            h_value = (h_value * x + str[i])%p;
        }
        return h_value % HASH_TABLE_SIZE;
    }

    // Add your own implementation for adding a contact
    void addContact(const PhoneRecord *record)
    {

        vector<string> nameparts;
        istringstream iss(record->getName());
        string word;
        while (getline(iss, word, ' '))
        {
            nameparts.push_back(word);
        }

        for (const string &word : nameparts)
        {
            int hash = computeHash(word);
            HashTableRecord *newRecord = new HashTableRecord(hash, const_cast<PhoneRecord *>(record));

            // Insert the new HashTableRecord into the hash table (chaining for collisions) at head position for O(1)
            newRecord->setNext(hashTable[hash]);
            hashTable[hash] = newRecord;
        }
    }

    // Add your own implementation for deleting a contact
    bool deleteContact(const string *searchName)
    {
        // Fetch contacts that match the searchName (or a word in it)
        vector<PhoneRecord *> matchingContacts = fetchContacts(searchName);

        if (matchingContacts.empty())
        {
            return false;
        }

        // Get the first matching contact (selected PhoneRecord object)
        PhoneRecord *selectedContact = matchingContacts[0];

        string name = selectedContact->getName();
        istringstream iss(name);
        vector<long long> hashvalue;
        string word;

        while (getline(iss, word, ' '))
        {
            long long hash = computeHash(word);
            hashvalue.push_back(hash);
        }

        bool deleted = false; 
        for (long long hash : hashvalue)
        {
            int index = hash % HASH_TABLE_SIZE;
            HashTableRecord *current = hashTable[index];
            HashTableRecord *prev = nullptr;
            while (current != nullptr)
            {
                if (current->getRecord() == selectedContact)
                {
                    if (prev == nullptr)
                    {
                        hashTable[index] = current->getNext();
                    }
                    else
                    {
                        prev->setNext(current->getNext());
                    }

                    // Delete the HashTableRecord
                    delete current;
                    deleted = true; // Contact deleted
                    break;          // Exit the loop after deleting one contact
                }

                prev = current;
                current = current->getNext();
            }
        }

        return deleted; // Return true if a contact was deleted, false otherwise
    }

    // Add your own implementation for fetching contacts
    vector<PhoneRecord *> fetchContacts(const string *query)
    {
        vector<PhoneRecord *> results;
        unordered_map<PhoneRecord *, int> recordCounts;

        // Step 1: Split the query into unique keywords
        vector<string> keywords;
        istringstream iss(*query);
        string keyword;
        while (iss >> keyword)
        {
            keywords.push_back(keyword);
        }

        // Step 2: Retrieve records for each keyword and count references
        for (const string &keyword : keywords)
        {
            long long hash = computeHash(keyword);
            HashTableRecord *current = hashTable[hash];

            while (current != nullptr)
            {
                PhoneRecord *record = current->getRecord();

                recordCounts[record]++;


                current = current->getNext();
            }
        }
        vector<pair<PhoneRecord *, int>> recordswithcount;
        for (auto &entry : recordCounts)
        {
            recordswithcount.push_back(make_pair(entry.first, entry.second));
        }

        // Step 3: Sort the results based on the number of references
        sort(recordswithcount.begin(), recordswithcount.end(),
             [](const pair<PhoneRecord *, int> &a, const pair<PhoneRecord *, int> &b)
             { return a.second > b.second; });

        for (auto &entry : recordswithcount)
        {
            results.push_back(entry.first);
        }

        return results;
    }

    // Add your own implementation for counting records pointing to a contact
    // int countRecordsPointingTo(const PhoneRecord* record) const{

    // }

    // Add your own implementation for reading records from a file
    void readRecordsFromFile(const string &filename)
    {
        ifstream inputfile;
        inputfile.open(filename);

        string line;

        while (getline(inputfile, line))
        {

            string Name;
            string contact_no1;
            string contact_no2="";
            vector<string> Number;
            string orgn;

            int counter = 0;

            Name = "";
            while (line[counter] != ',')
            {
                Name += line[counter];
                counter++;
            }
            // cout<<Name<<" ";
            counter++;

            contact_no1 = "";
            while (line[counter] != ',')
            {
                contact_no1 += line[counter];
                counter++;
            }
            // cout<<contact_no1<<" ";
            counter++;

            if (line[counter] == '1' || line[counter] == '2' || line[counter] == '3' || line[counter] == '4' || line[counter] == '5' || line[counter] == '6' || line[counter] == '7' || line[counter] == '8' || line[counter] == '9')
            {
                while (line[counter] != ',')
                {
                    contact_no2 += line[counter];
                    counter++;
                }
                // cout<<contact_no2<<" ";
                counter++;
            }
            else
            {
                orgn = "";
                while (line[counter] != '\0')
                {
                    orgn += line[counter];
                    counter++;
                }
                // cout<<orgn;
            }

            orgn = "";
            while (line[counter] != '\0')
            {
                orgn += line[counter];
                counter++;
            }
            // cout<<orgn;
            cout << endl;
            Number.push_back(contact_no1);
            if(contact_no2!=""){
              Number.push_back(contact_no2);  
            }
            PhoneRecord *record= new PhoneRecord(Name, orgn, Number);
            addContact(record);
        }
        // cout<<endl;
        inputfile.close();
        // inputfile.close();
    }

    // Destructor
    ~PhoneBook(){

    }
};

// void read_input_file(string file_path){

// }

// int main() {
//     // Your test cases and program logic can go here.
//     PhoneBook phoneBook;
//     phoneBook.readRecordsFromFile("Details.txt");
//     cout<<phoneBook.computeHash("Bob")<<endl;
//     cout<<phoneBook.computeHash("Brown")<<endl;
//     cout<<phoneBook.computeHash("Charlie")<<endl;
//     cout<<phoneBook.computeHash("Smith")<<endl;
//     return 0;
// }
