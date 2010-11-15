package eu.fbk.checkRules;

import eu.fbk.rules.NamingRules;

import nu.xom.Element;
import nu.xom.Node;
import nu.xom.Nodes;

public class CheckNamingRules extends CheckRules {
	
	private NamingRules satisfyNamingRule = new NamingRules();
	private String specialMessage = "";
	private String className;
	
	public CheckNamingRules(String configFile) {
		
		super("NamingRules", configFile);
		
	}
	

	public void verifyNamingConventions (String fileType) {
		
		if ( fileType.equals("HeaderFile")){
			checkDataMembersAndClassNR();
			checkMemberFunctionNR();
			className = this.getClassName();
			checkHeaderFileNR();
		}
		else if ( fileType.equals("ImplmentationFile"))
			checkImplmentationFileNR();
		
		checkEnumElementNR();		
		checkGlobalVariableNR();
		checkGlobalClassStaticVariableNR();
		checkGlobalConstNR();
		checkGlobalClassStaticConstNR();
		checkLocalVariableVariantsNR();
		
	}

	
	
	private void checkHeaderFileNR(){
				
		if (this.setRuleNameAndContent("RN4")){
			if (!(className.equals(">1") || className.equals("<1")))
				if (!satisfyNamingRule.headerFileName(xmlFileRoot.getAttributeValue("filename"), className)){
					specialMessage = "the header file name \"" + xmlFileRoot.getAttributeValue("filename") 
									 + "\" is not built using the class name \"" +  className + "\" and \".h\"";
					this.setViolationMessage(specialMessage);
				}
		}
	}
	

	
	private void checkImplmentationFileNR(){
		
		if (this.setRuleNameAndContent("RN5")){
			if (!(className.equals(">1") || className.equals("<1")))
				if (!satisfyNamingRule.implmentationFileName(xmlFileRoot.getAttributeValue("filename"), className)){
						specialMessage = "the implementation file name \"" + xmlFileRoot.getAttributeValue("filename") 
										 + "\"  is not built using the class name \"" +  className + "\" and \".cxx\"";
						this.setViolationMessage(specialMessage);
					}
		}
	}
	
	
	
	private void checkLocalVariableVariantsNR() {
		Nodes memberFunctionNodes= xmlFileRoot.query(functionImplementationXPath, context);

		for (int i=0; i<memberFunctionNodes.size(); i++)
			if (memberFunctionNodes.get(i) instanceof Element ){
				Element memberFunctionElement = (Element) memberFunctionNodes.get(i);
				
				checkLocalVariableNR(memberFunctionElement);					
			}
	}

	private void checkLocalVariableNR(Element memberFunctionElement) {
		Nodes nameValueNodes = this.extractElementNodes(localVariableXPath, nameXPath, memberFunctionElement, "NA", typeXPath);
	
		if (this.setRuleNameAndContent("RN13"))
			for (int j=0; j<nameValueNodes.size(); j++)
				if (!satisfyNamingRule.localVariable(nameValueNodes.get(j).getValue())){
					specialMessage = "the variable:\n         " + nameValueNodes.get(j).getValue() 
						      		 + "\n  [file: "+ actualFileName + "   line:" + ((Element)nameValueNodes.get(j)).getAttributeValue("lineNumber")+ "]\n"
							  		 + "    does not start with a lower case letter";
					this.setViolationMessage(specialMessage);
				}
		
		if (this.setRuleNameAndContent("RN14"))
			checkGeneralNR(nameValueNodes);
	}

	private void checkGlobalVariableNR() {
		Nodes nameValueNodes = this.extractElementNodes(variableDeclXPath, nameXPath, xmlFileRoot, "NoConstStatic", typeXPath);
		String globalVariableName = "";
		
		for (int j=0; j<nameValueNodes.size(); j++)
			if (this.setRuleNameAndContent("RN15")){
				if (nameValueNodes.get(j).getValue().contains("::"))
					continue;
				else
					globalVariableName = nameValueNodes.get(j).getValue();

				if (!satisfyNamingRule.globalNonConstVariable(globalVariableName)){
					specialMessage = "the global variable:\n         " + globalVariableName 
						      		 + "\n  [file:" + actualFileName + " " + "   line:" + ((Element)nameValueNodes.get(j)).getAttributeValue("lineNumber")+ "]\n"
						      		 + "    does not start with a prefix \"gAli\"\n";
					this.setViolationMessage(specialMessage);
				}
				
				if (this.setRuleNameAndContent("RN16"))
					checkGeneralNR(nameValueNodes.get(j));
			}	
	}

