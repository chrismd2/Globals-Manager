/*>COMPILED<*/
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>

using namespace std;

vector< vector<string> > undefinedVariables;
vector<string> v;
/*>VARIABLES<*/
extern bool TESTTWO = true;
extern bool TEST = true;
extern double BALANCE = 313.54;
extern double LENGTH = 8;
extern string FILENAME = "MasterData.txt";
extern bool COMPILED = true;
extern bool DEBUG = false;						/**< Set true for debugging output	  */
/*>FUNCTIONS<*/
void rewriteHeader(){
  //Copy the original header into a temporary file
  if(true){
    ifstream header;
    ofstream TFile;
    string str = "";
    header.open("Header.txt");
    TFile.open("TFile.txt");
    while(header.peek() != EOF){
      getline(header, str);
      TFile << str;
      TFile << endl;
    }
    header.close();
    TFile.close();
  }

  //Modify the original header (temporary file) and save it in the new header
  ofstream header;
  ifstream TFile;
  string str = "";
  unsigned int index = 0;
  header.open("Header.txt");
  TFile.open("TFile.txt");
  getline(TFile, str);
  header << "CURRENT = true\n";
  while(TFile.peek() != EOF){
    getline(TFile, str);
    index = 0;

    //Change variable names in header to uppercase
    while(str[index] != ' '){
      header << char(toupper(str[index]));
      index++;
    }

    //Append assigned values of original header into the new header
    while(index < str.length()){
      header << str[index];
      index++;
    }

    header << endl;
  }
  header.close();
  TFile.close();
}

