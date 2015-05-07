//
//  Scanner.h
//  CompilersHomework1
//
//  Created by Ronald Brown on 9/4/12.
//  Copyright (c) 2012 RonBrown. All rights reserved.
//

#ifndef __CompilersHomework1__Scanner__
#define __CompilersHomework1__Scanner__

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;


typedef struct entry {
    string token;
    string str;
    int count;
    

} entry;




class Scanner {
public:
    bool foundQuote;
    int varFollowedByFloatorIntCounter;
    bool justAddedVar;
    vector<entry *> varList;
    vector<entry *> floatList;
    vector<entry *> intList;
    vector<string *> quoteList;
    ifstream myfile;
    
    //Constructors
    Scanner();
    Scanner(string *fName);
    
    //Prototypes
    bool CheckLine(string line);
    void CreateEntry(string token, string str, string ty);
    string *SplitString(string str);
    void CheckString(string str);

    string CheckForVar(string str);
    string CheckForInt(string str);
    string CheckForFloat(string str);
    

    
    void Tokenize(const string& str, vector<string>& tokens);
    ~Scanner();
    
private:
    bool isValidChar(char c);
};





#endif /* defined(__CompilersHomework1__Scanner__) */
