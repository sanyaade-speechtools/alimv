package eu.fbk.checkRules;


import nu.xom.Attribute;
import nu.xom.Element;
import nu.xom.Elements;
import nu.xom.Node;
import nu.xom.Nodes;
import nu.xom.Text;
import eu.fbk.rules.CodingRules;
import eu.fbk.utilities.FileManager;
import eu.fbk.utilities.SrcMLRoot;

public class CheckCodingRules extends CheckRules {

	private CodingRules satisfyCodingRule = new CodingRules();
	private String className;
	private Nodes embeddedClassNameNodes;
	private Element headerFileRoot;
	private String specialMessage;
	private Nodes superClassAttributeNodes = new Nodes();
	private Element factRootElement = null;

	public CheckCodingRules(String configFile) {
		super("CodingRules", configFile);
	}

	public void verifyCodingConventions(String fileType, String factFilePath) {

		FileManager checkDirectory = new FileManager();
		
		if (fileType.equals("HeaderFile")) {
			headerFileRoot = xmlFileRoot;
			className = getClassName();
			embeddedClassNameNodes = xmlFileRoot.query(embeddedClassXPath + "/"
					+ nameXPath, context);
			
			SrcMLRoot getXmlRoot = new SrcMLRoot();
			factRootElement = getXmlRoot.getXmlFileRootElt(factFilePath, true);
			if (!(className.equals("<1") || className.equals(">1")))
				if (checkDirectory.isFile(factFilePath)) 
					collectSuperClassAttributes(className);

			checkMultipleInclusionProtectionCR();
			checkClassNumberInHeaderFileCR();
			checkAccessSpecifiersCR();
			checkFriendClasses();
			checkPointerAttributes();
			checkPublicAttribute();
			checkRedefinedAttributes();
		} 
		else{
			checkMethodsInImplementationFileCR();
			checkDummyArgument();
		}
		checkGlobalVariables();
		checkDestructorFunction();
		checkAttributeModificationInFunction();
		checkStruct();
		checkPtrAndRefParameters();
		checkForbiddenStrings();
		checkFloatingEquality();
	}

	private void checkMultipleInclusionProtectionCR() {

		if (this.setRuleNameAndContent("RC3")) {
			if (!(className.equals("<1") || className.equals(">1"))) {
				Element defineElement =  xmlFileRoot.getFirstChildElement("define", xmlFileRoot.getNamespaceURI("cpp"));
				String suggestedFormat ="";
				specialMessage = "";
				if (defineElement != null) {
					Element defineNameElement = defineElement.getFirstChildElement("name", defineElement.getNamespaceURI(""));
					if (defineNameElement != null){
						suggestedFormat = satisfyCodingRule.multipleInclusionProtection( defineNameElement.getValue(), className);
						if (!"".equals(suggestedFormat)) 
							specialMessage = "the multiple inclusion protection constant:\n         "
									+ defineNameElement.getValue()
									+ "\n in  [file:" + actualFileName + "]\n"
									+ "    is not the same as "
									+ suggestedFormat + "\n";
					}
					else
						specialMessage = "	[file:" + actualFileName + "]\n"
								+ " should have a multiple protection constant \n";
				}
				else
					specialMessage = "	[file:" + actualFileName + "]\n"
								+ " should have a multiple protection constant \n";
				if (!"".equals(specialMessage))		
					this.setViolationMessage(specialMessage);
				
					
			}

		}
	}

	private void checkClassNumberInHeaderFileCR() {

		if (this.setRuleNameAndContent("RC4")) {
			String className = this.getClassName();
			if (!satisfyCodingRule.classesInHeaderFile(className)) {
				specialMessage = "[file:" + actualFileName + "] \n"
						+ "    conatains more than one class.\n";
				this.setViolationMessage(specialMessage);
			}
		}
	}

