/*
Name:		Christenson, Mark
File:		main.cpp
Description:	This program is to facilitate testing of other programs
		User options can be defined in main
    The main function is the last one
    Developer test function
      - Named devTest
      - Second to last function
      - Called in developer mode in CLI with x
      - Can safely be edited to change its behavior for testing
*/

#include <iostream>
#include <string.h>
#include <stdio.h>

#include "GLOBALS.cpp"

using namespace std;

//Shows passed arguments
void showArgs(int argc, char const *argv[]){
  if(argc >= 1){
    cout << "All input arguments\n";
    for(int i = 0; i < argc; i++){
      cout <<"\t" << i << ": " << argv[i] << endl;
    }
  }
}

//Shows global variables
void showKnownGlobals(){
  cout  << "Known Globals:"
        << "\n\tDEBUG = "     <<    DEBUG
        << endl;
}

//Calls two funtions to update global variables and rewrite
void update(){
  FetchGlobalVariables();
  rewrite();
}

//Test function to be called for option x for developer mode
void devTest(){
  cout << TEXT << endl;
}

/*
Developer Options - activated by leading option with d (ie: dx, dd, dxux)
  - x: Calls devTest, a test function for developer to call with developer mode options
  - d: displays arguments data by calling showArgs
  - u: runs update

User Options
  - d: activate developer mode
  - u: updates exsisting global variables during runtime to reflect those in the header
*/
int main(int argc, char const *argv[]) {
  for(int i = 1; i < argc; i++){
    //Define user options as a case of this switch statement
    switch (argv[i][0]) {
      case 'd':
        if(DEBUG){cout << "developer mode case called\n";}
        //This loop checks all options after developer mode is activated
        for(int j = 1; j < strlen(argv[i]); j++){
          //define developer options in this switch statement
          switch (argv[i][j]) {
            case 'x':
              if(DEBUG){cout << "x case called\n";}
              devTest();
              break;
            case 'd':
              showArgs(argc, argv);
              break;
            case 'u':
              if(DEBUG){cout << "Update case called\n";}
              update();
              break;
            default:
              cout << "ERROR: option is undefined\n";
          }//End of developer switch statement
        }
        if(strlen(argv[i]) == 1){
          cout << "ERROR: must choose an option\n";
        }
        break;
      case 'u':
        FetchGlobalVariables();
        break;
      default:
        cout << "ERROR: there is no defined case for argv #" << i << "\n";
    }//End of switch statement that includes user options
  }

  if(DEBUG){cout << "\nMain complete\n";}

  return 0;
}
