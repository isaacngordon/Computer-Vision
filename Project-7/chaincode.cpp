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
                    if(ary[i+1][j+1] != 0) label = ary[i+1][j+1];
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

        //overoad operators
        bool operator!=(const Point& p){
            if(row == p.row )
                if(col == p.col)
                    return false;
            return true;
        }//not-equal
        bool operator==(const Point& p){
            if(row == p.row )
                if(col == p.col)
                    return true;
            return false;
        }//is-equal
        Point operator=(const Point& p){
            int r = p.row;
            int c = p.col;
            Point a = Point(r,c);
            return a;
        }//copy constructor
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
                    image.setLabel(ary[i][j]);

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
        cout << "The total number of boundry pixels: " << image.numBoundryPts << endl;
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
    void reconstructObject(ifstream &chainCodeFile, ofstream &boundaryFile ,ofstream &deCompressFile, int **&imgAry){
        int cRows, cCols, cMinVal, cMaxVal;
        int clabel, tmpLabel, startRow, startCol;
        
        //get cahin code header info
        chainCodeFile >> cRows;
        chainCodeFile >> cCols;
        chainCodeFile >> cMinVal;
        chainCodeFile >> cMaxVal;
        chainCodeFile >> startRow;
        chainCodeFile >> startCol;
        chainCodeFile >> clabel;

        //set start values
        tmpLabel = clabel + 2;
        startP = Point(startRow, startCol);

        //dynamically allocate imgAry
        imgAry = new int*[cRows];
        for(int i = 0; i < cRows; i++){
            imgAry[i] = new int[cCols];
            for(int j = 0; j < cCols; j++){
                imgAry[i][j] = 0;
            }//for
        }//for

        //output headers to files
        deCompressFile << cRows << " ";
        deCompressFile << cCols << " ";
        deCompressFile << cMinVal << " ";
        deCompressFile << cMaxVal << endl;

        boundaryFile << cRows << " ";
        boundaryFile << cCols << " ";
        boundaryFile << cMinVal << " ";
        boundaryFile << cMaxVal << endl;

        //construct boundary and output boundary to boundaryFile
        constructBoundary(chainCodeFile, boundaryFile, imgAry, tmpLabel);
        printImageAryToFile(imgAry, cRows, cCols, boundaryFile);

        //fill interior of the object then print the image to decompressedFile
        fillInterior(imgAry);
        printImageAryToFile(imgAry, cRows, cCols, deCompressFile);
    }//reconstructObject
    void constructBoundary(ifstream &chainCodeFile, ofstream &boundaryFile, int **&imgAry, int tmplabel){
        imgAry[startP.row][startP.col] = tmplabel;
        currentP = startP;
        
        //go through the chain add contruct the boundary
        do{
            chainCodeFile >> chainDir;
            loadNeighborsCoord(currentP);
            nextP = neighborhood[chainDir];
            imgAry[nextP.row][nextP.col] = tmplabel;
            currentP = nextP;
        }while(currentP != startP);
    }//constructBoundary
    void fillInterior(int **&imgAry){

    }//fillInterior
    void printImageAryToFile(int **&imgAry, int numRows, int numCols, ofstream &outputFileStream){
            for(int i = 0; i < numRows; i++){
                for(int j = 0; j < numCols; j++){
                    outputFileStream << imgAry[i][j] << " ";
                }//for
                outputFileStream << endl;
            }//for
        }//printImageToFile
    
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
    loadImage(imgFileInputStream, zeroFramedAry);
}//Image::Image(...)

ChainCode::ChainCode(){
    compassTable[0] = 6;
    compassTable[1] = 0;
    compassTable[2] = 0;
    compassTable[3] = 2;
    compassTable[4] = 2;
    compassTable[5] = 4;
    compassTable[6] = 4;
    compassTable[7] = 6;
}//ChainCode::ChainCode()


/* Data  */
ifstream inFile;
ofstream chainCodeFile, decompressFile, boundaryFile;
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
    string f = argv[1];
    string mainFileName = f.substr(0,f.find(".txt",0));

    //Step 0 (encomppases step 3)
    inFile.open(argv[1]);
    image = Image(inFile);
    chaincode = ChainCode();

    //Step 1
    string chainCodeFileName = mainFileName + "_chaincode.txt";

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
    string decompressedFileName = mainFileName + "_chainCodeDecompressed.txt";
    string boundaryFileName = mainFileName + "_Boundary.txt";

    //Step 5
    decompressFile.open(decompressedFileName);
    boundaryFile.open(boundaryFileName);


    //Step 6
    chaincode.reconstructObject(inChaincodeFile, boundaryFile, decompressFile, image.imgAry);

    //Step 7
    inFile.close();
    chainCodeFile.close();
    decompressFile.close();
    boundaryFile.close();

    return 0;
}//main