package eu.fbk.factExtractor;

import java.io.File;

import eu.fbk.utilities.FileManager;

public class FactExtractor {
	
	private ExtractFacts extractFacts;
	
	public FactExtractor (String baseFileDir){
		extractFacts = new ExtractFacts();
	}

	public static void main(String[] args) {
		
		boolean correctArguments = true;
		FileManager fileFinder = new FileManager();
		if (args.length == 2 ){
			if ( !fileFinder.isDirectory(args[0]) || !fileFinder.isDirectory(args[1]) )
				correctArguments = false;
		}
		else if (args.length == 1){
			if ("-version".equalsIgnoreCase(args[0]) || "-v".equalsIgnoreCase(args[0])){
				System.out.println("FactExtractor Version 1.2 \n August 04, 2009");
				//Bug fix - name space elimination
				//System.out.println("FactExtractor Version 1.1 \n July 08, 2009");
				//Adds attribute type information
				//System.out.println("FactExtractor Version 1.0 \n February 09, 2009");
				System.exit(0);
			}
			else
				correctArguments = false;
		}
		else
			correctArguments = false;
		
		if (!correctArguments){
			System.err.println("\nUsage: java -jar FactExtractor.jar xml_files_home_direcotory output_direcory\n" );
			System.exit(-1);
		}
		
		String fileDir;
		String outPutFileDir;
		
		if (args[0].lastIndexOf(File.separatorChar) == (args[0].length()-1))
			fileDir = args[0].substring(0, args[0].lastIndexOf(File.separatorChar));
		else
			fileDir = args[0];
		
		if (args[1].lastIndexOf(File.separatorChar) == (args[1].length()-1))
			outPutFileDir = args[1].substring(0, args[1].lastIndexOf(File.separatorChar));
		else
			outPutFileDir = args[1];
		
		FactExtractor factExtractor = new FactExtractor(fileDir);
		factExtractor.extractFacts.initXMLTag();
		factExtractor.getFiles(fileDir);
		factExtractor.extractFacts.closeXMLTag();
		fileFinder.writeToDisk(factExtractor.extractFacts.getFactClasses().toString(), outPutFileDir, "factFile.xml");

	}
	
	public void getFiles (String fileDir){
		
		FileManager fileFinder = new FileManager();
		String[] filesList = fileFinder.getSortedFileList(fileDir);
		String filePath;
		
		for (int i=0; i<filesList.length; i++){
			filePath = fileDir + File.separatorChar + filesList[i];
			
			if (fileFinder.isFile(filePath) && filesList[i].toLowerCase().endsWith(".h.xml"))
				extractFacts.getClassFacts(filePath);
			else if (fileFinder.isDirectory(filePath))
				getFiles(filePath);
		}
	}

}
