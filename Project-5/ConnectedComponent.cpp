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
        void setLabel(int i){
            label = i;
        }
        void incrementNumPixels(){
            numpixels++;
        }
        void setMinRow(int i){
            minRow = i;
        }
        void setMinCol(int i){
            minCol = i;
        }
        void setMaxRow(int i){
            maxRow = i;
        }
        void setMaxCol(int i){
            maxCol = i;
        }
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
int setup(int argc, char *argv[]);
void zeroFramed();
void loadImage();
void loadNeighbors(int r, int c);
void pass1();
void pass2();
void pass3();
void drawboxes();
void updateEQAry(int oldLabel, int newL);
void manageEQAry();
void printCCproperty(ofstream &file);
void prettyPrint(ofstream &file, bool wFrame);
void pp(ofstream &file, bool withFrame);
void printEQAry(ofstream &file);


int main(int argc, char *argv[]){
    //STEP 0
    int good = setup(argc, argv);
    if(good == -1) return 0;

    //STEP 1
    loadImage();

    //STEP 2
    pass1();
    cout << "PASS 1 done"<<endl;
    prettyPrint(outFile1, true);
    printEQAry(outFile1);

    //STEP 3
    pass2();
    cout << "PASS 2 done"<<endl;
    prettyPrint(outFile1, true);
    printEQAry(outFile1);

    //STEP 4
    manageEQAry();
    outFile1 << "---EQ AFTER MANAGEMENT---" << endl;
    printEQAry(outFile1);

    //STEP 5
    pass3();
    cout << "PASS 3 done"<<endl;
    prettyPrint(outFile1,true);
    printEQAry(outFile1);

    //STEP 6
    outFile2 << numRows << " ";
    outFile2 << numCols << " ";
    outFile2 << minVal << " ";
    outFile2 << maxVal << endl;

    //STEP 7 - Output the result of pass3 from zeroFramedAry to outFile2, begins at (1, 1) and ending at ?? 
    pp(outFile2, false);

    //STEP 8
    printCCproperty(outFile3);

    //STEP 9
    drawboxes();

    //STEP 10 - output zeroFrameAry w boxes to outFile4
    prettyPrint(outFile4, true);
    
    //STEP 11
    inFile1.close();
    outFile1.close();
    outFile2.close();
    outFile3.close();
    outFile4.close();

    return 0;
}//main


int setup(int argc, char *argv[]){
    string err = "";
    err += "Improper arguements. Correct syntax is: \n>> ... <input1.txt> <output1.txt> <output2.txt> <output3.txt> <output4.txt>";
    err += "\n\twhere: \n\tinput1.txt is a binary image with header \n\toutput1.txt is a file to ";
	err += " print the result of each pass \n\toutput2.txt is a file to print the labeled connected components after pass 3 with header";
    err += "information for future processing.\n\toutput3.txt is a file to print connected component properties.";
    err += "\n\toutput4.txt is a file to pretty print the bounding boxed connected components";

    //check for bad num or args
    if(argc != 6){
        cout << err <<endl;
        return -1;
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
    maxNumCC = (int)((numRows + numCols) / 4);
    ccProperties  = new Property[maxNumCC];
    newLabel = 0;
    eqAry = new int[maxNumCC];
    for(int k = 0; k < maxNumCC; k++) eqAry[k] = k;

    return 0;
}//setup

void zeroFramed(){
    zeroFramedAry = new int*[numRows + 2];
    for(int i = 0; i < numRows + 2; i++){
        zeroFramedAry[i] = new int[numCols+2];
        for(int j = 0; j < numCols+2; j++){
            zeroFramedAry[i][j] = 0;
        }//for
    }//for
}//zeroFramed 

void loadImage(){
    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numCols; j++){
            inFile1 >> zeroFramedAry[i+1][j+1];
        }//for
    }//for
}//loadImage

void loadNeighbors(int r, int c){
    neighborAry = new int[9];
    int index = 0;
    cout << "na for (" << r << ", " << c << "): ";
    for(int i = r-1; i <= r+1; i++){
        for(int j= c-1; j <= c+1; j++){
            neighborAry[index++] = zeroFramedAry[i][j];
            cout << neighborAry[index - 1];
        }//for
    }//fpr
    
    cout << endl;
}//loaadNeighbors

//O(2 + numRows*numCols*(c)) = O(numPixels) 
void pass1(){
    //scan L-R, T-B
    for(int i = 1; i < numRows+1; i++){
        for(int j = 1; j < numCols+1; j++){
            if(zeroFramedAry[i][j] == 0) continue;  //if p(i,j) is zero skip to next pixel
            loadNeighbors(i, j);                    //otherwise load neighbrs
    
            //Identify which case we are in by counting number of nonzero neighbors
            int countNonZeros = 0;
            for(int f = 0; f < 4; f++){
                if(neighborAry[f] != 0) countNonZeros++;
            }//for 4
    cout << "numNonZero: " << countNonZeros;
            //Case 1: all front neighbors are zero
            if(countNonZeros == 0){
                zeroFramedAry[i][j] = ++newLabel;
            }//case1

            //Case 2: Only one non-zero label
            else if(countNonZeros == 1){
               for(int f = 0; f <  4; f++){
                    if(neighborAry[f] != 0){
                        zeroFramedAry[i][j] = neighborAry[f];
                    }//if
                }//for
            }//if case 2

            //Case 3: more than one non-zero labeled neighbor
            else{
               //find smallesnt non-zero label
                int smallestLabel = 1000000;
                for(int f = 0; f <  4; f++){
                    if(neighborAry[f] != 0){
                        if(neighborAry[f] < smallestLabel) smallestLabel = neighborAry[f];
                    }//if
                }//for

                //Update EQ array 
                for(int f = 0; f <  4; f++){
                    if(neighborAry[f] > smallestLabel) 
                        updateEQAry(neighborAry[f], smallestLabel);
                }//for

                //set p(i,j) = smallestlabel
                zeroFramedAry[i][j] = smallestLabel;
            }//else case 3
            cout << " and newLabel is now " << newLabel << " while p(" << i << ", " << j << ")= "<< zeroFramedAry[i][j]<< endl;
        }//for cols
    }//for rows
}//pass1

