//
//  Practical.c
//  Project1 3251
//
//  Created by Ronald Brown on 9/19/11.
//  Copyright 2011 Georgia Institute of Technology. All rights reserved.
//

#include "Practical.h"
#include <stdio.h>
#include <stdlib.h>

void DieWithUserMessage(const char *msg, const char *detail) {
    fputs(msg, stderr);
    fputs(": ", stderr);
    fputs(detail, stderr);
    fputc('\n', stderr);
    exit(1);
}

void DieWithSystemMessage(const char *msg) {
    perror(msg);
    exit(1);
}