	private void checkGlobalClassStaticVariableNR() {
		Nodes nameValueNodes = this.extractElementNodes(variableDeclXPath, nameXPath, xmlFileRoot, "static", typeXPath);
		String globalVariableName = "";
		
		for (int j=0; j<nameValueNodes.size(); j++)
			if (this.setRuleNameAndContent("RN15")){
				if (nameValueNodes.get(j).getValue().contains("::"))
					globalVariableName = nameValueNodes.get(j).getValue().substring(nameValueNodes.get(j).getValue().indexOf("::")+2);
				else
					continue;
				
				if (!satisfyNamingRule.staticDataMember(globalVariableName)){
					specialMessage = "the global static variable:\n         " + globalVariableName 
						      		 + "\n  [file:" + actualFileName + " " + "   line:" + ((Element)nameValueNodes.get(j)).getAttributeValue("lineNumber")+ "]\n"
						      		 + "    does not start with a prefix \"k\"\n";
					this.setViolationMessage(specialMessage);
				}
				
				if (this.setRuleNameAndContent("RN16"))
					checkGeneralNR(nameValueNodes.get(j));
			}	
	}


	private void checkGlobalConstNR() {
		Nodes nameValueNodes = this.extractElementNodes(variableDeclXPath, nameXPath, xmlFileRoot, "const", typeXPath);
		String globalConstName = "";
		
		for (int j=0; j<nameValueNodes.size(); j++)
			if (this.setRuleNameAndContent("RN15")){
				if (nameValueNodes.get(j).getValue().contains("::"))
					continue; //because its a class static variable 
				else
					globalConstName = nameValueNodes.get(j).getValue();
				
				if (!satisfyNamingRule.globalConstVariable(globalConstName)){
					specialMessage = "the global constant :\n         " + globalConstName 
						      		 + "\n  [file:" + actualFileName + " " + "   line:" + ((Element)nameValueNodes.get(j)).getAttributeValue("lineNumber")+ "]\n"
						      		 + "    does not start with a prefix \"k\"\n";
					this.setViolationMessage(specialMessage);
				}
				if (this.setRuleNameAndContent("RN16"))
					checkGeneralNR(nameValueNodes.get(j));
			}
	}
	
	private void checkGlobalClassStaticConstNR() {
		Nodes nameValueNodes = this.extractElementNodes(variableDeclXPath, nameXPath, xmlFileRoot, "staticConst", typeXPath);
		String globalStaticConstName = "";
		
		for (int j=0; j<nameValueNodes.size(); j++)
			if (this.setRuleNameAndContent("RN15")){
				if (nameValueNodes.get(j).getValue().contains("::"))
					continue;
				else
					globalStaticConstName = nameValueNodes.get(j).getValue().substring(nameValueNodes.get(j).getValue().indexOf("::")+2);
				
				if (!satisfyNamingRule.globalConstVariable(globalStaticConstName)){
					specialMessage = "the global static constant:\n         " + globalStaticConstName 
						      		 + "\n  [file:" + actualFileName + " " + "   line:" + ((Element)nameValueNodes.get(j)).getAttributeValue("lineNumber")+ "]\n"
						      		 + "    does not start with a prefix \"k\"\n";
					this.setViolationMessage(specialMessage);
				}
				
				if (this.setRuleNameAndContent("RN16"))
					checkGeneralNR(nameValueNodes.get(j));
			}
	}


	private void checkEnumElementNR() {
		String elementName ="";
		Nodes nameValueNodes = this.extractElementNodes(enumXPath, nameXPath + "[1]", xmlFileRoot, "NA", typeXPath);
		
		if (this.setRuleNameAndContent("RN17"))
			for (int j=0; j<nameValueNodes.size(); j++){
				elementName = getName (nameValueNodes.get(j));
				if (elementName.contains("::"))
					elementName = elementName.substring(elementName.lastIndexOf(':')+1);
				
				if (!satisfyNamingRule.constants(elementName)){
					specialMessage = "the enumerated element:\n         " + elementName 
						      		 + "\n  [file:"+ actualFileName+ "  " + "   line:" + ((Element)nameValueNodes.get(j)).getAttributeValue("lineNumber")+ "]\n"
						      		 + "    does not start with a prefix \"k\"";
					this.setViolationMessage(specialMessage);
				}
			}
					
			
		if (this.setRuleNameAndContent("RN18"))
			checkGeneralNR(nameValueNodes);
	}



