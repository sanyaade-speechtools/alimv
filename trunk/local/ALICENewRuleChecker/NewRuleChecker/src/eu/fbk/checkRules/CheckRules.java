package eu.fbk.checkRules;

import eu.fbk.utilities.SrcMLRoot;

import nu.xom.Attribute;
import nu.xom.Element;
import nu.xom.Node;
import nu.xom.Nodes;
import nu.xom.XPathContext;

public class CheckRules {
	
	private StringBuffer violationMessage = new StringBuffer();
	private Nodes rulesToCheck;	
	protected Nodes forbiddenStrings;
	private String ruleName;
	private String ruleContent;
	private String xmlPrefix = "tmpPrefix";
	
	XPathContext context = new XPathContext (xmlPrefix, "http://www.sdml.info/srcML/src");	
	Element xmlFileRoot;
	String actualFileName = "";
	
	//Common for XPath queries
	final String nameXPath = xmlPrefix + ":name";		
	final String typeXPath = xmlPrefix + ":type";
	
	//relative from root
	final String classXPath = xmlPrefix + ":class";
	
	//from root element
	final String embeddedClassXPath = xmlPrefix + ":class//" + xmlPrefix + ":class";
	
	//relative from function
	final String functionCallXPath = "descendant::" + xmlPrefix + ":call|" + 
									 "descendant::" + xmlPrefix + ":constructor_decl";
	final String returnExprXPath = xmlPrefix + ":return/" + xmlPrefix + ":expr";

	//relative from function call
	final String functionCallArgumentsXPath = "descendant::" + xmlPrefix + ":argument_list/" + xmlPrefix + ":argument/" + xmlPrefix + ":expr";
	final String castTypeNameXPath = xmlPrefix + ":argument_list/" + xmlPrefix + ":argument/" + xmlPrefix + ":name";
	//Relative from root
	final String functionImplementationXPath = "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":public/"+ xmlPrefix + ":function|" + 
											   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+ xmlPrefix + ":function|" +
											   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":private/"+ xmlPrefix + ":function|" +
											   xmlPrefix + ":function|" +
											   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":public/"+xmlPrefix + ":constructor|" +
											   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+xmlPrefix + ":constructor|" +
											   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":private/"+xmlPrefix + ":constructor|" +
											   xmlPrefix + ":constructor|" +
											   xmlPrefix + ":destructor|" +
											   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":public/"+xmlPrefix + ":destructor|" +
											   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+xmlPrefix + ":destructor|" +
											   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":private/"+xmlPrefix + ":destructor";
	//Relative from class element
	final String functionImplementationInClassXPath = xmlPrefix + ":block/"+ xmlPrefix + ":public/"+ xmlPrefix + ":function|" + 
	   										   xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+ xmlPrefix + ":function|" +
	   										   xmlPrefix + ":block/"+ xmlPrefix + ":private/"+ xmlPrefix + ":function|" +
	   										   xmlPrefix + ":block/"+ xmlPrefix + ":public/"+xmlPrefix + ":constructor|" +
	   										   xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+xmlPrefix + ":constructor|" +
	   										   xmlPrefix + ":block/"+ xmlPrefix + ":private/"+xmlPrefix + ":constructor|" +
	   										   xmlPrefix + ":block/"+ xmlPrefix + ":public/"+xmlPrefix + ":destructor|" +
	   										   xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+xmlPrefix + ":destructor|" +
	   										   xmlPrefix + ":block/"+ xmlPrefix + ":private/"+xmlPrefix + ":destructor";
	//Relative to root element for implementation files
	final String methodInImplementationFileXPath = xmlPrefix + ":function|" +
												   xmlPrefix + ":constructor|" +
												   xmlPrefix + ":destructor";
	
	//Relative from root element
	final String methodXPath = xmlPrefix + ":function|" +
							   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":public/"+ xmlPrefix + ":function|" + 
							   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+ xmlPrefix + ":function|" +
							   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":private/"+ xmlPrefix + ":function|" +
							   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":public/"+ xmlPrefix + ":function_decl|" + 
							   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+ xmlPrefix + ":function_decl|" +
							   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":private/"+ xmlPrefix + ":function_decl";
	final String destructorXPath = "descendant::" + xmlPrefix + ":destructor|" +
								   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":public/"+xmlPrefix + ":destructor|" +
								   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+xmlPrefix + ":destructor|" +
								   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":private/"+xmlPrefix + ":destructor|" +
								   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":public/"+xmlPrefix + ":destructor_decl|" +
								   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+xmlPrefix + ":destructor_decl|" +
								   "descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":private/"+xmlPrefix + ":destructor_decl";
	final String constructorXPath = "descendant::"+ xmlPrefix + ":constructor|" +
									"descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":public/"+xmlPrefix + ":constructor|" +
									"descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+xmlPrefix + ":constructor|" +
									"descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":private/"+xmlPrefix + ":constructor|" +
									"descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":public/"+xmlPrefix + ":constructor_decl|" +
									"descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+xmlPrefix + ":constructor_decl|" +
									"descendant::" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":private/"+xmlPrefix + ":constructor_decl";
		
