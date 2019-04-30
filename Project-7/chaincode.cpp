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
        }
        void loadImage(ifstream &imgFileInputStream, int **&ary){
            setZeroFrame(ary);
            for(int i = 0; i < numRows; i++){
                for(int j = 0; j < numCols; j++){
                    imgFileInputStream >> ary[i+1][j+1];
                }//for
            }//for
        }
};

class ChainCode{
    public:

    //class declarations
    class Point{
        public: int row; int col;
        void setRow(int i){row = i;}
        void setCol(int i){col = i;}
        Point();
        Point(int r, int c){
            row = r;
            col = c;
        }
    };

    //data structs
    Point startP, currentP, nextP;
    Point neighborhood[8];
    int lastQ, chainDir;
    int compassTable[8];

    //Functions
    ChainCode();
    void getChainCode(int **&ZeroFramedAry, ofstream &chainCodeFile){

    }
    int findNextP(int lastQ, Point neighborCoord){

    }
    void loadNeighborsCoord(Point currentP){

    }
    void reconstructObject(ofstream &chainCodeFile, ofstream &deCompressFile, int **&imgAry){

    }
    void constructBoundary(ofstream &chainCodeFile, int **&imgAry, int tmplabel){

    }
    void fillInterior(int **&imgAry){

    }
    
};

/* Class Constructors */

Image::Image(){

}

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
}

ChainCode::ChainCode(){

}


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
    chaincode.getChainCode(image.zeroFramedAry, chainCodeFile);

    //Step 4
    string decompressedFileName;
    decompressedFileName = argv[1];
    string de = decompressedFileName.substr(0,decompressedFileName.find(".txt",0));
    decompressedFileName = de + "_chainCodeDecompressed.txt";

    //Step 5
    decompressFile.open(decompressedFileName);

    //Step 6
    chaincode.reconstructObject(chainCodeFile, decompressFile, image.imgAry);

    //Step 7
    inFile.close();
    chainCodeFile.close();
    decompressFile.close();

    return 0;
}//main