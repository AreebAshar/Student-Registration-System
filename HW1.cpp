//  Areeb-Project
//  Due 15/10/21
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <map>
#include <vector>
#include <sstream>
#include <math.h>

using namespace std;
using std::__fs::filesystem::exists;


class Student{
    
protected:
    string UID;
    string netID;
    string f_name;
    string s_name;
    string name;
    char gender;
    int age;
    int DoB;
    
public:
    Student(){}
    Student(string UID, string netID, string f_name, string s_name, int age, char gender, int DoB) {
        this->UID=UID;
        this->netID = netID;
        this->f_name = f_name;
        this->s_name = s_name;
        name = f_name + " " + s_name;
        this->age = age;
        this->gender = gender;
        this->DoB = DoB;
    }
    
    string GetUID() { return UID; }
    string GetnetID() { return netID; }
    string GetName() { return name; }
    string getFname() { return f_name; }
    string getSname() { return s_name; }
    int getDoB() {return DoB;}
    int GetAge() { return age; }
    char GetGender() { return gender; }
    
    string generateIDs(string f_name, string s_name, char gender, int DoB, bool dupliCheck){
        string ID;
        string dob;
        if(dupliCheck == false){
            ID = tolower(f_name[0]);
            ID += tolower(f_name[1]);
            ID += tolower(s_name[0]);
            ID += tolower(s_name[1]);
            ID += gender;
            dob += to_string(DoB);
            UID = ID;
            UID += dob;
            UID += to_string(age);
            ID += dob[0];
            ID += dob[1];
            ID += dob[2];
            ID += dob[3];
            netID = ID;
        } else {
            srand(unsigned(time(0)));
            ID = tolower(f_name[0]);
            ID += tolower(f_name[1]);
            ID += tolower(s_name[0]);
            ID += tolower(s_name[1]);
            ID += gender;
            dob += to_string(DoB);
            UID = ID;
            UID += dob;
            UID += to_string(age);
            ID += dob[0];
            ID += dob[1];
            ID += dob[2];
            ID += dob[3];
            ID += to_string((rand()%10));
            netID = ID;
            UID += to_string((rand()%10));
            
        }
        return UID;
    }
    void SetName(string first_name, string last_name) {
        f_name = first_name;
        s_name = last_name;
        name = f_name + " " + s_name;
    }
    void SetAge(int age) { this->age = age; }
    void SetGender(char gender) { this->gender = gender; }
    void print()  {
        cout<< "UID: " << UID
        << ", netID: " << netID
        << ", Name: " << name
        << ", Age: " << age
        << ", Gender: " << gender << endl;
    }
};

class LoginTeacher {
private:

    string tryusername;
    string trypassword;
    string tryclassName;


public:
    string classFileName;
    
    LoginTeacher(){}
    
    void Initialize(){
        string w, x, y;
        int choice;
        do{
            cout <<"================================================================================================"<< endl;
            cout <<"Welcome to the Student Registration System. Press 1 to register or 2 to login, and 3 to exit: ";
            cin >> choice;
            if(choice == 1){
                cout << "Create an account to use the Student Registration System \n";
                cout << "Enter your class: ";
                cin >> w;
                cout << "Enter your username: ";
                cin >> x;
                cout << "Enter your password: ";
                cin >> y;
                Register(w,x,y);
                cout << endl;
                Login();
                break;
            }
            else if(choice == 2){
                Login();
                break;
            }
        } while(choice!=3);
        
    }
    
    void Login()
    {
        cout << "Enter Class Name (i.e. 4A): ";
        cin >> tryclassName;
        int classKey = CheckFile(tryclassName, "classnames.dat");
        if(classKey!=0)
        {
            cout << "Username: ";
            cin >> tryusername;
            
            int userKey = CheckFile(tryusername, "usernames.dat");
            if(userKey!=0){
                cout << "Password: ";
                cin >> trypassword;
                
                int passwordKey = CheckFile(trypassword, "passwords.dat");
                if (userKey == passwordKey){
                    cout << "Logging in..." << endl;
                    setFileName(tryclassName);
                } else {
                    cout << "Incorrect password." << endl;
                    Login();
                }
            } else {
                cout << "Incorrect username." << endl;
                Login();
            }
        } else {
            cout << "Incorrect class." << endl;
            Login();
        }
    }
    void setFileName(string name){
        classFileName = name + ".csv";
    }
    
    string getFileName(){
        return classFileName;
    }
    
    void Register(const string className, const string username, const string password){
        if(CheckFile(className, "classnames.dat") != 0){
            cout << "Class is already taken" << endl;
            return;
        }
        
        if(CheckFile(username, "usernames.dat") != 0){
            cout << "Username is already taken" << endl;
            return;
        }
        
        int key = 1 + getLastKey();
        SaveFile(className, "classnames.dat", key);
        SaveFile(username, "usernames.dat", key);
        SaveFile(password, "passwords.dat", key);
    }
    