	//Relative from class element
	final String attributeXPath = "descendant::" + xmlPrefix + ":block/"+ xmlPrefix + ":public/"+ xmlPrefix + ":decl_stmt/"+ xmlPrefix + ":decl|" +
								  "descendant::" + xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+ xmlPrefix + ":decl_stmt/"+ xmlPrefix + ":decl|" +
								  "descendant::" + xmlPrefix + ":block/"+ xmlPrefix + ":private/"+ xmlPrefix + ":decl_stmt/"+ xmlPrefix + ":decl";
	
	//Relative from member functions
	final String localVariableXPath = "descendant::"+ xmlPrefix + ":block/"+ xmlPrefix + ":decl_stmt/"+ xmlPrefix + ":decl";

	//Relative from member functions
	final String parameterXPath = xmlPrefix + ":parameter_list/"+ xmlPrefix + ":param/" + xmlPrefix + ":decl";
	
	//Relative from root
	final String enumXPath = "descendant::" + xmlPrefix + ":enum/" + xmlPrefix + ":block/" + xmlPrefix + ":expr";
	
	//Relative from root
	final String variableDeclXPath = xmlPrefix + ":decl_stmt/" + xmlPrefix + ":decl";
	
	//Relative from root
	final String defineXPath = "cpp:define";
	
	//Relative from root
	final String classBlockXPath = "descendant::"+ xmlPrefix + ":class/" + xmlPrefix + ":block"; 
	
	//Relative from function
	final String functionBodyExprXPath = xmlPrefix + ":block//"+ xmlPrefix + ":expr_stmt/" + xmlPrefix + ":expr|" +
										 xmlPrefix + ":block//"+ xmlPrefix + ":incr/" + xmlPrefix + ":expr|" +
										 xmlPrefix + ":block//"+ xmlPrefix + ":init/" + xmlPrefix + ":expr|" +
										 xmlPrefix + ":block//"+ xmlPrefix + ":condition/" + xmlPrefix + ":expr";
	//relative from root
	final String structXPath = xmlPrefix + ":struct";
	final String dummyArgumentXPath = "//" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":public/"+xmlPrefix +  ":constructor_decl/" + xmlPrefix + ":parameter_list/" + xmlPrefix + ":param|"+
									  "//" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+xmlPrefix +  ":constructor_decl/" + xmlPrefix + ":parameter_list/" + xmlPrefix + ":param|"+
									  "//" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":private/"+xmlPrefix +  ":constructor_decl/" + xmlPrefix + ":parameter_list/" + xmlPrefix + ":param|"+
									  "//" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":public/"+xmlPrefix + ":function_decl/" + xmlPrefix + ":parameter_list/" + xmlPrefix + ":param|"+
									  "//" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+xmlPrefix + ":function_decl/" + xmlPrefix + ":parameter_list/" + xmlPrefix + ":param|"+
									  "//" + xmlPrefix + ":class/" + xmlPrefix + ":block/"+ xmlPrefix + ":private/"+xmlPrefix + ":function_decl/" + xmlPrefix + ":parameter_list/" + xmlPrefix + ":param";

	//relative from class
	final String superClassXPath = xmlPrefix + ":super";
	
	final String specifierXPath = xmlPrefix + ":specifier";
	//relative from decl_stmt/decl or param/decl
	final String templateUseXPath = xmlPrefix + ":type/" + xmlPrefix + ":name/" + xmlPrefix + ":argument_list";
	//relative from root
	final String templateXPath = "descendant::" + xmlPrefix + ":template";
	final String exceptionTryXPath = "descendant::" + xmlPrefix + ":try";
	final String includeFileXPath = "cpp:include/cpp:file";
	final String declarationTypeXPath = "descendant::" + xmlPrefix + ":decl_stmt/" + xmlPrefix + ":decl/" + xmlPrefix + ":type|"+
									"descendant::" + xmlPrefix + ":parameter_list/" + xmlPrefix + ":param/" + xmlPrefix + ":decl/" + xmlPrefix + ":type";
	final String exprOnlyXPath = "descendant::" + xmlPrefix + ":expr";
	final String ifConditionXPath = "descendant::" + xmlPrefix + ":if/" + xmlPrefix + ":condition";
	
