//
//  QueryEngine.cpp
//  CompilersHomework1
//
//  Created by Ronald Brown on 9/4/12.
//  Copyright (c) 2012 RonBrown. All rights reserved.
//

#include "QueryEngine.h"
#include <typeinfo>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>


/*
 string mainCmd = 
 "1. Find the most frequently found string in the document for the VAR token \n"
 "2.	What are the top k (k to be input by the user) strings for VAR token in the processed document? Maximum value of  k can be 20\n"
 "3.	Count the total number of  tokens of each class : VAR, INT or FLOAT\n"
 "4.	Maximum and minimum values of  INT  found in the document\n"
 "5.	Maximum and Minimum values  of  FLOAT found in the document.\n"
 "6.	Print all the quotes found in the document : a quote begins with “ and ends with “\n" 
 "7.	The number of occurences of : VAR being immediately followed in the document by the INT or FLOAT token\n";
 */

//Internal sort functions
bool CompareByStrs(entry *a,  entry *b) {
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

bool CompareByInts(entry *a, entry *b) {
    int aa = atoi(a->str.c_str());
    int bb = atoi(b->str.c_str());
    
    return aa < bb;
}

bool CompareByFloats(entry *a, entry *b) {
    long aa = atol(a->str.c_str());
    long bb = atol(b->str.c_str());
    
    return aa < bb;
}

bool CompareByCounts(entry *a, entry *b) {
    return a->count > b->count;
}

//Constructors
QueryEngine::QueryEngine() {
    s = new Scanner(new string("test.txt"));
}

QueryEngine::QueryEngine(string *fName) {
    s = new Scanner(fName);
}

//Functions

// FindMostFrequentString Function
// "1. Find the most frequently found string in the document for the VAR token \n"
void QueryEngine::FindMostFrequentString() {
    sort(s->varList.begin(), s->varList.end(), CompareByCounts);
    cout << "\n\nMost frequently found string for Var Token: " << s->varList[0]->str << " \nNumber of times: " << s->varList[0]->count<< "\n\n"<< endl;
    
}

// FindTopK Function
// "2.	What are the top k (k to be input by the user) strings for VAR token in the processed document? Maximum value of  k can be 20\n"
void QueryEngine::FindTopK(int k) {
    sort(s->varList.begin(), s->varList.end(), CompareByCounts);
    
    if(k > s->varList.size())
        k = (int)s->varList.size();
    cout << "\n\nThe top " << k << " strings for var are: " <<endl;
    for(vector<entry>::size_type i = 0; i < k; i++) {
        entry *k = s->varList.at(i);
        cout << "No:"<< i+1 << ", String: " << k->str << ", Count:" << k->count << endl;
    }
    
}
// CountTotalTokens Function
// "3.	Count the total number of  tokens of each class : VAR, INT or FLOAT\n"
void QueryEngine::CountTotalTokens() {
    cout << "Var List Tokens Count: "<<  s->varList.size() << endl;
    cout << "Int List Tokens Count: "<<  s->intList.size() << endl;
    cout << "Float List Tokens Count: "<<  s->floatList.size() << endl;
    
}

// FindMaxMin function
// "4.	Maximum and minimum values of  INT  found in the document\n"
// "5.	Maximum and Minimum values  of  FLOAT found in the document.\n"
void QueryEngine::FindMaxMin(string ty) {
    transform(ty.begin(), ty.end(), ty.begin(), ::tolower);
    
    //transform(data.begin(), data.end(), data.begin(), ::tolower);
    if(ty == "int") {
        sort(s->intList.begin(), s->intList.end(), CompareByFloats);
        cout << "Min int: " << s->intList[0]->str << endl;
        cout << "Max Int: " << s->intList[s->intList.size() - 1]->str << endl;
    }
    else {
        sort(s->floatList.begin(), s->floatList.end(), CompareByFloats);
        cout << "Min Float: " << s->floatList[0]->str << endl;
        cout << "Max Float: " << s->floatList[s->floatList.size() - 1]->str << endl;
    }
    
}
// Print Quotes Function
//"6.	Print all the quotes found in the document : a quote begins with “ and ends with “\n" 
void QueryEngine::PrintQuotes() {
    cout << "\nQUOTES: " << endl;
    for(vector<string>::size_type i = 0; i < s->quoteList.size(); i++) {
        string *k = s->quoteList.at(i);
        cout << "Quote: " << *k << endl;
    }

}

//Find Number of Occurances
// "7.	The number of occurences of : VAR being immediately followed in the document by the INT or FLOAT token\n";
void QueryEngine::FindNumberOfOccurances() {
    cout << "\n\nThe number of occurences of : VAR being immediately followed in the document by the INT or FLOAT token: " << s->varFollowedByFloatorIntCounter << "\n\n" << endl;
    
}




//Deconstructor
QueryEngine::~QueryEngine() {
    delete s;
}




