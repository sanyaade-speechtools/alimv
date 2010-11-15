package eu.fbk.utilities;

import java.io.IOException;

import nu.xom.Builder;
import nu.xom.Document;
import nu.xom.Element;
import nu.xom.ParsingException;

public class SrcMLRoot {

	/**
     * Returns the root element of the xml file fileName
     * 
     *  @param fileName Name of a file
     *  @param exitOnError if true, it terminates the execution while error
     *  is encountered
     *  @return root element of the xml file, fileName
     */
	public Element getXmlFileRootElt (String fileName, boolean exitOnError) {
		Element rootElt = null;
		try {
			Builder builder = new Builder();
			Document doc = builder.build(fileName);
			rootElt = doc.getRootElement();
		}catch (ParsingException parsingEx){
			System.err.println(fileName + " is not well-formed");
			if (exitOnError)
				System.exit(-1);
			else
				return null;
		}catch (IOException ioEx){
			System.err.println("File not found: " + fileName);
			if (exitOnError)
				System.exit(-1);
			else
				return null;
		}
		return rootElt;
	}
	
	
}
