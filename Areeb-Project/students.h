//  Students.h
//  Areeb-Project

#ifndef Students_h
#define Students_h

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <map>
#include <algorithm>
#include "teachersLogin.h"

using namespace std;
using std::__fs::filesystem::exists; //used to check if a file exists or not, used in the addStudent() function below

class Student{
    
protected:
    string UID; //UID stands for unique ID that is mainly used to make sure that there are no duplicates
    string netID; //much more concise ID that can be used by students
    string f_name;
    string s_name;
    string name;
    char gender;
    int age;
    string DoB;
    string ddmmyyyy; //defining another version of DoB variable to use in generating UID and netID
    string DD;
    string MM;
    string YYYY;
    
public:
    Student(){} //default constructor
    
    //constructor
    Student(string UID, string netID, string f_name, string s_name, int age, char gender, string DoB) {
        this->UID=UID;
        this->netID = netID;
        this->f_name = f_name;
        this->s_name = s_name;
        name = f_name + " " + s_name;
        this->age = age;
        this->gender = gender;
        this->DoB = DoB;
    }
    
    //getter functions, defined as inline to optimize
    inline string GetUID() { return UID; }
    inline string GetnetID() { return netID; }
    inline string GetName() { return name; }
    inline string getFname() { return f_name; }
    inline string getSname() { return s_name; }
    inline string getDoB() { return DoB; }
    inline int GetAge() { return age; }
    inline char GetGender() { return gender; }
    
    //function to generateID (both UID and netID)
    string generateIDs(string f_name, string s_name, char gender, string DoB, bool dupliCheck){
        string *ID = new string;
        if(dupliCheck == false){
            
            //to concatenate first two character of first name
            *ID = tolower(f_name[0]);
            *ID += tolower(f_name[1]);
            
            //to concatenate first two character of second name
            *ID += tolower(s_name[0]);
            *ID += tolower(s_name[1]);
            
            //to concatenate gender character
            *ID += gender;
            UID = *ID;
            UID += ddmmyyyy; //asigning the whole DoB to UID to increase efficiency while checking for duplicates
            UID += to_string(age); //concatenate age to UID
            
            //assigning only DD and MM from DoB to netID to make it concise
            *ID += ddmmyyyy[0];
            *ID += ddmmyyyy[1];
            *ID += ddmmyyyy[2];
            *ID += ddmmyyyy[3];
            netID = *ID;
        } else {
            
            //else to assign another UID to the student if the teacher believes its not a duplicate
            srand(unsigned(time(0)));
            *ID = tolower(f_name[0]);
            *ID += tolower(f_name[1]);
            *ID += tolower(s_name[0]);
            *ID += tolower(s_name[1]);
            *ID += gender;
            UID = *ID;
            UID += ddmmyyyy;
            UID += to_string(age);
            *ID += ddmmyyyy[0];
            *ID += ddmmyyyy[1];
            *ID += ddmmyyyy[2];
            *ID += ddmmyyyy[3];
            *ID += to_string((rand()%10)); //assigning a random integer towards the end to make sure netID is different
            netID = *ID;
            UID += to_string((rand()%10)); //assigning a random integer towards the end to make sure UID is different
        }
        return UID;
        delete ID; //deleting dynamic variable
    }
    
    //setter functions
    void SetName(string first_name, string last_name) {
        f_name = first_name;
        s_name = last_name;
        name = f_name + " " + s_name; //setting full name by concatenating first and second name
    }
    void SetAge(int age) { this->age = age; }
    void SetGender(char gender) { this->gender = gender; }
    void SetDoB(string DoB) { this->DoB = DoB; }
    void SetDateOfBirth(string dd, string mm, string yyyy){
        ddmmyyyy = dd+mm+yyyy; //setting the DoB in a format that could be used in generating UID and netID
    }
    
    //function to parse DoB which is in format DD/MM/YYYY into respective DD, MM and YYYY
    void parseDOB(){
        DD = DoB[0];
        DD += DoB[1];
        MM = DoB[3];
        MM += DoB[4];
        YYYY = DoB[6];
        YYYY += DoB[7];
        YYYY += DoB[8];
        YYYY += DoB[9];
    }
    
