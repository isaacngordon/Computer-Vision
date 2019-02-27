import java.io.FileNotFoundException;

public class MedianGaussFilter {

	public static void main(String[] args) {
		
		final String ARG_ERROR_STRING = "Improper arguements. Correct syntax is: \n>> ... <input1.txt> <input2.txt> <output1.txt> <output2.txt>"
				+ " \n\twhere: \n\tinput1.txt is a grey-scale image with header \n\tinput2.txt is a Gaussian Template \n\toutput1.txt is a file to "
				+ " print the result of the Median filter to \n\toutput2.txt is a file to print the result of the Gaussian Filter.";
		
		//STEP -1: Check for bad arguements
		if(args.length != 4) {
			System.out.println(ARG_ERROR_STRING);
			System.exit(0);
		}//if
		
		for(String s: args) {
			if(!s.endsWith("txt")) {
				System.out.println(ARG_ERROR_STRING);
				System.exit(0);
			}//if
		}//for
		
		String inFile1 = args[0];
		String inFile2 = args[1];
		String outFile1 = args[2];
		String outFile2 = args[3];
		
		Image image;
		try {
			image = new Image(inFile1, inFile2);
			
			//output new header details to outFile1, output medianAry to outFile1
			image.medianFilter();
			image.printMedianOutput(outFile1);
			
			//run gaussFilter and print new header and the new image to outFile2
			image.gaussianFilter();
			image.printGaussOutput(outFile2);
			
		} catch (FileNotFoundException e) {
			System.out.println("1 or more of your arguements could not be found. Check its path.");
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			System.out.println("1 or more of your arguements were not a text file.");
			e.printStackTrace();
		}
	}//main
}//class
