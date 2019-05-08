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

            Box(){ 
                minR = 0;
                minC = 0;
                maxR = 0;
                maxC = 0;
            }
            Box(int minRv, int minCv, int maxRv, int maxCv){
                minR = minRv;
                minC = minCv;
                maxR = maxRv;
                maxC = maxCv;
            }//Box
            void printBoxInfo(){
                cout << this << " " << minR << " " << minC << " " << maxR << " " << maxC << endl;
            }//printBoxInfo
            void operator=(const Box& b){
            /*
                cout << "operator= box called\n";
                
                int lr, lc, br, bc;
                lr = b.minR;
                lc = b.minC;
                br = b.maxR;
                bc = b.maxC;
                return Box(lr, lc, br, bc);
            */
                
                minR = b.minR;
                minC = b.minC;
                maxR = b.maxR;
                maxC = b.maxC;
            }
    };//Box

    class BoxNode{
        public: 
            int boxType; // 1=docBox, 2=paragraph, 3=textline
            Box bbox;
            BoxNode *nextBox;

            BoxNode(){}
            BoxNode(int lr, int lc, int br, int bc){
                setType(0);
                setBox(new Box(lr, lc, br, lc));
            }
            BoxNode(int type, Box *box, BoxNode *nextB){
                setType(type);
                setBox(box);
                setNextBoxNode(nextB);
            }//BoxNode
            BoxNode(int type, Box *box){
                setType(type);
                setBox(box);
            }//BoxNode
            void setType(int type){
                boxType = type;
            }
            void setBox(Box *box){
                bbox = *box;
            }
            void setNextBoxNode(BoxNode *nextB){
                nextBox = nextB;
            }
            void printBoxDetails(ofstream &file){
                cout <<" printing box details to file... " << this << endl;
                file << boxType << endl;
                file << bbox.minR << " " << bbox.minC << " ";
                file << bbox.maxR << " " << bbox.maxC << "\n";
            }
            void printToConsole(){
                cout << "Print to boxnode to console called\n";
                cout << boxType << endl;
                bbox.printBoxInfo();
            }
            void determineType(bool horizontal){
                int s,f;
                //if horizontal
                if(horizontal){
                    s = bbox.minR;
                    f = bbox.maxR;
                }
                //if vertical
                else{
                    s = bbox.minC;
                    f = bbox.maxC;
                }

                //textLine
                if(s == f){
                    boxType = 1;
                } 
                //paragraph
                else{
                    boxType = 2;
                }
            }
            BoxNode* getNextNode(){
                return nextBox;
            }//
            BoxNode operator=(const BoxNode& b){
                BoxNode n;
                Box k = b.bbox;
                n.setBox(&k);
                n.setType(b.boxType);
                n.setNextBoxNode(b.nextBox);
                return n;
            }//
    };//BoxNode

    class BoxQueue{
        BoxNode front; //will be dummy
        BoxNode back;
        int size;

    public:
        BoxQueue(){
            BoxNode n;
            n.setType(-1);
            front = n;
            back = n;
            size = 0;
        }//BoxQueue
        BoxQueue(BoxNode *n){
            front = *n;
            front.setNextBoxNode(&back);
            size = 1;
        }
        void insertNode(BoxNode *newBoxNode){
            //TODO: fix this shit
            back.setNextBoxNode(newBoxNode);
            back = *newBoxNode;
            size++;
        }//insertNode
        void printBoxQueue(ofstream &outputFile){
            cout << "**print box queue called**\n";
            BoxNode n;
            n = front; int count = 0;
            for(int i = 0; i < size; i++){
                cout << ">> file print round: " << ++count << endl;
                cout << ">>> n address: " << &n << endl;
                n.printToConsole();
                n.printBoxDetails(outputFile);
                n = *n.nextBox;
            }
            
            // int count = 0;
            // BoxNode p;
            // BoxNode *pk;
            // p = front;
            // do{
            //     p.printToConsole();
            //     p.printBoxDetails(outputFile);
            //     outputFile << endl;
            //     cout << endl;
            //     pk = p.nextBox;
            //     p = *pk;
            // } while(&p != back.nextBox);
        }//printBoxQueue
    };//BoxQueue

    public: 
        //DATA STRUCTS
        int numRows, numCols, minVal, maxVal;
        int heightPP, widthPP;
        int** imgAry;
        Box imgBox;
        BoxQueue boxQueue;
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
        }//loadImage
        void setThreshVal(int thr){
            thrVal = thr;
        }//setThreshVal
        Box computeBBox(){
            Box b;
            int* aryA;
            int* aryB;
            int* aryC;
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
            aryA = new int[heightPP];
            aryB = new int[heightPP];
            aryC = new int[heightPP];
            for(int i = 0; i < heightPP; i++){
                aryA[i] = 0;
                aryB[i] = 0;
                aryC[i] = 0;
            }//for
            hpp = aryA;
            hppBin = aryB;
            hppMorph = aryC;

            //allocate VPP
            aryA = new int[widthPP];
            aryB = new int[widthPP];
            aryC = new int[widthPP];
            for(int j = 0; j < widthPP; j++){
                aryA[j] = 0;
                aryB[j] = 0;
                aryC[j] = 0;
            }//for
            vpp = aryA;
            vppBin = aryB;
            vppMorph = aryC;

            //compute bounding box of document
            b = Box(top, left, bottom, right);
            return b;
        }//computeBBox
        void computeHPP(int **&imageArray, Box imageBox, int *horpp){
            //count up the pixels on each row within bbox
            int top = imageBox.minR;
            int bottom = imageBox.maxR;

            for(int i = top; i <= bottom; i++){
                for(int j = 0; j < numCols; j++){
                    if(imageArray[i][j] > 0) horpp[i-top+1]++;
                }//for
            }//for
        }//computeHPP
        void computeVPP(int **&imageArray, Box imageBox, int *verpp){
            //count up the pixels on each col within bbxo
            int left = imageBox.minC;
            int right = imageBox.maxC;

            for(int i = 0; i < numRows; i++){
                for(int j = left; j <= right; j++){
                    if(imageArray[i][j] > 0) verpp[j-left+1]++;
                }//for
            }//for
        }//computeVPP
        void applyThreshold(){
            //within pp, change all vals less than thr to 0, else 1
            for(int x = 0; x < heightPP; x++){
                if(hpp[x] >= thrVal) hppBin[x] = 1;
                else hppBin[x] = 0;
            }
            for(int x = 0; x < widthPP; x++){
                if(vpp[x] >= thrVal) vppBin[x] = 1;
                else vppBin[x] = 0;
            }
        }//applyThreshold
        void printPP(int *pp, ofstream &file, int start, int size){
            //print indices and vals
            file << "==========================\n";
            file << "INDEX(Row) | VALUE\t |\n";
            file << "==========================\n";
            for(int k = 0; k < size+1; k++){
                file << k << "(" << k + start << ")\t   | "<< pp[k] << "\t\t | ";
                for(int b = 0; b < pp[k]; b++) file << "*";
                file << endl;
            }
            file << endl;
        }//printPP
        void morphClosing(int *ppBin, int *ppMorph, int size, int *structElement, int origin, int structSize){
            //Allocate m
            int *m = new int[size+1];
            for( int i = 0; i < size+1; i++){
                int n = ppBin[i];
                m[i] = n;
            }//for

            //DILATION
            for(int d = 1; d < size+1; d++){
                if(ppBin[d] == structElement[origin]){
                    int x = d-1;
                    for(int s = 0; s < structSize; s++){
                        if(structElement[s] == 1)
                            m[x + s] = 1;
                            ppMorph[x + s] = 1;
                    }//for
                }//if
            }//for

            //EROSION
            for(int d = 1; d < size+1; d++){
                if(m[d] == 1){
                    bool matches = true;
                    int x = d-1;
                    for(int s = 0; s < structSize; s++){
                        if(!(m[x+s] == structElement[s])){
                            matches = false;
                        }
                    }
                if(!matches) ppMorph[d] = 0;
                }
            }//for
        }//morphClosing
        int* computeReadingDirection(int factor){
            int* readDir;
            
            //compute hppRuns
            int prevVal = hppMorph[0];
            hppRuns = 1;
            for(int x = 1; x < heightPP+1; x++){
                if(prevVal != hppMorph[x]){
                    prevVal = hppMorph[x];
                    hppRuns++;
                }//if
            }//for

            //cimpute vppRuns
            prevVal = vppMorph[0];
            vppRuns = 1;
            for(int x = 1; x < widthPP+1; x++){
                if(prevVal != vppMorph[x]){
                    prevVal = vppMorph[x];
                    vppRuns++;
                }//if
            }//for

            //find reading direction
            /*
                if HPPruns >= factor * VPPruns  // try factor 3 
                    return hppVmorph
                else if VPPruns >= factor * HPPruns  
                    return vppMorph
                else : 
                    return empty array reference
            */
            if(hppRuns >= factor * vppRuns) readDir = hppMorph;
            else if(vppRuns >= factor * hppRuns) readDir = vppMorph;

            return readDir;
        }//computeReadingDirection

        void findLineBoxes(int *pp, bool horizontal){
            int rowOffset = imgBox.minR;
            int colOffset = imgBox.minC;

            //get correct size value
            int size;
            if(pp == hppMorph) size = heightPP;
            else if (pp == vppMorph) size = widthPP;
            else cout << "ERROR findLineBoxes(int *pp): pp is not hppMorph or vppMorph.\n";

            //Compute the text-line bounding boxes
            int start = 0;
            int end = 0;
            for(int x = 0; x < size+1; x++){
                Box *tLineBox = new Box();
                if(pp[x] == 0){
                    //compute and append box
                    if(!(end == x)){
                        //make box and append to Queue
                        int lr, lc, br, bc;
                        
                        if(pp == hppMorph){
                            //find start and end of row
                            int f = 999999;
                            int l = -1;
                            for(int k = colOffset; k < widthPP + colOffset; k++){
                                if(imgAry[x + rowOffset][k] > 0){
                                    if(k < f) f = k;
                                    if(k > l) l = k;
                                }//if
                            }//for

                            //set box values
                            lr = start + rowOffset;
                            br = end + rowOffset;
                            lc = f;
                            bc = l;
                        } else{
                            //find start and end of col
                            int f = 999999;
                            int l = -1;
                            for(int k = rowOffset; k < heightPP + rowOffset; k++){
                                if(imgAry[k][x + colOffset] > 0){
                                    if(k < f) f = k;
                                    if(k > l) l = k;
                                }//if
                            }//for

                            //set box values
                            lr = f;
                            br = l;
                            lc = start + colOffset;
                            br = end + colOffset;
                        }//else
                        BoxNode node = BoxNode(lr, lc, br, bc);
                        node.determineType(horizontal);
                        boxQueue.insertNode(&node);
                        node.bbox.printBoxInfo();
                    }//addBox
                    start = x+1;
                    end = x+1;
                }else if (pp[x] == 1){
                    end = x;
                }//else
            }//for
        }//findLineBoxes

        void printQueue(ofstream &file){
            cout << "sending queue to file\n";
            boxQueue.printBoxQueue(file);
        }//printQueue
};