	//For the fact file
	final static String factClassXPath = "class";
	final static String factSuperClassXPath = "superClasses";
	final static String factAttributeXPath = "attributes";
	final static String factSignatureXPath = "signatures/constructor_decl|signatures/destructor_decl|signatures/function_decl";
	final static String factFunctionDeclXPath = "signatures/function_decl";
	final static String factFunctionParameterTypeXPath = "parameter_list/param/decl/type";
	final static String factParameterTypeNameXPath = "decl/type/name";//relative from parameter (param)
	
	final static String factDummyArgumentXPath = "//constructor_decl/parameter_list/param|"+
	  											 "//function_decl/parameter_list/param";
	final static String factFunctionParameterNameXPath = "parameter_list/param/decl/name";
	final static String factNameXPath = "name";
	
	public CheckRules(String ruleCategory, String configFile){
		
		context.addNamespace("cpp", "http://www.sdml.info/srcML/cpp");
		SrcMLRoot getXmlRoot = new SrcMLRoot();
		Element rulesRootElt = getXmlRoot.getXmlFileRootElt(configFile, true);
		rulesToCheck = rulesRootElt.query("/AliceConfig/AliceCodingRules[@check='true']/Rules[@category='" + ruleCategory + "' and @check='true']/Rule[@check='true']");
		forbiddenStrings = rulesRootElt.query("/AliceConfig/ForbiddenStrings/String");
	}
	
	public CheckRules(){
		
	}
	
	/**
     * This method returns name of a class for the current header file, if found
     *  and is only one, otherwise >1 (more than one class in the header file)
     *  or <1 (no class in header file)
     * 
     * @return name of a class or <1 or >1
     */
	protected String getClassName(){
		Nodes classNameNodes = xmlFileRoot.query(classXPath + "/" + nameXPath, context);
		String className = "";
		
		int classNamesInHeaderFile = 0;
		int indexOfClassNameNode = 0;
		
		for (int i=0; i<classNameNodes.size(); i++) 			
			if (classNameNodes.get(i) instanceof Element){		
				if (!classNameNodes.get(i).getValue().contains("::")){
					classNamesInHeaderFile++;
					indexOfClassNameNode = i;
				}
			}
		
		if (classNamesInHeaderFile == 1 )			
			className = classNameNodes.get(indexOfClassNameNode).getValue().trim();
		else if (classNamesInHeaderFile == 0) 
			className = "<1";
		else
			className = ">1";
		
		return className;
    }
	
	/**
     * Appends the name of the rule that has been 
     * violated and its content with optional special message
     * violationMessage.
     * 
     * 	@param specialMessage Contains a more specific message about
     * 	the violation.
     * 
     */
    protected void setViolationMessage (String specialMessage) {
    	
    	violationMessage.append("RULE " + ruleName + " violated:\n");
    	violationMessage.append( "\n" + ruleContent + "\n\n");
    	if (specialMessage != null) {
    		violationMessage.append( "--> " + specialMessage + "\n");
    	}
    	violationMessage.append( "--------------------------------------------------\n\n");
    }
    
    
    public void printViolationMessage (String reportTitle){
    	
    	if (!violationMessage.toString().equals("")){
    		System.out.println("\n########### " + reportTitle + " Violation Report ######\n\n");
    		System.out.println(violationMessage.toString());
    	}
    		
    }
    
