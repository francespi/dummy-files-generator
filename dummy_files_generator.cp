#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

using namespace std;

const string DEFAULT = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string BLUE = "\033[34m";
const string YELLOW = "\033[33m";

// This function is responsible for files number input parsing
int parse_num(string str){
    
    // If the string contains characters other than those specified,
    // return 1 (input is not valid)
    if(str.find_first_not_of("0123456789") != string::npos)
        return 1;
    
    return 0;
}

// This function is responsible for file name input parsing
int parse_name(string str){
          
    // If the string contains any of the characters specified
    // return 1 (input is not valid)
    if(str.find_first_of("\t\n/\\:;,*?<>|\0\r\"") != string::npos)
        return 1;
    
    // If the string does not contain a dot followed by some characters,
    // return 2 (input is not valid, file name does not contain an extension)
    size_t pos = str.find('.');
    
    if(pos == string::npos)
        return 2;
    
    else if (pos != string::npos) {
        
        if(str.substr(pos + 1).length() == 0)
            return 2;
        
    }
    
    return 0;
    
}

// This function is responsible for file size input parsing
int parse_size(string str){
        
    // If the string contains characters other than those specified,
    // return 1 (input is not valid)
    if(str.find_first_not_of("1234567890.MiB") != string::npos)
        return 1;
    
    // If the string does not contain "MiB" or "MB", return 1
    // (input is not valid: "M", "i", and/or "B" may be present
    // but do not as "MiB" or "MB")
    if((str.find("MiB") == string::npos && str.find("MB") == string::npos))
        return 1;
    
    // If the string contains more than one dot
    // return 1 (input is not valid)
    size_t pos = str.find('.');
    
    if(pos != string::npos) {
        
        if(str.substr(pos + 1).find('.') != string::npos)
            return 1;
    }
    
    return 0;
    
}

int write(string file_name, string file_size){
    
    // Numeric value of size
    float size = 0;
    
    // Multiplicative factor
    int factor = 0;
    
    // Position of "MB" or "MiB" within the string (file_size)
    size_t pos = 0;
    
    // If the string contains "MB" or "MiB", extract the numeric
    // value from it and set the multiplicative factor accordingly,
    // in order to generate a file of the correct size
    if((pos = file_size.find("MB")) != string::npos){
        size = stof(file_size.substr(0, pos));
        factor = 1000;
    }
    else if((pos = file_size.find("MiB")) != string::npos){
        size = stof(file_size.substr(0, pos));
        factor = 1024;
    }
    
    // Create and open file for writing in binary mode
    ofstream ofs(file_name, ios::binary | ios::out);
    
    // Create a vector of 'factor' number of chars and
    // initialize each element to 0
    vector<char> dummy(factor, 0);

    // Write dummy vector to output file 'factor * size' times
    for (int i = 0; i < factor * size; i++) {
        
        if (!ofs.write(&dummy[0], dummy.size()))
            return 1;
        
    }
    
    return 0;
}

int main() {

    int err = 0;
    string files_num;
    string file_size;
    string file_name;
    
    cout << BLUE << "*********************" << endl;
    cout << "Dummy Files Generator" << endl;
    cout << "*********************" << DEFAULT << endl;
    
    // Dummy files number input and parsing
    do{
        
        cout << endl << "Dummy files number: ";
        getline(cin, files_num);
       
        err = parse_num(files_num);
        
        if(err)
            cout << endl << YELLOW << "Files number is not valid." << DEFAULT << endl;
        
    }while(err);

    // Loop over the number of dummy files to generate
    for (int i = 0; i < stoi(files_num); i++) {
        
        // Dummy file name input and parsing
        do{
            
            cout << endl << "Dummy file name: ";
            getline(cin, file_name);
            
            err = parse_name(file_name);
            
            if(err == 1)
                cout << endl << YELLOW << "File name contains characters that are not allowed." << DEFAULT << endl;
            
            else if(err == 2)
                cout << endl << YELLOW << "File name has no extension." << DEFAULT << endl;
                        
        }while(err);
        
        
        // Dummy file size input and parsing
        do{
            
            cout << endl << "Dummy file size (MiB/MB): ";
            getline(cin, file_size);
            
            err = parse_size(file_size);
            
            if(err)
                cout << endl << YELLOW << "File size is not valid." << DEFAULT << endl;
            
        }while(err);

        // Dummy file generation
        if(!write(file_name, file_size))
            cout << endl << GREEN << "File generated successfully!" << DEFAULT << endl;
        
        else
            cerr << endl << RED << "Error writing to file..." << DEFAULT << endl;

    }

    return 0;

}