    //print function to display certain details of the students
    void print()  {
        cout<< "UID: " << UID
        << ", netID: " << netID
        << ", Name: " << name
        << ", Age: " << age
        << ", Gender: " << gender << endl;
    }
};

//a StudentStore class which acts as a "working" class, inherits LoginTeacher and Student class
//This is where "student registration" happens
class StudentStore: public LoginTeacher, public Student{
    
private:
    map<string, Student> students_store; //a map to temporarily store student records while reading/writing file
    string *fileName = new string; //dynamic variable to store file name that will be used to read/write file
public:
    
    StudentStore(){}; //default constructor
    
    //a function to read the file for a specific teacher's class
    void readFile(){
        char *elcomma =  new char; //dynamic variable used to eleminate the commas while reading the csv
        string *header = new string; //dynamic variable used to eleminate the header line from the file
        ifstream inFile;
        *fileName = getFileName();
        inFile.open(*fileName);
        if(inFile.fail()){
            cerr << "The file for the class does not exist yet.\n";
        }
        getline(inFile, *header); //removing header
        
        //infinite loop that reads the file and only breaks when the if statement in the loop returns true
        while(1){
            getline(inFile, UID, ',');
            getline(inFile, netID, ',');
            getline(inFile, f_name, ',');
            getline(inFile, s_name, ',');
            inFile >> age >> *elcomma >> gender >> *elcomma >> DoB;
            getline(inFile, *header, '\n');
            
            //if statement to break the loop when eof is reached
            if(inFile.eof()){
                break;
            }
            parseDOB();
            SetDateOfBirth(DD, MM, YYYY);
            students_store[UID] = Student(UID, netID, f_name, s_name, age, gender, DoB); //reading the records into the map
        }
        inFile.close();
        
        //deleting dynamic variables that were created within the scope of this function
        delete header;
        delete elcomma;
    }
    
    //a function too add student records
    void addStudent(){
        *fileName = getFileName();
        if(exists(*fileName)!=true){ //checks if the file exists or not...if it doesnt, creates the file by calling exportCSV()
            exportCSV();
        } else {
            readFile();
        }
        char *sure = new char; //variable that keeps track if the user is "sure" that the student being addes isn't duplicate
        cout<<"Enter First Name : ";
        cin >> f_name;
        cout<<"Enter Last Name : ";
        cin >> s_name;
        cout<<"Enter Age : ";
        cin >> age;
        cout<<"Enter Date of Birth (DD/MM/YYYY) : ";
        cin >> DoB;
        parseDOB();
        SetDateOfBirth(DD, MM, YYYY);
        
        //validating gender
        do{
            cout << "Enter Gender (m/f): ";
            cin >> gender;
        } while(gender != 'm' && gender != 'f');
        
        //generating UIDs and netIDs
        UID = generateIDs(f_name, s_name, gender, DoB, false);
        
        //initializing iterator to go through map to 1) check if record exist or not, 2) and to add the record
        map<string,Student>::iterator t;
        t = students_store.find(UID);
        if(t==students_store.end()){
            students_store[UID] = Student(UID, netID, f_name, s_name, age, gender, DoB);
            cout << "Success!\n";
        } else{
            cout << "\nERROR: The student is already registered!\n" << endl;
            cout << "Are you sure this is a differnet student and still want to proceed? (y/n): ";
            cin >> *sure;
            while(*sure == 'y'){
                
                //generating new UID and netID if user is sure the student being added isn't duplicate
                UID = generateIDs(f_name, s_name, gender, DoB, true);
                t = students_store.find(UID);
                if(t==students_store.end()){
                    students_store[UID] = Student(UID, netID, f_name, s_name, age, gender, DoB);
                    cout << "Success!\n";
                    *sure = 'n';
                } else{
                    cout << "\nERROR: The student is already registered!\n" << endl;
                    cout << "Are you sure this is a differnet student and still want to proceed? (y/n): ";
                    cin >> *sure;
                }
            }
        }
        delete sure;
        exportCSV(); //automatically exporting into the CSV file each time a student is added
    }
    
    //a function to display student records
    void displayStudents(){
        readFile(); //reads the file, consequently storing any records into the map
        if(students_store.empty()==1){
            cout<<"No students"<<endl;
        }
        else{
            for (map<string, Student>::iterator i = students_store.begin(); i != students_store.end(); i++) {
                i->second.print();
            }
        }
    }
    
