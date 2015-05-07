//
//  main.cpp
//  CompilersHomework1
//
//  Created by Ronald Brown on 9/4/12.
//  Copyright (c) 2012 RonBrown. All rights reserved.
//

#include <iostream>
#include "Scanner.h"
#include "QueryEngine.h"
using namespace std;


#define DEBUG 0 
#define RELEASE 1
int main(int argc, const char * argv[])
{
    string input;
    cout << "Please input the name of your file to use" << endl;
    cin >> input;
    cout << "Scanning Document." << endl;;
    string *myDoc = new string(input);
    //string *myStr = new string("/Users/ronaldbrown/Code/CS 3240/CompilersHomework1/");
    //*myStr+=*myDoc;

    QueryEngine *s = new QueryEngine(myDoc);
    cout << "...Done\n" << endl;
    cout << "input document : " << *myDoc << endl;
   // string line;
    
    
#if DEBUG
    cout << "VAR : " << endl;
    for(vector<entry>::size_type i = 0; i < s->s->varList.size(); i++) {
        entry *k = s->s->varList.at(i);
        cout << k->str << ", " << k->count << endl;
    }
    
    cout << "\nINT: " << endl;
    for(vector<entry>::size_type i = 0; i < s->s->intList.size(); i++) {
        entry *k = s->s->intList.at(i);
        cout << k->str << ", " << k->count << endl;
    }
    
    cout << "\nFLOAT: " << endl;
    for(vector<entry>::size_type i = 0; i < s->s->floatList.size(); i++) {
        entry *k = s->s->floatList.at(i);
        cout << k->str << ", " << k->count << endl;
    }
    
    cout << "\nQUOTES: " << endl;
    for(vector<string>::size_type i = 0; i < s->s->quoteList.size(); i++) {
        string *k = s->s->quoteList.at(i);
        cout << "Quote: " << *k << endl;
    }
#endif
    
    
#if RELEASE
    cout << "\n\nWelcome to the query engine, please choose from the following: (enter a negative number to quit or hit ctrl+c)" << endl;
    string mainCmd = "\n\n"
    "1. Find the most frequently found string in the document for the VAR token \n"
    "2.	What are the top k (k to be input by the user) strings for VAR token in the processed document? Maximum value of  k can be 20\n"
    "3.	Count the total number of  tokens of each class : VAR, INT or FLOAT\n"
    "4.	Maximum and minimum values of  INT  found in the document\n"
    "5.	Maximum and Minimum values  of  FLOAT found in the document.\n"
    "6.	Print all the quotes found in the document : a quote begins with “ and ends with “\n" //TODO
    "7.	The number of occurences of : VAR being immediately followed in the document by the INT or FLOAT token\n"
    "-1. Exit\n\n";
    
    int cmd = 0;
    int kUser = 0;
    while (cmd >= 0) {
        cout << mainCmd << endl;
        cin >> cmd;
        switch (cmd) {
            case 0:
                break;
            case 1:
                s->FindMostFrequentString();
                break;
            case 2:
                cout << "\nValue for K?" << endl;
                cin >> kUser;
                if(kUser > 20) {
                    cout << "Invalid k, greater then 20";
                    break;
                }
                s->FindTopK(kUser);
                break;
            case 3:
                s->CountTotalTokens();
                break;
            case 4:
                s->FindMaxMin("int");
                break;
            case 5:
                s->FindMaxMin("float");
                break;
            case 6:
                s->PrintQuotes();
                break;
            case 7:
                s->FindNumberOfOccurances();
                break;
            default:
                break;
        }
        //cout << mainCmd << endl;
        
    }
    
    cout << "\n\nEnding now" << endl;
#endif
    
    return 0;
}


