#include <iostream>
#include <string>

#include "GLOBALS.cpp"

using namespace std;

void showKnownGlobals(){
  cout  << "Known Globals:"
        << "\n\tBALANCE = "   <<  BALANCE
        << "\n\tLENGTH = "    <<   LENGTH
        << "\n\tFILENAME = "  << FILENAME
        << "\n\tCOMPILED = "  << COMPILED
        << "\n\tDEBUG = "     <<    DEBUG
        << endl;
}

int main(int argc, char const *argv[]) {
  showKnownGlobals();
  FetchGlobalVariables();
  /*
  if(DEBUG){
    cout << "\nIn Main\n";
    if(argc > 1){
      cout << "All input arguments\n";
      for(int i = 0; i < argc; i++){
        cout <<"\t" << i << ": " << argv[i] << endl;
      }
    }
    getUndefinedVariables();
  }
  */
  rewrite();
  showKnownGlobals();

  if(DEBUG){cout << "\nMain complete\n";}

  return 0;
}
