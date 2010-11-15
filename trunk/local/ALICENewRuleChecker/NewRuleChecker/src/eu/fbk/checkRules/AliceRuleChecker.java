package eu.fbk.checkRules;

import eu.fbk.utilities.FileManager;
import eu.fbk.utilities.GenerateDummyFile;
import eu.fbk.utilities.ReplaceMacros;
import eu.fbk.utilities.SrcMLRoot;
import eu.fbk.utilities.XmlCheckAndFix;
import nu.xom.Element;

public class AliceRuleChecker {

	public static void main(String[] args) {
		
		if (args.length != 4) {
			if (args.length == 1){
				if ("-version".equalsIgnoreCase(args[0]) || "-v".equalsIgnoreCase(args[0])){
					System.out.println("NewAliceRuleChecker Version 1.4.01 \n August 04, 2009");
					//System.out.println("NewAliceRuleChecker Version 1.4 \n July 08, 2009");
					//System.out.println("NewAliceRuleChecker Version 1.2 \n February 25, 2009");
					//System.out.println("NewAliceRuleChecker Version 1.3 \n March 12, 2009");
					//System.out.println("NewAliceRuleChecker Version 1.3.01 \n March 20, 2009");
					//System.out.println("NewAliceRuleChecker Version 1.3.02 \n June 13, 2009");
					System.exit(0);
				}
				else {
					System.err.println("\nUsage: java -jar NewRuleChecker.jar file.cxx.xml file.h.xml factFile.xml AliceCodingConventions.xml\n" );
					System.exit(-1);
				}
			}
			else {
				System.err.println("\nUsage: java -jar NewRuleChecker.jar file.cxx.xml file.h.xml factFile.xml AliceCodingConventions.xml\n" );
				System.exit(-1);
			}
		}
		String cxxFile = args[0];
		String hFile = args[1];
		String factFile = args[2];
		String configFile = args[3];
		Element cxxRootElt;
		Element hRootElt;
		String hFileName;
		String cxxFileName;
		GenerateDummyFile dummy;
		SrcMLRoot getXmlRoot = new SrcMLRoot();
		CheckNamingRules namingRulesVerifier = new CheckNamingRules(configFile);	
		CheckCodingRules codingRulesVerifier = new CheckCodingRules(configFile);
		CheckGuidelines guidelinesVerifier = new CheckGuidelines(configFile); 
		CheckStyleRules styleRulesVerifier = new CheckStyleRules(configFile);
		
		FileManager checkDirectory = new FileManager();
		
		if (!checkDirectory.isFile(factFile) || !checkDirectory.isFile(configFile) 
				|| (!checkDirectory.isFile(factFile) && !checkDirectory.isFile(configFile))){
			System.err.println("Usage: java -jar NewRuleChecker.jar file.cxx.xml file.h.xml factFile.xml AliceCodingConventions.xml" );
			System.exit(-1);
		}
		
		ReplaceMacros fileWithMacro;
		if (!checkDirectory.isFile(cxxFile) && checkDirectory.isFile(hFile)) {
			hRootElt = getXmlRoot.getXmlFileRootElt(hFile, true);
			fileWithMacro = new ReplaceMacros(hRootElt);
			hRootElt = fileWithMacro.checkHeaderFileAndMergMacro();
			hFileName = hRootElt.getAttributeValue("filename");
			dummy = new GenerateDummyFile(".cxx", hFileName.substring(0, hFileName.lastIndexOf(".")), "");
			cxxRootElt = dummy.getDummyRootElement();
			cxxFileName = cxxRootElt.getAttributeValue("filename");
		}else if (checkDirectory.isFile(cxxFile) && !checkDirectory.isFile(hFile)) {
			cxxRootElt = getXmlRoot.getXmlFileRootElt(cxxFile, true);
			cxxFileName = cxxRootElt.getAttributeValue("filename");
			dummy = new GenerateDummyFile(".h", cxxFileName.substring(0, cxxFileName.lastIndexOf(".")), "");
			hRootElt = dummy.getDummyRootElement();
			hFileName = hRootElt.getAttributeValue("filename");
		}else {
			hRootElt = getXmlRoot.getXmlFileRootElt(hFile, true);
			fileWithMacro = new ReplaceMacros(hRootElt);
			hRootElt = fileWithMacro.checkHeaderFileAndMergMacro();
			hFileName = hRootElt.getAttributeValue("filename");
			cxxRootElt = getXmlRoot.getXmlFileRootElt(cxxFile, true);
			cxxFileName = cxxRootElt.getAttributeValue("filename");
		}

		if (!hFileName.endsWith(".h")) {
			System.err.println("Usage: java -jar NewRuleChecker.jar file.cxx.xml file.h.xml factFile.xml AliceCodingConventions.xml\n" +
							   "		xml version of the header file \"file.h.xml\" should be the second argument." );
			System.exit(-1);
		}
		
		int lastDotIndex = cxxFileName.lastIndexOf('.');
		if (lastDotIndex == -1 ){
			System.err.println("Usage: java -jar NewRuleChecker.jar file.cxx.xml file.h.xml factFile.xml AliceCodingConventions.xml\n" +
			   				   "		\"file.cxx.xml\" file must be srcML converted version of \"file.cxx\"." );
			System.exit(-1);
		}
		
		if (!cxxFileName.substring(lastDotIndex + 1, lastDotIndex + 2).equalsIgnoreCase("c")){
			System.err.println("Usage: java -jar NewRuleChecker.jar file.cxx.xml file.h.xml factFile.xml AliceCodingConventions.xml\n" +
							   "		xml version of the implementation file \"file.cxx.xml\" should be the first argument." );
			System.exit(-1);
		}
		
		XmlCheckAndFix checkXml = new XmlCheckAndFix();
		
		hRootElt = checkXml.checkAndFix(hRootElt, false);
		cxxRootElt = checkXml.checkAndFix(cxxRootElt, false);
		//System.err.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ----Begin (also cxx file)----- " + args[1]);
		namingRulesVerifier.setXmlFileRoot(hRootElt);	
		namingRulesVerifier.setActualFileName(hRootElt);
		namingRulesVerifier.verifyNamingConventions("HeaderFile");
		
		namingRulesVerifier.setXmlFileRoot(cxxRootElt);
		namingRulesVerifier.setActualFileName(cxxRootElt);
		namingRulesVerifier.verifyNamingConventions("ImplmentationFile");
		
		namingRulesVerifier.printViolationMessage("Naming Rules");
		
		codingRulesVerifier.setXmlFileRoot(hRootElt);
		codingRulesVerifier.setActualFileName(hRootElt);
		codingRulesVerifier.verifyCodingConventions("HeaderFile", factFile);
		
		codingRulesVerifier.setXmlFileRoot(cxxRootElt);
		codingRulesVerifier.setActualFileName(cxxRootElt);
		codingRulesVerifier.verifyCodingConventions("ImplmentationFile", factFile);
		
		codingRulesVerifier.printViolationMessage("Coding Rules");
		
		guidelinesVerifier.setXmlFileRoot(hRootElt);
		guidelinesVerifier.setActualFileName(hRootElt);
		guidelinesVerifier.verifyGuidelineConventions("HeaderFile");
		
		guidelinesVerifier.setXmlFileRoot(cxxRootElt);
		guidelinesVerifier.setActualFileName(cxxRootElt);
		guidelinesVerifier.verifyGuidelineConventions("ImplmentationFile");	
		
		guidelinesVerifier.printViolationMessage("Guidelines");
		
		styleRulesVerifier.setXmlFileRoot(hRootElt);
		styleRulesVerifier.setActualFileName(hRootElt);
		styleRulesVerifier.verifyStyleRules("HeaderFile");
		
		styleRulesVerifier.setXmlFileRoot(cxxRootElt);
		styleRulesVerifier.setActualFileName(cxxRootElt);
		styleRulesVerifier.verifyStyleRules("ImplmentationFile");	
		
		styleRulesVerifier.printViolationMessage("Style Rules");
		//System.err.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ----End (also cxx file)----- " + args[1]);
		
	}
	
}