	private void checkMethodsInImplementationFileCR() {
		
		Nodes nameValueNodes = this.extractElementNodes(methodXPath, nameXPath,
				xmlFileRoot, "NA", typeXPath);
		String classNameOfMethod = "";

		if (!(className.equals(">1") || className.equals("<1")))
			for (int j = 0; j < nameValueNodes.size(); j++) {
				if (this.setRuleNameAndContent("RC5")) {
					if ( ! nameValueNodes.get(j).getValue().contains("::"))
						continue;
	
						classNameOfMethod = nameValueNodes.get(j).getValue()
										.substring(0, nameValueNodes.get(j).getValue().indexOf(':'));
	
						if (!satisfyCodingRule.methodInImplementationFile(classNameOfMethod, className,
							embeddedClassNameNodes)) {
							
							specialMessage = "the method :\n         "
									+ nameValueNodes.get(j).getValue()
										.substring(nameValueNodes.get(j).getValue().indexOf("::") + 2)
									+ "\n in implementation file, " 
									+ "[file:" + actualFileName + "  "+ "   line:"
									+ ((Element) nameValueNodes.get(j)).getAttributeValue("lineNumber")
									+ "]\n    is member of a different class.\n";
							
							this.setViolationMessage(specialMessage);
						}
					}
			}
	}

	private void checkAccessSpecifiersCR() {

		if (this.setRuleNameAndContent("RC6")) {
			Nodes classBlockNodes = xmlFileRoot.query("//" + classBlockXPath, context);
			for (int i=0; i<classBlockNodes.size(); i++)
				if (classBlockNodes.get(i) instanceof Element)
					if (!satisfyCodingRule.accessSpecifier(((Element)classBlockNodes.get(i)).getChildElements(),
							variableDeclXPath, typeXPath, nameXPath, context)) {
						specialMessage = "In header file [file:"
								+ actualFileName
								+ "]\n    the access specifiers and friend declarations "
								+ "are not in the correct order.\n";
						this.setViolationMessage(specialMessage);
					}
		}
	}

	private void checkGlobalVariables() {

		if (this.setRuleNameAndContent("RC7")) {
			Nodes nameValueNodes = this.extractElementNodes(
					variableDeclXPath, nameXPath, xmlFileRoot, "NoConstStatic", typeXPath);
			for (int j = 0; j < nameValueNodes.size(); j++) {
				if (nameValueNodes.get(j) instanceof Element)
					if (nameValueNodes.get(j).getValue().contains("::"))
						continue;

				if (!satisfyCodingRule.globalVariables(nameValueNodes.get(j).getValue())) {
					specialMessage = "the use of global variable :\n         "
							+ nameValueNodes.get(j).getValue()
							+ "\n in [file:"
							+ actualFileName
							+ "     line:"
							+ ((Element) nameValueNodes.get(j)).getAttributeValue("lineNumber") 
							+ "]\n    should be avoided\n";
					this.setViolationMessage(specialMessage);
				}
			}
		}
	}

	private void checkFriendClasses() {

		if (this.setRuleNameAndContent("RC8")) {
			Nodes nameValueNodes = this.extractElementNodes(typeXPath,
					nameXPath, xmlFileRoot, "NA", typeXPath);
			for (int j = 0; j < nameValueNodes.size(); j++)
				if (nameValueNodes.get(j) instanceof Element)
					if (!satisfyCodingRule.friendClasses(nameValueNodes.get(j).getValue())
							&& !((nameValueNodes.get(j).getParent().getParent())
									.getValue().contains("<<") || (nameValueNodes
									.get(j).getParent().getParent()).getValue()
									.contains(">>"))) {
						specialMessage = "the use of friend :\n         "
								+ nameValueNodes.get(j).getParent().getParent().getValue()
								+ "\n in [file:"
								+ actualFileName
								+ "     line:"
								+ ((Element) nameValueNodes.get(j)).getAttributeValue("lineNumber")
								+ "]\n" + "    should be avoided.\n";
						this.setViolationMessage(specialMessage);
					}
		}
	}

	private void checkDestructorFunction() {

		if (this.setRuleNameAndContent("RC9"))
			if (!satisfyCodingRule.destructorFunction(xmlFileRoot, methodXPath,
					typeXPath, nameXPath, destructorXPath, context)) {
				String className = "";
				if (!this.className.equals(">1")
						&& !this.className.equals("<1"))
					className = this.className;
				specialMessage = "the class:\n       "
						+ className
						+ "\n"
						+ "    has virtual method(s) but no virtual destructor\n";
				this.setViolationMessage(specialMessage);
			}
	}

