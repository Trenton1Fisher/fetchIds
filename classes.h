#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
#include <bits/stdc++.h>
#include <fstream>
using namespace std;

class Record
{
public:
    int id, manager_id;
    std::string bio, name;

    Record(vector<std::string> fields)
    {
        id = stoi(fields[0]);
        name = fields[1];
        bio = fields[2];
        manager_id = stoi(fields[3]);
    }

    void print()
    {
        cout << "\tID: " << id << "\n";
        cout << "\tNAME: " << name << "\n";
        cout << "\tBIO: " << bio << "\n";
        cout << "\tMANAGER_ID: " << manager_id << "\n";
    }
};

class StorageBufferManager
{

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
    // The number of pages read in the search so far
    int num_pages_read = 0;

    int end_pointer = BLOCK_SIZE;

    int getnumRecords()
    {
        return numRecords;
    }
    void setnumRecords(int num)
    {
        numRecords = num;
    }

    char *page;
    // Insert new record
    void printLastPage()
    {
        ofstream destfile;
        destfile.open("EmployeeRelation.dat", fstream::app);
        if (!destfile.is_open())
        {
            cout << "Failed to open the dest file\n";
            return;
        }
        destfile << page;
        free(page);
        destfile.close();
    }
    void setupFirstPage()
    {
        page = (char *)malloc(BLOCK_SIZE);
        memset(page, '@', BLOCK_SIZE);
    }

