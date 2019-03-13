import java.io.*;
import java.util.*;


public abstract class Morpher {

    /**
     * Dilates a framed image using a specified structuring element. Assumes the image is framed
     * using the specified structElement.
     * @param originalImage the image to dilate
     * @param structElement the structuring element to use
     */
    public static Image dilation(Image originalImage, StructuringElement structElement) throws IllegalArgumentException{
        //make sure the image inputted is a framed image
        if(!originalImage.isFramed()){
            throw new IllegalArgumentException("Inputted Image must be framed.");
        }//if
        
        //copy original image
        Image morphedImage = new Image();
        morphedImage.copyImage(originalImage);
        int[] frameDims = structElement.computeFrame();

        int top = frameDims[0];
        int bottom = frameDims[1];
        int left = frameDims[2];
        int right = frameDims[3];
        int originVal = structElement.structImgArray[structElement.getOriginX()][structElement.getOriginY()];
        int[][] oImg = originalImage.getImgAry();
        
        //the actual dilation ASSUMES FRAMED IMAGE
        for(int r = top; r < originalImage.getNumRows() - bottom; r++){
            for(int c = left; c < originalImage.getNumCols() - right; c++){
                
                //if the origin matches the current pixel, then dilate
            	if(oImg[r][c] == originVal){
                    //dilate using each element of the structElement
                    for(int i = 0; i < structElement.getNumRows(); i++){
                        for(int j = 0; j < structElement.getNumCols(); j++){
                            int x = r+i-top;
                            int y = c+j-right;
                            
                            if(structElement.structImgArray[i][j] == 1)
                                morphedImage.imgAry[x][y] = 1;
                        }//for j
                    }//for i
                }//if
            }//for c	
        }//for r
        return morphedImage;
    }//dilation

    /**
     * Erodes a framed image using a specified structuring element. Assumes the image is framed
     * using the specified structElement.
     * @param originalImage the image to erode
     * @param structElement the structuring element to use
     */
    public static Image erosion(Image originalImage, StructuringElement structElement){
        //make sure the image inputted is a framed image
        if(!originalImage.isFramed()){
            throw new IllegalArgumentException("Inputted Image must be framed.");
        }//if
        
        //copy original image
        Image morphedImage = new Image();
        morphedImage.copyImage(originalImage);  
        int[] frameDims = structElement.computeFrame();

        int top = frameDims[0];
        int bottom = frameDims[1];
        int left = frameDims[2];
        int right = frameDims[3];
        
        int[][] newImgAry = new int[morphedImage.getNumRows()][morphedImage.getNumCols()];
        for(int i = 0; i < morphedImage.getNumRows(); i++){
        	for(int j = 0; j < morphedImage.getNumCols(); j++){
        		newImgAry[i][j] = morphedImage.imgAry[i][j];
        	}
        }

        //the actual erosion ASSUMES FRAMED IMAGE
        for(int r = top; r < originalImage.getNumRows() - bottom; r++){
            for(int c = left; c < originalImage.getNumCols() - right; c++){
                
                //if the structElement can "stand" at r,c then do nothing, otherwise wipe to zeros
                boolean matches = true; 																int hit=0;
                for(int i = 0; i < structElement.getNumRows(); i++){
                    for(int j = 0; j < structElement.getNumCols(); j++){
                        int x = r-top+i;
                        int y = c-left+j;
                        
                        //if structElem at this index is 1 but diesnt natch, set mayeches=false
                        if(!(structElement.structImgArray[i][j] == originalImage.imgAry[x][y]) && (structElement.structImgArray[i][j] == 1)){
                            matches = false;
                        }//if 
                    }//for j
                }//for i
                
                //if there is no match here, wipe to zero
                if(!matches) newImgAry[r][c] = 0;
            }//for
        }//for
        morphedImage.setImgAry(newImgAry);
        return morphedImage;
    }//erosion

    public static Image opening(Image originalImage, StructuringElement structElement){
        //copy origional image
        Image morphedImage = new Image();
        morphedImage.copyImage(originalImage);

        //run an erosion and then dialtion
        morphedImage = erosion(morphedImage, structElement);
        morphedImage = dilation(morphedImage, structElement);
        return morphedImage;
    }//opening

    public static Image closing(Image originalImage, StructuringElement structElement){
        //copy origional image
        Image morphedImage = new Image();
        morphedImage.copyImage(originalImage);

        //run a dilation then erosion
        morphedImage = dilation(morphedImage, structElement);
        morphedImage = erosion(morphedImage, structElement);
        return morphedImage;
    }//closing
}//class