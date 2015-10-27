//
//  main.cpp
//  cip
//
//  Created by cms on 15/10/26.
//  Copyright © 2015年 Maosen Chen. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ctype.h>
#include <queue>
#include <cstring>
#include <sstream>
#include <string.h>

#define Maxn 330
#define NodeNum 1100
#define CompressedFlag "###!!!CIP!!!COMPRESSED!!!FILE\n"

using namespace std;

struct Info
{
    int num;
    int value;
    int position;
    
    bool operator < (const Info & a) const
    {
        return a.num < num;
    }
    
} charInfo[Maxn];

int child[NodeNum][2]; //left is 0, right is 1
int parent[NodeNum][2]; //parent node is 0, from left or right decided by 1
priority_queue<struct Info> charPrioriytQueue;
int nodeNum, rootNodeNum, sumCharNum;
int positionToValue[Maxn], lastOneDigit;

void init()
{
    for(int i=0; i < Maxn; i++)
    {
        charInfo[i].num = 0;
        charInfo[i].value = 0;
        charInfo[i].position = 0; //The char in which node
    }
    memset(child, -1, sizeof(child));
    memset(parent, -1, sizeof(parent));
    nodeNum = 0;
}

void writeCompressInfo(FILE * out)
{
    //FILE * out = fopen(targetFilename, "a+");
    fputs(CompressedFlag, out);
    fprintf(out, "  %d %d ",rootNodeNum, sumCharNum);
    int j = 0;
    
    for(int i=0; i<Maxn; i++) //Decompress only need leaf position and value
    {
        if(charInfo[i].num)
        {
            j++;
            fprintf(out, "%d %d", charInfo[i].position, charInfo[i].value);
            if(j != sumCharNum)
            {
                fprintf(out, " ");
            }
            else
            {
                fprintf(out, "\n");
            }
        }
    }
    fprintf(out, "%d ", nodeNum);
    for(int i=sumCharNum; i<nodeNum; i++) //Decompress only need child array
    {
        fprintf(out, "%d %d", child[i][0], child[i][1]);
        if(i != (nodeNum-1))
        {
            fprintf(out, " ");
        }
        else
        {
            fprintf(out, "\n");
        }
    }
    //fclose(out);
    
}

void compresss(FILE * in, const char * targetFilename)
{
    init();
    unsigned char c;
    
    while(fscanf(in,"%c",&c) != EOF)
    {
        charInfo[c].num++;
        charInfo[c].value = int(c);
        //printf("%c ",c);
    }
    for(int i=0; i < Maxn; i++)
    {
        if(charInfo[i].num)
        {
            //printf(":::%c ",charInfo[i].value);
            charInfo[i].position = nodeNum++;
            charPrioriytQueue.push(charInfo[i]);
        }
    }
    
    sumCharNum = nodeNum;
    while(charPrioriytQueue.size() > 1)
    {
        struct Info first, second;
        
        first = charPrioriytQueue.top();
        charPrioriytQueue.pop();
        second = charPrioriytQueue.top();
        charPrioriytQueue.pop();
        
        struct Info next;
        next.num = first.num + second.num;
        next.position = nodeNum++;
        
        child[next.position][0] = first.position;
        child[next.position][1] = second.position;
        parent[first.position][0] = next.position;
        parent[second.position][0] = next.position;
        parent[first.position][1] = 0; //from left
        parent[second.position][1] = 1; //from right
        
        charPrioriytQueue.push(next);
    }
    rootNodeNum = nodeNum-1;
    FILE * out = fopen(targetFilename, "w+");
    writeCompressInfo(out);
    
    fseek(in, 0, SEEK_SET);
    
    int current = 0, digitNum = 0;
    //string compressString = "";
    while(fscanf(in, "%c", &c) != EOF)
    {
        //printf("%c",c);
        int leafPosition = charInfo[c].position;
        string temp = "";
        while(leafPosition != rootNodeNum)
        {
            if(parent[leafPosition][1])
            {
                temp += "1";
            }
            else
            {
                temp += "0";
            }
            leafPosition = parent[leafPosition][0];
        }
        for(int i=temp.length()-1; i>=0; i--)
        {
            digitNum++;
            if(temp[i]=='0')
                current = current<<1;
            else
                current = (current<<1)+1;
            if(digitNum == 8)
            {
                digitNum = 0;
                //printf("%d:%d",current,compressString.length());
                fputc(current, out);
                //stringstream ss;
                //ss<<(unsigned char)current;
                //string tt = ss.str();
                //compressString = compressString + tt;
                current = 0;
            }
        }
    }
    if(digitNum)
    {
        //printf("%d\n",current);
        //stringstream ss;
        //ss<<(unsigned char)current;
        //string tt = ss.str();
        //compressString = compressString + tt;
        //printf("digit::%d\n", digitNum);
        fputc(current, out);
        //fprintf(out, "%s", compressString.c_str());
        string temp = CompressedFlag;
        fseek(out, temp.length(), SEEK_SET);
        fprintf(out, "%d", digitNum);
    }
    else
    {
        string temp = CompressedFlag;
        fseek(out, temp.length(), SEEK_SET);
        fprintf(out, "8");
    }
    
    printf("Compress successful done.\n");
    fclose(out);
    fclose(in);
    
    return ;
}

