#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

using namespace std;

/* Data Structures */
//files
ifstream inFile1;
ofstream outFile1, outFile2;

//data 
int numRows, numCols, minVal, maxVal, newMinVal, newMaxVal;

int** zeroFramedAry, skeletonAry;

/* Function Headers */
void setup();
void setZero(int** ary);
void loadImage();
int compute8Distance(...);
void firstPass8Distance();
void secondPass8Distance();
void skeletonExtraction();
int isMaxima(int i, int j);
int computeLocalMaxima(int i, int j);
void extractLocalMaxima(ofstream &file);
void skeletonExpansion();
void firstPassExtraction();
void secondPassExtraction();
void sendToFile(int** ary, ofstream &file);
void prettyPrint(int** ary, ofstream &file, bool includeZero);


int main(int argc, char *argv[]){
    
}//main