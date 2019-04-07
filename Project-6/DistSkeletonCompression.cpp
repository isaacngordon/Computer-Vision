#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

using namespace std;

/* Data Structures */
//files
ifstream inFile1;
ofstream outFile1, outFile2, skeletonFile, decompressionFile;

//data 
int numRows, numCols, minVal, maxVal, newMinVal, newMaxVal;

int** zeroFramedAry;
int** skeletonAry;

int setup(int argc, char *argv[]);
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
    //Step 0: open files
    int notgood = setup(argc, argv);
    if(notgood == -1){
        return 0;
    }//if

    //Step 1: skeletonFileName <- argv[1] + “_skeleton”
    string skeletonFileName;
    skeletonFileName = argv[1];
    string sk = skeletonFileName.substr(0,skeletonFileName.find(".txt",0));
    skeletonFileName = sk + "_skeleton.txt";

    //Step 2: skeletonFile <- open (skeletonFileName)
    skeletonFile.open(skeletonFileName);

    //Step 3: deCompressedFileName <- argv[1] + “_deCompressed”
    string decompressionFileName;
    decompressionFileName = argv[1];
    string de = decompressionFileName.substr(0,decompressionFileName.find(".txt",0));
    decompressionFileName = de + "_decompressed.txt";

    //Step 4: deCompressFile <- open (deCompressedFileName)
    decompressionFile.open(decompressionFileName);

    //step 5: setZero (ZeroFramedAry) 
	        //setZero (skeletonAry)

    //Step 6: loadImage (inFile, ZeroFramedAry) // begins at ZeroFramedAry(1,1)

    //Step 7: compute8Distance(ZeroFramedAry, outFile1)  // Perform distance transform

    //Step 8: skeletonExtraction (ZeroFramedAry, skeletonAry, skeletonFile, outFile1)  
            // perform compression

    //Step 9: skeletonExpansion(ZeroFramedAry, skeletonFile, outFile2)  
		// perform decompression

    //step 10: Output numRows, numCols, newMinVal, newMaxVal to deCompressFile

    //Step 11: ary2File(ZeroFramedAry, deCompressFile) 
    //// dump ZeroFramedAry to deCompressFile
  
    //Step 12: close all files
    inFile1.close();
    outFile1.close();
    outFile2.close();
    skeletonFile.close();
    decompressionFile.close();

}//main

int setup(int argc, char *argv[]){
    string err = "";
    err += "Improper arguements. Correct syntax is: \n>> ... <input1.txt> <output1.txt> <output2.txt>";
    err += "\n\twhere: \n\tinput1.txt is a binary image with header \n\toutput1.txt is a file to ";
	err += " print the result of each pass of the compression \n\toutput2.txt is a file to print the ";
    err += "results of each pass in the decompression.\n";

    //check for bad num or args
    if(argc != 4){
        cout << err <<endl;
        return -1;
    }//if

    //get filestreams
    inFile1.open(argv[1]);
    outFile1.open(argv[2]);
	outFile2.open(argv[3]);

    inFile1 >> numRows;
    inFile1 >> numCols;
    inFile1 >> minVal;
    inFile1 >> maxVal;

    setZero(zeroFramedAry);
    setZero(skeletonAry);

    return 0;
}//setup

void setZero(int** ary){
    ary = new int*[numRows + 2];
    for(int i = 0; i < numRows + 2; i++){
        ary[i] = new int[numCols + 2];
        for(int j = 0; j < numCols + 2; j++){
            ary[i][j] = 0;
        }//for
    }//for
}//setZero