    //a function to delete student records
    void deleteStudent(){
        readFile(); //reads the file, consequently storing any records into the map
        string *ID = new string; //dynamic variable to store the UID of the student to be deleted
        cout << "Enter the UID of the student you like to delete: ";
        cin >> *ID;
        map<string,Student>::iterator t;
        t = students_store.find(*ID); //iterator to find the records with the key same as the UID entered by user
        if (t != students_store.end()){
            students_store.erase(t);
            exportCSV(); //automatically exporting into the CSV file after the record is deleted to reflect the updates immediately
            cout << "Success!" << endl;
        } else{
            cout << "\nERROR: The student doesn't exist. Try Again!\n\n";
        }
        delete ID; //deleting dynamic variable
    }
    
    //a function to edit student records
    void editStudent(){
        readFile(); //reads the file, consequently storing any records into the map
        string *ID =  new string; //dynamic variable to store the UID of the student to be edited

        cout << "Enter the ID of the student you like to edit: ";
        cin >> *ID;
        
        map<string,Student>::iterator t;
        t = students_store.find(*ID); //iterator to find the records with the key same as the UID entered by user
        
        if (t == students_store.end()){
            cout << "\nERROR: The student doesn't exist \n\n";
        } else{
            cout << "Select the attribute you want to edit: ";
            int *sel = new int; //creating a dynamic varible to monitor user's selection for the edit menu below
            do{
                cout <<"\n====================="<< endl;
                cout <<"[1] NAME"<< endl;
                cout <<"[2] AGE"<< endl;
                cout <<"[3] GENDER"<< endl;
                cout <<"[4] Date of Birth"<< endl;
                cout <<"[5] EXIT"<< endl;
                cout <<"====================="<< endl;
                cout <<"Enter Selection : ";
                cin>>*sel;
                if(*sel == 1){
                    cout<<"Enter First Name : ";
                    cin >> f_name;
                    cout<<"Enter Last Name : ";
                    cin >> s_name;
                    t->second.SetName(f_name, s_name);
                    cout << "Success!" << endl;
                }
                else if(*sel == 2){
                    cout<<"Enter Age : ";
                    cin >> age;
                    t->second.SetAge(age);
                    cout << "Success!" << endl;
                }
                else if(*sel == 3){
                    cout << "Enter the gender (m/f): ";
                    cin >> gender;
                    t->second.SetGender(gender);
                    cout << "Success!" << endl;
                }
                else if(*sel == 4){
                    cout << "Enter the Date of Birth (DD/MM/YYYY): ";
                    cin >> DoB;
                    t->second.SetDoB(DoB);
                    cout << "Success!" << endl;
                }
                else{
                    if (*sel!=5) {
                        cout << "Please enter a valid selection. Try Again" << endl;
                    }
                    break;
                }
            } while(*sel!=4);
            delete sel; //deleting dynamic variable
        }
        delete ID; //deleting dynamic variable
        
        exportCSV(); //automatically exporting into the CSV file after the record is edited to reflect the updates immediately
    }
    
    //a function to export the student records from the map to the CSV file
    void exportCSV(){
        *fileName = getFileName();
        ofstream exportFile;
            exportFile.open(*fileName, ios::trunc);
            exportFile << "UID," << "netID," << "First Name," << "Last Name," << "Age," << "Gender," << "Date of Birth" << "\n";
            if(exportFile.fail()){
                cerr << "Could not open and export the output file. \n";
            }
            for (map<string, Student>::iterator t = students_store.begin(); t != students_store.end(); t++) {
                exportFile << t->second.GetUID() << "," << t->second.GetnetID() << "," << t->second.getFname()
                << "," << t->second.getSname() << "," << t->second.GetAge() << "," << t->second.GetGender() << "," << t->second.getDoB() << "\n";
            }
            exportFile.close();
    }
    
    //function to empty map in case the teacher wants to log back in without exiting the application
    void clearMap(){
        students_store.clear();
    }
    
    // destructor that deletes the dynamic variable fileName
    ~StudentStore() {
        delete fileName;
    }
};
#endif /* Students_h */
