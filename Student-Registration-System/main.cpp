#include <iostream>
#include "students.h" //only students.h is called as the LoginTeacher class is inherited in the StudentStore class which
                      //acts as the "working class"-i.e. where everything happens.

using namespace std;

int main() {
    
    int *sel = new int; //creating a dynamic varible to monitor user's selection for the menu below
    StudentStore *myGrade = new StudentStore; //calling a dynamic object in heap as in the end, all records are stored in the file.
    myGrade->Initialize(); //calling the initializing function that would display the Register/Login Menu for the teachers
    
    //a loop that will keep displaying the menu until the teacher chooses to Log Out by selecting option 5
    if(myGrade->choice != 3){
        do{
            cout <<"====================="<< endl;
            cout <<"[1] ADD STUDENT"<< endl;
            cout <<"[2] DISPLAY STUDENTS"<< endl;
            cout <<"[3] EDIT STUDENTS"<< endl;
            cout <<"[4] DELETE STUDENTS"<< endl;
            cout <<"[5] LOG OUT" << endl;
            cout <<"====================="<< endl;
            cout <<"Enter Selection : ";
            cin>>*sel;
            
            if(*sel == 1){
                myGrade->addStudent();
            }
            else if(*sel == 2){
                myGrade->displayStudents();
            }
            else if(*sel == 3){
                myGrade->editStudent();
            }
            else if(*sel == 4){
                myGrade->deleteStudent();
            }
            else{
                if (*sel!=5) {
                    cout << "Please enter a valid selection. Try Again" << endl;
                }
                if (*sel == 5){
                    cout << endl;
                    myGrade->Initialize();
                    if(myGrade->choice != 3){ //in the case teacher deos not exits the application
                        *sel = 10;            //sel is set at 10 to make sure the main menu loop starts again
                        myGrade->clearMap();
                    }
                }
            }
        } while(*sel!=5);
    }
    //deleting variables from heap
    delete sel;
    delete myGrade;
    return 0;
}
