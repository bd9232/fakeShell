#ifndef DCLASS_H
#define DCLASS_H

#include <vector>
#include <utility>
#include <ctime>
using namespace std;

void printCode(int num, string &obj);//forward declaration

string input(string &str, int part);//forward declaration

class Directory
{
  public:
    pair<string, string> id;//id, first=name, second=timestamp
    string permiss;//permission string
    vector< pair<string, string>> files;
    vector<string> filesper;
    vector<Directory *> children;
    Directory *parent;

    Directory()//constructor
    {
      parent = NULL;//set parent directory to NULL
      id.first = "home";
      time_t now = time(0);
      id.second = ctime(&now);//initialize timestamp
      permiss="777";//default permission string
      files.resize(1);//initialize file vector
      filesper.resize(1);//initialize file permission vector
      children.resize(1);//initialize subdirectory vector
      for(int i=0;i<1;i++)
      {
        children[i]= NULL;//initialize subdirectories
      }
    }

    //destructor
    ~Directory()
    {

    }

    //prints the contents of the current directory
    //prints permissions and timestamps if -l flag is included
    void lsFunc(string &obj)
    {
      for(int i=0;i<files.size();i++)
      {
        if(files.at(i).first.length())
        {
          cout<<"\033[1;32m"<<files.at(i).first<<"\033[0m";//print file name
          if(obj == "-l")//print file permissions/timestamps
            cout<<"   "<<filesper.at(i)<<"   \033[1;35m"<<files.at(i).second<<"\033[0m";
          cout<<endl;
        }
      }
      for(int i=0;i<children.size();i++)
      {
        if(children[i]!=NULL)
        {
          cout<<"\033[1;36m"<<children[i]->id.first<<"\033[0m";//print directory name
          if(obj == "-l")//print directory permissions/timestamps
            cout<<"   "<<children[i]->permiss<<"   \033[1;35m"<<children[i]->id.second<<"\033[0m";
          cout<<endl;
        }
      }
      return;
    }

    //creates a file with indicated name if it does not already exist
    //updates timestamp if such a file/directory already exists
    void touchFunc(string &obj)
    {
      //update directory if it exists
      for(int i=0;i<children.size();i++)
      {
        if(children[i]!=NULL)
        {
          if(children[i]->id.first==obj)
          {
            //change directory timestamp
            time_t now = time(0);
            children[i]->id.second = ctime(&now);
            printCode(1, obj);
            return;
          }
        }
      }

      for(int i=0;i<files.size();i++)
      {
        //update file if it exists
        if(files[i].first==obj)
        {
          //change file timestamp
          time_t now = time(0);
          files[i].second = ctime(&now);
          printCode(1, obj);
          return;
        }
        //create file if it does not exist
        if(!files[i].first.length())
        {
          //creates space for file in vector
          files.resize(files.size()+1);
          filesper.resize(filesper.size()+1);
          files[i].first=obj;//set name
          time_t now = time(0);
          files[i].second = ctime(&now);//create timestamp
          filesper[i]=permiss;//set permission
          printCode(2, obj);
          return;
        }
      }
      return;
    }

    //removes the indicated file
    void rmFunc(string &obj)
    {
      for(int i=0;i<files.size();i++)
      {
        //remove file if it exists
        if(files[i].first==obj)
        {
          files.erase(files.begin()+i);//remove file from vector
          filesper.erase(filesper.begin()+i);//remove file permission
          printCode(3, obj);
          return;
        }
      }
      //if file does not exist
      printCode(4, obj);
      return;
    }

    //changes permission string for indicated file/directory
    void chmodFunc(string &per, string &obj)
    {
      string perKey = "01234567";
      int valid=0;
      //check for valid file/directory name
      if(obj=="")
      {
        printCode(5, obj);
        return;
      }
      //check for valid permission string length
      if(per=="" || per.length()!=3)
      {
        printCode(6, per);
        return;
      }

      //check validity of permission string against key
      for(int i=0;i<per.length();i++)
      {
        for(int j=0;j<perKey.length();j++)
        {
          if(per[i]==perKey[j])
            valid++;
        }
      }
      //if the permission string is invalid
      if(valid!=3)
      {
        printCode(6, per);
        return;
      }

      //check if directory
      for(int i=0;i<children.size();i++)
      {
        if(children[i]!=NULL)
        {
          if(children[i]->id.first==obj)
          {
            children[i]->permiss=per;
            printCode(7, obj);
            return;
          }
        }
      }

      //check if file
      for(int i=0;i<files.size();i++)
      {
        if(files[i].first==obj)
        {
          filesper[i]=per;
          printCode(7, obj);
          return;
        }
      }
      //if there is no such file/directory
      printCode(4, obj);
      return;

    }
};

