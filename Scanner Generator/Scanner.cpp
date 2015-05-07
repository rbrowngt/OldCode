//
//  Scanner.cpp
//  CompilersHomework1
//
//  Created by Ronald Brown on 9/4/12.
//  Copyright (c) 2012 RonBrown. All rights reserved.
//

#include "Scanner.h"
#include <iostream>
#include <stdio.h>
#include <typeinfo>
#include <strings.h>
#include <fstream>
#include <algorithm>
#include <stdio.h>
using namespace std;



/**
 Algorithm
    - take input file   
    - Scan and generate tokens and strings based off following rules:
    VAR : Begins with a lowercase (a-z) or uppercase letter (A-Z) and is followed by one or more of the same or by a digit (0-9) or a hyphen (-)
 
    INT : Starts with a digit (0-9) followed by one or more of the same (0-9)
        Regex: [0-9][0-9]+
 
    FLOAT: Starts with a digit (0-9) followed by zero or more of (0-9) followed by a dot (.) followed by zero or more of (0-9)\
        Regex: [0-9]([0-9]* | [.0-9]*)
 
    Must use longest match rule.
    Longest Match Rule:
        Find token.
        Follow description above and read until it finds somthing that is not part of the token
        Terminate the token and return it with associated string 
    Everything else is ignored
 
*/

bool CompareByStr(entry *a,  entry *b) {
    string left = a->str;
    string right = b->str;
    
    for(string::const_iterator lit = left.begin(), rit = right.begin(); lit != left.end() && rit != right.end(); ++lit, ++rit )
        if(*lit  < *rit)
            return true;
        else if(*lit  > *rit)
            return false;
    if( left.size() < right.size() )
        return true;
    return false;
}

bool CompareByCount(entry *a, entry *b) {
    return a->count == b->count? CompareByStr(a, b) : a->count > b->count;
}

Scanner::Scanner() {
    cout << "Constructor called" << endl;
}
Scanner::Scanner(string *fName) {
    foundQuote = false;
    string quoteString;
    varFollowedByFloatorIntCounter = 0;
    justAddedVar = false;
    string line;
    myfile.open(fName->c_str());
    if(myfile.is_open()) {
        while(myfile.good()) {
            getline(myfile, line);
            
            for(int i = 0; i < line.length(); i++) {
            if(line[i] == '"') {
                if(foundQuote) {
                        quoteList.push_back(new string(quoteString));
                        quoteString = "";
                    }
                    foundQuote = !foundQuote;
                    //cout << "Quote found" << endl;
                }
                if(foundQuote) {
                    if(line[i] != '"') {
                        quoteString.push_back(line[i]);
                    }
                }
            
            }
            
            CheckString(line);
        }
        myfile.close();
    }

    sort(varList.begin(), varList.end(), CompareByStr);
    sort(intList.begin(), intList.end(), CompareByStr);
    sort(floatList.begin(), floatList.end(), CompareByStr);
   
   
}

/*
 This function will determine if the passed in string meets the requirements for being a var
 */
string Scanner::CheckForVar(string str) {
    string parsedStr;
    int i = 0;
    if(islower(str.at(i)) || isupper(str.at(i))) {
        //See if it follows var rules
        i++;
        if(islower(str[i]) || isupper(str[i]) || isdigit(str[i]) || str[i] == '-') {
            parsedStr.push_back(str[0]);
            parsedStr.push_back(str[1]);
            i++;
            for(; i < str.length(); i++) {
                if(isValidChar(str[i])) {
                    parsedStr.push_back(str[i]);
                }
                else {
                    string v = "var";
                    CreateEntry(v, parsedStr, v);
                    parsedStr = str.substr(i, str.length());
                    str = CheckForInt(parsedStr);
                    i = 0;
                    parsedStr = "";
                }
            }//End For Loop
            string v = "var";
            if(parsedStr.length() > 1) {
                CreateEntry(v, parsedStr, v);
            }
        }//End If
        else if(str.length() > 2) {
            str = str.substr(1, str.length());
            CheckForVar(str);
        }
    }//End If
    else if(str.length() < 2) {
        return "";
    }
    else if(!isdigit(str[i])) {
        str = str.substr(1, str.length());
        CheckForVar(str);
    }
    else {
        CheckForInt(str);
    }
    
    
    return "";
}
/*
 This function will determine if the passed in string meets the requirements for being a Int
 */
string Scanner::CheckForInt(string str) {
    
    int i = 0;
    string parsedStr = "";
    bool isFloat = false;
    
    //Check to see if float or int
    if(isdigit(str[i])) {
        parsedStr.push_back(str[i]);
        i++;
        size_t pos = str.find(".");
        if(string::npos != pos) {
            //Found a match
            isFloat = true;
            
            //cout << "String contains a ., check for float because previous char is a digit" << endl;
            //CheckForFloat(str);
            //isFloat = true;
        }
        //Ints must have length 2 atleast
        if(isdigit(str[i])) {
            parsedStr.push_back(str[i]);
            i++;  
        for(;i < str.length(); i++) {
            if(isFloat) {
                if(str[i]== '.') {
                    
                    CheckForFloat(str);
                    return "";
                }
            }
            //Check to see if it's a .
            if(isdigit(str[i])) {
               parsedStr.push_back(str[i]); 
            }
            else {
                string v = "int";
                CreateEntry(v, parsedStr, v);
                parsedStr = str.substr(i, str.length());
                //return parsedStr;
                str = CheckForVar(parsedStr);
                parsedStr = "";
            }
        }//End for Loop
            string v = "int";
            if(parsedStr.length() > 1) {
                CreateEntry(v, parsedStr, v);
            }
        }//End check for 2nd digit
        else {
            //Let's see if it could be a float
            size_t pos = str.find(".");
            if(string::npos != pos) {
                //Found a match
                //cout << "String contains a ., check for float because previous char is a digit" << endl;
                CheckForFloat(str);
                //isFloat = true;
            }
            else {
                str = str.substr(1, str.length());
                CheckForVar(str);
            }
        }
    }//End-if (isDigit)
    else if(str.length() < 2) {
        return "";
    }
    else if(!isdigit(str[i])) {
        str = str.substr(1, str.length());
        CheckForVar(str);
    }
    else {
         CheckForVar(str);
    }
    return "";
}
/*
 This function will determine if the passed in string meets the requirements for being a Float
 */