void pass2(){
     //scan R-L, B-T
    for(int i = numRows+1; i > 0; i--){
        for(int j = numCols+1; j > 0; j--){
            if(zeroFramedAry[i][j] == 0) continue;  //if p(i,j) is zero skip
            loadNeighbors(i,j);                     //else load neighbors

            //Identify which case we are in by counting number of nonzero neighbors
            int countNonZeros = 0;
            int pVal = neighborAry[4];
            bool isCase3 = false;
            for(int f = 5; f < 9; f++){
                if(neighborAry[f] != 0){
                    countNonZeros++;
                    if(pVal != neighborAry[f] && pVal != -1)
                        isCase3 = true;
                }//if non zero neighbor
            }//for 4

            //Case 1: if lower neighbors are all zero do nothing
            if(countNonZeros == 0) continue;

            //Case 2: all non zero labels are the same  
            if(!isCase3) continue;

            //Case 3: There is a label conflict
            else{
                //find smallest label in the conflict
                int minLabel = 1000000;
                for(int f = 4; f < 9; f++){
                    if((neighborAry[f] != 0) && (neighborAry[f] < minLabel)) 
                        minLabel = neighborAry[f];
                }//for

                //update EQTable if needed
                if(neighborAry[4] < minLabel){
                    updateEQAry(neighborAry[4], minLabel);
                    zeroFramedAry[i][j] = minLabel;
                }//if
            }//case 3        
        }//for cols
    }//for rows
}//pass2

void pass3(){
    //fill up CCproperties array
    for(int i = 0; i < maxNumCC; i++){
        Property p;
        p.setLabel(i);
        p.setMaxCol(-1);
        p.setMaxRow(-1);
        p.setMinCol(1000000);
        p.setMinRow(1000000);
        ccProperties[i] = p;
    }//for

    for(int i = 1; i < numRows+1; i++){
        for(int j = 1; j < numCols+1; j++){
            //use EQtable to fix all labels
            int v = zeroFramedAry[i][j];
            if(v != eqAry[v]){
                zeroFramedAry[i][j] = eqAry[v];
                v = zeroFramedAry[i][j];
            }//if
            
            //edit property values for Property V
            if(i < ccProperties[v].minRow) ccProperties[v].setMinRow(i);
            if(i > ccProperties[v].maxRow) ccProperties[v].setMaxRow(i);
            if(j < ccProperties[v].minCol) ccProperties[v].setMinCol(j);
            if(j > ccProperties[v].maxCol) ccProperties[v].setMaxCol(j);
            ccProperties[v].incrementNumPixels();
        }//for cols
    }//for rows
}//pass3

void drawboxes(){
    //Step 1 and 2
    for(int i = 1; i < newLabel+1; i++){
        int minRow = ccProperties[i].minRow ;// + 1; // need to add 1 for frame compensation
        int minCol = ccProperties[i].minCol ;// + 1; // need to add 1 for frame compensation
        int maxRow = ccProperties[i].maxRow ;// + 1; // need to add 1 for frame compensation
        int maxCol = ccProperties[i].maxCol ;// + 1; // need to add 1 for frame compensation
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

void updateEQAry(int index, int newL){
    eqAry[index] = newL;
}//updatedEQAry

void manageEQAry(){
    for(int i = 1; i < maxNumCC; i++){
        int val = eqAry[i];
        if(val == i) continue;
        while(!(val==eqAry[val])){
            val = eqAry[val];
        }//while
        eqAry[i] = val;
    }//for
}//manageEQAry

void printCCproperty(ofstream &file){
    file << "CONNECTED COMPONENT PROPERTIES" << endl;
    file << "------------------------------" << endl << endl;
    for(int i = 0; i < newLabel+1; i++){
        Property p = ccProperties[i];
        file << p.label << endl;
        file << p.numpixels << endl;
        file << p.minRow << ", " << p.minCol << endl;
        file << p.maxRow << ", " << p.maxCol << endl;
        file << endl;
    }//for
}//printCCproperty

void prettyPrint(ofstream &file, bool withFrame){
    int b;
    if(withFrame) b = 0;
    else b = 1;
    
    for(int i = 0; i < numRows + (2-b); i++){
        for(int j = 0; j < numCols + (2-b); j++){
            int x = zeroFramedAry[i+b][j+b];
            if(x == 0)
                file << " " ;
            else 
                file << x;
            file << " ";
        }//for
        file << endl;
    }//for
    file << endl;
}//prettyPrint

void pp(ofstream &file, bool withFrame){
    int b;
    if(withFrame) b = 0;
    else b = 1;
    
    for(int i = 0; i < numRows + (2-b); i++){
        for(int j = 0; j < numCols + (2-b); j++){
            int x = zeroFramedAry[i+b][j+b];
            file << x;
            file << " ";
        }//for
        file << endl;
    }//for
    file << endl;
}//prettyPrint

void printEQAry(ofstream &file){
    file << "EQ Table: " << endl;
    for(int i = 0; i < maxNumCC; i ++){
        file << i << "\t|";
    }//for

    file << endl;

    for(int i = 0; i < maxNumCC; i ++){
        file << eqAry[i] << "\t|";
    }//for

    file << endl <<"newLabel: " << newLabel << endl << endl;
}//printEQAry