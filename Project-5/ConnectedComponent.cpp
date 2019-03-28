#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

using namespace std;

//make class to store composite property objects
class Property{
        int label, numpixels, minRow, minCol, maxRow, maxCol;
    public:
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

//files
ifstream inFile1;
ofstream outFile1, outFile2, outFile3, outFile4;

//data 
int numRows, numCols, minVal, maxVal, newMin, newMax;
int newLabel, numNb;  
int* neighborAry;
int* eqAry;
Property property;
Property* ccProperties;

void setup(int argc, char *argv);
void zeroFramed();
void loadImage();
void loadNeighbors();
void pass1();
void pass2();
void pass3();
UNKNOWN drawboxes(...);
UNKNOWN updateEQAry(...);
UNKNOWN manageEQAry(...);
void printCCproperty();
void prettyPrint(ofstream &file);
void printEQAry();





int main(int argc, char *argv[]){



}//main