	private void checkPointerAttributes() {

		if (this.setRuleNameAndContent("RC10")) {
			Nodes classNodes = xmlFileRoot.query(classXPath, context);
			for (int i = 0; i < classNodes.size(); i++)
				if (classNodes.get(i) instanceof Element) {
					Nodes attributeNodes = ((Element) classNodes.get(i)).query(attributeXPath, context);
					for (int j = 0; j < attributeNodes.size(); j++)
						if (attributeNodes.get(j) instanceof Element)
							if (attributeNodes.get(j).getValue().contains("*"))
								if (!satisfyCodingRule.copyConstructorAssignmentOpr(
										xmlFileRoot, methodXPath, constructorXPath, parameterXPath, 
										typeXPath, nameXPath, context)) {
									String className = "";
									if (!this.className.equals(">1")
											&& !this.className.equals("<1"))
										className = this.className;
									specialMessage = "the class:\n       "
											+ className
											+ "\n [file: "
											+ actualFileName
											+ "     line: "
											+ this.getLineNumber(attributeNodes
													.get(j).getValue())
											+ "]\n    contains the field:\n       "
											+ attributeNodes.get(j).getValue()
											+ "\n    but does not have a copy constructor and/or an assignment operator\n";
									this.setViolationMessage(specialMessage);
								}
				}
		}
	}

	private void checkAttributeModificationInFunction() {

		if (this.setRuleNameAndContent("RC11") && !this.className.trim().equals("<1")) {
			String functionName;
			String memberOfClass =this.className;
			Nodes attributeNameNodes = collectAttributeNames("AllClasses", "NA");

			Nodes functionNodes = xmlFileRoot.query(functionImplementationXPath, context);
			
			for (int j = 0; j < functionNodes.size(); j++)
				if (functionNodes.get(j) instanceof Element) {
					if (checkToSkipFunction(functionNodes.get(j)))
						continue;
					
					if (((Element)functionNodes.get(j)).getFirstChildElement("name", ((Element)functionNodes.get(j))
							.getNamespaceURI())== null)
						continue;
					
					functionName = ((Element)functionNodes.get(j)).getFirstChildElement("name", ((Element)functionNodes.get(j))
								.getNamespaceURI()).getValue().trim();
					
					if (functionName.contains("::"))
							functionName = functionName.substring(functionName.lastIndexOf("::") + 2).trim();
					
					if (!satisfyCodingRule.memberFunctionChangesAttribute( functionName, memberOfClass,
							attributeNameNodes, superClassAttributeNodes, (Element)functionNodes.get(j), 
							functionBodyExprXPath, functionCallXPath, functionCallArgumentsXPath, context,
							factRootElement, factClassXPath, factSignatureXPath, factParameterTypeNameXPath,
							localVariableXPath, parameterXPath)) {

						specialMessage = "the method:\n        "
								+ functionName + "\n     in file [file:"
								+ actualFileName + "     line: "
								+ this.getLineNumber(functionNodes.get(j).getValue()
										.substring(0, functionNodes.get(j).getValue().indexOf(')') + 1)) + " ]\n"
								+ "        can be declared const\n";
						this.setViolationMessage(specialMessage);
					}
				}
		}
	}
	
