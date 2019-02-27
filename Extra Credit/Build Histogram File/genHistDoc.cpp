#include <iostream>
#include <fstream>

using namespace std;


int main(){
    ifstream inFile;
    inFile.open("Data.txt");
    ofstream outFile;
    outFile.open("histogram.txt");

    outFile << "64 64 0 63" << endl;
    for(int i = 0; i < 64; i++){
        int val = 0;
        inFile >> val;
        outFile << i << " " << val << endl; 
    }

    inFile.close();
    outFile.close();

    return 1;
}