void rewriteVars(){
  ofstream TFile;
  ofstream globalsFile;
  ifstream currentFile;
  TFile.open("TFile.txt");
  currentFile.open("GLOBALS.cpp");

  //Find /*>VARIABLES<*/ in currentFile writing lines along the way
  string str;
  unsigned int position = 0;
  while(str != "/*>VARIABLES<*/"){
    getline(currentFile, str);
    if(position == 0){
      str = "/*>UNCOMPILED<*/";
    }
    position += str.length();
    TFile << str;
    TFile << "\n";
  }

  //Write undef vars TFile (choose auto type or comments)
  for (size_t i = 0; i < undefinedVariables.size(); i++) {
    string Tstr;
    TFile << "extern "; //here I skip the data type and store the rest into Tstr
    str = undefinedVariables[i].at(0);
    for (size_t j = 0; j < str.length(); j++) {
      char tChar = (str[j]);
      Tstr += char(toupper(tChar));
    }
    Tstr += " = ";
    str = undefinedVariables[i].at(1);
    if((str[0] <= int('9') && str[0] >= int('0')) || str[0] == '-'){
      TFile << "double ";
      Tstr += str;
    }
    if(str == "true" || str == "false"){
      TFile << "bool ";
      Tstr += str;
    }
    if(!(str[0] <= int('9') && str[0] >= int('0') || str[0] == '-') && !(str == "true" || str == "false")){
      TFile << "string ";
      Tstr += "\"";
      Tstr += str;
      Tstr += "\"";
    }
    TFile << Tstr;
    TFile << ";\n";
  }
  //Find variable name (ie DEBUG) in next line of currentFile
      // and write " = " << variableName << ";\n" to TFile
  //Get next line of currentFile and rinse and repeat until /*>FUNCTIONS<*/ is found
  getline(currentFile, str);
  while (str != "/*>FUNCTIONS<*/") {
    string var = "";
    string TStr = "";
    unsigned int index = 0;
    bool testing = false;
    if(testing && DEBUG){cout << str << endl;}
    if(str != ""){
      while(str[index] != '='){
        TFile << str[index];
        index++;
      }
      unsigned int eqIndex = index;
      TFile << str[index];
      index--;
      while(str[index] == ' '){index--;}
      while (str[index - 1] != ' ') {
        TStr += str[index];
        if(testing && DEBUG) {cout << TStr << endl;}
        index--;
      }
      TStr += str[index];
      if(testing && DEBUG){cout << TStr << endl;}
      for (size_t i = TStr.length(); i > 0; i--) {
        var += TStr[i-1];
      }
      if(DEBUG){cout << "Found "<< var <<" in GLOBALS variables\n";}

      // find var in header.txt
        // get value in header.txt
        // write value to TFile
        //find semicolon in str
      ifstream header;
      string oldStr = str;
      bool notFound = true;
      index = eqIndex + 1;
      header.open("Header.txt");
      while(header.peek() != EOF && notFound){
        str = "";
        TStr = "";
        getline(header, str);
        if(testing && DEBUG){
          cout << "str: " << str << endl;
          cout << "TStr: " << TStr << endl;
        }
        int strindex = 0;
        while (strindex < var.length() && var[strindex] == str[strindex]) {
          strindex++;
        }
        if(strindex == var.length()){
          if(DEBUG){cout << "Found " << var << " in header\n";}
          notFound = false;
        }
        while(!notFound && str[strindex] != '='){
          strindex++;
        }
        strindex++;
        while (!notFound && strindex < str.length()) {
          if(str[strindex] != ' '){TStr += str[strindex];}
          if(testing && DEBUG){cout << "TStr: " << TStr << endl;}
          strindex++;
        }
        if(!notFound && DEBUG){
          cout  << "The value found in the header.txt for "
                << var << ": "
                << TStr << "\n";
        }
        str = oldStr;
        while (str[index] == ' ' || str[index] == '=') {
          TFile << str[index];
          index++;
        }
/*******THIS IS THE AREA THE PROGRAM SHOULD CHECK THE DATA TYPE*****************/
        if(TStr[0] != '-' && TStr != "" && TStr != "true" && TStr != "false" && !(TStr[0] >= '0' && TStr[0] <= '9')){TFile << "\"";}
        TFile << TStr;
        if(TStr[0] != '-' && TStr != "" && TStr != "true" && TStr != "false" && !(TStr[0] >= '0' && TStr[0] <= '9')){TFile << "\"";}
        while (str[index] != ';') {
          index++;
        }
      }
      //write the rest of str to TFile
      while (index < str.length()) {
        TFile << str[index];
        index++;
      }
      TFile << "\n";
    }
    getline(currentFile, str);
  }
  TFile << str;
  TFile << endl;
  //Write lines of currentFile into TFile until /*>VALID OPTIONS<*/ is found
  while (str != "/*>VALID OPTIONS<*/") {
    getline(currentFile, str);
    TFile << str;
    TFile << "\n";
  }
  //Add new vars to if statements
  for (size_t j = 0; j < undefinedVariables.size(); j++) {
    for (size_t i = 0; i < undefinedVariables[j].at(0).length(); i++) {
      undefinedVariables[j].at(0)[i] = toupper(undefinedVariables[j].at(0)[i]);
    }
    TFile << "\n\t\tif(word == \"";
    TFile << undefinedVariables[j].at(0);
    TFile <<"\"){\n\t\t\t";
    TFile << "if(DEBUG){cout << word << \" is a valid option\";}\n\t\t\t";
    TFile << "validOption = true;\n\t\t\t";
    TFile << "word = \"\";\n\n\t\t\t";
    TFile << "while(correctHeader && index < str.length()\n\t\t\t\t";
    TFile << "&& (str[index] < int(\'A\') || str[index] > int(\'z\'))\n\t\t\t\t";
    TFile << "&& (str[index] < int(\'0\') || str[index] > int(\'9\'))\n\t\t\t\t";
    TFile << "&& str[index] != '-'){\n\t\t\t\t";
    TFile << "index++;\n\t\t\t";
    TFile << "}\n\n\t\t\t";
    TFile << "while(index < str.length()){\n\t\t\t\t";
    TFile << "word+=str[index];\n\t\t\t\t";
    TFile << "index++;\n\t\t\t";
    TFile << "}\n\t\t";
    TFile << "\n\t\t//New functionality for ";
    TFile << undefinedVariables[j].at(0);
    TFile << " starts here\n\t\t";

    if((undefinedVariables[j].at(1)[0] <= int('9') && undefinedVariables[j].at(1)[0] >= int('0')) || undefinedVariables[j].at(1)[0] == '-'){

    TFile << "double value = 0;\n\t\t";
    TFile << "int decimal = 1;\n\t\t";
    TFile << "bool fractional = false;\n\t\t";
    TFile << "bool negative = false;\n\t\t";
    TFile << "for (size_t i = 0; i < word.length(); i++) {\n\t\t\t";
      TFile << "if(word[i] != \'.\' && word[i] != \'-\'){\n\t\t\t";
          TFile << "value *= 10;\n\t\t\t";
          TFile << "value += int(word[i]) - int(\'0\');\n\t\t\t";
      TFile << "}\n\t\t";
      TFile << "if(fractional){\n\t\t\t";
        TFile << "decimal*=10;\n\t\t";
      TFile << "}\n\t\t";
      TFile << "if(word[i] == \'.\'){\n\t\t\t";
        TFile << "fractional = true;\n\t\t";
      TFile << "}\n\t\t";
      TFile << "if(word[i] == \'-\'){\n\t\t\t";
        TFile << "negative = true;\n\t\t";
      TFile << "}\n\t\t";
    TFile << "}\n\t\t";
    TFile << "value = value/decimal;\n\t\t";
    TFile << "if(negative){value = -1 * value;}";

    TFile << undefinedVariables[j].at(0);
    TFile << " = ";
    TFile << "value";
    TFile << ";";
    }
    if(undefinedVariables[j].at(1) == "true" || undefinedVariables[j].at(1) == "false"){
    TFile << "if(word == \"false\"){";
    TFile << undefinedVariables[j].at(0);
    TFile << " = false;}\n\t\t\t";
    TFile << "if(word == \"true\"){";
    TFile << undefinedVariables[j].at(0);
    TFile << " = true;}";
    }
    if(!(undefinedVariables[j].at(1)[0] <= int('9') && undefinedVariables[j].at(1)[0] >= int('0') || undefinedVariables[j].at(1)[0] == '-') && !(undefinedVariables[j].at(1) == "true" || undefinedVariables[j].at(1) == "false")){
    TFile << undefinedVariables[j].at(0);
    TFile << " = ";
    TFile << "word";
    TFile << ";";
    }

    TFile << "}\n";
  }
  //Write remaining lines of currentFile to TFile
  while (currentFile.peek() != EOF) {
    getline(currentFile, str);
    position += str.length();
    TFile << str;
    TFile << "\n";
  }
  TFile.close();

  currentFile.close();
  //write TFile to globalsFile
  globalsFile.open("GLOBALS.cpp");
  currentFile.open("TFile.txt");
  str = "";
  while(currentFile.peek() != EOF){
    getline(currentFile, str);
    globalsFile << str;
    globalsFile << "\n";
  }
  currentFile.close();
  globalsFile.close();
}
extern void rewrite(){
  ifstream lineCheck;
  string cppstr, hstr;
  lineCheck.open("GLOBALS.cpp");
  getline(lineCheck, cppstr);
  lineCheck.close();
  lineCheck.open("Header.txt");
  getline(lineCheck, hstr);
  lineCheck.close();
  if(cppstr == "/*>COMPILED<*/" && hstr == "CURRENT = false"){
    rewriteVars();
    rewriteHeader();
  }
  else{
    if(cppstr != "/*>COMPILED<*/"){
      cout  << "ERROR: Modify the first line in GLOBALS.cpp to read "
            <<"\"/*>COMPILED<*/\""
            <<" then recompile before writing again\n";
    }
    if(hstr != "CURRENT = false"){
      cout << "ERROR: Change CURRENT in Header.txt to be false to rewrite\n";
    }
  }

  cout << "NOTICE: Recompile to have global variables updated";
}