	private boolean checkToSkipFunction (Node functionNode){
		boolean skipFunction = false;
		
		//destructor and constructor
		if (((Element)functionNode).getQualifiedName().equals("destructor") || 
				((Element)functionNode).getQualifiedName().equals("constructor"))
			skipFunction = true;
		
		//operator overriding methods
		Node functionNameNode = ((Element)functionNode).getFirstChildElement("name", 
				((Element)functionNode).getNamespaceURI());
		if (functionNameNode != null && !skipFunction)
			if (functionNameNode.getValue().trim().startsWith("operator") )
				skipFunction = true;
		
		//Virtual and static methods
		if (!skipFunction)
			skipFunction = virtualOrStaticMethod(functionNode);
		
		//const function
		if (!skipFunction){
			boolean constFound = false;
			Nodes functionSpecifierNodes = ((Element)functionNode).query(specifierXPath, context);
			if (functionSpecifierNodes.size() > 0)
				for (int i = 0; i < functionSpecifierNodes.size(); i++)
					if (functionSpecifierNodes.get(i) instanceof Element)
						if (functionSpecifierNodes.get(i).getValue().equals("const")) {
							constFound = true;
							break;
						}
			if (constFound)
				skipFunction = true;
			}
		
		//Functions that return by reference
		if (!skipFunction){
			Node functionType = ((Element)functionNode).getFirstChildElement("type", ((Element)functionNode).getNamespaceURI());
			if (functionType != null)
				if (functionType.getValue().contains("&"))
					skipFunction = true;
					
		}
		
		return skipFunction;
	}
	
	private boolean virtualOrStaticMethod (Node functionNode){
		boolean virtualOrStatic = false;
		Nodes factClassNodes;
		Nodes factMethodNodes;
		String methodName;
		
		Node functionTypeNode;
		if (((Element)functionNode).getFirstChildElement("name", 
				((Element)functionNode).getNamespaceURI()) != null){
			if (!((Element)functionNode).getFirstChildElement("name", 
					((Element)functionNode).getNamespaceURI()).getValue().contains("::")){
				functionTypeNode= ((Element)functionNode).getFirstChildElement("type", 
						((Element)functionNode).getNamespaceURI());
				if (functionTypeNode != null )
					if (((Element)functionTypeNode).getFirstChildElement("name", ((Element)functionTypeNode)
							.getNamespaceURI())!=null)
						if ( ((Element)functionTypeNode).getFirstChildElement("name", 
							((Element)functionTypeNode).getNamespaceURI()).getValue().trim().equals("virtual") 
							||((Element)functionTypeNode).getFirstChildElement("name", 
									((Element)functionTypeNode).getNamespaceURI()).getValue().trim().equals("static") )
							 virtualOrStatic = true;
			}
			else{
				methodName = ((Element)functionNode).getFirstChildElement("name", 
						((Element)functionNode).getNamespaceURI()).getValue().substring(((Element)functionNode).getFirstChildElement("name", 
						((Element)functionNode).getNamespaceURI()).getValue().lastIndexOf("::")+2);
				if (factRootElement != null){
					factClassNodes = factRootElement.query(factClassXPath + "[@name='" + className.trim() + "']");
					
					if ( factClassNodes != null )
						for (int j=0; j<factClassNodes.size(); j++){
							factMethodNodes = factClassNodes.get(j).query(factSignatureXPath);
							
							if (factMethodNodes != null)
								for (int k=0; k<factMethodNodes.size(); k++)
									if (factMethodNodes.get(k) instanceof Element){
										if ( ((Element)factMethodNodes.get(k)).getFirstChildElement("name").getValue().trim()
												.equals(methodName) && parameterMatch(factMethodNodes.get(k), functionNode)){
											functionTypeNode= ((Element)factMethodNodes.get(k)).getFirstChildElement("type");
											if (functionTypeNode != null )
												if (((Element)functionTypeNode).getFirstChildElement("name")!=null)
													if ( ((Element)functionTypeNode).getFirstChildElement("name").getValue().trim().equals("virtual") 
														||((Element)functionTypeNode).getFirstChildElement("name").getValue().trim().equals("static") )
														 virtualOrStatic = true;
										}
									}
						}
				}
			}
		}
		return virtualOrStatic;
	}
	
	private boolean parameterMatch (Node signatureNode, Node functionNode){
		boolean matched = true;
		Nodes functionTypeNameNodes = functionNode.query(parameterXPath + "/" + typeXPath + "/" + nameXPath, context);
		Nodes signatureTypeNameNodes = signatureNode.query(factFunctionParameterTypeXPath + "/" + factNameXPath);
		
		if (functionTypeNameNodes != null && signatureTypeNameNodes == null)
			matched = false;
		else if (functionTypeNameNodes == null && signatureTypeNameNodes != null)
			matched = false;
		else if (functionTypeNameNodes != null && signatureTypeNameNodes != null)
			if (functionTypeNameNodes.size() != signatureTypeNameNodes.size())
				matched = false;
			else
				for (int i=0; i<functionTypeNameNodes.size(); i++)
					if (!functionTypeNameNodes.get(i).getValue().equals(signatureTypeNameNodes.get(i).getValue())){
						matched = false;
						break;
					}
		return matched;
	}

