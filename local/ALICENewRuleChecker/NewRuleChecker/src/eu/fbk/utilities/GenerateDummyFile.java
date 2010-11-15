package eu.fbk.utilities;

import java.io.IOException;

import nu.xom.Builder;
import nu.xom.Document;
import nu.xom.Element;
import nu.xom.ParsingException;

public class GenerateDummyFile {
	
	private String fileType;
	private String className;
	private String uri;
	private String dummyFileContent;
	
	public GenerateDummyFile(String fileType, String className, String uri){
		this.fileType = fileType;
		//if (className.trim().startsWith("Ali"))
			this.className = className;
		//else 
		//	this.className = "Ali" + className;
		this.uri = uri;
		constructDummyContent();
	}
	
	private void constructDummyContent(){
		if (fileType.equals(".h"))
			dummyFileContent = 
				"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n" +
				"<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\""+ uri +"\" filename=\""+className+".h\"><cpp:ifndef>#<cpp:directive>ifndef</cpp:directive> <name>"+ className.toUpperCase()+"_H</name></cpp:ifndef>\n"+
				"<cpp:define>#<cpp:directive>define</cpp:directive> <name>"+ className.toUpperCase() + "_H</name></cpp:define>\n" +
				"<comment type=\"line\">// Void header file for the RuleChecker</comment>\n" +
				"<comment type=\"line\">// Void header file for the RuleChecker</comment>\n" +
				"<comment type=\"line\">// Void header file for the RuleChecker</comment>\n" +
				/*"<class>class <name>" + className + "</name>\n" +
				"<block>{<private type=\"default\">\n" +
				"</private>}</block>;</class>\n" +
				 */
				"<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>\n" +
				"</unit>";
		else
			dummyFileContent = 
				"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n" +
				"<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\""+ uri +"\" filename=\"" + className +".cxx\"><comment type=\"line\">// Dummy cxx file for RuleChecker</comment>\n" +
				"<comment type=\"line\">// Dummy cxx file for RuleChecker</comment>\n" +
				"<comment type=\"line\">// Dummy cxx file for RuleChecker</comment>\n" +
				"<comment type=\"line\">// Dummy cxx file for RuleChecker</comment>\n" +
				"<comment type=\"line\">// Dummy cxx file for RuleChecker</comment>\n" +
				"</unit>";
	}
	
	public Element getDummyRootElement(){
		Element dummyRootElement = null;
		Builder builder = new Builder();
		Document doc;
		try {
			doc = builder.build(dummyFileContent, uri);
			dummyRootElement = doc.getRootElement();
		} catch (ParsingException e) {
			System.err.println("Error in generating dummy file. It is not well-formed");
			System.exit(-1);
		} catch (IOException e) {
			System.err.println("Error in reading contents of dummy file.");
			System.exit(-1);
		}
		
		return dummyRootElement;
	}
	
	public static void main(String args[]){
		
		GenerateDummyFile dummyH =  new GenerateDummyFile(".h", "ITSOnlineCalibrationSPDhandler", "");
		GenerateDummyFile dummyCxx =  new GenerateDummyFile(".cxx", "ITSOnlineCalibrationSPDhandler", "");
		
		Element rootElt = dummyH.getDummyRootElement();
		System.out.println("H dummy file \n" + rootElt.getValue().toString());
		rootElt = dummyCxx.getDummyRootElement();
		System.out.println("\n\n Cxx dummy file \n" + rootElt.getValue().toString());
	}

}
