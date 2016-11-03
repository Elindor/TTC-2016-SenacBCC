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
    int i, j;
    srand (time(NULL));
    long count = 0;
    long innercount = 0;
    double time_spent = 0;
    for(i = 0; i < 1; i++){
        clock_t start = clock();

        printf("Starting to generate %d\n", i);
        GPMap *m = generateMap("core.json");
//        printf("c = %d\n", m->generation);

        //innercount += m->generation;
        //releaseMap(m);
        clock_t end = clock();
        time_spent += (double)(end - start) / CLOCKS_PER_SEC;
        
        //for(j = 0; j < 1000; j ++);
        
        if(i % 50){
            count += innercount / 50;
            innercount = 0;
        
        }
        
    }

    printf("Average change of %ld tiles per operation, and with a total of %f seconds, with average %f seconds per cycle.\n", count/10, time_spent, time_spent/100);
    
    
    return 0;
}
