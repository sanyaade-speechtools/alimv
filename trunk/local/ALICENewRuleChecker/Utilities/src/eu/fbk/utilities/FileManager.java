package eu.fbk.utilities;


import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;

import nu.xom.Document;
import nu.xom.Element;
import nu.xom.Node;
import nu.xom.Serializer;
import nu.xom.UnavailableCharacterException;

public class FileManager {

	/**
	 * This method returns sorted list of files and folders found under folderName 
	 * 
	 * @param folderName Contains name of a folder with its path
	 * @return sorted list of files and folders found under folderName or null
	 */
	public String[] getSortedFileList (String folderName){
		
		String[] fileList = new File(folderName).list();

		if (fileList != null)
			java.util.Arrays.sort(fileList);
		
		return fileList;
	}
	
	/**
	 * This method tells whether value of filePath is a file or not
	 * 
	 * @param filePath Contains path of a file or a folder
	 * @return true if filePath is a file otherwise false
	 */
	public boolean isFile (String filePath){
		
		if (new File (filePath).isFile())
			return true;
		else
			return false;
	}
	
	/**
	 * This method tells whether value of filePath is a directory or not
	 * 
	 * @param filePath Contains path of a file or a folder
	 * @return true if filePath is a directory otherwise false
	 */
	public boolean isDirectory (String filePath){
		
		if (new File (filePath).isDirectory())
			return true;
		else
			return false;
	}
	
	/**
	 * This method writes the specified content to a file named fileName under the specified directory
	 * 
	 * @param content Contains the content to be written
	 * @param directory Contains the directory in which the file has to be created/overwrite
	 * @param fileName Contains the name of the file in which the content is to be stored
	 */
	public void writeToDisk( String content, String directory, String fileName){
		
		String filePath = directory + File.separatorChar + fileName;
		PrintWriter textWriter = openTextWriter (filePath, false);
		if (textWriter != null){
			textWriter.println(content);
			textWriter.flush();
			textWriter.close();
		}
		else
			System.out.println("Unable to create a file \"" + filePath + "\" for writing ");
		
	}
	
	/**
	 * This method appends the specified content to a file named, fileName, under the specified directory
	 * 
	 * @param content Contains the content to be appended
	 * @param directory Contains the directory in which the file has to be created/overwrite
	 * @param fileName Contains the name of the file in which the content is to be stored
	 */
	public void appendToFile(String content, String directory, String fileName){
		
		String filePath = directory + File.separatorChar + fileName;
		PrintWriter textWriter = openTextWriter (filePath, true);
		if (textWriter != null){
			textWriter.println(content);
			textWriter.flush();
			textWriter.close();
		}
		else
			System.out.println("Unable to create a file \"" + filePath + "\" for writing ");
	}

	
	
	/**
	 * This method writes the specified content to the given file specified by filePath
	 * @param content Contains the content to be written
	 * @param filePath Contains path to a specific file
	 */
	public void writeToDisk( String content, String filePath){
		
		PrintWriter textWriter = openTextWriter (filePath, false);
		if (textWriter != null){
			textWriter.println(content);
			textWriter.flush();
			textWriter.close();
		}
		else
			System.out.println("Unable to create a file \"" + filePath + "\" for writing ");
	}
	
	/**
	 * This method appends the specified content to the given file specified by filePath
	 * 
	 * @param content Contains the content to be appended
	 * @param filePath Contains path to a specific file
	 */
	public void appendToFile( String content, String filePath){
		
		PrintWriter textWriter = openTextWriter (filePath, true);
		if (textWriter != null){
			textWriter.println(content);
			textWriter.flush();
			textWriter.close();
		}
		else
			System.out.println("Unable to create a file \"" + filePath + "\" for writing ");
	}
	
