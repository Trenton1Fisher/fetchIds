#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
#include <bits/stdc++.h>
#include <fstream>
using namespace std;

class Record {
public:
    int id, manager_id;
    std::string bio, name;

    Record(vector<std::string> fields) {
        id = stoi(fields[0]);
        name = fields[1];
        bio = fields[2];
        manager_id = stoi(fields[3]);
    }

    void print() {
        cout << "\tID: " << id << "\n";
        cout << "\tNAME: " << name << "\n";
        cout << "\tBIO: " << bio << "\n";
        cout << "\tMANAGER_ID: " << manager_id << "\n";
    }
};


class StorageBufferManager {

private:
    
    const int BLOCK_SIZE = 4096; // initialize the  block size allowed in main memory according to the question 

    // You may declare variables based on your need

    // Number of records on this page.
    int numRecords = 0;
    // How many pages have we created. Start at 0
    int num_pages = 0;
    // Running counter to keep track of how much memory is currently on the page
    int buffer_size;
    // How far from beginning of the page we are. Used for records.
    int beginning_offset = 0;
    // How far from the end of the page we are. Used for slots.
    int beginning_pointer = 0;
    // The number of employees we've read from the source file
    int num_employees = 0;

    int end_pointer = BLOCK_SIZE;

    int getnumRecords(){
        return numRecords;
    }
    void setnumRecords(int num){
        numRecords = num;
    }
    
    char* page = (char*) malloc(BLOCK_SIZE);
    // Insert new record 
    void printLastPage(Record record){
        ofstream destfile;
        destfile.open("EmployeeRelation.dat", fstream::app);
        if (!destfile.is_open()){
            cout << "Failed to open the dest file\n";
            return;
        }
        destfile << page;
        free(page);
        destfile.close();

    }

