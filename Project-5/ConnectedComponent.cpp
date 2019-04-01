#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

using namespace std;

/* Class Definitions */
class Property{
    public:
        int label, numpixels, minRow, minCol, maxRow, maxCol;
        Property();
        void setLabel(int i);
        void setNumPixels(int i);
        void setMinRow(int i);
        void setMinCol(int i);
        void setMaxRow(int i);
        void setMaxCol(int i);
};//PropertyClass

//Property constructor
Property::Property(){
    label = 0;
    numpixels = 0;
    minRow = 0;
    minCol = 0;
    maxRow = 0;
    maxCol = 0;
}//constructor


/* Data Structures */
//files
ifstream inFile1;
ofstream outFile1, outFile2, outFile3, outFile4;

//data 
int numRows, numCols, minVal, maxVal, newMin, newMax;
int newLabel, numNb, maxNumCC;  
int* neighborAry;
int* eqAry;
int** zeroFramedAry;
Property property;
Property* ccProperties;

/* Function Headers */
void setup(int argc, char *argv[]);
void zeroFramed();
void loadImage();
void loadNeighbors(int r, int c);
void pass1();
void pass2();
void pass3();
void drawboxes(int** framedAr, Property* cc);
void updateEQAry(int oldLabel, int newLabel);
void manageEQAry(...);
void printCCproperty(ofstream &file);
void prettyPrint(ofstream &file);
void printEQAry(ofstream &file);


int main(int argc, char *argv[]){

    //STEP 0
    setup(argc, argv);

    //STEP 1
    loadImage();

    //STEP 2
    pass1();
    prettyPrint(outFile1);
    printEQAry(outFile1);

    //STEP 3
    pass1();
    prettyPrint(outFile1);
    printEQAry(outFile1);

    //STEP 4
    pass2();
    prettyPrint(outFile1);
    printEQAry(outFile1);

    //STEP 5
    manageEQAry(...);
    printEQAry(outFile1);

    //STEP 6
    pass3();
    prettyPrint(outFile1);
    printEQAry(outFile1);

    //TODO: STEP 7 - Output the result of pass3 from zeroFramedAry to outFile2, begins at (1, 1) and ending at ?? 


    //STEP 8
    printCCproperty(outFile3);

    //STEP 9
    drawboxes(zeroFramedAry, ccProperties);

    //TODO: STEP 10 - output zeroFrameAry to outFile4
    
    
    //STEP 11
    inFile1.close();
    outFile1.close();
    outFile2.close();
    outFile3.close();
    outFile4.close();

}//main


void setup(int argc, char *argv[]){

    string err = "";
    err += "Improper arguements. Correct syntax is: \n>> ... <input1.txt> <output1.txt> <output2.txt> <output3.txt> <output4.txt>";
    err += "\n\twhere: \n\tinput1.txt is a binary image with header \n\toutput1.txt is a file to ";
	err += " print the result of each pass \n\toutput2.txt is a file to print the labeled connected components after pass 3 with header";
    err += "information for future processing.\n\toutput3.txt is a file to print connected component properties.";
    err += "\n\toutput4.txt is a file to pretty print the bounding boxed connected components";

    //check for bad num or args
    if(argc != 6){
        cout << err <<endl;
        exit(0);
    }//if

    //get filestreams
    inFile1.open(argv[1]);
    outFile1.open(argv[2]);
	outFile2.open(argv[3]);
    outFile3.open(argv[4]);
    outFile4.open(argv[5]);

    inFile1 >> numRows;
    inFile1 >> numCols;
    inFile1 >> minVal;
    inFile1 >> maxVal;

    zeroFramed();
    ccProperties  = new Property[(int)((numRows + numCols) / 4)];
    
}//setup

void zeroFramed(){
    zeroFramedAry = new int*[numRows + 2];
    for(int i = 0; i < numRows + 2; i++){
        zeroFramedAry[i] = new int[numCols+2];
    }//for
}//zeroFramed 

void loadImage(){
    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numCols; j++){
            inFile1 >> zeroFramedAry[i+1][j+1];
                                                    // cout << zeroFramedAry[i+1][j+1];
        }//for
    }//for
}//loadImage

void loadNeighbors(int r, int c){
    neighborAry = new int[9];
    int index = 0;
    for(int i = -1; i <= 1; r++){
        for(int j= -1; j <= 1; j++){
            neighborAry[index++] = zeroFramedAry[r+i][c+i];
        }//for
    }//fpr
}//loaadNeighbors

void pass1();
void pass2();
void pass3();

void drawboxes(){
    //Step 1 and 2
    for(int i = 1; i < ccProperties.length; i++){
        int minRow = ccProperties[i].minRow; // need to add 1
        int minCol = ccProperties[i].minCol; // need to add 1
        int maxRow = ccProperties[i].maxRow; // need to add 1
        int maxCol = ccProperties[i].maxCol; // need to add 1
        int label = ccProperties[i].label;

        //Step 3
        for(int x = minRow; x < maxRow + 1; x++){
            zeroFramedAry[x][minCol] = label;
            zeroFramedAry[x][maxCol] = label;
        }//for
        for(int x = minCol; x < maxCol + 1; x++){
            zeroFramedAry[minRow][x] = label;
            zeroFramedAry[maxRow][x] = label;
        }//for
    }//for

}//drawboxes

void updateEQAry(int index, int newLabel){
    eqAry[index] = newLabel;
}//updatedEQAry

void manageEQAry(){
    for(int i = 1; i < newLabel; i++){
        int val = eqAry[i];
        while(!(val==i)){
            val = eqAry[val];
        }//while
        eqAry[i] = val;
    }//for
}//manageEQAry

void printCCproperty(ofstream &file){
    file << "CONNECTED COMPONENT PROPERTIES" << endl;
    file << "------------------------------" << endl << endl;
    for(int i = 0; i < ccProperties.length; i++){
        Property p = ccProperties[i];
        file << p.label << endl;
        file << p.numpixels << endl;
        file << p.minRow << ", " << p.minCol << endl;
        file << p.maxRow << "," << p.maxCol << endl;
        file << endl;
    }//for
}//printCCproperty

void prettyPrint(ofstream &file){
    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numCols; j++){
            int x = zeroFramedAry[i+1][j+1];
            if(x == 0)
                file << " " ;
            else 
                file << x ;
        }//for
        file << endl;
    }//for
    file << endl;
}//prettyPrint

void printEQAry(ofstream &file){
    file << "EQ Table: " << endl;
    for(int i = 0; i <= newLabel; i ++){
        file << i << "|";
    }//for

    file << endl;

    for(int i = 0; i <= newLabel; i ++){
        file << eqAry[i] << "|";
    }//for

    file << "   newLabel: " << newLabel << endl << endl;
}//printEQAry