	private void checkMemberFunctionNR() {
		Nodes nameValueNodes = this.extractElementNodes(methodXPath, nameXPath, xmlFileRoot, "NA", typeXPath);
		String methodName = ""; 

		if (this.setRuleNameAndContent("RN9"))
			
			for (int j=0; j<nameValueNodes.size(); j++){
				if (nameValueNodes.get(j).getValue().contains("::"))
						methodName = nameValueNodes.get(j).getValue().substring(nameValueNodes.get(j).getValue().indexOf("::")+2).trim();
				else
					methodName = nameValueNodes.get(j).getValue().trim();
				
				if ( methodName.equalsIgnoreCase("main") || methodName.startsWith("operator"))
					continue;
				if (!satisfyNamingRule.memberFunction(methodName)){
					specialMessage = "the method:\n         " + methodName 
						      		 + "\n  [file:" + actualFileName + " " + "   line:" + ((Element)nameValueNodes.get(j)).getAttributeValue("lineNumber")+ "]\n"
							  		 + "    does not start with a capital";
					this.setViolationMessage(specialMessage);
				}
			}
		
		if (this.setRuleNameAndContent("RN10"))
			checkGeneralNR(nameValueNodes);
	}

	
	private void checkDataMembersAndClassNR() {
		
		Nodes classNodes = xmlFileRoot.query(classXPath, context);
	
		for (int i=0; i<classNodes.size(); i++) 			
			if (classNodes.get(i) instanceof Element){				
				Element clsElement = (Element) classNodes.get(i);				
				checkClassNR(clsElement);
				checkDataMemberNR(clsElement);					
				checkConstDataMemberNR(clsElement);				
				checkStaticDataMemberNR(clsElement);				
				checkStaticConstDataMemberNR(clsElement);
			}
	}

	
	
	private void checkClassNR(Element clsElement) {
		String elementName;
		String statment;
		if (this.setRuleNameAndContent("RN6")){
			if (clsElement.getValue().indexOf('\n') != -1)
				statment = clsElement.getValue().substring(0, clsElement.getValue().indexOf('\n'));
			else
				statment = clsElement.getValue();
			elementName = clsElement.getFirstChildElement("name", clsElement.getNamespaceURI()).getValue();
			if (!satisfyNamingRule.className(elementName)){
				specialMessage = "Class:\n         " + elementName 
						      	 + "\n  [file: "+ actualFileName + " " + "   line:" + this.getLineNumber(statment)+ "]\n" 
							  	 +  "   does not start with \"Ali\"\n";
				this.setViolationMessage(specialMessage);
			}		
		}
		if (this.setRuleNameAndContent("RN8"))
			checkGeneralNR(clsElement.getFirstChildElement("name", clsElement.getNamespaceURI()));
	}

	
	
	private void checkStaticConstDataMemberNR(Element clsElement) {
		String elementName = "";
		Nodes nameValueNodes = this.extractElementNodes(attributeXPath, nameXPath, clsElement, "staticConst", typeXPath);

		if (this.setRuleNameAndContent("RN19"))
			for (int j=0; j<nameValueNodes.size(); j++){
				elementName = getName (nameValueNodes.get(j));
				if (elementName.contains("::"))
					elementName = elementName.substring(elementName.lastIndexOf(':')+1);
				
				if (!satisfyNamingRule.staticConstantDataMember(elementName)){
					specialMessage = "the static constant data member:\n         " + elementName 
						      		 + "\n  [file:" + actualFileName + "   line:" + ((Element)nameValueNodes.get(j)).getAttributeValue("lineNumber")+ "]\n"
						      		 + "    does not start with a prefix \"fgk\"";
					this.setViolationMessage(specialMessage);	
				}
			}
		
		if (this.setRuleNameAndContent("RN20"))
			checkGeneralNR(nameValueNodes);
	}



	private void checkStaticDataMemberNR(Element clsElement) {
		String elementName = "";
		Nodes nameValueNodes = this.extractElementNodes(attributeXPath, nameXPath, clsElement, "static", typeXPath);

		if (this.setRuleNameAndContent("RN19"))
			for (int j=0; j<nameValueNodes.size(); j++){
				elementName = getName (nameValueNodes.get(j));
				if (elementName.contains("::"))
					elementName = elementName.substring(elementName.lastIndexOf(':')+1);
				
				if (!satisfyNamingRule.staticDataMember(elementName)){
					specialMessage = "the static data member:\n         " + elementName 
						      		 + "\n  [file:" + actualFileName + " " + "   line:" + ((Element)nameValueNodes.get(j)).getAttributeValue("lineNumber")+ "]\n"
				      		 		 + "    does not start with a prefix \"fg\"";
					this.setViolationMessage(specialMessage);
				}
			}
		
		if (this.setRuleNameAndContent("RN20"))
			checkGeneralNR(nameValueNodes);
	}