	private Nodes collectAttributeNames(String inFile, String attributeType) {
		Nodes attributeNameNodes = new Nodes();
		Nodes nameValueNodes = null;

		Nodes classNodes = headerFileRoot.query(classXPath, context);
		for (int i = 0; i < classNodes.size(); i++)
			if (classNodes.get(i) instanceof Element) {
				if (inFile.equals("shouldBeClass")) {
					Nodes classNameNodes = ((Element) classNodes.get(i)).query(	nameXPath, context);
					for (int k = 0; k < classNameNodes.size(); k++)
						if (classNameNodes.get(k) instanceof Element)
							if ((classNameNodes.get(k).getValue()).equalsIgnoreCase(
											xmlFileRoot.getAttributeValue("filename").substring(0, xmlFileRoot.getAttributeValue("filename").lastIndexOf('.'))))
								nameValueNodes = this.extractElementNodes(attributeXPath, nameXPath,
											((Element) classNodes.get(i)),attributeType, typeXPath);
				} else{
					// in AllClasses
					nameValueNodes = this.extractElementNodes(attributeXPath, nameXPath, ((Element)
							classNodes.get(i)),	attributeType, typeXPath);
				}
				if (nameValueNodes != null)
					for (int j = 0; j < nameValueNodes.size(); j++){
						if (nameValueNodes.get(j) instanceof Element){
							if (((Element)nameValueNodes.get(j)).getFirstChildElement("name", 
									((Element)nameValueNodes.get(j)).getNamespaceURI()) != null)
								attributeNameNodes.append(((Element)nameValueNodes.get(j)).getFirstChildElement("name", 
									((Element)nameValueNodes.get(j)).getNamespaceURI()).copy());
							else
								attributeNameNodes.append(nameValueNodes.get(j).copy());
						}
					}
			}

		return attributeNameNodes;
	}

	private void checkDummyArgument() {

		if (this.setRuleNameAndContent("RC12")) {
			Nodes functionNodes = xmlFileRoot.query(methodInImplementationFileXPath,
					context);
			for (int j = 0; j < functionNodes.size(); j++)
				if (functionNodes.get(j) instanceof Element)
					if (((Element)functionNodes.get(j)).getFirstChildElement("name"
							, ((Element)functionNodes.get(j)).getNamespaceURI()) != null){
						
						String fullFunctionName = ((Element)functionNodes.get(j)).getFirstChildElement("name"
							, ((Element)functionNodes.get(j)).getNamespaceURI()).getValue();
						
						if (!fullFunctionName.contains("::"))
							continue;
						
						String functionOfClass;
						if (fullFunctionName.indexOf("::") != fullFunctionName.lastIndexOf("::"))
							functionOfClass = fullFunctionName.substring(fullFunctionName
									.substring(0, fullFunctionName.lastIndexOf("::")).lastIndexOf("::") + 2, fullFunctionName.lastIndexOf("::"));
						else											
							functionOfClass = fullFunctionName.substring(0, fullFunctionName.indexOf("::"));
						String functionName = fullFunctionName.substring(fullFunctionName.lastIndexOf("::")+2);
						
						Nodes parameterTypeNodes = ((Element)functionNodes.get(j)).query(parameterXPath+ "/"+ typeXPath, context);
						Nodes parameterNameNodes = ((Element)functionNodes.get(j)).query(parameterXPath+ "/"+ nameXPath, context);
						
						int numberOfParamName = parameterNameNodes.size();
						
						if (numberOfParamName<=0)
							continue;
												
						if (factRootElement != null)
							if (satisfyCodingRule.dummyArgument(functionOfClass, functionName, parameterTypeNodes, numberOfParamName,
									factRootElement, factClassXPath, factSignatureXPath, factFunctionParameterTypeXPath,
									factFunctionParameterNameXPath, functionNodes.get(j), exprOnlyXPath, nameXPath, context)) {
								specialMessage = "the method implementation:\n       "
										+ functionNodes.get(j).getValue().substring(0, functionNodes.get(j).getValue().indexOf(')')+1)
										+ "\n  in [file: "
										+ actualFileName
										+ "]\n    does not have dummy argument(s) in its signature\n" 
										+ "    in [file: "
										+ headerFileRoot.getAttributeValue("filename")
										+ "]\n";
								this.setViolationMessage(specialMessage);
							}
				}
		}

	}

	
	