    int getLastKey()
    {
        fstream file;
        file.open("usernames.dat", ios::in);
        file.seekg(0, ios::end);
        
        if(file.tellg() == -1)
            return 0;
        
        
        string s;
        
        for (int i = -1; s.find("#") == string::npos; i--)
        {
            file.seekg(i, ios::end);
            file >> s;
        }//if # is found at a nonvalid position, its false
        
        file.close();
        s.erase(0, 5);//erase first 5, aka #Key:
        
        int key;
        istringstream(s) >> key;//convert string to integer
        
        return key;
    }
    
    int CheckFile(string attempt, const char* param_filename)
    {
        string line;
        fstream file;
        int PrtcChar;
        string cChar;
        
        file.open(param_filename, ios::in);//reading file
        
        while(1) //infinitely
        {
            file >> cChar;
            if (cChar.find("#Key:") != string ::npos)//not an invalid postition
            {
                if(attempt == line)
                {
                    file.close();
                    cChar.erase(0,5);//deletes #Key:
                    int key;
                    istringstream(cChar) >> key;
                    return key;
                }
                else
                {
                    line.erase(line.begin(), line.end());
                }
            }
            else
            {
                istringstream(cChar) >> PrtcChar;
                line += (char)Decrypt(PrtcChar); //overload
                cChar = " ";
            }
            
            if(file.peek()== EOF)
            {
                file.close();
                return 0;
            }
        }
    }// end of int CheckFile
    
    void SaveFile(string param_line, const char* param_filename, const int& key)
    {
        fstream file;
        file.open(param_filename, ios::app);//writing to file, not overwrite but append
        file.seekg(0, ios::end);//set cursor very end of file
        
        if(file.tellg() != 0)
        {
            file << "\n";
        }//setting cursor end of file, with .seekg and .tellg to "tell" where the cursor is.
        
        file.seekg(0, ios::beg);
        
        for(int i = 0; i < param_line.length(); i++ )
        {
            file << Encrypt(param_line[i]);
            file <<"\n"; // differentiate between characters
        }// to encrypt each letter
        file << "#Key:" << key;
        file.close();
    }// end of string SaveFile, const char* less memory than string, but not as manipulable
    
    int Encrypt(int param_letter)
    {
        return powf(param_letter, 4);
    }// end of int Encrypt, float for less memory instead of double
    
    int Decrypt(int param_letter)
    {
        return powf(param_letter, 1/4.f);
    }// end of int Decrypt (to reverse encryption), float for less memory instead of double
};




class StudentStore: public LoginTeacher, public Student{
    
private:
    vector<Student> stu_store;
    map<string, Student> students_store;
    string fileName;
public:
    
    void readFile(){
        char elcomma;
        string header;
        ifstream inFile;
        fileName = getFileName();
        inFile.open(fileName);
        if(inFile.fail()){
            cerr << "The file for the class does not exist yet.\n";
        }
        getline(inFile, header);
        while(1){
 
//            inFile >> UID
//            >> elcomma
//            >> netID
//            >> elcomma
//            >> name
//            >> elcomma
//            >> age
//            >> elcomma
//            >> gender
//            >> elcomma
//            >> DoB
//            >> elcomma;
            getline(inFile, UID, ',');
            getline(inFile, netID, ',');
            getline(inFile, f_name, ',');
            getline(inFile, s_name, ',');
            inFile >> age >> elcomma >> gender >> elcomma >> DoB;
            getline(inFile, header, '\n');

            if(inFile.eof()){
                break;
            }

//            cout << UID;
//            cout  << ", " << netID;
//            cout  << ", " << f_name;
//            cout  << ", " << s_name;
//            cout  << ", " << age;
//            cout  << ", " << gender;
//            cout  << ", " << DoB << endl;
            students_store[UID] = Student(UID, netID, f_name, s_name, age, gender, DoB);

        }
        inFile.close();
    }
    
    void addStudent(){
        
        char sure;
        cout<<"Enter First Name : ";
        cin >> f_name;
        cout<<"Enter Last Name : ";
        cin >> s_name;
        cout<<"Enter Age : ";
        cin >> age;
        cout<<"Enter Date of Birth (DDMMYY) : ";
        cin >> DoB;
        do{
            cout << "Enter Gender (m/f): ";
            cin >> gender;
        } while(gender != 'm' && gender != 'f');
        UID = generateIDs(f_name, s_name, gender, DoB, false);
        map<string,Student>::iterator t;
        t = students_store.find(UID);
        if(t==students_store.end()){
            stu_store.push_back(Student(UID, netID, f_name, s_name, age, gender, DoB));
            students_store[UID] = Student(UID, netID, f_name, s_name, age, gender, DoB);
            cout << "Success!\n";
        } else{
            cout << "\nERROR: The student is already registered!\n" << endl;
            cout << "Are you sure this is a differnet student and still want to proceed? (y/n): ";
            cin >> sure;
            while(sure == 'y'){
                UID = generateIDs(f_name, s_name, gender, DoB, true);
                t = students_store.find(UID);
                if(t==students_store.end()){
                    students_store[UID] = Student(UID, netID, f_name, s_name, age, gender, DoB);
                    cout << "Success!\n";
                    sure = 'n';
                } else{
                    cout << "\nERROR: The student is already registered!\n" << endl;
                    cout << "Are you sure this is a differnet student and still want to proceed? (y/n): ";
                    cin >> sure;
                }
            }
        }
        exportCSV();
    }
    void displayStudents(){
        readFile();
        if(students_store.empty()==1){
            cout<<"No students"<<endl;
        }
        else{
            for (map<string, Student>::iterator i = students_store.begin(); i != students_store.end(); i++) {
                i->second.print();
            }
        }
    }
    