int inverse(int a, bool last)
{
    int b = 0;
    int end = 8;
    if(last)
        end = lastOneDigit;
        
    for(int i=0; i<end; i++)
    {
        if(a&1)
            b = (b<<1)+1;
        else
            b = b<<1;
        a = a>>1;
    }
    return b;
}

void decompress(FILE * in, const char * targetFilename)
{
    init();
    fscanf(in, "%d %d %d",&lastOneDigit, &rootNodeNum, &sumCharNum);
    printf("r and s %d %d\n",rootNodeNum, sumCharNum);
    for(int i=0; i<sumCharNum; i++)
    {
        int a,b;
        fscanf(in, "%d%d", &a, &b);
        positionToValue[a] = b;
    }
    fscanf(in, "%d", &nodeNum);
    for(int i=sumCharNum; i<nodeNum; i++)
    {
        fscanf(in, "%d%d", &child[i][0], &child[i][1]);
    }
    fgetc(in); //get the Enter key
    //fscanf(in, "%d\n", &lastOneDigit);
    //printf("last:%d\n", lastOneDigit);
    unsigned char c;
    unsigned char nextc;
    FILE * out = fopen(targetFilename, "w+");
    int current = rootNodeNum;
    bool lastOne = false;
    fscanf(in, "%c", &c);
    while(true)
    {
        if(fscanf(in, "%c", &nextc) == EOF)
        {
            lastOne = true;
        }
        //printf("c:%d nextc:%d \n", c, nextc);
        int value = inverse(c, lastOne), i = 1;
        //printf("value:%d\n", value);
        while(i <= 8)
        {
            i++;
            if(value&1)
            {
                current = child[current][1];
            }
            else
            {
                current = child[current][0];
            }
            if((child[current][0] == -1) && (child[current][1] == -1))
            {
                fputc(positionToValue[current], out);
                //printf("->->%d i:%d ", positionToValue[current], i);
                //printf("%c", positionToValue[current]);
                if(lastOne && i > lastOneDigit)
                {
                    printf("Decompress successful.\n");
                    return;
                }
                current = rootNodeNum;
            }
            value = value>>1;
        }
        c = nextc;
    }
    
    
}


void handle(const char * filename, const char * targetFilename)
{
    FILE * in;
    
    in = fopen(filename, "r");
    
    if(in == NULL)
    {
        printf("File %s open failed.",filename);
        return ;
    }
    
    char isComOrDecom[Maxn];
    fgets(isComOrDecom, Maxn, in);
    
    
    if(!strcmp(isComOrDecom, CompressedFlag))
    {
        printf("Decompressing file %s to file %s begins ...\n", filename, targetFilename);
        decompress(in, targetFilename);
        
    }
    else
    {
        fseek(in, 0, SEEK_SET);
        printf("Compress file %s to file %s begins ...\n",filename, targetFilename);
        compresss(in, targetFilename);
        
    }
    return ;
}

void testReadAChar()
{
    FILE * in = fopen("/Users/cms/UCAS/code/cip/a","r");
    
    unsigned char c;
    while(fscanf(in, "%c", &c) != EOF)
    {
        //fscanf(in, "%c", &c);
        //c = fgetc(in);
        printf("%d\n", c);
    }
    return ;
}

void testWriteSize()
{
    FILE * out = fopen("/Users/cms/UCAS/code/cip/testsize","a+");
    fprintf(out, "   4");
    fseek(out, 0, SEEK_SET);
    fprintf(out, "456");
    fclose(out);
}

int main(int argc, const char * argv[]) {
    //cout << argc << " " << argv[0];
    //cout << argv[1];
    //testReadAChar();
    //testWriteSize();
    
    if(argc != 3)
    {
        printf("Argument must be 3.Eg ./main.cpp sourceFile targetFile\n");
        return 0;
    }
    handle(argv[1], argv[2]);
    
    return 0;
}
