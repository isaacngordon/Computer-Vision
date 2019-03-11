import java.io.*;
import java.util.*;

public class Image {
    int numRows;
    int numCols;
    int minVal;
    int maxVal;

    int[][] imgAry;

    public Image(){

    }//null contructor

    public Image(File imageFile){
        Scanner inStream = new Scanner(new FileReader(imageFile));

        //get header values
        int rows, cols, min, max;
        rows = inStream.nextInt();
        cols = inStream.nextInt();
        min = inStream.nextInt();
        max = inStream.nextInt();
        Image(rows, cols, min, max);

        //get all image data
        for(int i = 0; i < this.numRows; i++){
            for(int j = 0; j < this.numCols; j++){
                imgAry[i][j] = inStream.nextInt();
            }//for
        }//for
        inStream.close();
    }//constrcutor from file

    public Image(int numRows, int numCols, int minVal, int maxVal){
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
        initWithZeros();
    }//constructor w header

    public void initWithZeros(){
        imgAry = new int[numRows][numCols];
        for(int r = 0; r < numRows; r++){
            for(int c = 0; c < numCols; c++){
                imgAry[r][c] = 0;
            }//for
        }//for
    }//initWithZeroes

    /**
     * @return the numRows
     */
    public int getNumRows() {
        return numRows;
    }

    /**
     * @param numRows the numRows to set
     */
    public void setNumRows(int numRows) {
        this.numRows = numRows;
    }

    /**
     * @return the numCols
     */
    public int getNumCols() {
        return numCols;
    }

    /**
     * @param numCols the numCols to set
     */
    public void setNumCols(int numCols) {
        this.numCols = numCols;
    }

    /**
     * @return the minVal
     */
    public int getMinVal() {
        return minVal;
    }

    /**
     * @param minVal the minVal to set
     */
    public void setMinVal(int minVal) {
        this.minVal = minVal;
    }

    /**
     * @return the maxVal
     */
    public int getMaxVal() {
        return maxVal;
    }

    /**
     * @param maxVal the maxVal to set
     */
    public void setMaxVal(int maxVal) {
        this.maxVal = maxVal;
    }

    /**
     * @return the imgAry
     */
    public int[][] getImgAry() {
        return imgAry;
    }

    /**
     * @param imgAry the imgAry to set
     */
    public void copyImage(Image image) {
        this.imgAry = image.getImgAry();
        this.numCols = image.getNumCols();
        this.numRows = image.getNumRows();
        this.minVal = image.getMinVal();
        this.maxVal = image.getMaxVal();
    }//copyImage

    

}//class