ImagePP::ImagePP(ifstream &inputFile){
    inputFile >> numRows;
    inputFile >> numCols;
    inputFile >> minVal;
    inputFile >> maxVal;
    loadImage(inputFile);
    imgBox = computeBBox();
    BoxNode n = BoxNode(3, &imgBox);
    BoxNode *np = &n;
    boxQueue.insertNode(np);
}//ImagePP constructor


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
    // threshholdValue = stoi(argv[2]);
    // if(threshholdValue < 1 || threshholdValue > 255){
    //     cout << "<threshVal> must be an integer greater than 0, less than 256.\n";
    //     return -1;
    // }
    threshholdValue=3;

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
    image.computeVPP(image.imgAry, image.imgBox, image.vpp);

    //Step 4
    outFile << "HPP:\n";
    image.printPP(image.hpp, outFile, image.imgBox.minR, image.heightPP);
    outFile << "VPP:\n";
    image.printPP(image.vpp, outFile, image.imgBox.minC, image.widthPP);

    //Step 5
    image.applyThreshold();
    image.applyThreshold();

    //Step 6
    outFile << "HPP Binary:\n";
    image.printPP(image.hppBin, outFile, image.imgBox.minR, image.heightPP);
    outFile << "VPP Binary:\n";
    image.printPP(image.vppBin, outFile, image.imgBox.minC, image.widthPP);

    //Step 7
    int structElem[3] = {1,1,1};
    image.morphClosing(image.hppBin, image.hppMorph, image.heightPP, structElem, 1, 3);
    image.morphClosing(image.vppBin, image.vppMorph, image.widthPP, structElem, 1, 3);

    outFile << "HPPMorph:\n";
    image.printPP(image.hppMorph, outFile, image.imgBox.minR, image.heightPP);
    outFile << "VPPMorph:\n";
    image.printPP(image.vppMorph, outFile, image.imgBox.minC, image.widthPP);

    //Step 8
    int* readingPP;
    readingPP = image.computeReadingDirection(3);

    //Step 9
    bool horizontal;
    if(readingPP == image.hppMorph){
        outFile << "Reading Direction: HORIZONTAL\n";
        horizontal = true;
    }
    else if(readingPP == image.vppMorph){
        outFile << "Reading Direction: VERTICAL\n";
        horizontal = false;
    } else {
        cout << "Can not determine reading direction. Program will now exit.\n";
        outFile << "Reading Direction: CANNOT BE DETERMINED\n";
        inFile.close();
        outFile.close();
        return -2;
    }//if else chain

    //Step 10
    
    image.findLineBoxes(readingPP, horizontal);

    //Step 11
    image.printQueue(outFile);

    //Step 12
    inFile.close();
    outFile.close();

    return 1;
}//main