import java.io.*;
import java.util.*;

public class Image {
	
	int numRows;							
	int numCols;
	int minVal;
	int maxVal;
	int newMin;
	int newMax;
	
	int templateRows;
	int templateCols;
	int totalWeight;
	
	int[][] imgAry;
	int[][] gaussTemplate;
	int[][] medianAry;
	int[][] gaussAry;
	int[] neighborAry = new int[9];
	
	public Image(String imgFilePath, String templateFilePath) throws FileNotFoundException, IllegalArgumentException {
		//check for wrong file types
		if(!imgFilePath.endsWith("txt")) throw new IllegalArgumentException("Not a text file...");
		if(!templateFilePath.endsWith("txt")) throw new IllegalArgumentException("Not a text file...");
		
		//get files and load data
		File img = new File(imgFilePath);
		File template = new File(templateFilePath);
		loadImage(img);
		loadTemplate(template);
	}//constructor
	
	private void loadImage(File imgFile) throws FileNotFoundException {
		Scanner inputReader = new Scanner(new FileReader(imgFile));
		
		//get header values
		numRows = inputReader.nextInt();
		numCols = inputReader.nextInt();
		minVal = inputReader.nextInt();
		maxVal = inputReader.nextInt();
		
		//allocate all 2D img arrays
		imgAry = new int[numRows][numCols];
		medianAry = new int[numRows][numCols];
		gaussAry = new int[numRows][numCols];
		
		//load img data into imgArray, L->R, T->B
		int i = 0;
		int j = 0;
		while(inputReader.hasNextInt()) {
			if(i == numRows) {
				i = 0;
				j++;
			}//if
			if(j == numCols) {
				System.out.println("Shit might be messed up... Check loadImage().");
			}//if
			
			imgAry[i][j] = inputReader.nextInt();
			//TODO: might load all this info into medianAry and gaussAry
			i++;
		}//while
	inputReader.close();
	}//loadImage
	
	private void loadTemplate(File templateFile) throws FileNotFoundException {
		Scanner inputReader = new Scanner(new FileReader(templateFile));
		
		//get header values
		templateRows = inputReader.nextInt();
		templateCols = inputReader.nextInt();
		
		//allocate 2D template array
		gaussTemplate = new int[templateRows][templateCols];
		
		//load template data into gaussTemplate, L->R, T->B
		int i = 0;
		int j = 0;
		while(inputReader.hasNextInt()) {
			if(i == templateRows) {
				i = 0;
				j++;
			}//if
			if(j == templateCols) {
				System.out.println("Shit might be messed up... Check loadTemplate().");
			}//if
			
			gaussTemplate[i][j] = inputReader.nextInt();
			i++;
		}//while
		inputReader.close();
	}//loadTemplate
	
	public void medianFilter() {
		newMin = maxVal;
		newMax = minVal;
		
		//process imgAry using a median filter
		for(int i = 1; i <= numRows -2 ; i++) {
			for(int j = 1; j <= numCols - 2; j++) {
				loadNeighbors(i,j);
				selectionSort5x(neighborAry);
				medianAry[i][j] = neighborAry[4];
				
				//get newMin and newMax
				if(neighborAry[4] < newMin) newMin = neighborAry[4];
				if(neighborAry[4] > newMax) newMax = neighborAry[4];
			}//for
		}//for
	}//medianFilter

	private void selectionSort5x(int[] arr) {
		int currMin = arr[0];
		//run selection sort only 5x because we only need the 5th smallest number
		for(int p = 0; p < 5; p++){
			for(int i = p + 1; i < arr.length; i++) {
				if(arr[i] < currMin) {
					currMin = arr[i];
					int temp = arr[p];
					arr[p] = arr[i];
					arr[i] = temp;
				}
				p++;
			}//for
		}//for
	}//selectionSort

	private void loadNeighbors(int i, int j) {
		neighborAry[0] = imgAry[i-1][j-1];
		neighborAry[1] = imgAry[i][j-1];
		neighborAry[2] = imgAry[i+1][j-1];
		neighborAry[3] = imgAry[i-1][j];
		neighborAry[4] = imgAry[i][j];
		neighborAry[5] = imgAry[i+1][j];
		neighborAry[6] = imgAry[i-1][j+1];
		neighborAry[7] = imgAry[i][j+1];
		neighborAry[8] = imgAry[i+1][j+1];
	}//loadNeighbors
	
	public void gaussianFilter() {
		newMin = maxVal;
		newMax = minVal;
		totalWeight = computeTemplateWeight();									
		
		//process imgAry with a gaussian filter from L->R, T->B
		for(int i = 2; i <= numRows - 3; i++) {
			for(int j = 3; j <= numCols - 3; j++) {
				int c = convolution(i, j);												
				gaussAry[i][j] = (int)(c / totalWeight);
				if(gaussAry[i][j] < newMin) newMin = gaussAry[i][j];
				if(gaussAry[i][j] > newMax) newMax = gaussAry[i][j];
			}//for
		}//for
	}//gausssianFilter

	private int convolution(int i, int j) {
		int result = 0;
		int iOffset = (int)(i - (templateRows / 2));										
		int jOffset = (int)(j - (templateCols / 2));									
		
		for(int m = 0; m < templateRows; m++) {
			for(int n = 0; n < templateCols; n++) {
				result += imgAry[iOffset + m][jOffset + n] * gaussTemplate[m][n];			
			}//for
		}//for
		
		return result;
	}//convolution

	private int computeTemplateWeight() {
		int sum = 0;
		for(int i = 0; i < templateRows; i++) {
			for( int j = 0; j < templateCols; j++) {
				 sum += gaussTemplate[i][j];
			}//for
		}//for
		return sum;
	}//computeTemplateWeight

	public void printMedianOutput(String outFile) throws FileNotFoundException {
		File file = new File(outFile);
		PrintWriter outputStream = new PrintWriter(file);
		outputStream.println(numRows + " " + numCols  + " " + newMin  + " " + newMax);
		for(int i = 0; i < numRows; i++) {
			for(int j = 0; j < numCols; j++) {
				outputStream.print(medianAry[i][j]);
				if(j!= numCols - 1)outputStream.print(" ");
			}//for
			outputStream.println();
		}//for
		outputStream.close();
	}//printMedianOutput
	
	public void printGaussOutput(String outFile) throws FileNotFoundException {
		File file = new File(outFile);
		PrintWriter outputStream = new PrintWriter(file);
		outputStream.println(numRows + " " + numCols  + " " + newMin  + " " + newMax);
		for(int i = 0; i < numRows; i++) {
			for(int j = 0; j < numCols; j++) {
				outputStream.print(gaussAry[i][j]);
				if(j!= numCols - 1)outputStream.print(" ");
			}//for
			outputStream.println();
		}//for
		outputStream.close();
	}//printOutput
}//class