    void insertRecord(Record record) {
        //char page[BLOCK_SIZE];
        
        // No records written yet
        /*
        Num_records = 0
        Create a new page
        Write the size
        Add slot at the end

        Num_records > 1
        Check size
        if new_record_size + slot_size + curr_page_size >= 4096
            --Write to file and wipe. Set Num_records to 0.
        otherwise:
        Add to page
            -Record at the last record end
            -Slot at beginning of last slot - size
        */
        ofstream destfile;
        destfile.open("EmployeeRelation.dat", fstream::app);
        if (!destfile.is_open()){
            cout << "Failed to open the dest file\n";
            return;
        }

        int curr_rec_size = (8 + record.name.size() + record.bio.size() + 8 + 4);
        //Under block size
        if ((curr_rec_size + buffer_size) < end_pointer){
            
            
            cout << "Adding Record to page, buffer size = ";
            cout << buffer_size;
            cout << ". New record size is: ";
            cout << curr_rec_size;
            cout << ". Endpointer is : ";
            cout << end_pointer << endl;
            buffer_size += curr_rec_size;
            string temp;
            for(int i = 0; i < 8; i++){
                temp = to_string(record.id);
                //cout << temp[i] << endl;
                page[beginning_offset] = temp[i];
                beginning_offset++;
            }
            
            page[beginning_offset] = '$';
            beginning_offset++;
           
            for (int i = 0; i < record.name.size(); i++){
                page[beginning_offset] = record.name[i];
                beginning_offset++;
            }
            page[beginning_offset] = '$';
            beginning_offset++;

            for (int i = 0; i < record.bio.size(); i++){
                page[beginning_offset] = record.bio[i];
                beginning_offset++;
            }
            page[beginning_offset] = '$';
            beginning_offset++;

            for(int i = 0; i < 8; i++){
                temp = to_string(record.manager_id);
                page[beginning_offset] = temp[i];
                beginning_offset++;
            }
            page[beginning_offset] = '$';
            beginning_offset++;

            string end_slot = "#";
            temp = to_string(num_pages);
            end_slot = end_slot + temp;
            end_slot = end_slot + "#";
            temp = to_string(beginning_pointer);
            end_slot = end_slot + temp;
            end_slot = end_slot + "#";
            
            for (int m = 0; m < end_slot.size(); m++){
                page[end_pointer-end_slot.size()+m] = end_slot[m];
            }
            buffer_size += end_slot.size();

            end_pointer = end_pointer-end_slot.size()-1;
            //cout << "Done Adding Record to page" << endl;

        } //If the new record would exceed the block size, reset
        else if ((curr_rec_size+buffer_size) >= end_pointer){
            cout << "Created a new page" << endl;
            buffer_size = 0;
            
            //Clear the page
            destfile << page;
            free(page);
            page = (char*) malloc(BLOCK_SIZE);
            memset(page, '0', BLOCK_SIZE);
            num_pages += 1;
            //Move beginning offset Ex: 0 4096 8192, and thus pointer
            beginning_offset = 0;
            beginning_pointer = beginning_offset;
            //Move end pointer Ex: 4096 8192
            end_pointer = BLOCK_SIZE;
            string temp;
            
            for(int i = 0; i < 8; i++){
                temp = to_string(record.id);
                //cout << temp[i] << endl;
                page[beginning_offset] = temp[i];
                beginning_offset++;
            }
            
            page[beginning_offset] = '$';
            beginning_offset++;
           
            for (int i = 0; i < record.name.size(); i++){
                page[beginning_offset] = record.name[i];
                beginning_offset++;
            }
            page[beginning_offset] = '$';
            beginning_offset++;

            for (int i = 0; i < record.bio.size(); i++){
                page[beginning_offset] = record.bio[i];
                beginning_offset++;
            }
            page[beginning_offset] = '$';
            beginning_offset++;

            for(int i = 0; i < 8; i++){
                temp = to_string(record.manager_id);
                page[beginning_offset] = temp[i];
                beginning_offset++;
            }
            page[beginning_offset] = '$';
            beginning_offset++;
            //8's = id's. 4 = $, others self explainatory.
            buffer_size += (8 + record.name.size() + record.bio.size() + 8 + 4);

            //cout << "Before slot 1" << endl;
            string end_slot = "#";
            temp = to_string(num_pages);
            end_slot = end_slot + temp;
            end_slot = end_slot + "#";
            temp = to_string(beginning_pointer);
            end_slot = end_slot + temp;
            end_slot = end_slot + "#";
            //cout << "Before slot 2" << endl;
            
            for (int m = 0; m < end_slot.size(); m++){
                //cout << end_slot[m] << endl;
                // cout << "m: ";
                // cout << end_pointer-end_slot.size()+m;
                // cout << " Slot: ";
                // cout << end_slot[m] << endl;
                page[end_pointer-end_slot.size()+m] = end_slot[m];
            }
            buffer_size += end_slot.size();       
        } 
        //Calcate size of next buffer.


        //cout << numRecords << endl;
        
        // Add record to the block
        //destfile << record;
        //cout << getnumRecords() << endl;
        //num_pages += 1;
        // Take neccessary steps if capacity is reached (you've utilized all the blocks in main memory)
        //if the buffer + sizeof new record exceeds 4093*3, send to data file.
        
        destfile.close();
        // cout << "dest close " << endl;
    }

public:
    StorageBufferManager(string NewFileName) {
        
        //initialize your variables

        // Create your EmployeeRelation file 
        ofstream destfile("EmployeeRelation.dat");
        destfile.close();

        
    }

    // Read csv file (Employee.csv) and add records to the (EmployeeRelation)
    void createFromFile(string csvFName) {
        
        ifstream sourcefile;
        sourcefile.open("Employee.csv");

        //ofstream destfile;
        //destfile.open("EmployeeRelation.dat");
        if (!sourcefile.is_open()){
            cout << "Failed to open the file\n";
            return;
        } else {
            
            
            //Open the file
            string curr_line;
            while(std::getline(sourcefile, curr_line)) {
                //cout << curr_line << endl;
                vector<string> fields;
                string substr;
                //replace(curr_line.begin(), curr_line.end(), ',', '$');
                stringstream ss(curr_line);
            
                while(ss.good()){
                    //cout << "ss good" << endl;
                    string substr;

                    getline(ss, substr, ',');
                    fields.push_back(substr);
                    //cout << "Field pushback" << endl;
                }
                Record new_record(fields);
                //cout << "Grabbed new record" << endl;
                insertRecord(new_record);
                //cout << "after insert new record" << endl;
                //cout << curr_line << endl;
            }

            
            printLastPage(new_record);
            
            
            //destfile.close();
            
            sourcefile.close();
        }

    }
};
