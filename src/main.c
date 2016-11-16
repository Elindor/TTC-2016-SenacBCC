//
//  main.c
//  TCC
//
//  Created by Gabriel Nopper on 09/09/16.
//  Copyright © 2016 Gabriel Nopper. All rights reserved.
//

#include <stdio.h>
#include "GPMap.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    double time_spent = 0;
    clock_t start = clock();
    srand (time(NULL));
    printf("Starting to generate\n");

    generateMap("core.json");

    clock_t end = clock();
    time_spent += (double)(end - start) / CLOCKS_PER_SEC;


    return 0;
}
