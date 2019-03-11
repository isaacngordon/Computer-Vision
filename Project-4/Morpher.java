import java.io.*;
import java.util.*;

public abstract class Morpher {

    public Image dilation(Image originalImage, StructureElement structElement){
        //copy origional image
        Image morphedImage = new Image();
        morphedImage.copyImage(originalImage);

    }//dilation

    public Image erosion(Image originalImage, StructureElement structElement){
        //copy origional image
        Image morphedImage = new Image();
        morphedImage.copyImage(originalImage);
        
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