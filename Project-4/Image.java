import java.io.*;
import java.util.*;

public class Image {
    int numRows;                //number of rows based on header
    int numCols;                //number of cols based on header
    int minVal;                 //lowest value based on header
    int maxVal;                 //largest value based on header

    int[][] imgAry;             //actual image data
    boolean isFramed;           //is this a framed image?

    public Image(){

    }//null contructor
    
    public Image(int numRows, int numCols, int minVal, int maxVal){
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
        initWithZeros();
    }//constructor w header

    public Image(File imageFile) throws FileNotFoundException{
        Scanner inStream = new Scanner(new FileReader(imageFile));

        //get header values
        this.numRows = inStream.nextInt();
        this.numCols = inStream.nextInt();
        this.minVal = inStream.nextInt();
        this.maxVal = inStream.nextInt();
        initWithZeros();

        //get all image data
        for(int i = 0; i < this.numRows; i++){
            for(int j = 0; j < this.numCols; j++){
                imgAry[i][j] = inStream.nextInt();
            }//for
        }//for
        inStream.close();

        //set as unframed image
        isFramed = false;
    }//constrcutor from file


	/**
     * Initializs imgAry with a 2D array of zeros.
     */
    public void initWithZeros(){
        imgAry = new int[numRows][numCols];
        for(int r = 0; r < numRows; r++){
            for(int c = 0; c < numCols; c++){
                imgAry[r][c] = 0;
            }//for
        }//for
    }//initWithZeroes

    /**
     * @param imgAry the imgAry to set
     */
    public void copyImage(Image image) {
        this.numCols = image.getNumCols();
        this.numRows = image.getNumRows();
        this.minVal = image.getMinVal();
        this.maxVal = image.getMaxVal();
        this.isFramed = image.isFramed();
        this.imgAry = new int[this.numRows][this.numCols];
        
        int[][] copyAry = image.getImgAry();
        for(int i = 0; i < numRows; i++){
        	for(int j=0; j < numCols; j++){
        		this.imgAry[i][j] = copyAry[i][j];
        	}//for
        }//fpr
        		
    }//copyImage

    /**
     * Creates a frame around this image based of the origin of the structuring element
     * @param structuringElement
     * @return a copy of this image framed, null if the image is already framed
     */
    public Image frame(StructuringElement structuringElement){
        Image framedImage = null;
        if(this.isFramed) return framedImage;

        //get all frame dims
        int[] frameDims = structuringElement.computeFrame();
        int top = frameDims[0];
        int bottom = frameDims[1];
        int left  = frameDims[2];
        int right = frameDims[3];

        //get new imgAry, new row count, new col count
        int newNumRows = this.numRows + top + bottom;
        int newNumCols = this.numCols + left + right;
        int[][] newImgAry = new int[newNumRows][newNumCols];

        //copy this.imgAry into the framed newImgAry
        for(int r = top; r < newNumRows - bottom; r++){
            for(int c = left; c < newNumCols - right; c++){
                newImgAry[r][c] = this.imgAry[r - top][c - left];
            }//for
        }//for

        //create new framedImage to return
        framedImage = new Image(newNumRows, newNumCols, this.minVal, this.maxVal);
        framedImage.setImgAry(newImgAry);
        framedImage.setFramed(true);
        return framedImage;
    }//frame()

    /**
     * Unframe an image and return the unframed image
     * @return
     */
    public Image unframe(StructuringElement structuringElement){
        if(!this.isFramed) return this;
        Image unframedImage = new Image();

        //find unframe image dims
        int[] frameDims = structuringElement.computeFrame();
        int top = frameDims[0];
        int bottom = frameDims[1];
        int left  = frameDims[2];
        int right = frameDims[3];

        int newNumRows = this.numRows - top - bottom;
        int newNumCols = this.numCols - left - right;
        int[][] newImgAry = new int[newNumRows][newNumCols];

        //copy this.imgAry into the framed newImgAry
        for(int r = 0; r < newNumRows; r++){
            for(int c = 0; c < newNumCols; c++){
                newImgAry[r][c] = this.imgAry[r + top][c + left];
            }//for
        }//for
        
        unframedImage.setNumCols(newNumCols);
        unframedImage.setNumRows(newNumRows);
        unframedImage.setImgAry(newImgAry);
        unframedImage.setFramed(false);
        return unframedImage;
    }//unframe

    /**
     * Prints the image to the console
     */
    public void prettyprint(boolean doBinary){
        //header
        System.out.println("\n"+ numRows + " " + numCols + " " + minVal + " " + maxVal);

        //imgAry print -> if doBinary then print 0's else replace 0's with space
        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numCols; j++){
                if(this.imgAry[i][j] == 1)
                    System.out.print("1");
                else{
                    if(doBinary) System.out.print("0");
                    else System.out.print(" ");
                }//if-else
            }//for
            System.out.println();
        }//for
        System.out.println("\n");
    }//prettyprint to console

    /**
     * Prints the image to an outputfile
     * @param outputFile the file to send the image
     * @throws FileNotFoundException 
     */
    public void prettyprint(File outputFile, boolean doBinary) throws FileNotFoundException{
        PrintWriter outputstream = new PrintWriter(outputFile);
        outputstream.println(numRows + " " + numCols  + " " + minVal  + " " + maxVal);
		for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numCols; j++){
                if(this.imgAry[i][j] == 1)
                    outputstream.print("1");
                else{
                    if(doBinary) outputstream.print("0");
                    else outputstream.print(" ");
                }//if-else
            }//for
            outputstream.println();
        }//for
		outputstream.close();
    }//preetyprint to outputFile

    /**
     * @return the numRows
     */
    public int getNumRows() {
        return numRows;
    }//getNumRows

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
    public void setImgAry(int[][] imgAry) {
    	this.imgAry = new int[this.numRows][this.numCols];
        for(int i = 0; i < imgAry.length; i++){
        	for(int j = 0; j < imgAry[i].length;j++){
        		this.imgAry[i][j] = imgAry[i][j];
        	}
        }
    }

    /**
     * @return the isFramed
     */
    public boolean isFramed() {
        return isFramed;
    }

    /**
     * @param isFramed the isFramed to set
     */
    public void setFramed(boolean isFramed) {
        this.isFramed = isFramed;
    }

}//class