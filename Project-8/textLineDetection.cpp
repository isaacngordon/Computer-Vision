#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<cmath>

using namespace std;

/* PROGRAM CLASS DEFS */

class ImagePP{
    class Box{
        public:
            int minR, minC, maxR, maxC;

            Box(){ }
            Box(int minRv, int minCv, int maxRv, int maxCv){
                minR = minRv;
                minC = minCv;
                maxR = maxRv;
                maxC = maxCv;
            }
            void printBoxInfo(ofstream &outputFile){
                //TODO: 
            }
    };

    class BoxNode{
        public: 
            int boxType; // 1=docBox, 2=paragraph, 3=textline
            Box bbox;
            BoxNode* nextBox;

            BoxNode();
            BoxNode(int type, Box box, BoxNode* nextB){
                setType(type);
                setBox(box);
                setNextBoxNode(nextB);
            }
            void setType(int type){
                boxType = type;
            }
            void setBox(Box box){
                bbox = box;
            }
            void setNextBoxNode(BoxNode *nextB){
                nextBox = nextB;
            }
    };

    class BoxQueue{
        BoxNode* front;
        BoxNode* back;

        
        void insertNode(BoxNode* newBoxNode){
            //TODO: fix this shit
            BoxNode b = *back;
            b.setNextBoxNode(newBoxNode);
            back = newBoxNode;
        }
        void printBoxQueue(ofstream &outputFile){
            //TODO: 
        }
    };

    public: 
        //DATA STRUCTS
        int numRows, numCols, minVal, maxVal;
        int heightPP, widthPP;
        int** imgAry;
        Box imgBox;
        int thrVal;
        int* hpp; int* vpp; int* hppBin; int* vppBin; int* hppMorph; int* vppMorph;
        int hppRuns, vppRuns;

        ImagePP(){}
        ImagePP(ifstream &inputFile);
        void loadImage(ifstream &inputFile){
            int** ary;
            ary = new int*[numRows];
            for(int i = 0; i < numRows; i++){
                ary[i] = new int[numCols];
                for(int j = 0; j < numCols; j++){
                    inputFile >> ary[i][j];
                }//for
            }//for
            imgAry = ary;
        }
        void setThreshVal(int thr){
            thrVal = thr;
        }
        Box computeBBox(){
            Box b;
            int* ary;
            int top, bottom, left, right, width;

            //find top, bottom, left, right
            top = numRows;
            bottom = 0;
            left = numCols;
            right = 0;
            for(int i = 0; i < numRows; i++){
                for(int j = 0; j < numCols; j++){
                    if(imgAry[i][j] > 0){
                        if(i < top) top = i;
                        if(i > bottom) bottom = i;
                        if(j < left) left = j;
                        if(j > right) right = j;
                    }
                }//for
            }//for

            //compute dims
            heightPP = bottom - top + 2;
            widthPP = right - left + 2;

            //allocate HPP
            ary = new int[heightPP];
            for(int i = 0; i < heightPP; i++){
                ary[i] = 0;
            }//for
            hpp = ary;

            //allocate VPP
            ary = new int[widthPP];
            for(int j = 0; j < widthPP; j++){
                ary[j] = 0;
            }//for
            vpp = ary;

            //compute bounding box of document
            b = Box(top, left, bottom, right);
            return b;
        }
        void computeHPP(int **&imageArray, Box imageBox, int* horpp){
            //TODO: count up the pixels on each row within bbox
            int top = imageBox.minR;
            int bottom = imageBox.maxR;

            for(int i = top; i <= bottom; i++){
                for(int j = 0; j < numCols; j++){
                    if(imageArray[i][j] > 0) horpp[i-top+1]++;
                }
            }

        }
        void computeVPP(int **&imageArray, Box imageBox, int* verpp){
            //TODO: count up the pixels on each col within bbxo
            int left = imageBox.minC;
            int right = imageBox.maxC;

            for(int i = 0; i < numRows; i++){
                for(int j = left; j <= right; j++){
                    if(imageArray[i][j] > 0) verpp[j-left+1]++;
                }
            }
        }
        void applyThreshold(int* pp, int thr, int* binPP){
            //TODO: within pp, change all vals less than thr to 0, else 1
        }
        void printPP(int* pp, ofstream &file, int start, int size){
            
            //print indices and vals
            file << "==========================\n";
            file << "INDEX(Row) | VALUE\t |\n";
            file << "==========================\n";
            for(int k = 0; k < size+1; k++)
                file << k << "(" << k + start << ")\t   | "<< pp[k] << "\t\t |\n";
            file << endl;
        }
        void morphClosing(int* ppMorph, int* structElement){
            //TODO: get funtions from morph project
        }
        int* computeReadingDirection(){
            int* readDir;
            /*
                if HPPruns >= factor * VPPruns  // try factor 3 
                    return hppVmorph
                else if VPPruns >= factor * HPPruns  
                    return vppMorph
                else : 
                    return empty array reference
            */
            return readDir;
        }
        void findLineBoxes(int* pp){
            //TODO: Base on the reading direction, you are to compute the text-line bounding
            
            //TODO: add box to QUEUE
        }
        void printQueue(ofstream &file){
            //TODO: for every box in the queue, print
            /*
                box type
                minRow minCol maxRow 	maxCol
            */
        }
};