    void deleteStudent(){
        readFile();
        string ID;
        cout << "Enter the UID of the student you like to delete: ";
        cin >> ID;
        map<string,Student>::iterator t;
        t = students_store.find(ID);
        if (t != students_store.end()){
            students_store.erase(t);
            exportCSV();
            cout << "Success!" << endl;
        } else{
            cout << "\nERROR: The student doesn't exist. Try Again!\n\n";
        }
    }
    
    void editStudent(){
        readFile();
        string ID;

        cout << "Enter the ID of the student you like to edit: ";
        cin >> ID;
        
        map<string,Student>::iterator t;
        t = students_store.find(ID);
        
        if (t == students_store.end()){
            cout << "\nERROR: The student doesn't exist \n\n";
        } else{
            cout << "Select the attribute you want to edit: ";
            int sel;
            do{
                cout <<"\n====================="<< endl;
                cout <<"[1] NAME"<< endl;
                cout <<"[2] AGE"<< endl;
                cout <<"[3] GENDER"<< endl;
                cout <<"[4] Date of Birth"<< endl;
                cout <<"[5] EXIT"<< endl;
                cout <<"====================="<< endl;
                cout <<"Enter Selection : ";
                cin>>sel;
                if(sel == 1){
                    cout<<"Enter First Name : ";
                    cin >> f_name;
                    cout<<"Enter Last Name : ";
                    cin >> s_name;
                    t->second.SetName(f_name, s_name);
                }
                else if(sel == 2){
                    cout<<"Enter Age : ";
                    cin >> age;
                    t->second.SetAge(age);
                }
                else if(sel == 3){
                    cout << "Enter the gender (m/f): ";
                    cin >> gender;
                    t->second.SetGender(gender);
                }
                else if(sel == 4){
                    cout << "Enter the Date of Birth (DDMMYY): ";
                    cin >> DoB;
                    t->second.SetGender(gender);
                }
                else{
                    if (sel!=5) {
                        cout << "Please enter a valid selection. Try Again" << endl;
                    }
                    break;
                }
            } while(sel!=4);
            cout << "Success!" << endl;
        }
        exportCSV();
        
    }

    void exportCSV(){
        fileName = getFileName();
        ofstream exportFile;
        if(exists(fileName)!=true){
            exportFile.open(fileName, ios::trunc);
            exportFile << "UID," << "netID," << "First Name," << "Last Name," << "Age," << "Gender," << "Date of Birth" << "\n";
            if(exportFile.fail()){
                cerr << "Could not open and export the output file. \n";
            }
            for (map<string, Student>::iterator t = students_store.begin(); t != students_store.end(); t++) {
                exportFile << t->second.GetUID() << "," << t->second.GetnetID() << "," << t->second.getFname()
                << "," << t->second.getSname() << "," << t->second.GetAge() << "," << t->second.GetGender() << "," << t->second.getDoB() << "\n";
            }
            exportFile.close();
        } else {
            exportFile.open(fileName, ios::trunc);
            if(exportFile.fail()){
                cerr << "Could not open and export the output file. \n";
            }
            for (map<string, Student>::iterator t = students_store.begin(); t != students_store.end(); t++) {
                exportFile << t->second.GetUID() << "," << t->second.GetnetID() << "," << t->second.getFname()
                << "," << t->second.getSname() << "," << t->second.GetAge() << "," << t->second.GetGender() << "," << t->second.getDoB() << "\n";
            }
            exportFile.close();
        }
    }
    
};




int main() {
    
    
    int sel;
    StudentStore myGrade;
    myGrade.Initialize();
    do{
        cout <<"====================="<< endl;
        cout <<"[1] ADD STUDENT"<< endl;
        cout <<"[2] DISPLAY STUDENTS"<< endl;
        cout <<"[3] EDIT STUDENTS"<< endl;
        cout <<"[4] DELETE STUDENTS"<< endl;
        cout <<"[5] EXPORT INTO CSV"<< endl;
        cout <<"[6] EXIT" << endl;
        cout <<"====================="<< endl;
        cout <<"Enter Selection : ";
        cin>>sel;
        
        if(sel == 1){
            myGrade.addStudent();
        }
        else if(sel == 2){
            myGrade.displayStudents();
        }
        else if(sel == 3){
            myGrade.editStudent();
        }
        else if(sel == 4){
            myGrade.deleteStudent();
        }
        else if(sel == 5){
            cout << "Exporting Data..." << endl;
            myGrade.exportCSV();
        }
        else{
            if (sel!=6) {
                cout << "Please enter a valid selection. Try Again" << endl;
            }
            break;
        }
    } while(sel!=6);
    return 0;
}