	private void checkPublicAttribute() {

		if (this.setRuleNameAndContent("RC14"))
			if (!satisfyCodingRule.attributeScope(xmlFileRoot, classBlockXPath, context)) {
				specialMessage = "Public attribute(s) is/are used in\n        "
						+ "[file: " + actualFileName + "   ]";
				this.setViolationMessage(specialMessage);
			}
	}

	private void checkStruct() {

		if (this.setRuleNameAndContent("RC15"))
			if (satisfyCodingRule.structFound(xmlFileRoot, structXPath, context)) {
				specialMessage = "Struct is used in\n        [file: "
						+ actualFileName + "   ]";
				this.setViolationMessage(specialMessage);
			}
	}

	private void checkRedefinedAttributes() {

		if (this.setRuleNameAndContent("RC16")) {
			Nodes attributeNameNodes = collectAttributeNames("shouldBeClass", "NA");
			if (!(className.equals(">1") || className.equals("<1")))
				for (int i = 0; i < attributeNameNodes.size(); i++) {
					int testResult = satisfyCodingRule.attributeNotRedefined(
							attributeNameNodes.get(i).getValue(),
							superClassAttributeNodes);
					if (testResult != -1) {
						specialMessage = "Data member\n        "
								+ attributeNameNodes.get(i).getValue()
								+ "\nin [file: "
								+ actualFileName
								+ "   line: "
								+ ((Element) attributeNameNodes.get(i))
										.getAttributeValue("lineNumber")
								+ " ]"
								+ "\nis defined in\n   [file: "
								+ ((Element) superClassAttributeNodes
										.get(testResult))
										.getAttributeValue("fileName") + " ]";
						this.setViolationMessage(specialMessage);
					}
				}
		}
	}
	
	private void checkPtrAndRefParameters(){
		if (this.setRuleNameAndContent("RC17")) {
			Nodes parameterNodes;
			int suggestedConstType;
			String methodName = "";
			Nodes attributeNameNodes = collectAttributeNames("shouldBeClass", "NA");
			Nodes implMethodNodes = xmlFileRoot.query(functionImplementationXPath, context);
			if (implMethodNodes != null)
				for (int i = 0; i < implMethodNodes.size(); i++) 
					if (implMethodNodes.get(i) instanceof Element) {
						parameterNodes = implMethodNodes.get(i).query(parameterXPath, context);
						methodName = ((Element)implMethodNodes.get(i)).getFirstChildElement("name"
								,((Element)implMethodNodes.get(i)).getNamespaceURI() ).getValue();
						if (skipPtrAndRefCheck(parameterNodes, methodName))
							continue;
						
						for (int j=0; j<parameterNodes.size(); j++)
							if (parameterNodes.get(j) instanceof Element){
								suggestedConstType = satisfyCodingRule.constParameterType(parameterNodes.get(j), 
										implMethodNodes.get(i), constPosition(parameterNodes.get(j)), localVariableXPath, className, 
										functionBodyExprXPath, functionCallXPath, functionCallArgumentsXPath, context, factRootElement,
										factClassXPath, factSignatureXPath, factFunctionParameterTypeXPath, typeXPath, nameXPath, 
										parameterXPath, attributeNameNodes, returnExprXPath);
								
								if (suggestedConstType > 0) {
									specialMessage = "Parameter declaration \n        "
										+ parameterNodes.get(j).getValue()
										+ "\n    in method \n     "
										+ methodName
										+ "\n    in [file: "
										+ actualFileName
										+ "   line: "
										+ this.getLineNumber(implMethodNodes.get(i).getValue())
										+ " ]"
										+ "\n    can be declared as\n   "
										+ suggestConstParameterDeclaration(suggestedConstType);
									this.setViolationMessage(specialMessage);
								}
							}
					}
		}
	}
	
