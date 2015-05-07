//
//  QueryEngine.h
//  CompilersHomework1
//
//  Created by Ronald Brown on 9/4/12.
//  Copyright (c) 2012 RonBrown. All rights reserved.
//

#ifndef __CompilersHomework1__QueryEngine__
#define __CompilersHomework1__QueryEngine__

#include <iostream>
#include <vector>
#include <fstream>
#include "Scanner.h"
using namespace std;

class QueryEngine {
    public:
    Scanner *s;
    //Constructor
    QueryEngine();
    QueryEngine(string *fName);
    void FindMostFrequentString();
    void FindTopK(int k);
    void CountTotalTokens();
    void FindMaxMin(string ty);
    void PrintQuotes();
    void FindNumberOfOccurances();
    //Deconstructor
    ~QueryEngine();
    
    private:
    
};

#endif /* defined(__CompilersHomework1__QueryEngine__) */