bool boolCheck(string boolValue){
  return (boolValue == "True" || boolValue == "TRUE" || boolValue == "true" || boolValue == "T" || boolValue == "t");
}
extern vector< vector<string> > getUndefinedVariables(bool show){
  if(DEBUG){cout << "\nGetting undef vars in GLOBALS.cpp\n";}
  if(show){
    cout << "Undefined variables:\n";
    for(int i = 0; i < undefinedVariables.size(); i++){
      cout << "\t" << undefinedVariables[i].at(0)
           << " = "
           << undefinedVariables[i].at(1) << endl;
    }
  }
  return undefinedVariables;
}
extern vector< vector<string> > getUndefinedVariables(){
  return getUndefinedVariables(DEBUG);
}

extern void FetchGlobalVariables(){
  if(DEBUG){cout << "\nFetch from GLOBALS.cpp\n";}
  ifstream header;
  bool correctHeader = true;
  header.open("Header.txt");
  if(header != NULL){
    if(DEBUG){cout << "Header.txt opened\n";}
    string str, word;
    int index = 0;
    getline(header, str);
    while(index < str.length() && str[index] != ' '){
      word += toupper(str[index]);
      index++;
    }
    if(DEBUG){cout << "Word: " << word << endl;}
    if(word!="CURRENT"){
      cout << "BAD FILE STARTING FROM SCRATCH\n";
      correctHeader = false;
    }
    //reset the Word
    word = "";
    //consume equals sign
    while(correctHeader && index < str.length() && (str[index] < int('A') || str[index] > int('z'))){
      index++;
    }
    while(correctHeader && index < str.length()){
      word+=str[index];
      index++;
    }
    if(DEBUG){cout << "Word: " << word << endl;}
    if(correctHeader && !boolCheck(word)){
      if(DEBUG){cout << "Header has been changed and global variables need to be updated\n";}
      unsigned int newWordCount = 0;
      while (header.peek() != EOF) {
        word = "";
        index = 0;
        getline(header, str);
        while(index < str.length() && str[index] != ' '){
          word+=str[index];
          index++;
        }
        if(DEBUG){cout << "Word: " << word << endl;}
        if(word != ""){
          bool validOption = false;
/*>VALID OPTIONS<*/

		if(word == "TESTTWO"){
			if(DEBUG){cout << word << " is a valid option";}
			validOption = true;
			word = "";

			while(correctHeader && index < str.length()
				&& (str[index] < int('A') || str[index] > int('z'))
				&& (str[index] < int('0') || str[index] > int('9'))
				&& str[index] != '-'){
				index++;
			}

			while(index < str.length()){
				word+=str[index];
				index++;
			}

		//New functionality for TESTTWO starts here
		if(word == "false"){TESTTWO = false;}
			if(word == "true"){TESTTWO = true;}}

		if(word == "TEST"){
			if(DEBUG){cout << word << " is a valid option";}
			validOption = true;
			word = "";

			while(correctHeader && index < str.length()
				&& (str[index] < int('A') || str[index] > int('z'))
				&& (str[index] < int('0') || str[index] > int('9'))
				&& str[index] != '-'){
				index++;
			}

			while(index < str.length()){
				word+=str[index];
				index++;
			}

		//New functionality for TEST starts here
		if(word == "false"){TEST = false;}
			if(word == "true"){TEST = true;}}

		if(word == "BALANCE"){
			if(DEBUG){cout << word << " is a valid option";}
			validOption = true;
			word = "";

			while(correctHeader && index < str.length()
				&& (str[index] < int('A') || str[index] > int('z'))
				&& (str[index] < int('0') || str[index] > int('9'))
				&& str[index] != '-'){
				index++;
			}

			while(index < str.length()){
				word+=str[index];
				index++;
			}

		//New functionality for BALANCE starts here
		double value = 0;
		int decimal = 1;
		bool fractional = false;
		bool negative = false;
		for (size_t i = 0; i < word.length(); i++) {
			if(word[i] != '.' && word[i] != '-'){
			value *= 10;
			value += int(word[i]) - int('0');
			}
		if(fractional){
			decimal*=10;
		}
		if(word[i] == '.'){
			fractional = true;
		}
		if(word[i] == '-'){
			negative = true;
		}
		}
		value = value/decimal;
		if(negative){value = -1 * value;}BALANCE = value;}

		if(word == "LENGTH"){
			if(DEBUG){cout << word << " is a valid option";}
			validOption = true;
			word = "";

			while(correctHeader && index < str.length()
				&& (str[index] < int('A') || str[index] > int('z'))
				&& (str[index] < int('0') || str[index] > int('9'))
				&& str[index] != '-'){
				index++;
			}

			while(index < str.length()){
				word+=str[index];
				index++;
			}

		//New functionality for LENGTH starts here
		double value = 0;
		int decimal = 1;
		bool fractional = false;
		bool negative = false;
		for (size_t i = 0; i < word.length(); i++) {
			if(word[i] != '.' && word[i] != '-'){
			value *= 10;
			value += int(word[i]) - int('0');
			}
		if(fractional){
			decimal*=10;
		}
		if(word[i] == '.'){
			fractional = true;
		}
		if(word[i] == '-'){
			negative = true;
		}
		}
		value = value/decimal;
		if(negative){value = -1 * value;}LENGTH = value;}

		if(word == "FILENAME"){
			if(DEBUG){cout << word << " is a valid option";}
			validOption = true;
			word = "";

			while(correctHeader && index < str.length()
				&& (str[index] < int('A') || str[index] > int('z'))
				&& (str[index] < int('0') || str[index] > int('9'))
				&& str[index] != '-'){
				index++;
			}

			while(index < str.length()){
				word+=str[index];
				index++;
			}

		//New functionality for FILENAME starts here
		FILENAME = word;}

		if(word == "COMPILED"){
			if(DEBUG){cout << word << " is a valid option";}
			validOption = true;
			word = "";

			while(correctHeader && index < str.length()
				&& (str[index] < int('A') || str[index] > int('z'))
				&& (str[index] < int('0') || str[index] > int('9'))
				&& str[index] != '-'){
				index++;
			}

			while(index < str.length()){
				word+=str[index];
				index++;
			}

		//New functionality for COMPILED starts here
		if(word == "false"){COMPILED = false;}
			if(word == "true"){COMPILED = true;}}
          if(word == "DEBUG"){
            if(DEBUG){cout << word << " is a valid option\n";}
            validOption = true;
            word = "";

            while(correctHeader && index < str.length()
              && (str[index] < int('A') || str[index] > int('z'))
              && (str[index] < 0 || str[index] > 9)){
              index++;
            }

            while(index < str.length()){
              word+=str[index];
              index++;
            }

            //Debug specific actions, other vars may need hands on development
            if(word == "false"){DEBUG = false;}
            if(word == "true"){DEBUG = true;}
          }
/*>INVALID OPTIONS<*/
          if(!validOption){
            if(DEBUG){cout << word << " not found, adding to undefined variables\n";}
            undefinedVariables.push_back(v);
            undefinedVariables[newWordCount].push_back(word);

            word = "";

            while(correctHeader && index < str.length() && str[index] != '-'  && (str[index] < '0' || (str[index] > '9' && str[index] < int('A')) || str[index] > int('z'))){
              index++;
            }
            while(index < str.length()){
              word+=str[index];
              index++;
            }

            undefinedVariables[newWordCount].push_back(word);

            if(DEBUG){cout << "As a vector\n\t" <<
                              undefinedVariables[newWordCount].at(0) <<
                              " = " <<
                              undefinedVariables[newWordCount].at(1) <<
                              endl;}
            newWordCount++;
          }
        }
      }
    }
  }
  header.close();
  header.open("Header.txt");
  if(!correctHeader || header == NULL){
    if(DEBUG){cout << "Entered repair block\n";}
    if(DEBUG && NULL){ cout << "Header.txt doesn't exist, new file made\n";}
    if(DEBUG && !correctHeader){ cout << "Replacing Header.txt\n";}
    ofstream writeHeader;
    writeHeader.open("Header.txt");
    writeHeader << "CURRENT = true\n";
    /*>HEADER VALUES<*/
    writeHeader << "DEBUG = ";
    if( DEBUG){writeHeader << "true"; }
    if(!DEBUG){writeHeader << "false";}
    writeHeader << "\n";
    writeHeader.close();
  }
  header.close();
}