	private void checkForbiddenStrings (){
		
		if (this.setRuleNameAndContent("RC18")) {
			String lineNumbers;
			for (int i=0; i<this.forbiddenStrings.size(); i++) {
				lineNumbers = satisfyCodingRule.searchForbiddenString(xmlFileRoot, forbiddenStrings.get(i).getValue());
				if (!lineNumbers.equals("")){
					specialMessage = "The forbidden string \n        "
						+ forbiddenStrings.get(i).getValue()
						+ "\n    is used in [file: "
						+ actualFileName
						+ "   line(s): "
						+ lineNumbers
						+ " ]";
					this.setViolationMessage(specialMessage);
				}
			}
		}
		
	}
	
	private void checkFloatingEquality () {
		if (this.setRuleNameAndContent("RC19")){
			Nodes conditionStmts;
			Nodes functionBody = xmlFileRoot.query(functionImplementationXPath, context);
			if (functionBody != null){
				
				for (int i=0; i<functionBody.size(); i++)
					if (functionBody.get(i) instanceof Element ){
						conditionStmts = ((Element)functionBody.get(i)).query(ifConditionXPath, context);
						if (conditionStmts != null){
							for (int j=0; j<conditionStmts.size(); j++){
								if (satisfyCodingRule.floatingNumberEqualityComparison(conditionStmts.get(j), 
										functionBody.get(i), localVariableXPath, parameterXPath, context, 
										factClassXPath, factAttributeXPath + "/" + factNameXPath, 
										factFunctionDeclXPath, factRootElement, className, 
										factSuperClassXPath)){
									specialMessage = "In the conditional statement \n        "
										+ conditionStmts.get(j).getValue()
										+ "\nin [file: "
										+ actualFileName
										+ "   line: "
										+ this.getLineNumber(conditionStmts.get(j).getValue())
										+ " ]"
										+ "\n== or != comparison involves double/floating point data.";
								this.setViolationMessage(specialMessage);
								}
							}
						}
					}
			}
		}
	}
	
	private String suggestConstParameterDeclaration (int suggestedConstType){
		String parameterDeclFormat = "";
		if (suggestedConstType == 1)
			parameterDeclFormat= "'const type & param'";
		else if (suggestedConstType == 2)
			parameterDeclFormat= "'type * const param'";
		else if (suggestedConstType == 3)
			parameterDeclFormat= "'const type * param'";
		else if (suggestedConstType == 4)
			parameterDeclFormat= "'const type * const param'";
		
		return parameterDeclFormat;
	}
	
	private boolean skipPtrAndRefCheck (Nodes parameterNodes, String fileName){
		boolean skipMethod = false;
		int numPtrOrRefParamFound = 0;
		int numConstPtrOrRefParamFound = 0;
		Element parameterTypeElement;
		
		if (parameterNodes ==  null) 
			skipMethod = true;
		else
			if (parameterNodes.size() == 0) 
				skipMethod = true;
			else{
				for (int i = 0; i < parameterNodes.size(); i++ )
					if (parameterNodes.get(i) instanceof Element){
						parameterTypeElement = ((Element)parameterNodes.get(i)).getFirstChildElement("type",
								((Element)parameterNodes.get(i)).getNamespaceURI());
						if (parameterTypeElement != null){
							for (int j=0; j<parameterTypeElement.getChildCount(); j++)
								if (parameterTypeElement.getChild(j) instanceof Text ){
									if (parameterTypeElement.getChild(j).getValue().trim().equals("&")){
										numPtrOrRefParamFound ++;
										if (constPosition((Node)parameterTypeElement) == 1)
											numConstPtrOrRefParamFound ++;
									}
									if (parameterTypeElement.getChild(j).getValue().trim().equals("*")){
										numPtrOrRefParamFound ++;
										if (constPosition((Node)parameterTypeElement) == 3)
											numConstPtrOrRefParamFound ++;
									}
								}
						}
					}
				if (numPtrOrRefParamFound == numConstPtrOrRefParamFound && numPtrOrRefParamFound > 0)
					skipMethod = true;
			}
		
		return skipMethod;
	}

