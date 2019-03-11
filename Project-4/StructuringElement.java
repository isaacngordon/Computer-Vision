import java.io.*;
import java.util.*;

public class StructuringElement{

    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int originX;
    int originY;

    int[][] structImgArray;

    public StructuringElement(){

    }//null constructor

    public StructuringElement(File structFile){
        Scanner inStream = new Scanner(new FileReader(structFile));

        //get header values
        int rows, cols, min, max, x, y;
        rows = inStream.nextInt();
        cols = inStream.nextInt();
        min = inStream.nextInt();
        max = inStream.nextInt();
        x = inStream.nextInt();
        y = inStream.nextInt();

        StructuringElement(rows, cols, min, max, x, y);

        //get all image data
        for(int i = 0; i < this.numRows; i++){
            for(int j = 0; j < this.numCols; j++){
                imgAry[i][j] = inStream.nextInt();
            }//for
        }//for
        inStream.close();
    }//main constructor

    public StructuringElement(int numRows, int numCols, int minVal, int maxVal, int originX, int originY){
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
        this.originX = originX;
        this.originY = originY;
        initZeroArray();
    }//value contructor

    public void initZeroArray(){
        for(int r = 0; r < this.numRows; r++){
            for(int c = 0; c < numCols; c++){
                structImgArray[r][c] = 0;
            }//for
        }//for
    }//initZeroArray

}//class