ImagePP::ImagePP(ifstream &inputFile){
    inputFile >> numRows;
    inputFile >> numCols;
    inputFile >> minVal;
    inputFile >> maxVal;
    loadImage(inputFile);
    imgBox = computeBBox();
}


/* PROGRAM DATA STRUCTURES */
ifstream inFile;
ofstream outFile;
int threshholdValue;
ImagePP image;

int main(int argc, char *argv[]){

    //Step -1
    if(argc != 3){
        cout << "Correct syntax is >> ./chainCode <inputFile> <threshVal>\n"; 
        cout << "where <inputFile> is a binary image containgina single object and a header.\n";
        cout << "and <threshVal> is the threshold value to be applied to projection profiles.\n";
        return -1;
    }

    //Step 0
    threshholdValue = stoi(argv[2]);
    if(threshholdValue < 1 || threshholdValue > 255){
        cout << "<threshVal> must be an integer greater than 0, less than 256.\n";
        return -1;
    }

    inFile.open(argv[1]);
    string f = argv[1];
    string mainFileName = f.substr(0,f.find(".txt",0));
    string outFileName = mainFileName + "_results.txt";
    char ccCharAry[outFileName.length() + 1];
    strcpy(ccCharAry, outFileName.c_str());
    outFile.open(ccCharAry);

    //Step 1&2
    image = ImagePP(inFile);
    image.setThreshVal(threshholdValue);

    //Step 3
    image.computeHPP(image.imgAry, image.imgBox, image.hpp);
    image.computeHPP(image.imgAry, image.imgBox, image.vpp);

    //Step 4
    outFile << "HPP:\n";
    image.printPP(image.hpp, outFile, image.imgBox.minR, image.heightPP);
    outFile << "VPP:\n";
    image.printPP(image.vpp, outFile, image.imgBox.minC, image.widthPP);

    //Step 5
    image.applyThreshold(image.hpp, 3, image.hppBin);
    image.applyThreshold(image.vpp, 3, image.vppBin);

    //Step 6
    outFile << "HPP Binary:\n";
    image.printPP(image.hppBin, outFile, image.imgBox.minR, image.heightPP);
    outFile << "VPP Binary:\n";
    image.printPP(image.vppBin, outFile, image.imgBox.minC, image.widthPP);

    //Step 7
    int structElem[3] = {1,1,1};
    image.morphClosing(image.hppMorph, structElem);
    image.morphClosing(image.vppMorph, structElem);

    //Step 8
    int* readingPP;
    readingPP = image.computeReadingDirection();

    //Step 9
    if(&readingPP == &image.hppMorph){
        outFile << "Reading Direction: HORIZONTAL\n";
    }
    else if(&readingPP == &image.vppMorph){
        outFile << "Reading Direction: VERTICAL\n";
    } else {
        cout << "Can not determine reading direction. Program will now exit.\n";
        inFile.close();
        outFile.close();
        return -2;
    }//if else chain

    //Step 10
    image.findLineBoxes(readingPP);

    //Step 11
    image.printQueue(outFile);

    //Step 12
    inFile.close();
    outFile.close();

    return 1;
}