	/**
	 * Checks if there is a const in the parameter type and returns the position of the parameter or zero
	 * @param parameterNode is parameter of a method
	 * @return 	0, if parameter type doesnt have const
	 * 			1, if parameter type has const in 'const type *' or 'type const *' or 'const type &' or 'type const &'order
	 * 			2, if parameter type has const in 'type * const' order
	 * 			3, if parameter type has const in 'const type * const' order 
	 */
	private int constPosition(Node parameterNode) {
		
		int constPosition = 0;
		Elements typeNameElements;
		Element parameterTypeElement;
		boolean charAtTheEnd = false; 
		String typeText;
		
		if (parameterNode instanceof Element){
			parameterTypeElement = ((Element)parameterNode).getFirstChildElement("type",
					((Element)parameterNode).getNamespaceURI());
			if (parameterTypeElement != null){
				typeText = parameterTypeElement.getValue().trim();
				if (typeText.contains("*")){
					if (typeText.length()-1 == typeText.indexOf('*'))
						charAtTheEnd = true;
				}
				else if (typeText.contains("&"))
					if (typeText.length()-1 == typeText.indexOf('&'))
						charAtTheEnd = true;
				
				typeNameElements = parameterTypeElement.getChildElements("name", parameterTypeElement
						.getNamespaceURI());
				for (int j=0; j<typeNameElements.size(); j++)
					if (typeNameElements.get(j).getValue().trim().equals("const"))
						if(j < (typeNameElements.size()-1) && constPosition == 0)
							constPosition = 1;
						else if (j == (typeNameElements.size()-1) && constPosition == 0 && charAtTheEnd)
							constPosition = 1;
						else if (j == (typeNameElements.size()-1) && constPosition == 0 && !charAtTheEnd)
							constPosition = 2;
						else if (j == (typeNameElements.size()-1) && constPosition > 0)
							constPosition = 3;
			}
		}
		return constPosition;
	}
	
	private void collectSuperClassAttributes(String className) {

		if (factRootElement != null){
			Nodes factClassNodes = factRootElement.query(factClassXPath + "[@name=\"" + className.trim() + "\"]");
	
			for (int i = 0; i < factClassNodes.size(); i++)
				if (factClassNodes.get(i) instanceof Element) {
					Element factClassElement = (Element) factClassNodes.get(i);
					if (!this.className.equals(className)) {
						String filePath = factClassElement.getAttributeValue("filePath");
						Nodes attributeNameNodes = factClassElement.query(factAttributeXPath + "/" + factNameXPath);
						
						for (int j = 0; j < attributeNameNodes.size(); j++)						
							if (attributeNameNodes.get(j) instanceof Element) {
								Element nodeElement;
								
								if (((Element)attributeNameNodes.get(j)).getFirstChildElement("name", 
										((Element)attributeNameNodes.get(j)).getNamespaceURI()) != null ){
									
									nodeElement = (Element)((Element)attributeNameNodes.get(j)).getFirstChildElement("name", 
										((Element)attributeNameNodes.get(j)).getNamespaceURI()).copy();
								}
								else{
									nodeElement = (Element)attributeNameNodes.get(j).copy();
								}
								Attribute fileName = new Attribute("fileName", filePath);
								nodeElement.addAttribute(fileName);
								superClassAttributeNodes.append((Node)nodeElement);
							}
					}
	
					Nodes superClassNodes = factClassElement.query(factSuperClassXPath + "/" + factNameXPath);
					for (int j = 0; j < superClassNodes.size(); j++)
						if (superClassNodes.get(j) instanceof Element){
							collectSuperClassAttributes(superClassNodes.get(j).getValue());
						}
				}
	
		}
	}

	
}
