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

        //the actual dilation ASSUMES FRAMED IMAGE
        for(int r = top; r < originalImage.getNumRows() - bottom; r++){
            for(int c = left; c < originalImage.getNumCols() - right; c++){
                dilate(r,c,originalImage,morphedImage);
            }//for
        }//for

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
                erode(r,c,originalImage,morphedImage);
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