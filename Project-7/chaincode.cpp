#include<iostream>
#include<fstream>
#include<string>
#include<cmath>

using namespace std;

/* Class Definitions */

class Image{
    public:
        //data structures
        int numRows, numCols, minVal, maxVal, label, numBoundryPts;
        int** imgAry;
        int** zeroFramedAry;

        //Functions
        Image();
        Image(ifstream &imgFileInputStream);
        void setLabel(int i){
            label = i;
        }
        void setNumRows(int i){
            numRows = i;
        }
        void setNumCols(int i){
            numCols = i;
        }
        void setMinVal(int i){
            minVal = i;
        }
        void setMaxVal(int i ){
            maxVal = i;
        }
        void setNumBoundryPts(int i){
            numBoundryPts = i;
        }
        void setZeroFrame(int **&ary){
            ary = new int*[numRows + 2];
            for(int i = 0; i < numRows + 2; i++){
                ary[i] = new int[numCols + 2];
                for(int j = 0; j < numCols + 2; j++){
                    ary[i][j] = 0;
                }//for
            }//for
        }//setZeroFrame
        void loadImage(ifstream &imgFileInputStream, int **&ary){
            setZeroFrame(ary);
            for(int i = 0; i < numRows; i++){
                for(int j = 0; j < numCols; j++){
                    imgFileInputStream >> ary[i+1][j+1];
                }//for
            }//for
        }//loadImage
};//Image Class

class ChainCode{
    public:

    //class declarations
    class Point{
        public: int row; int col;
        void setRow(int i){row = i;}
        void setCol(int i){col = i;}
        int getRow(){return row;}
        int getCol(){return col;}
        Point(){};
        Point(int r, int c){
            row = r;
            col = c;
        }//contructor
    };//Point Class

    friend class Image;

    //data structs
    Point startP, currentP, nextP;
    Point neighborhood[8];
    int lastQ, chainDir;
    int compassTable[8];
    Image image;

    //Functions
    ChainCode();
    void getChainCode(Image img, ofstream &chainCodeFile){
        //STEP 0
        image = img;
        chainCodeFile << image.numRows << " ";
        chainCodeFile << image.numCols << " ";
        chainCodeFile << image.minVal << " ";
        chainCodeFile << image.maxVal << " " << endl;
        image.setNumBoundryPts(0);

        //STEP 1 and 2
        int** ary = image.zeroFramedAry;
        for(int i = 1; i < image.numRows + 1; i++){
            for(int j = 1; j < image.numCols; j++){
                if(ary[i][j] > 0){
                    chainCodeFile << i << " ";
                    chainCodeFile << j << " ";
                    chainCodeFile << ary[i][j] << " "; 

                    currentP = Point(i,j);
                    lastQ = 4;
                    break;
                }
            }//for cols
            if(lastQ) break;
        }//for rows
        do {
            //STEP 3
            lastQ = (lastQ + 1) % 8;

            //STEP 4
            chainDir = findNextP(lastQ, currentP);
            image.setNumBoundryPts(image.numBoundryPts + 1);

            //STEP 5
            chainCodeFile << chainDir << " ";

            //STEP 6
            nextP = neighborhood[chainDir];
            currentP = nextP;                       //i feel that this and the previous line should be swapped
            lastQ = compassTable[chainDir];
        } while(currentP != startP);
        //STEP 7

    }//getChainCode
    int findNextP(int lastD, Point currentPoint){
        int cd;
        loadNeighborsCoord(currentPoint);
        int index = lastD;
        bool found = false;
        int iRow = neighborhood[index].row;
        int jCol = neighborhood[index].col;

        while(!found){
            if(image.zeroFramedAry[iRow][jCol] == image.label){
                cd = index;
                found = true;
            }
            index = (index+1) % 8;
        }//while
        
        return cd;
    }//findNextP
    void loadNeighborsCoord(Point currentP){
        int r = currentP.getRow();
        int c = currentP.getCol();
        int index = 0;

        for(int i = r-1; i <= r+1; i++){
            for(int j = c-1; j <= c+1; j++){
                if(i==r && j==c) continue;
                else{
                    Point p = Point(i,j);
                    neighborhood[index] = p;
                }//else
            }//for
        }//for
    }//loadNeighborsCoord
    void reconstructObject(ifstream &chainCodeFile, ofstream &deCompressFile, int **&imgAry){
        
    }//reconstructObject
    void constructBoundary(ofstream &chainCodeFile, int **&imgAry, int tmplabel){

    }//constructBoundary
    void fillInterior(int **&imgAry){

    }//fillInterior
    
};//ChainCode Class

/* Class Constructors */

Image::Image(){

}//Image::Image()

Image::Image(ifstream &imgFileInputStream){
    imgFileInputStream >> numRows;
    imgFileInputStream >> numCols;
    imgFileInputStream >> minVal;
    imgFileInputStream >> maxVal;

    setZeroFrame(zeroFramedAry);

    imgAry = new int*[numRows];
    for(int i = 0; i < numRows; i++){
        imgAry[i] = new int[numCols];
        for(int j = 0; j < numCols; j++){
            imgAry[i][j] = 0;
        }//for
    }//for

    loadImage(imgFileInputStream, zeroFramedAry);
}//Image::Image(...)

ChainCode::ChainCode(){

}//ChainCode::ChainCode()


/* Data  */
ifstream inFile;
ofstream chainCodeFile, decompressFile;
Image image;
ChainCode chaincode;

/* Function Headers */


/* Program Begin */
int main(int argc, char *argv[]){

    //Step -1
    if(argc != 2){
        cout << "Correct syntax is >> ./chainCode <inputFile>\n where inputFile is a binary image containgina single object and a header.\n";
        return -1;
    }

    //Step 0 (encomppases step 3)
    inFile.open(argv[1]);
    image = Image(inFile);
    chaincode = ChainCode();

    //Step 1
    string chainCodeFileName;
    chainCodeFileName = argv[1];
    string cc = chainCodeFileName.substr(0,chainCodeFileName.find(".txt",0));
    chainCodeFileName = cc + "_chaincode.txt";

    //Step 2
    chainCodeFile.open(chainCodeFileName);

    //Step 3
    chaincode.getChainCode(image, chainCodeFile);

    //Step
    chainCodeFile.close();

    //Step
    ifstream inChaincodeFile;
    inChaincodeFile.open(chainCodeFileName);

    //Step 4
    string decompressedFileName;
    decompressedFileName = argv[1];
    string de = decompressedFileName.substr(0,decompressedFileName.find(".txt",0));
    decompressedFileName = de + "_chainCodeDecompressed.txt";

    //Step 5
    decompressFile.open(decompressedFileName);

    //Step 6
    chaincode.reconstructObject(inChaincodeFile, decompressFile, image.imgAry);

    //Step 7
    inFile.close();
    chainCodeFile.close();
    decompressFile.close();

    return 0;
}//main