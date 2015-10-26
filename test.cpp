//
//  test.cpp
//  cip
//
//  Created by cms on 15/10/26.
//  Copyright © 2015年 Maosen Chen. All rights reserved.
//

#include <cstdio>
#include <cstdlib>

using namespace std;

int main()
{
    FILE * in = fopen("/Users/cms/UCAS/code/cip/a", "r");
    unsigned char c ;
    
    while((fscanf(in, "%c", &c)) != EOF)
    {
        printf("%d\n",c);
    }
    return 0;
}