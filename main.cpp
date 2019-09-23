//Brendan Dunne
//8 September 2019

#include <iostream>
#include <string>
#include "dClass.h"
using namespace std;

int main()
{
    bool quit = false;
    //filler strings, don't worry about it
    string friday_the, out_of_10;
    Directory* curDir = new Directory;
    printCode(13, friday_the);
    printCode(10, out_of_10);
    do
    {
        cout<<"/";
        string str;
        //take input to be passed to parsing function
        getline(cin, str);
        string command = comParse(str, 0);
        string object = comParse(str, 1);
        string chmodFile = comParse(str, 2);

        //if-then statements for each command that call respective functions
        if(command == "quit")
        {
            printCode(12, object);
            quitFunc(curDir);
            quit=true;
        }
        else if(command == "ls")
        {
            curDir->lsFunc(object);
        }
        else if(command == "cd")
        {
          if(object=="..")
            curDir=cdUpFunc(curDir);
          else
            curDir=cdFunc(curDir, object);
        }
        else if(command == "pwd")
        {
            pwdFunc(curDir);
        }
        else if(command == "mkdir")
        {
            mkDir(curDir, object);
        }
        else if(command == "rmdir")
        {
            rmdirFunc(curDir, object);
        }
        else if(command == "rm")
        {
            curDir->rmFunc(object);
        }
        else if(command == "chmod")
        {
            curDir->chmodFunc(object, chmodFile);
        }
        else if(command == "touch")
        {
            curDir->touchFunc(object);
        }
        else if(command == "help")
        {
          printCode(10, object);
        }
        else
            printCode(11, object);
    }while(!quit);
    return 0;
}