	private void checkConstDataMemberNR(Element clsElement) {
		String elementName = ""; 
		Nodes nameValueNodes = this.extractElementNodes(attributeXPath, nameXPath, clsElement, "const", typeXPath);

		if (this.setRuleNameAndContent("RN19"))
			for (int j=0; j<nameValueNodes.size(); j++){
				elementName = getName (nameValueNodes.get(j));
				if (elementName.contains("::"))
					elementName = elementName.substring(elementName.lastIndexOf(':')+1);
			
				if (!satisfyNamingRule.constantDataMember(elementName)){
					specialMessage = "the constant data member:\n         " + elementName 
						      		 + "\n  [file:" + actualFileName + " " + "   line:" + ((Element)nameValueNodes.get(j)).getAttributeValue("lineNumber")+ "]\n"
				      		 		 + "    does not start with a prefix \"fk\"";
					this.setViolationMessage(specialMessage);
				}
			}
		
		if (this.setRuleNameAndContent("RN20"))
			checkGeneralNR(nameValueNodes);
	}



	private void checkDataMemberNR(Element clsElement) {
		String elementName = "";
		Nodes nameValueNodes= this.extractElementNodes(attributeXPath, nameXPath, clsElement, "NoConstStatic", typeXPath);

		if (this.setRuleNameAndContent("RN11"))
			for (int j=0; j<nameValueNodes.size(); j++){
				elementName = getName (nameValueNodes.get(j));
				if (elementName.contains("::"))
					elementName = elementName.substring(elementName.lastIndexOf(':')+1);
				
				if (!satisfyNamingRule.dataMember(elementName)){
					specialMessage = "the member data:\n         " + elementName 
						      		 + "\n  ["+ actualFileName +"   line:" + ((Element)nameValueNodes.get(j)).getAttributeValue("lineNumber")+ "]\n"
							  		 + "    does not start with a prefix \"f\"";
					this.setViolationMessage(specialMessage);
				}
			}
		
		if (this.setRuleNameAndContent("RN12"))
			checkGeneralNR (nameValueNodes);
	}


	private String getName (Node nameNode){
		String name =nameNode.getValue();
		Element eltName;
		if (nameNode instanceof Element){
			eltName = ((Element)nameNode).getFirstChildElement("name", ((Element)nameNode).getNamespaceURI());
			if (eltName != null)
				name = eltName.getValue();
		}
		return name;
	}

	private void checkGeneralNR(Nodes nameValueNodes) {
		String elementName= "";
		
		if (this.setRuleNameAndContent("RN3"))
			if (nameValueNodes != null)
				for (int j=0; j<nameValueNodes.size(); j++){
					if (nameValueNodes.get(j).getValue().contains("::"))
						elementName = nameValueNodes.get(j).getValue().substring(nameValueNodes.get(j).getValue().lastIndexOf(':')+1);
					else
						elementName = nameValueNodes.get(j).getValue();
				
					if (!satisfyNamingRule.generalName(elementName)){
						specialMessage = "the name:\n         " + elementName 
							      		 + "\n  [file:" + actualFileName + "   line:" + ((Element)nameValueNodes.get(j)).getAttributeValue("lineNumber")+ "]\n"
								  		 + "    contains \"_\"\n";
						this.setViolationMessage(specialMessage);
					}	
				}
	}
	
	private void checkGeneralNR(Node nameValueNode) {
		String elementName= "";
		Node parentNode;
		
		if (this.setRuleNameAndContent("RN3"))
			if (nameValueNode != null){
				if (nameValueNode.getValue().contains("::"))
					elementName = nameValueNode.getValue().substring(nameValueNode.getValue().lastIndexOf(':')+1);
				else
					elementName = nameValueNode.getValue();
				if (nameValueNode.getParent() != null)
					parentNode = nameValueNode.getParent();
				else
					parentNode = nameValueNode;
			
				if (!satisfyNamingRule.generalName(elementName)){
					specialMessage = "the name:\n         " + elementName 
						      		 + "\n  [file:" + actualFileName + "   line:" + getLineNumber(parentNode.getValue())+ "]\n"
							  		 + "    contains \"_\"\n";
					this.setViolationMessage(specialMessage);
				}	
			}
	}
	
}
