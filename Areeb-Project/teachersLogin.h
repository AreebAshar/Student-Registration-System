//  teachersLogin.h
//  Areeb-Project

#ifndef teachersLogin_h
#define teachersLogin_h

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

class LoginTeacher {
private:
    string username;
    string password;
    string className;
    string classFileName;

public:

    int choice;
    LoginTeacher(){} //default constructor
    
    //a function to display the regiser/login menu for the teachers
    void Initialize(){
        string w, x, y;
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
                cout << "Proceeding to login...\n";
                Login();
                break;
            }
            else if(choice == 2){
                Login();
                break;
            }
            else if(choice == 3){
                break;
            }
            else {
                if(choice != 3){
                    cout << "Please enter a valid choice. Try Again!" << endl;
                }
            }
        } while(choice!=3);
        
    }
    
    //a function to login the teachers into the program
    void Login()
    {
        cout << "Enter Class Name (i.e. 4A): ";
        cin >> className;
        int classKey = CheckFile(className, "classnames.dat");
        if(classKey!=0)
        {
            cout << "Username: ";
            cin >> username;
            
            int userKey = CheckFile(username, "usernames.dat");
            if(userKey!=0){
                cout << "Password: ";
                cin >> password;
                
                int passwordKey = CheckFile(password, "passwords.dat");
                if (userKey == passwordKey){
                    cout << "Logging in..." << endl;
                    setFileName(className);
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
    
    //a function to register the user
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
        
        //saving the class name, username, and passwords to respective files
        SaveFile(className, "classnames.dat", key);
        SaveFile(username, "usernames.dat", key);
        SaveFile(password, "passwords.dat", key);
    }
    
    //a setter function to set filename for the respective teacher's class
    void setFileName(string name){
        classFileName = name + ".csv";
    }
    
    //a getter function to retrive the filename
    string getFileName(){
        return classFileName;
    }
    
    //function that checks how many teachers are registered
    int getLastKey() {
        fstream file;
        file.open("usernames.dat", ios::in);
        file.seekg(0, ios::end); //setting cursor to end of file
        
        //checking if file is empty or not. if true, returns 0
        if(file.tellg() == -1) {
            return 0;
        }

        string s;
        
        for (int i = -1; s.find("#") == string::npos; i--) //if the # isn't found, returns false
        {
            file.seekg(i, ios::end);
            file >> s;
        }
        
        file.close();
        s.erase(0, 5); //erase first 5 characters , as "Key:n" is 5 characters where n is the number of teachers
        
        int key;
        istringstream(s) >> key; //convert string to integer
        
        return key;
    }
    
    //function to check class, username, and password in the respective files
    int CheckFile(string attempt, const char* filename) {
        string line;
        fstream inFile;
        int PrtcChar;
        string encChar;
        
        inFile.open(filename, ios::in);
        
        while(1) //infinite loop
        {
            inFile >> encChar;
            if (encChar.find("#Key:") != string ::npos) { //checks if "#Key:" exists
                if(attempt == line) {
                    inFile.close();
                    encChar.erase(0,5); //to delete the "#Key:"
                    int key;
                    istringstream(encChar) >> key;
                    return key;
                }
                else {
                    line.erase(line.begin(), line.end());
                }
            }
            else {
                istringstream(encChar) >> PrtcChar;
                line += (char)Decrypt(PrtcChar);
                encChar = " ";
            }
            
            // if statement to break infinite loop if EOF character is reached
            if(inFile.peek() == EOF) { //.peek() only checks what is next instead of actually assigning the content of the file
                inFile.close(); // closing the file
                return 0;
            }
        }
    }
    
    //functiont to save class, username, and password in the respective files
    void SaveFile(string line, const char* filename, const int& key) {
        fstream file;
        file.open(filename, ios::app); //appending to file
        file.seekg(0, ios::end); //set cursor to the end of file
        
        //checking if the file is empty or not, if not a newline is added to store new data
        if(file.tellg() != 0) {
            file << "\n";
        }
        
        file.seekg(0, ios::beg); //setting cursor to the beginning of file
        
        for(int i = 0; i < line.length(); i++ ) {
            file << Encrypt(line[i]); // to encrypt each letter
            file <<"\n";
        }
        file << "#Key:" << key;
        file.close();
    }
    
    //function to encrypt
    int Encrypt(int digit) {
        return powf(digit, 4);
    }
    
    //function to decrypt
    int Decrypt(int digit) {
        return powf(digit, 1/4.f);
    }
};
#endif /* teachersLogin_h */
