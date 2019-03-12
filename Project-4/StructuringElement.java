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

    public void prettyprint(boolean doBinary){
        //headers
        System.out.println(numRows + " " + numCols + " " + minVal + " " + maxVal);
        System.out.println(originX + " " + originY);

        //structElement print -> if doBinary then print 0's else replace 0's with space
        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numCols; j++){
                if(structImgArray[i][j] == 1)
                    System.out.print("1");
                else{
                    if(doBinary) System.out.print("0");
                    else System.out.print(" ");
                }//if-else
            }//for
            System.out.println();
        }//for
    }//prettyPrint

        /**
     * Computes the framing dimesntions based on a given structuring element.
     * @return int[4] = [topFrameDim, bottomFrameDim, leftFrameDim, rightFrameDim]
     */
    public int[] computeFrame(){
        int[] dims = new int[4];

        //find distance from origin to all sides, and add said dims to dims[]
        dims[0] = this.originY;
        dims[1] = (this.numRows - 1) - this.originY;
        dims[2] = this.originX;
        dims[3] = (this.numCols - 1) - this.originX;

        return dims;
    }//computeFrame

    /**
     * @return the numRows
     */
    public int getNumRows() {
        return numRows;
    }

    /**
     * @return the numCols
     */
    public int getNumCols() {
        return numCols;
    }

    /**
     * @return the minVal
     */
    public int getMinVal() {
        return minVal;
    }

    /**
     * @return the maxVal
     */
    public int getMaxVal() {
        return maxVal;
    }

    /**
     * @return the originX
     */
    public int getOriginX() {
        return originX;
    }

    /**
     * @return the originY
     */
    public int getOriginY() {
        return originY;
    }

    /**
     * @return the structImgArray
     */
    public int[][] getStructImgArray() {
        return structImgArray;
    }

}//class