    void insertRecord(Record record)
    {

        ofstream destfile;
        destfile.open("EmployeeRelation.dat", fstream::app);
        if (!destfile.is_open())
        {
            cout << "Failed to open the dest file\n";
            return;
        }

        int curr_rec_size = (8 + record.name.size() + record.bio.size() + 8 + 4);
        // Under block size
        end_pointer = end_pointer - 1;
        if ((curr_rec_size + buffer_size) < end_pointer)
        {
            buffer_size += curr_rec_size;
            string temp;
            beginning_pointer = beginning_offset;

            page[beginning_offset] = '$';
            beginning_offset++;
            
            for (int i = 0; i < 8; i++)
            {
                temp = to_string(record.id);
                page[beginning_offset] = temp[i];
                beginning_offset++;
            }

            page[beginning_offset] = '$';
            beginning_offset++;

            for (int i = 0; i < record.name.size(); i++)
            {
                page[beginning_offset] = record.name[i];
                beginning_offset++;
            }
            page[beginning_offset] = '$';
            beginning_offset++;

            for (int i = 0; i < record.bio.size(); i++)
            {
                page[beginning_offset] = record.bio[i];
                beginning_offset++;
            }
            page[beginning_offset] = '$';
            beginning_offset++;

            for (int i = 0; i < 8; i++)
            {
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
            end_slot = end_slot + "%";

            for (int m = 0; m < end_slot.size(); m++)
            {
                page[end_pointer - end_slot.size() + m + 1] = end_slot[m];
            }
            buffer_size += end_slot.size();

            end_pointer = end_pointer - end_slot.size();

        } // If the new record would exceed the block size, reset
        else if ((curr_rec_size + buffer_size) >= end_pointer)
        {
            buffer_size = 0;

            destfile << page;

            memset(page, '@', BLOCK_SIZE);
            num_pages += 1;

            beginning_offset = 1;
            beginning_pointer = beginning_offset;

            end_pointer = BLOCK_SIZE - 1;
            string temp;
            int start_id = beginning_offset;
            page[beginning_offset] = '$';
            beginning_offset++;

            for (int i = 0; i < 8; i++)
            {
                temp = to_string(record.id);
                //cout << temp;
                page[beginning_offset] = temp[i];
                beginning_offset++;
            }

            // for (int i=0;i < 8; i++){
            //     cout << page[start_id+i];
            // }
            // cout << endl;

            page[beginning_offset] = '$';
            beginning_offset++;

            for (int i = 0; i < record.name.size(); i++)
            {
                page[beginning_offset] = record.name[i];
                beginning_offset++;
            }
            page[beginning_offset] = '$';
            beginning_offset++;

            for (int i = 0; i < record.bio.size(); i++)
            {
                page[beginning_offset] = record.bio[i];
                beginning_offset++;
            }
            page[beginning_offset] = '$';
            beginning_offset++;

            for (int i = 0; i < 8; i++)
            {
                temp = to_string(record.manager_id);
                page[beginning_offset] = temp[i];
                beginning_offset++;
            }
            page[beginning_offset] = '$';
            beginning_offset++;

            buffer_size += (8 + record.name.size() + record.bio.size() + 8 + 4);

            string end_slot = "#";
            temp = to_string(num_pages);
            end_slot = end_slot + temp;
            end_slot = end_slot + "#";
            temp = to_string(beginning_pointer);
            end_slot = end_slot + temp;
            end_slot = end_slot + "%";

            for (int m = 0; m < end_slot.size(); m++)
            {
                page[end_pointer - end_slot.size() + m + 1] = end_slot[m];
            }

            end_pointer = end_pointer - end_slot.size();
            buffer_size += end_slot.size();
        }

        destfile.close();
    }

public:
    StorageBufferManager(string NewFileName)
    {

        // initialize your variables

        // Create your EmployeeRelation file
        ofstream destfile("EmployeeRelation.dat");
        destfile.close();
    }
    // Read csv file (Employee.csv) and add records to the (EmployeeRelation)
    void createFromFile(string csvFName)
    {

        ifstream sourcefile;
        sourcefile.open("Employee.csv");

        if (!sourcefile.is_open())
        {
            cout << "Failed to open the file\n";
            return;
        }
        else
        {

            // Open the file
            string curr_line;
            setupFirstPage();
            while (std::getline(sourcefile, curr_line))
            {
                // cout << curr_line << endl;
                vector<string> fields;
                string substr;
                // replace(curr_line.begin(), curr_line.end(), ',', '$');
                stringstream ss(curr_line);

                while (ss.good())
                {
                    // cout << "ss good" << endl;
                    string substr;

                    getline(ss, substr, ',');
                    fields.push_back(substr);
                    // cout << "Field pushback" << endl;
                }
                Record new_record(fields);
                // cout << "Grabbed new record" << endl;
                insertRecord(new_record);
                // cout << "after insert new record" << endl;
                // cout << curr_line << endl;
            }

            printLastPage();
            sourcefile.close();
        }
    }

    void findRecordById(int id)
    {
                page = (char *)malloc(BLOCK_SIZE);
                // ID found or not to stop loop
                bool found = false;
                // Open File
                ifstream destfile;
                destfile.open("EmployeeRelation.dat");

                while (!found)
                {
                    memset(page, '@', BLOCK_SIZE);
                    int page_offset = num_pages_read * 4096;
                    for (int i = 0; i < BLOCK_SIZE; i++)
                    {
                        char character;
                        destfile.get(character);
                        page[i] = character;
                    }
                    int at_count = 0;
                    for (int i = BLOCK_SIZE - 1; i >= 0; --i)
                    {
                        if (page[i] == '%')
                        {
                            // Start reading the offset value
                            string offset = "";
                            --i; // Move back to the offset value
                            while (page[i] != '#' && i >= 0)
                            {
                                offset = page[i] + offset; // Append the character to the offset (reversed order)
                                --i;
                            }
                            int int_offset = stoi(offset)+1;
                           
                            char temp[8];
                            memset(temp, '0', 8);
                            int j = 0;
                            while (page[int_offset] != '$')
                            {
                                temp[j] = page[int_offset];
                                ++int_offset;
                                j++;
                            }
                            int compare_id = stoi(temp);
                            if (compare_id == id)
                            {
                                vector<string> return_vector;
                                // Temp already holds the id value
                                return_vector.push_back(temp);
                                int count = 0;
                                // Pointer is at a delimiter, increment 1 for content
                                int_offset++;
                                string string_value_holder;
                                while (count != 3 && int_offset < BLOCK_SIZE) { // Add int_offset < page.size() condition to avoid accessing out of range
                                    if (page[int_offset] == '$') {
                                        count++;
                                        return_vector.push_back(string_value_holder);
                                        string_value_holder = "";
                                    } else {
                                        string_value_holder += page[int_offset];
                                    }
                                    int_offset++;
                                }
                                Record return_record(return_vector);
                                return_record.print();
                                num_pages_read = 0; //reset for next search
                                found = true;
                                break;
                            }
                        }
                        else if (page[i] == '@')
                        {
                            ++at_count;
                            if (at_count >= 2)
                            {
                                // Found consecutive '@@', indicating the end of the slot directory
                                break;
                            }
                        }
                        else
                        {
                            // # or page number was found set @ to 0 to not break loop
                            at_count = 0;
                        }
                    }
                    if(!found){
                        if (num_pages == num_pages_read){
                            cout << "Employee ID not in database." << endl;
                            num_pages_read = 0; //reset for next search
                            break;
                    }
                    num_pages_read++;        
}
                }
        destfile.close();
        free(page);
    }
};
       