string Scanner::CheckForFloat(string str) {
    int i = 0;
    string parsedStr = "";
    bool foundDecimal = false;
    //Must start with a digit
    if(isdigit(str[i])) {
        //Must contain a . somewhere so let's find out if it does.
        size_t pos = str.find(".");
        if(string::npos != pos) {
            //It does contain a dot
            parsedStr.push_back(str[i]);
            i++;
        }
        else {
            str = str.substr(1, str.length());
            CheckForVar(str);
            return "";
        }
        /*
            Adding rules:
                if . already found, then add all digits until invalid char
                if . NOT found yet, add digits until . found OR invald char
                    if invalid char found and no dot, then not valid string for float
         */
        for(; i < str.length(); i++) {
            if(!foundDecimal) {
                str[i] == '.' ? foundDecimal = true : foundDecimal = foundDecimal;
                if(foundDecimal) {
                    //if you found a decimal, add it
                    parsedStr.push_back(str[i]);
                }
                else {
                    if(isdigit(str[i])){
                        parsedStr.push_back(str[i]);
                    }
                    else {
                        
                        parsedStr = str.substr(i, str.length());
                        //return parsedStr;
                        str = CheckForVar(parsedStr);
                        parsedStr = "";
                    }
                }
            }//end check for decimal
            else {
                if(isdigit(str[i])){
                    //You've already found a decimal and this is a number, add it
                    parsedStr.push_back(str[i]);
                }
                else {
                    //You found an invalid char/value
                    string v = "float";
                    CreateEntry(v, parsedStr, v);
                    parsedStr = str.substr(i, str.length());
                    //return parsedStr;
                    str = CheckForVar(parsedStr);
                    parsedStr = "";
                }
            }
            
        }//End for loop
        string v = "float";
        if(parsedStr.length() >= 2) {
            CreateEntry(v, parsedStr, v);
        }

        
        
        
    }
    //string v = "float";
    //CreateEntry(v, str);
    else if(str.length() < 2) {
        return "";
    }
    else if(!isValidChar(str[i])) {
        str = str.substr(1, str.length());
        CheckForVar(str);
    }
    else {
        CheckForVar(str);
    }
    return "";
}


/*
VAR : Begins with a lowercase (a-z) or uppercase letter (A-Z) and is followed by one or more of the same or by a digit (0-9) or a hyphen (-)
 REGEX: [a-zA-Z][-a-zA-Z0-9]+
 
INT : Starts with a digit (0-9) followed by one or more of the same (0-9)
 Regex: [0-9][0-9]+
 
 FLOAT: Starts with a digit (0-9) followed by zero or more of (0-9) followed by a dot (.) followed by zero or more of (0-9)
 Regex: [0-9]([0-9]* | [.0-9]*)
 */
void Scanner::CheckString(string pstr) {
    string parsedStr;
    string quoteString;
    vector<string> tokens;
    Tokenize(pstr, tokens);
    for(vector<string>::size_type ii = 0; ii < tokens.size(); ii++) {
        parsedStr = "";
        string str = tokens.at(ii);
        string rString;
        
        rString = CheckForVar(str);
    }
}

void Scanner::Tokenize(const string& str, vector<string>& tokens) {
    const string& delimiters = " ";
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
    
    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}



bool Scanner::isValidChar(char c) {
    if (islower(c) || isupper(c) || isdigit(c) || (c == '-')) {
        return true;
    }
    else
        return false;
}


/*
    Function: Create a new entry and add it to the internalList
 */

void Scanner::CreateEntry(string token, string str, string ty) {
    
    if(ty == "var") {
        justAddedVar = true;
        for(int i = 0; i < varList.size(); i++) {
            if (strcmp(varList.at(i)->str.c_str(), str.c_str()) == 0) {
                varList.at(i)->count++;
                return;
            }
            
        }
        
        varList.push_back(new entry());
        varList[varList.size() - 1]->token = token;
        varList[varList.size() - 1]->str = str;
        varList[varList.size() - 1]->count = 1;
    }
    else if (ty == "int") {
        if(justAddedVar) {
            varFollowedByFloatorIntCounter++;
        }
        for(int i = 0; i < intList.size(); i++) {
            if (strcmp(intList.at(i)->str.c_str(), str.c_str()) == 0) {
                intList.at(i)->count++;
                return;
            }
            
        }
        
        intList.push_back(new entry());
        intList[intList.size() - 1]->token = token;
        intList[intList.size() - 1]->str = str;
        intList[intList.size() - 1]->count = 1;
        justAddedVar = false;
    }
    else {
        if(justAddedVar) {
            varFollowedByFloatorIntCounter++;
        }
        for(int i = 0; i < floatList.size(); i++) {
            if (strcmp(floatList.at(i)->str.c_str(), str.c_str()) == 0) {
                floatList.at(i)->count++;
                return;
            }
            
        }
        
        floatList.push_back(new entry());
        floatList[floatList.size() - 1]->token = token;
        floatList[floatList.size() - 1]->str = str;
        floatList[floatList.size() - 1]->count = 1;
        justAddedVar = false;
    }
   
}

Scanner::~Scanner() {
    cout << "Deconstructor called" << endl;
}


