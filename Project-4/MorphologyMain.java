import java.io.*;
import java.util.*;


public class MorphologyMain {

    public static void main(String args[]){
        File imageInFile, structElementInFile;
        File dilationResultOutFile, erosionResultOutFile;
        File openingResultOutFile, closingResultOutFile;

        final String ARG_ERROR_STRING = "Improper arguements. Correct syntax is: \n>> ... <input1.txt> <input2.txt> <output1.txt> <output2.txt> <output3.txt> <output4.txt>"
				+ " \n\twhere: \n\tinput1.txt is a grey-scale image with header \n\tinput2.txt is a Structuring Element with a double header \n\toutput1.txt is a file to "
                + " print the result of the a Dilation to \n\toutput2.txt is a file to print the result of the Erosion.\n\toutput3.txt is a file to print the result of the Opening."
                + "\n\toutput4.txt is a file to print the result of the Closing.";
		
		//STEP -1: Check for bad arguements
		if(args.length != 6) {
			System.out.println(ARG_ERROR_STRING);
			System.exit(0);
		}//if
		
		for(String s: args) {
			if(!s.endsWith("txt")) {
				System.out.println(s + " is not a valid arguement. Must be a .txt file.");
				System.exit(0);
			}//if
		}//for

        try{
            //Step 0
            imageInFile = new File(args[0]);
            structElementInFile = new File(args[1]);
            dilationResultOutFile = new File(args[2]);
            erosionResultOutFile = new File(args[3]);
            openingResultOutFile = new File(args[4]);
            closingResultOutFile = new File(args[5]);
            Image img = new Image(imageInFile);
            StructuringElement structuringElement = new StructuringElement(structElementInFile);
            
            //Step 1
            Image framedImage = img.frame(structuringElement);

            //Step 2
            framedImage.prettyprint(true);
            structuringElement.prettyprint(true);
            
            //Step 3
            Image dilatedImage = Morpher.dilation(framedImage, structuringElement);
            dilatedImage.prettyprint(true);
            dilatedImage = dilatedImage.unframe(structuringElement);
            dilatedImage.prettyprint(dilationResultOutFile, true);

            //Step 4
            Image erodedImage = Morpher.erosion(framedImage, structuringElement);
            erodedImage.prettyprint(true);
            erodedImage = erodedImage.unframe(structuringElement);
            erodedImage.prettyprint(erosionResultOutFile, true);

            //Step 5
            Image openedImage = Morpher.opening(framedImage, structuringElement);
            openedImage.prettyprint(true);
            openedImage = openedImage.unframe(structuringElement);
            openedImage.prettyprint(openingResultOutFile, true);

            //Step 6
            Image closedImage = Morpher.closing(framedImage, structuringElement);
            closedImage.prettyprint(true);
            closedImage = closedImage.unframe(structuringElement);
            closedImage.prettyprint(closingResultOutFile, true);

        } catch(FileNotFoundException fnf){
            fnf.printStackTrace();
            System.exit(0);
        } catch(IllegalArgumentException iae){
        	iae.printStackTrace();
        	System.exit(0);
        }
       


    }//main

}//class