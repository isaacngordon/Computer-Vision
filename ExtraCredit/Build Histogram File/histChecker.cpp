#include <iostream>
#include <fstream>

using namespace std;

// int getVal(int prev, int *total){
//   int x = 0;
//   x = (rand() % (120));
//   return x;
// }

int main(){
  ifstream file;
  file.open("data.txt");
  //file << "64 64 1 63" << endl;
  int total = 64 * 64;
  cout<< "total is "<< total << endl <<endl;
  // int a = int[64];
  for(int i = 0; i < 120; i++){
    // if(i == 0) a[i] = getVal(0, total);
    // else a[i] = getVal(a[i - 1], total);
    // file << i << " " << a[i] << endl;
    int val = 0;
    if(!file.eof()){
      file >> val;
      total -= val;
      cout << i << " with " << val << " is "<< total << endl;
    }  
  }
  file.close();
  return 1;
}