//makes a new subdirectory with desired name if one does not already exist
void mkDir(Directory* curDir, string &obj)
{
  //check if name is available
  for(int i=0;i<curDir->children.size();i++)
  {
    if(curDir->children[i]!=NULL && curDir->children[i]->id.first==obj)
    {
      //name is already taken by another directory
      printCode(8, obj);
      return;
    }
    if(curDir->files[i].first==obj)
    {
      //name is already taken by a file
      printCode(8, obj);
      return;
    }
  }

  //making space for new directory
  curDir->children.resize(curDir->children.size()+1);
  //creating new directory
  for(int i=0;i<curDir->children.size();i++)
  {
    if(curDir->children[i]==NULL)
    {
      curDir->children[i]= new Directory;
      curDir->children[i]->id.first=obj;
      time_t now = time(0);
      curDir->children[i]->id.second = ctime(&now);
      curDir->children[i]->permiss = curDir->permiss;
      printCode(2, obj);
      return;
    }
  }
  return;
}

//travels to indicated subdirectory, if it exists
Directory * cdFunc(Directory* curDir, string &obj)
{
  for(int i=0;i<curDir->children.size();i++)
  {
    if(curDir->children[i]!=NULL)
    {
      if(curDir->children[i]->id.first==obj)
      {
        //link directories so travel is possible
        curDir->children[i]->parent = curDir;
        //return desired directory
        return(curDir->children[i]);
      }
    }
  }
  //if the directory does not exist
  printCode(4, obj);
  return(curDir);
}

//travels to parent directory, if it exists
Directory * cdUpFunc(Directory* curDir)
{
  string filler = "boyisurehopeipass";
  //check if the parent directory exists
  if(curDir->parent!=NULL)
    return(curDir->parent);
  else
  printCode(9, filler);
  return(curDir);
}

//recursively calls up to the home directory keeping track of the path
void pwdFunc(Directory* curDir)
{
  vector<string> wD;
  if(curDir->parent!=NULL)//checking for parent directory
  {
    wD.push_back(curDir->id.first);
    pwdFunc(curDir->parent);
  }
  if(curDir->parent==NULL)//at home directory
    wD.push_back(curDir->id.first);
  for(int i=0;i<wD.size();i++)
    cout<<"/"<<wD[i];//print the directory path
  return;
}

void delFunc(Directory* curDir);//forward declaration

//if the directory name is valid, calls delete function on it
void rmdirFunc(Directory* curDir, string &obj)
{
  for(int i=0;i<curDir->children.size();i++)
  {
    if(curDir->children[i]!=NULL)
    {
      if(curDir->children[i]->id.first==obj)
      {
        delFunc(curDir->children[i]);
        curDir->children.erase(curDir->children.begin()+i);//removes directory from vector
        printCode(3, obj);
        return;
      }
    }
  }
  //if name is invalid
  printCode(4, obj);
  return;
}

//recursively travels down directory subtree and deletes all subdirectories
//and then finally the intial directory
void delFunc(Directory* curDir)
{
  if (curDir == NULL)
    return;
  for(int i=0;i<curDir->children.size();i++)
  {
      if(curDir->children[i]!=NULL)
        delFunc(curDir->children[i]);//recursive call on subdirectory
  }
  delete curDir;//delete directory
  curDir=NULL;//remove dangling pointer
  return;
}

//recursively travels to home directory then calls delete function on itself
void quitFunc(Directory* curDir)
{
  if(curDir->parent!=NULL)//if not at home
    quitFunc(curDir->parent);
  if(curDir->parent==NULL)//if at home
    delFunc(curDir);
  return;
}

//prints the appropriate message for the action
void printCode(int num, string &obj)
{
  switch(num)
  {
    case 1:
      cout<<obj<<" has been touched.\n";
      break;
    case 2:
      cout<<obj<<" has been created.\n";
      break;
    case 3:
      cout<<obj<<" has been removed.\n";
      break;
    case 4:
      cout<<obj<<" does not exist.\n";
      break;
    case 5:
      cout<<"You must choose a file or directory.\n";
      break;
    case 6:
      cout<<obj<<" is not a valid permission string.\n";
      break;
    case 7:
      cout<<obj<<" permission has been updated.\n";
      break;
    case 8:
      cout<<obj<<" already exists.\n";
      break;
    case 9:
      cout<<"You're already in the home directory.\n";
      break;
    case 10:
      cout<<"Available commands: ls, cd, pwd, mkdir, rmdir, rm,";
      cout<<" chmod, touch, quit.\n";
      break;
    case 11:
      cout<<"Invalid command. Try 'help' for a list of commands.\n";
      break;
    case 12:
      cout<<"Quitting. Come again!\n";
      break;
    case 13:
      cout<<"\033[1;33mWelcome to the Real Fake Shell!\n"<<
      "It looks and feels like a real shell but it doesn't do anything!\n"<<
      "Absolutely nothing! And it almost always works!\033[0m\n";
      break;
    default:
      cout<<"I really hope I get an A on this.\n";
      break;
  }
}

//splits the user input into three parts: the command, the object of the command,
//and the permission string(only used with chmod)
string comParse(string &str, int part)
{
  string input[3];
  int counter = 0;
  for(int i=0;i<str.length();i++)
  {
    if(str[i] == ' ')
    {
      if(i!=0)
      {
        counter++;
        i++;
      }
      else if(i==0)
        i++;
    }
    if(counter<3)
      input[counter] += str[i];
  }
  return(input[part]);
}

#endif