	/**
	 * Writes the content of the root element to a file using XOM's Serializer
	 * 
	 * @param xmlFileRoot contains root element of the xml file to be written on the disk
	 * @param directory Contains the directory in which the file has to be created/overwrite
	 * @param fileName Contains the name of the file in which the content is to be stored
	 */
	public void writeToDisk( Element xmlFileRoot, String directory, String fileName){
		
		String filePath = directory + File.separatorChar + fileName;
		Document doc;

		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filePath);
			Serializer serializer = new Serializer ( fileOutputStream );
			if (((Node)xmlFileRoot).getDocument() == null){
				doc = new Document(xmlFileRoot);
				serializer.write(doc);
			}
			else
				serializer.write(((Node)xmlFileRoot).getDocument());
		} catch (FileNotFoundException e) {
			System.err.println( filePath + "  is not a regular file, it can not be created.");
			e.printStackTrace();
		} catch (SecurityException e){
			System.err.println(filePath + " is being used by another application or you don't have permission to modifiy it.");
			e.printStackTrace();
		} catch (UnavailableCharacterException e){
			System.err.println("Some character(s) cant be represented in UTF-8");
			e.printStackTrace();
		} catch (UnsupportedEncodingException e){
			System.err.println("The java virtual machine does not support UTF-8");
			e.printStackTrace();
		} catch (IOException e) {
			System.err.println("XML file write error.");
			e.printStackTrace();
		}
			
	}
	
	/**
	 * Writes the content of the root element to a file using XOM's Serializer
	 * 
	 * @param xmlFileRoot contains root element of the xml file to be written on the disk
	 * @param filePath Contains path to a specific file
	 */
	public void writeToDisk( Element xmlFileRoot, String filePath){
		
		Document doc;
		try {
			FileOutputStream fileOutputStream = new FileOutputStream(filePath);
			Serializer serializer = new Serializer ( fileOutputStream );
			if (((Node)xmlFileRoot).getDocument() == null){
				doc = new Document(xmlFileRoot);
				serializer.write(doc);
			}
			else
				serializer.write(((Node)xmlFileRoot).getDocument());
		} catch (FileNotFoundException e) {
			System.err.println( filePath + "  is not a regular file, it can not be created.");
			e.printStackTrace();
		} catch (SecurityException e){
			System.err.println(filePath + " is being used by another application or you don't have permission to modifiy it.");
			e.printStackTrace();
		} catch (UnavailableCharacterException e){
			System.err.println("Some character(s) cant be represented in UTF-8");
			e.printStackTrace();
		} catch (UnsupportedEncodingException e){
			System.err.println("The java virtual machine does not support UTF-8");
			e.printStackTrace();
		} catch (IOException e) {
			System.err.println("XML file write error.");
			e.printStackTrace();
		}
			
	}
	
	
	/**
	 * This method creates a print writer object for writing a text
	 * 
	 * @param filePath Contains name and path of the file to be created/overwritten for writing 
	 * @return PrintWriter object if file created/overwritten successfully otherwise null
	 */
//	private PrintWriter openTextWriter (String filePath){
//		
//		try{
//			File outPutFile = new File(filePath);
//			PrintWriter textWriter = new PrintWriter(
//					new BufferedWriter(
//							new FileWriter(outPutFile)));
//			return  textWriter;
//		}catch (IOException e){
//			System.err.println("Error while opening file.");
//			System.exit(-1);
//		}
//		
//		return null;
//	}
	
	/**
	 * This method creates a print writer object for writing a text
	 * 
	 * @param filePath Contains name and path of the file to be created/overwritten for writing 
	 * @return PrintWriter object if file created/overwritten successfully otherwise null
	 */
	private PrintWriter openTextWriter (String filePath, Boolean append){
		
		try{
			File outPutFile = new File(filePath);
			PrintWriter textWriter = new PrintWriter(
					new BufferedWriter(
							new FileWriter(outPutFile, append)));
			return  textWriter;
		}catch (IOException e){
			System.err.println("Error while opening file " + filePath);
			System.exit(-1);
		}
		
		return null;
	}
	
	
//	public String readLineFromFile(String filePath){
//		String lineText ="";
//		
//		return lineText;
//	}
	
	/**
	 * This method creates a folder which is at the end of the fileDestination path, if it doesn't exist
	 * @param fileDestinationDir contains the full path with the new folder to be created
	 * @return true upon success, false otherwise
	 */
	public boolean createFolder(String fileDestinationDir) {
		
		if (!fileDestinationDir.equals("") && !this.isDirectory(fileDestinationDir)){
			File destinationFolderInOld = new File(fileDestinationDir);
			if (!destinationFolderInOld.mkdir() && !destinationFolderInOld.exists()){
				return false;
				//System.err.println("Unable to create output folder " + fileDestinationDir);
				//System.exit(-1);
			}
		}
		return true;
	}

}