    /**
     * Sets the name of the rule and its content to the member variables
     * ruleName and ruleContent resp. if the rule number exists and configured
     * to be checked on the xml configuration file.
     *  
     *  @param ruleNumber Contains the rule number to which we need to
     *  set rule name and content
     *  @return true if the rule specified by ruleNumber is found in 
     *  rulesToCheck otherwise false
     */
    protected boolean setRuleNameAndContent (String ruleNumber ){
    	
    	boolean success = false;
    	Element ruleElt;
    	
    	for (int i=0; i< rulesToCheck.size(); i++){
    		if (rulesToCheck.get(i) instanceof Element) {
    			ruleElt = (Element) rulesToCheck.get(i);  			
    			if (ruleElt.getFirstChildElement("Name").getValue().equalsIgnoreCase(ruleNumber)){
    				ruleName = ruleElt.getFirstChildElement("Name").getValue();
    				ruleContent = ruleElt.getFirstChildElement("Description").getValue();
    				success = true;
    				break;
    			}
    				
    		}				
    	}   	
    	return success;
    }

    
    /**
     * This function returns line number of which "statement" occurs first in the
     * actual document
     *   
     * @param statement contains the string (statement) to be searched in the 
     * actual document
     * @return line number at which the first line of "statement" occurs first 
     * in the current file otherwise 0
     */
	protected int getLineNumber (String statement){
		
		int lineNumber=0;
		boolean matchFound = false;
		String actualCode = xmlFileRoot.getValue();
		int lineEnd;
		
		if (statement.contains("\n"))
			statement = statement.substring(0, statement.indexOf('\n'));

		int lineBegin = 0;
		if (actualCode.contains("\n"))
			lineEnd = actualCode.indexOf('\n');
		else
			lineEnd = actualCode.length()-1;
		
		while(lineEnd >= 0 && lineEnd < actualCode.length()){
			lineNumber = lineNumber+1;
			String codeSubContent = actualCode.substring(lineBegin, lineEnd);
			lineBegin = lineEnd +1;
			lineEnd = actualCode.indexOf('\n', lineBegin);
			
			if (codeSubContent.contains(statement)){
				matchFound = true;
				break;
			}
		}
		
		if (!matchFound)
			lineNumber = 0;
		
		return lineNumber;
	}
    
    
	/**
	 * Queries xPath and valueXPath from the relative root element, relativeRootElement.
	 * For each returned element of the query, it checks the value type required and 
	 * returns the corresponding names with the line number (in which they are found) using nodes.
	 *
	 * @param xPath Contains XPath of the element for which we want the value
	 * @param valueXPath Contains XPath of the value node
	 * @param relativeRootElement Contains the element from which XPath is queried
	 * @param valueType Contains the type of the value that we are looking for
	 * @param typeXPath Contains the XPath which gives us the type of the value
	 * @return Nodes containing the names as values and line number as attribute or an empty Nodes object
	 */
	protected Nodes extractElementNodes(String xPath, String valueXPath, Element relativeRootElement, String valueType, String typeXPath){
		
		Nodes typeElementNodes, valueElementNodes;
		Nodes elementNodes = new Nodes();
		Nodes returnedNodes = relativeRootElement.query(xPath, context);
		int typeIndex;
		
		for (int j=0; j<returnedNodes.size(); j++)
			if (returnedNodes.get(j) instanceof Element) {
				Element nameElement = (Element)returnedNodes.get(j);
				typeElementNodes = nameElement.query(typeXPath + "/" + valueXPath, context);
				valueElementNodes = nameElement.query(valueXPath, context);
				typeIndex = 0;
				
				if (typeElementNodes.size() > 1)
					if (typeElementNodes.get(typeIndex).getValue().equals("extern"))
						typeIndex = 1;
				
				if (valueType.equals("const")){		
					if (typeElementNodes.size() > 0)
						if (typeElementNodes.get(typeIndex).getValue().equals("const") )						
							collectNodes(valueElementNodes, elementNodes);
				}
				else if (valueType.equals("static")){		
					if (typeElementNodes.size() > 1)
						if ( ( typeElementNodes.get(typeIndex).getValue().equals("static") ) && !(typeElementNodes.get(typeIndex + 1).getValue().equals("const")) )						
							collectNodes(valueElementNodes, elementNodes);
				}
				else if (valueType.equals("staticConst")){		
					if (typeElementNodes.size() > 1)
						if ( (typeElementNodes.get(typeIndex).getValue().equals("static")) && (typeElementNodes.get(typeIndex + 1).getValue().equals("const")) )						
							collectNodes(valueElementNodes, elementNodes);
				}
				else if (valueType.equals("NoConstStatic")){	
					if (typeElementNodes.size() > 0)
						if ( !(typeElementNodes.get(typeIndex).getValue().equals("static") ) && !(typeElementNodes.get(typeIndex).getValue().equals("const")) 
								&& !(typeElementNodes.get(typeIndex).getValue().equals("friend")))		
							collectNodes(valueElementNodes, elementNodes);
				}
				else if (valueType.equals("NA")){
					collectNodes(valueElementNodes, elementNodes);
				}
			}	
		
		return elementNodes;
	}


	private void collectNodes(Nodes valueElementNodes, Nodes elementNodes) {
		String statement;
		for (int i=0; i<valueElementNodes.size(); i++){
			int firstSentenceLength;
			if (valueElementNodes.get(i).getParent().getValue().indexOf("\n")>0)
				firstSentenceLength = valueElementNodes.get(i).getParent().getValue().indexOf('\n');
			else
				firstSentenceLength = valueElementNodes.get(i).getParent().getValue().length();
			
			statement = valueElementNodes.get(i).getParent().getValue().substring(0,firstSentenceLength);

			Attribute lineNumber = new Attribute ("lineNumber", String.valueOf( getLineNumber(statement) ));
			Element nodeElement = ((Element)((Element)valueElementNodes.get(i)).copy());
			
			nodeElement.addAttribute(lineNumber);								
			elementNodes.append((Node)nodeElement);
		}
	}

	public void setXmlFileRoot(Element xmlFileRoot) {
		this.xmlFileRoot = xmlFileRoot;
	}

	public void setActualFileName(Element xmlFileRoot) {	
		this.actualFileName = xmlFileRoot.getAttributeValue("filename");
	}

}
