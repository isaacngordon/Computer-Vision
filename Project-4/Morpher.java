import java.io.*;
import java.util.*;

public abstract class Morpher {

    /**
     * Dilates a framed image using a specified structuring element. Assumes the image is framed
     * using the specified structElement.
     * @param originalImage the image to dilate
     * @param structElement the structuring element to use
     */
    public Image dilation(Image originalImage, StructureElement structElement) throws IllegalArgumentException{
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
        int originVal = structElement.imgAry[structElement.getOriginX()][structElement.getOriginY()];

        //the actual dilation ASSUMES FRAMED IMAGE
        for(int r = top; r < originalImage.getNumRows() - bottom; r++){
            for(int c = left; c < originalImage.getNumCols() - right; c++){
                
                //if the origin matches the current pixel, then dilate
                if(originalImage.imgAry[r][c] == originVal){
                    //dilate using each element of the structElement
                    for(int i = 0; i < structElement.getNumRows(); i++){
                        for(int j = 0; j < structElement.getNumCols(); j++){
                            int x = r-top+i;
                            int y = j-left+j;
                            if(structElement.imgAry[i][j] == 1)
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
    public Image erosion(Image originalImage, StructureElement structElement){
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

        //the actual erosion ASSUMES FRAMED IMAGE
        for(int r = top; r < originalImage.getNumRows() - bottom; r++){
            for(int c = left; c < originalImage.getNumCols() - right; c++){
                
                //if the structElement can "stand" at r,c then do nothing, otherwise wipe to zeros
                boolean matches = true;
                for(int i = 0; i < structElement.getNumRows(); i++){
                    for(int j = 0; j < structElement.getNumCols(); j++){
                        int x = r-top+i;
                        int y = j-left+j;

                        //if matches=false, not a match then just set =0
                        if(!matches){
                            morphedImage.imgAry[x][y] = 0;
                            continue;
                        }//if

                        //if the element cannot stand, set matches = false, reset i,j 
                        //this will cause the struct loops to start over triggering the above is statement
                        if(!(structElement.imgAry[i][j] == originalImage.imgAry[x][y])){
                            matches = false;
                            i = 0;
                            j = 0;
                        }//if 
                    }//for j
                }//for i

            }//for
        }//for

        return morphedImage;
    }//erosion

    public Image opening(Image originalImage, StructureElement structElement){
        //copy origional image
        Image morphedImage = new Image();
        morphedImage.copyImage(originalImage);

        //run an erosion and then dialtion
        morphedImage = erosion(morphedImage, structElement);
        morphedImage = dilation(morphedImage, structElement);
        return morphedImage;
    }//opening

    public Image closing(Image originalImage, StructureElement structElement){
        //copy origional image
        Image morphedImage = new Image();
        morphedImage.copyImage(originalImage);

        //run a dilation then erosion
        morphedImage = dilation(morphedImage, structElement);
        morphedImage = erosion(morphedImage, structElement);
        return morphedImage;
    }//closing
}//class