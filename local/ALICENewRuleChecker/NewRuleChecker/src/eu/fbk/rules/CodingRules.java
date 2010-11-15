package eu.fbk.rules;

import java.util.ArrayList;
import java.util.List;

import eu.fbk.utilities.CamelCaseParsing;
import nu.xom.Element;
import nu.xom.Elements;
import nu.xom.Node;
import nu.xom.Nodes;
import nu.xom.ParentNode;
import nu.xom.Text;
import nu.xom.XPathContext;

public class CodingRules {

	public String multipleInclusionProtection (String name, String className ){
		boolean satisfied = true;
		String suggestedFormat ="";

		String newHeaderName = ""; //Will contain the name as separated by CamelCasing but numbers are attached to the leading text
		String newHeaderNameAlternative = ""; //Will contain the name as separated by CamelCasing
		String possibleWord;
		CamelCaseParsing camelCaseParsing = new CamelCaseParsing (className);

		while ((possibleWord = camelCaseParsing.getNextTerm()) != null){
			try {
			   Double.parseDouble(possibleWord);
			   if (newHeaderName.length() >2)
				   newHeaderName = newHeaderName.substring(0, newHeaderName.length()-1) + possibleWord.toUpperCase() + "_";
			   else
				   newHeaderName = newHeaderName + possibleWord.toUpperCase() + "_";
		   } catch (NumberFormatException e) {
			   newHeaderName = newHeaderName + possibleWord.toUpperCase() + "_";
		   }
		   newHeaderNameAlternative = newHeaderNameAlternative + possibleWord.toUpperCase() + "_";
			//newHeaderName = newHeaderName + possibleWord.toUpperCase() + "_";
		}
		newHeaderName = newHeaderName + "H";
		newHeaderNameAlternative = newHeaderNameAlternative + "H";
		if (!name.contains("_"))
			satisfied = false;
		else
			if (!(newHeaderName.equals(name) || newHeaderNameAlternative.equals(name) 
					|| ( (className.toUpperCase() + "_H").equals(name)) 
						 && (name.indexOf('_') == name.lastIndexOf('_')) ) )
				satisfied = false;
		
		if (!satisfied)
			suggestedFormat = '"' + className.toUpperCase() + "_H\"\n      or \"" + newHeaderNameAlternative;
			
		return suggestedFormat;
	}
	
	public boolean classesInHeaderFile (String className){
		boolean satisfied = true;
			
		if (className.equals(">1"))
			satisfied = false;
		
		return satisfied;
	}
	
	public boolean methodInImplementationFile (String name, String className, Nodes classNameNodes){
		boolean satisfied = true;
		
		if (!name.equals(className)){
			boolean methodOfEmbeddedClass = false;
			
			for (int i=0; i<classNameNodes.size(); i++) 			
				if (classNameNodes.get(i) instanceof Element)		
					if (classNameNodes.get(i).getValue().trim().equals(name))
						methodOfEmbeddedClass = true;
			
			if (!methodOfEmbeddedClass)
				satisfied = false;
		}
		
		return satisfied;
	}
	
	public boolean accessSpecifier (Elements accessSpecifierElements, String variableDeclXPath, String typeXPath, 
			String nameXPath, XPathContext context){
		boolean satisfied = true;
		boolean nonFriendPrivateDeclExists = false;
		String previousSpecifier = "";
		String currentSpecifier = "";
		//String previousTypeName = "";
		Nodes friendNameNodes;
		
		for (int i=0; i<accessSpecifierElements.size(); i++){
				Element accessSpecifierElement = accessSpecifierElements.get(i);
				if (previousSpecifier.equals("")){
					if (accessSpecifierElements.get(i).getQualifiedName().equals("private") 
							&& accessSpecifierElements.get(i).getValue().trim().equals("")) 
						continue;
					
					previousSpecifier = accessSpecifierElement.getQualifiedName();
					currentSpecifier = accessSpecifierElement.getQualifiedName();
				}
				else {
					previousSpecifier = currentSpecifier;
					currentSpecifier = accessSpecifierElement.getQualifiedName();
				}
				
				if (previousSpecifier.compareTo(currentSpecifier)== 0){
					
					friendNameNodes = accessSpecifierElement.query(variableDeclXPath + "/" + typeXPath + "/" + nameXPath, context);
					String previousTypeName = "";
					for (int j=0; j<friendNameNodes.size(); j++)
						if(friendNameNodes.get(j) instanceof Element)
							if (((Element)friendNameNodes.get(j)).getValue().trim().equals("friend")){
								if (!previousSpecifier.equals("private") || nonFriendPrivateDeclExists){
									satisfied = false;
									break;
								}
								else{
									if (previousTypeName.equals("")){
										previousTypeName = "zfriend";
										currentSpecifier = "zfriend";
									}
									else {
										if (!previousTypeName.equals("zfriend")){
											satisfied = false;
											break;
										}
									}
								}
							}
							else{
								previousTypeName = ((Element)friendNameNodes.get(j)).getValue().trim();
								if (currentSpecifier.equals("zfriend"))
									currentSpecifier = "private";
								if (currentSpecifier.equals("private"))
									nonFriendPrivateDeclExists = true;
							}
					if (!satisfied) break;
				}
				else if (previousSpecifier.compareTo(currentSpecifier) > 0){
					friendNameNodes = accessSpecifierElement.query(variableDeclXPath + "/" + typeXPath + "/" + nameXPath, context);
					for (int j=0; j<friendNameNodes.size(); j++)
						if(friendNameNodes.get(j) instanceof Element)
							if (((Element)friendNameNodes.get(j)).getValue().trim().equals("friend")){
								satisfied = false;
								break;
							}
					if (!satisfied) break;
				}
				else {
					satisfied = false;
					break;
				}
			}
					
		return satisfied;
	}
	
	public boolean globalVariables(String name){
		boolean satisfied = true;
		
		if (!name.trim().equals("gAlice"))
			satisfied = false;
		
		return satisfied;
	}
	
	public boolean friendClasses (String name){
		boolean satisfied = true;
		
		if (name.trim().equals("friend"))
			satisfied = false;
		
		return satisfied;
	}
	
	public boolean destructorFunction(Element xmlFileRoot, String methodXPath, String typeXPath, 
			String nameXPath, String destructorXPath, XPathContext context){
		
		boolean satisfied = true;
		boolean virtualFound = false;
		Element functionElement; 
		Nodes functionNodes = xmlFileRoot.query(methodXPath, context);
		
		for (int i=0; i<functionNodes.size(); i++)
			if (functionNodes.get(i) instanceof Element){
				functionElement = (Element) functionNodes.get(i);
				Nodes functionTypeNames = functionElement.query(typeXPath + "/" + nameXPath, context);
				for (int j=0; j<functionTypeNames.size(); j++)
					if (functionTypeNames.get(j) instanceof Element)
						if (functionTypeNames.get(j).getValue().trim().equals("virtual")){
							virtualFound = true;
							break;
						}
				if (virtualFound)
					break;
			}
		
		if (virtualFound){
			Nodes destructorNodes = xmlFileRoot.query(destructorXPath, context);
			if (destructorNodes.size()<1)
				satisfied = false;
		}
		
		return satisfied;
	}
	
	public boolean copyConstructorAssignmentOpr (Element xmlFileRoot, String methodXPath, 
			String constructorXPath, String parameterXPath, String typeXPath, String nameXPath, 
			XPathContext context){
		
		boolean satisfied = true;
		boolean copyConstructorFound = false;
		boolean assignmentOperatorFound = false;
		
		Nodes functionNodes = xmlFileRoot.query(methodXPath, context);
		
		for (int i=0; i<functionNodes.size(); i++)
			if (functionNodes.get(i) instanceof Element){
				Nodes functionNameNodes = ((Element)functionNodes.get(i)).query(nameXPath, context);
				for (int k=0; k<functionNameNodes.size(); k++){
					if (functionNameNodes.get(k) instanceof Element )
						if (functionNameNodes.get(k).getValue().contains("=") 
								&& functionNameNodes.get(k).getValue().trim().startsWith("operator"))
							assignmentOperatorFound = true;
					if (assignmentOperatorFound){
						assignmentOperatorFound = false;
						Nodes functionTypeNodes = ((Element)functionNodes.get(i)).query(typeXPath, context);
						for (int l=0; l<functionTypeNodes.size(); l++){
							if (functionTypeNodes.get(l) instanceof Element )
								if (functionTypeNodes.get(l).getValue().contains("&") ){
									assignmentOperatorFound = true;
									break;
								}
						}
						if (assignmentOperatorFound)
							break;
					}
				}
				if (assignmentOperatorFound)
					break;
			}
		
		if (assignmentOperatorFound)
			copyConstructorFound = checkCopyConstructor(xmlFileRoot,
					constructorXPath, parameterXPath, typeXPath, context);
		
		if (!copyConstructorFound)
			satisfied = false;
		
		return satisfied;
	}

	private boolean checkCopyConstructor(Element xmlFileRoot,
			String constructorXPath, String parameterXPath, String typeXPath,
			XPathContext context ) {
		
		Nodes functionNodes;
		functionNodes = xmlFileRoot.query(constructorXPath, context);
		boolean copyConstructorFound = false;

		for (int i=0; i<functionNodes.size(); i++)
			if (functionNodes.get(i) instanceof Element){
				Nodes parameterNodes = ((Element)functionNodes.get(i)).query(parameterXPath + "/" + typeXPath, context);
				
				for (int k=0; k<parameterNodes.size(); k++)
					if (parameterNodes.get(k) instanceof Element)
						if (((Element)parameterNodes.get(k)).getFirstChildElement("name", xmlFileRoot.getNamespaceURI()) != null)
							if (((Element)parameterNodes.get(k)).getFirstChildElement("name", xmlFileRoot.getNamespaceURI()).getValue().trim().equals("const") &&
									parameterNodes.get(k).getValue().contains("&")){
								copyConstructorFound = true;
								break;
							}
				}
		return copyConstructorFound;
	}

	public boolean memberFunctionChangesAttribute(String functionName, String memberOfClass, Nodes attributeNameNodes, Nodes 
			inheritedAttributeNameNodes, Element functionElement, String functionBodyExprXPath, String functionCallXPath, 
			String functionCallArgumentsXPath, XPathContext context, Element factRootElement, String factClassXPath,
			String factSignatureXPath, String factParameterTypeNameXPath, String localVariableXPath, String parameterXPath){
		//check static method calls class::function(args)
		boolean satisfied;
		satisfied = attributeModificationInExpr(attributeNameNodes, inheritedAttributeNameNodes, functionElement,
				functionBodyExprXPath, context); //true if modified
		//if there is a non const method call and the corresponding params for the args are not passed as const it returns true
		if (!satisfied)
			satisfied = nonConstMethodCall(functionName, memberOfClass, functionElement, functionCallXPath, context, factRootElement, 
					factClassXPath, factSignatureXPath, attributeNameNodes, inheritedAttributeNameNodes,
					functionCallArgumentsXPath, factParameterTypeNameXPath, localVariableXPath, parameterXPath);
		return satisfied;
	}
	
	private boolean nonConstMethodCall(String functionName, String memberOfClass, Element functionElement, String functionCallXPath, 
			XPathContext context, Element factRootElement, String factClassXPath, String factSignatureXPath, 
			Nodes attributeNameNodes, Nodes inheritedAttributeNameNodes, String functionCallArgumentsXPath,
			String factParameterTypeNameXPath, String localVariableXPath, String parameterXPath){
		boolean satisfied = false;
		String className = memberOfClass;//only true for member functions
		String calledFunctionName="";
		Nodes functionCallArgumentNodes;
		
		Nodes functionCalls = functionElement.query(functionCallXPath, context);
		for (int i=0; i<functionCalls.size(); i++){
			if (functionCalls.get(i) instanceof Element) 
				if ( ((Element)functionCalls.get(i)).getFirstChildElement("name", 
						((Element)functionCalls.get(i)).getNamespaceURI()) !=  null ){
					
					calledFunctionName = ((Element)functionCalls.get(i)).getFirstChildElement("name", 
						((Element)functionCalls.get(i)).getNamespaceURI()).getValue().trim();
					if (calledFunctionName.contains("::"))
						continue;
					functionCallArgumentNodes = functionCalls.get(i).query(functionCallArgumentsXPath, context);
					//Check if the argument is this - the current object
					satisfied = thisObjectPassed(functionCallArgumentNodes);
					if (satisfied) break;
					//Here there are two function call types member and objects function
					if (!isMemberFunction(functionCalls.get(i))){
						className = objectType (functionCalls.get(i), functionElement, localVariableXPath, parameterXPath, context);
						if (className.trim().equals("")) 
							satisfied = true;//Assumes the called method of the object (whose type not known) is not const
						else
							satisfied = attributeModificationByObjMethodCall(attributeNameNodes,inheritedAttributeNameNodes, 
								functionName, functionCallArgumentNodes, context, className, factRootElement, 
								factClassXPath, factSignatureXPath, factParameterTypeNameXPath);
					}
					else//cascade effect of member method calls
						satisfied = isConstMemberMethodCalled(factRootElement, factClassXPath,
							factSignatureXPath, className, calledFunctionName, argumentSize(functionCallArgumentNodes));
						
				}
			if (satisfied) break;
		}
		
		return satisfied;
	}
	
	private boolean thisObjectPassed(Nodes functionCallArgumentNodes){
		boolean thisAsArgument = false;
		Element nameElt;
		
		if (functionCallArgumentNodes != null)
			for (int i=0; i<functionCallArgumentNodes.size(); i++){
				if (functionCallArgumentNodes.get(i) instanceof Element){
					nameElt = ((Element)functionCallArgumentNodes.get(i)).getFirstChildElement("name", 
							((Element)functionCallArgumentNodes.get(i)).getNamespaceURI());
					if (nameElt != null){
						if ("this".equals(nameElt.getValue().trim()))
							thisAsArgument = true;
						break;
					}
				}
			}
		
		return thisAsArgument;
	}
	
	private int argumentSize(Nodes functionCallArgumentNodes){
		int numberOfArguments = 0;
		
		for (int i=0; i<functionCallArgumentNodes.size(); i++)
			if (functionCallArgumentNodes.get(i) instanceof Element)
				numberOfArguments ++;
		
		return numberOfArguments;
	}
	private boolean isMemberFunction(Node functionCall){
		boolean memberFunction = false;
		ParentNode callParentNode;
		int indexOfCallFunction;
		
		if (((Element)functionCall.getParent()).getQualifiedName().equals("expr")){
			callParentNode = functionCall.getParent();
			indexOfCallFunction = callParentNode.indexOf(functionCall);
			if (indexOfCallFunction == 0)
				memberFunction = true;
			else if (indexOfCallFunction > 0){
				if (callParentNode.getChild(indexOfCallFunction -1) instanceof Text){
					if (!(callParentNode.getChild(indexOfCallFunction -1).getValue().trim().equals(".") 
							|| callParentNode.getChild(indexOfCallFunction -1).getValue().trim().equals("->")))
						memberFunction = true;
					if (indexOfCallFunction > 1 && !memberFunction){
						if (callParentNode.getChild(indexOfCallFunction -2) instanceof Element){
							if (callParentNode.getChild(indexOfCallFunction -2).getValue().trim().equals("this"))
								memberFunction = true;
						}
					}
				}
				else
					memberFunction = true;
			}
		}
		
		return memberFunction;
	}
	
	private String objectType (Node functionCall, Element functionElement, String localVariableXPath, String parameterXPath,
			XPathContext context){
		String objectType ="";
		ParentNode callParentNode;
		int indexOfCallFunction;
		String objectName ="";
		Nodes variableDeclNodes;
		
		if (((Element)functionCall.getParent()).getQualifiedName().equals("expr")){
			callParentNode = functionCall.getParent();
			indexOfCallFunction = callParentNode.indexOf(functionCall);
			if (indexOfCallFunction >=2 )
				if (callParentNode.getChild(indexOfCallFunction -1) instanceof Text){
					if ((callParentNode.getChild(indexOfCallFunction -1).getValue().trim().equals(".") 
							|| callParentNode.getChild(indexOfCallFunction -1).getValue().trim().equals("->")))
						if (callParentNode.getChild(indexOfCallFunction -2) instanceof Element )
							if (((Element)callParentNode.getChild(indexOfCallFunction -2)).getQualifiedName().equals("name"))
								objectName = callParentNode.getChild(indexOfCallFunction -2).getValue().trim();
				}
		}
		
		if (!objectName.equals("")){
			variableDeclNodes = functionElement.query(localVariableXPath, context);
			if (variableDeclNodes != null)
				for (int i = 0; i < variableDeclNodes.size(); i++)
					objectType = getObjectType(objectName, variableDeclNodes.get(i));
			if (objectType.equals("")){
				variableDeclNodes = functionElement.query(parameterXPath, context);
				if (variableDeclNodes != null)
					for (int i = 0; i < variableDeclNodes.size(); i++)
						objectType = getObjectType(objectName, variableDeclNodes.get(i));
			}
				
		}
		
		return objectType;
	}

	private String getObjectType(String objectName,
			Node variableDecl) {
		String objectType="";
		Element typeElement;
		Elements nameElements;
		if (variableDecl instanceof Element){
			nameElements = ((Element)variableDecl).getChildElements("name", 
					((Element) variableDecl).getNamespaceURI());
			for (int j=0; j<nameElements.size(); j++)
				if (nameElements.get(j).getValue().trim().equals(objectName)){
					typeElement = ((Element)variableDecl).getFirstChildElement("type", 
							((Element) variableDecl).getNamespaceURI());
					if (typeElement.getChildElements("name", typeElement.getNamespaceURI()).size()>0)
						objectType =typeElement.getChildElements("name", typeElement.getNamespaceURI())
							.get(typeElement.getChildElements("name", typeElement.getNamespaceURI()).size()-1)
							.getValue().trim();
				}
		}
		return objectType;
	}

	private boolean isConstMemberMethodCalled(Element factRootElement, String factClassXPath, String factSignatureXPath,
			String className, String calledFunctionName, int functionCallArgumentsNumber) {
		boolean satisfied = false;
		boolean matchingMethodInFactFileFound = false;
		if (factRootElement != null){
			Nodes classNodes = factRootElement.query(factClassXPath + "[@name='" + className.trim() + "']");
			
			if ( classNodes != null )
				for (int j=0; j<classNodes.size(); j++){
					Nodes methodNodes = classNodes.get(j).query(factSignatureXPath);
					
					if (methodNodes != null)
						for (int k=0; k<methodNodes.size(); k++)
							if (methodNodes.get(k) instanceof Element){
								if ( ((Element)methodNodes.get(k)).getFirstChildElement("name").getValue().trim()
										.equals(calledFunctionName) && this.getParameterCount(methodNodes.get(k)) 
										== functionCallArgumentsNumber ){
									matchingMethodInFactFileFound = true;
									if (((Element)methodNodes.get(k)).getFirstChildElement("specifier") != null){
										if (!((Element)methodNodes.get(k)).getFirstChildElement("specifier").getValue().trim().equals("const")){
											//The function called is not const 
											satisfied = true;
											break;
										}													
									}
									else {
										satisfied = true;
										break;
									}
								}
							}
					if (satisfied) break;
				}
		}
		if (!matchingMethodInFactFileFound)
			satisfied = true; //Assumes the called method is not const, if not found if the fact file
		return satisfied;
	}

	private boolean attributeModificationByObjMethodCall(Nodes attributeNameNodes,Nodes inheritedAttributeNameNodes, 
			String methodName, Nodes functionCallArgumentNodes, XPathContext context,
			String memberOfClass, Element factRootElement, String factClassXPath, String factSignatureXPath,
			String factParameterTypeNameXPath) {
		
		boolean refOrPtrUsed = false;

		for (int argumentIndex=0; argumentIndex<functionCallArgumentNodes.size(); argumentIndex++)
			if (functionCallArgumentNodes.get(argumentIndex) instanceof Element){
				Element argumentElement;
				String argumentName;
				if (((Element) functionCallArgumentNodes.get(argumentIndex)).getFirstChildElement("name", 
					((Element) functionCallArgumentNodes.get(argumentIndex)).getNamespaceURI() ) == null)
					continue;
				else
					argumentElement = ((Element) functionCallArgumentNodes.get(argumentIndex)).getFirstChildElement("name", 
										((Element) functionCallArgumentNodes.get(argumentIndex)).getNamespaceURI());
					if (argumentElement.getFirstChildElement("name", argumentElement.getNamespaceURI()) != null)
						argumentName = argumentElement.getFirstChildElement("name", argumentElement.getNamespaceURI()).getValue().trim();
					else
						argumentName = argumentElement.getValue().trim();
								
					for (int k=0; k<attributeNameNodes.size(); k++)
						if (argumentName.equals(attributeNameNodes.get(k).getValue().trim())){
							//check if the corresponding formal parameter is a reference or pointer 
							refOrPtrUsed = checkParameterType(factRootElement, memberOfClass, methodName,
										argumentIndex, this.argumentSize(functionCallArgumentNodes), factClassXPath, 
										factSignatureXPath, factParameterTypeNameXPath);
							break;
							
						}
					if (!refOrPtrUsed){
						for (int k=0; k<inheritedAttributeNameNodes.size(); k++)
							if (argumentName.equals(inheritedAttributeNameNodes.get(k).getValue().trim())){
								//check if the corresponding formal parameter is a reference or pointer 
								refOrPtrUsed = checkParameterType(factRootElement, memberOfClass, methodName,
										argumentIndex, this.argumentSize(functionCallArgumentNodes), factClassXPath, 
										factSignatureXPath, factParameterTypeNameXPath);
								break;
							}
					}
				if (refOrPtrUsed) break;
			}
		return refOrPtrUsed;
	}
	
	
	
	private int getParameterCount (Node signatureNode){
		int parameterCount=0;
		if (signatureNode instanceof Element){
			Element signatureElement =  (Element)signatureNode;
			Elements parameterListElements = signatureElement.getChildElements("parameter_list/param");
			
			if (parameterListElements != null)
				parameterCount = parameterListElements.size();
		}
		return parameterCount;
	}
	
	
	private boolean attributeModificationInExpr(Nodes attributeNameNodes, Nodes inheritedAttributeNameNodes, 
			Element functionElement, String functionBodyExprXPath, XPathContext context) {
		boolean satisfied = false;
		boolean fileStream = false;
		Nodes functionBodyExprNodes = functionElement.query(functionBodyExprXPath, context);
		
		for (int i=0; i<functionBodyExprNodes.size(); i++){
			if (functionBodyExprNodes.get(i) instanceof Element){
				if (((Element)functionBodyExprNodes.get(i).getParent()).getQualifiedName().equals("init")
						|| ((Element)functionBodyExprNodes.get(i).getParent()).getQualifiedName().equals("condition"))
					continue;
				
				satisfied = attributeInLeftSideOfExpr(functionBodyExprNodes.get(i), attributeNameNodes,
						inheritedAttributeNameNodes);
				
				if (functionBodyExprNodes.get(i).getValue().contains("<<"))
					fileStream = true;
				
				if (!satisfied && (functionBodyExprNodes.get(i).getValue().contains(">>") //consol input stream
						|| functionBodyExprNodes.get(i).getValue().contains("<<")) ) //File Input stream 
					satisfied = attributeModificationByInputStream(functionBodyExprNodes.get(i), attributeNameNodes,
							inheritedAttributeNameNodes, fileStream);
				
				if (satisfied)
					break;
			}
		}
		return satisfied;
	}
	
	private boolean attributeModificationByInputStream(Node functionBodyExprNode, Nodes attributeNameNodes,
			Nodes inheritedAttributeNameNodes, boolean fileStream){
		boolean satisfied = false;
		Elements inputStreamNameElements;
		String rightSideExprName;
		
		inputStreamNameElements = ((Element)functionBodyExprNode).getChildElements("name", 
				((Element)functionBodyExprNode).getNamespaceURI());
		
		for (int i=0; i<inputStreamNameElements.size(); i++){
			if (inputStreamNameElements.get(i).getQualifiedName().equals("name")){
				rightSideExprName = inputStreamNameElements.get(i).getValue().trim();
				if (fileStream && i > 0)
					break;
				
				for (int j=0; j<attributeNameNodes.size(); j++)
					if (rightSideExprName.equals(attributeNameNodes.get(j).getValue().trim())){
						satisfied = true;
						break;
					}
					
				if (!satisfied){
					for (int k=0; k<inheritedAttributeNameNodes.size(); k++)
						if (rightSideExprName.equals(inheritedAttributeNameNodes.get(k).getValue().trim())){
							satisfied = true;
							break;
						}
				}
				
				if (satisfied)
					break;
			}
		}
		
		return satisfied;
	}
	
	private boolean attributeInLeftSideOfExpr(Node functionBodyExprNode, Nodes attributeNameNodes,
			Nodes inheritedAttributeNameNodes){
		boolean satisfied = false;
		Element leftSideOfExpr;
		String leftSideOfExprName;
		
		leftSideOfExpr = leftSideOfExpr(functionBodyExprNode);
		if (leftSideOfExpr != null){
			if (leftSideOfExpr.getFirstChildElement("name", leftSideOfExpr.getNamespaceURI()) != null )
				leftSideOfExprName = leftSideOfExpr.getFirstChildElement("name", 
						leftSideOfExpr.getNamespaceURI()).getValue().trim();
			else
				leftSideOfExprName = leftSideOfExpr.getValue().trim();
			
			for (int j=0; j<attributeNameNodes.size(); j++)
				if (leftSideOfExprName.equals(attributeNameNodes.get(j).getValue().trim())){
					satisfied = true;
					break;
				}
				
			if (!satisfied){
				for (int k=0; k<inheritedAttributeNameNodes.size(); k++)
					if (leftSideOfExprName.equals(inheritedAttributeNameNodes.get(k).getValue().trim())){
						satisfied = true;
						break;
					}
			}
		}
		return satisfied;
	}
	
	
	public boolean dummyArgument(String functionOfClass, String functionName, Nodes parameterTypeNodes, 
			int numberOfParamName, Element factRootElement, String factClassXPath, String factSignatureXPath,
			String factFunctionParameterTypeXPath, String factFunctionParameterNameXPath, Node functionNode, 
			String exprOnlyXPath, String nameXPath, XPathContext context){
		boolean matchingSignatureFound = false;
		boolean dummyArgumentRequired = false;
		String parameterName="";
		
		Nodes factClassNodes = factRootElement.query(factClassXPath + "[@name=\"" + functionOfClass.trim() + "\"]");
		for (int l=0; l<factClassNodes.size(); l++){
			if (factClassNodes.get(l) instanceof Element){
				Nodes functionDecl =((Element)factClassNodes.get(l)).query(factSignatureXPath);
				for (int m=0; m<functionDecl.size(); m++)
					if (functionDecl.get(m) instanceof Element)
						if (((Element)functionDecl.get(m)).getFirstChildElement("name")!=null){
							if (!((Element)functionDecl.get(m)).getFirstChildElement("name")
									.getValue().trim().equals(functionName.trim()))
								continue;
							
							Nodes signatureParamTypeNodes = ((Element)functionDecl.get(m)).query(factFunctionParameterTypeXPath);
							if (countNumberOfNonVoidTypes(parameterTypeNodes) == countNumberOfNonVoidTypes(signatureParamTypeNodes)){
								for (int i=0; i<countNumberOfNonVoidTypes(parameterTypeNodes); i++)
									if ((parameterTypeNodes.get(i) instanceof Element) && (signatureParamTypeNodes.get(i) instanceof Element)){
										
										String paramTypeInImp = parameterTypeNodes.get(i).getValue().replace(" ", "").trim();
										String paramTypeInSig = signatureParamTypeNodes.get(i).getValue().replace(" ", "").trim();
										
										if (!paramTypeInImp.equals(paramTypeInSig) ){
											matchingSignatureFound = false;
											dummyArgumentRequired = false;
											break;
										}
										else{
											matchingSignatureFound = true;
											Element parameterNameElement = ((Element)parameterTypeNodes.get(i).getParent()).getFirstChildElement("name", ((Element)parameterTypeNodes.get(i).getParent()).getNamespaceURI());
											Element factParameterNameElement = ((Element)signatureParamTypeNodes.get(i).getParent()).getFirstChildElement("name");
											if (parameterNameElement != null && factParameterNameElement == null){
												//check if the parameter name is used in the function here. B/c there is no dummy parameter in the signature
												if (parameterNameElement.getFirstChildElement("name", parameterNameElement.getNamespaceURI()) == null)
													parameterName = parameterNameElement.getValue().trim();
												else
													parameterName = parameterNameElement.getFirstChildElement("name", parameterNameElement.getNamespaceURI()).getValue().trim();
												if (!dummyArgumentRequired)
													dummyArgumentRequired = parameterUsedInExpr(parameterName, functionNode, exprOnlyXPath, nameXPath, context);
												//if parameter name is used dummyArgumentRequired = true;
											}//param and signature param names match/not
											
										}//parameter types are the same
									}//parameter types are instance of element
									else{
										matchingSignatureFound = false;
										dummyArgumentRequired = false;
									}
							}//number of parameter types matches
							else {
								matchingSignatureFound = false;
								dummyArgumentRequired = false;
							}
							
							if (matchingSignatureFound)
								break;
					}//Function for loop
					if (matchingSignatureFound)
						break;
				}//class if element
			}//class without if
		//If theres no matching signature, we assume that it might exist in files not included in fact file.
		return dummyArgumentRequired;
	}
	
	private boolean parameterUsedInExpr(String parameterName, Node functionNode, String exprOnlyXPath, String nameXPath, XPathContext context){
		boolean parameterUsed = false;
		Nodes exprNameNodes  = functionNode.query(exprOnlyXPath + "/" + nameXPath, context);
		String exprName;
		
		for (int i = 0; i<exprNameNodes.size(); i++)
			if (exprNameNodes.get(i) instanceof Element){
				if (((Element)exprNameNodes.get(i)).getFirstChildElement("name", ((Element)exprNameNodes.get(i)).getNamespaceURI()) != null)
					exprName = ((Element)exprNameNodes.get(i)).getFirstChildElement("name", ((Element)exprNameNodes.get(i)).getNamespaceURI()).getValue().trim();
				else
					exprName = exprNameNodes.get(i).getValue().trim();
				
				if (exprName.equals(parameterName.trim())){
					parameterUsed = true;
					break;
				}
			}
		return parameterUsed;
	}
	private int countNumberOfNonVoidTypes(Nodes parameterTypeNode){
		int numberOfTypes = 0;
		
		for (int i=0; i< parameterTypeNode.size(); i++)
			if (parameterTypeNode.get(i) instanceof Element){
				if (!parameterTypeNode.get(i).getValue().trim().equals("void"))
					numberOfTypes++;
			}
		return numberOfTypes;
	}
	
	public boolean attributeScope(Element xmlFileRoot, String classBlockXPath, XPathContext context){
		
		boolean satisfied = true;
		Nodes classBlockNodes = xmlFileRoot.query(classBlockXPath, context);
		
		for (int k=0; k<classBlockNodes.size(); k ++)
			if (classBlockNodes.get(k) instanceof Element){
				Elements accessSpecifierElements=  ((Element)classBlockNodes.get(k)).getChildElements();
		
				for (int i=0; i<accessSpecifierElements.size(); i++){
						Element accessSpecifierElement = (Element)accessSpecifierElements.get(i);
						if ( accessSpecifierElement.getQualifiedName().equals("public"))
							for (int j=0; j<accessSpecifierElement.getChildCount(); j++)
								if (accessSpecifierElement.getChild(j) instanceof Element)
									if ( ((Element)accessSpecifierElement.getChild(j)).getQualifiedName().equals("decl_stmt") ){
										//|| ((Element)accessSpecifierElement.getChild(j)).getQualifiedName().equals("expr_stmt")){
										satisfied = false;
										break;
									}
						if (!satisfied)
							break;
					}
			}
				
		return satisfied;
	}
	
	public boolean structFound (Element xmlFileRoot, String structXPath, XPathContext context){

		boolean satisfied = false;
		Nodes structNodes = xmlFileRoot.query(structXPath, context);
		
		for (int i=0; i<structNodes.size(); i++)
			if (structNodes.get(i) instanceof Element){
				satisfied = true;
				break;
			}
		
		return satisfied;
	}
	
	public String searchForbiddenString(Element xmlFileRoot, String forbiddenString){
		String lineNumbers ="";
		int lineNumber=0;
		String actualCode = xmlFileRoot.getValue();
		int lineEnd;
		int lineBegin = 0;
		String codeSubContent;
		
		if (actualCode.contains("\n"))
			lineEnd = actualCode.indexOf('\n');
		else
			lineEnd = actualCode.length()-1;
		
		forbiddenString = forbiddenString.toLowerCase().replaceAll(" ", "");
			
		while(lineEnd >= 0 && lineEnd < actualCode.length()){
			lineNumber = lineNumber+1;
			codeSubContent = actualCode.substring(lineBegin, lineEnd);
			lineBegin = lineEnd +1;
			lineEnd = actualCode.indexOf('\n', lineBegin);
			
			codeSubContent = codeSubContent.toLowerCase().replaceAll(" ", "");	
			if (codeSubContent.matches(".*\".*" +forbiddenString + ".*\".*") 
					&& !(codeSubContent.startsWith("//") || codeSubContent.startsWith("*") || codeSubContent.startsWith("/*") )){
				if (lineNumbers.equals(""))
					lineNumbers = Integer.toString(lineNumber);
				else
					lineNumbers = lineNumbers +", " + Integer.toString(lineNumber);
			}
		}
			
		return lineNumbers;
	}
	
	public int attributeNotRedefined (String attributeName, Nodes attributesInSuperClasses){
		int satisfied = -1;
		
		for (int i=0; i<attributesInSuperClasses.size(); i++)
			if (attributesInSuperClasses.get(i).getValue().trim().equals(attributeName.trim())){
				satisfied = i;
				break;
			}
		
		return satisfied;
	}
	
	/**
	 * constParameterType returns an integer (-1 to 4) which tells the possible position of const to a parameter
	 * 
	 * @param parameterNode
	 * @param functionNode
	 * @param constPosition : 1 ='const type & param'; 2 ='type * const param'; 3 ='const type * param'; 
	 * 						  4= 'const type * const param';
	 * @param functionBodyExprXPath
	 * @param context
	 * @return -1, if parameter has const and doesn't need to add any other const 
	 * 			0, if parameter doesn't need to be declared using const
	 * 			1, if parameter should have a form 'const type & param'
	 * 			2, if parameter should have a form 'type * const param'
	 * 			3, if parameter should have a form 'const type * param'
	 * 			4, if parameter should have a form 'const type * const param'
	 */
	public int constParameterType (Node parameterNode, Node functionNode, int constPosition, String localVariableXPath,
			String memberOfClass, String functionBodyExprXPath, String functionCallXPath, String functionCallArgumentsXPath,
			XPathContext context, Element factRootElement, String factClassXPath, String factSignatureXPath, 
			String factFunctionParameterTypeXPath, String typeXPath, String nameXPath, String parameterXPath, Nodes attributeNameNodes,
			String returnExprXPath){
		int newConstPosition = 0;
		String parameterNameToBeChecked ="";
		Element parameterNameElement=null;
		
		
		if (parameterNode instanceof Element){
			parameterNameElement = ((Element)parameterNode).getFirstChildElement("name", 
					((Element)parameterNode).getNamespaceURI());
			if (parameterNameElement != null)
				if (parameterNameElement.getFirstChildElement("name", parameterNameElement.getNamespaceURI()) != null)
					parameterNameToBeChecked = parameterNameElement.getFirstChildElement("name", parameterNameElement.getNamespaceURI()).getValue().trim();
				else
					parameterNameToBeChecked = parameterNameElement.getValue().trim();
		}
		
		if (!parameterNameToBeChecked.trim().equals(""))
			if (isReferenceParameter(parameterNode)){
				if (constPosition == 1)
					newConstPosition = -1;
				else if (constPosition == 0){
					if (!refPtrModificationByAssignment(functionNode, functionBodyExprXPath, context, 
							parameterNameToBeChecked, false))
						if (!methodInvocation(functionNode, functionBodyExprXPath, context, parameterNameToBeChecked,
								functionCallArgumentsXPath, factRootElement, factClassXPath, factSignatureXPath, memberOfClass, true))
							if (!passedToFunction(parameterNameToBeChecked, memberOfClass, functionNode, 
									functionCallXPath, context, factRootElement, factClassXPath, factSignatureXPath, 
									functionCallArgumentsXPath, factFunctionParameterTypeXPath, parameterXPath, localVariableXPath, true))
								newConstPosition = 1;
				}
			}
			else if (isPointerParameter(parameterNode)){
				if (constPosition == 0 ){
					if (!refPtrModificationByAssignment(functionNode, functionBodyExprXPath, context, 
							parameterNameToBeChecked, false)) //type * const param
						if (!passedToFunction(parameterNameToBeChecked, memberOfClass, functionNode, 
								functionCallXPath, context, factRootElement, factClassXPath, factSignatureXPath, 
								functionCallArgumentsXPath, factFunctionParameterTypeXPath, parameterXPath, localVariableXPath, false))
							newConstPosition = 2;
					
					if (!refPtrModificationByAssignment(functionNode, functionBodyExprXPath, context, 
							parameterNameToBeChecked, true))//const type * param
						if (!passedToFunction(parameterNameToBeChecked, memberOfClass, functionNode, 
								functionCallXPath, context, factRootElement, factClassXPath, factSignatureXPath, 
								functionCallArgumentsXPath, factFunctionParameterTypeXPath, parameterXPath, localVariableXPath, false))
							if (!methodInvocation(functionNode, functionBodyExprXPath, context, parameterNameToBeChecked, 
									functionCallArgumentsXPath, factRootElement, factClassXPath, factSignatureXPath, memberOfClass, false))
								if (!assignedToPointer (parameterNode, functionNode, localVariableXPath, functionBodyExprXPath, 
										attributeNameNodes, context))
									if (!usedInReturn (parameterNameToBeChecked, functionNode, returnExprXPath, context))
										if ( newConstPosition == 0 )
											newConstPosition = 3;
										else if (newConstPosition == 2)
											newConstPosition = 4;
				}	
			}
		return newConstPosition;
	}
	
	private boolean usedInReturn (String parameterNameToBeChecked, Node functionNode, String returnExprXPath, XPathContext context){
		boolean parameterReturned = false;
		String returnedVar;
		Elements returnExprNameElements;
		Nodes returnedNames = functionNode.query(returnExprXPath, context);
		
		if (returnedNames != null){
			for (int i= 0; i<returnedNames.size(); i++)
				if (returnedNames.get(i) instanceof Element){
					returnExprNameElements = ((Element)returnedNames.get(i)).getChildElements("name", ((Element)returnedNames.get(i)).getNamespaceURI());
					if (returnExprNameElements != null)
						for (int j=0; j<returnExprNameElements.size(); j++){
							if (returnExprNameElements.get(j).getFirstChildElement("name", returnExprNameElements.get(j).getNamespaceURI()) != null)
								returnedVar = returnExprNameElements.get(j).getFirstChildElement("name", returnExprNameElements.get(j).getNamespaceURI()).getValue().trim();
							else
								returnedVar = returnExprNameElements.get(j).getValue().trim();
							if (returnedVar.equals(parameterNameToBeChecked)){
								parameterReturned = true;
								break;
							}
						}
				}
		}
		
		return parameterReturned;
	}
	
	private boolean assignedToPointer (Node parameterNode, Node functionNode, String localVariableXPath, String functionBodyExprXPath,
			Nodes attributeNameNodes, XPathContext context){
		boolean satisfied = false;
		Element parameterNameElement;
		String parameterName;
		Nodes localVariableNodes;
		Element localPtrNameElement;
		String localPtrName;
		
		if (((Element)parameterNode).getFirstChildElement("name", ((Element)parameterNode).getNamespaceURI()) != null){
			parameterNameElement = ((Element)parameterNode).getFirstChildElement("name", ((Element)parameterNode).getNamespaceURI());
			if (parameterNameElement.getFirstChildElement("name", parameterNameElement.getNamespaceURI()) != null)
				parameterName = parameterNameElement.getFirstChildElement("name", parameterNameElement.getNamespaceURI()).getValue().trim();
			else
				parameterName = parameterNameElement.getValue().trim();
			
			localVariableNodes = functionNode.query(localVariableXPath, context);
			
			if (localVariableNodes != null)
				for (int i=0; i<localVariableNodes.size(); i++){
					if (!(localVariableNodes.get(i) instanceof Element))
						continue;
					if (((Element)localVariableNodes.get(i)).getFirstChildElement("type", ((Element)localVariableNodes.get(i))
							.getNamespaceURI()) == null)
						continue;
					if (!((Element)localVariableNodes.get(i)).getFirstChildElement("type", ((Element)localVariableNodes.get(i))
							.getNamespaceURI()).getValue().contains("*"))
						continue;
					if (((Element)localVariableNodes.get(i)).getFirstChildElement("name", ((Element)localVariableNodes.get(i))
							.getNamespaceURI()) == null)
						continue;
					localPtrNameElement = ((Element)localVariableNodes.get(i)).getFirstChildElement("name", ((Element)localVariableNodes
							.get(i)).getNamespaceURI());
					if(localPtrNameElement.getFirstChildElement("name", localPtrNameElement.getNamespaceURI()) != null)
						localPtrName = localPtrNameElement.getFirstChildElement("name", localPtrNameElement.getNamespaceURI()).getValue()
						.trim();
					else
						localPtrName = localPtrNameElement.getValue().trim();
					
					satisfied = isPtrParameterAssigned(parameterName, localPtrName, localVariableNodes.get(i), functionNode, functionBodyExprXPath, context);
					if (satisfied) break;
				}
			
			if (attributeNameNodes != null && !satisfied)
				for (int i=0; i<attributeNameNodes.size(); i++){
					
					if (!(attributeNameNodes.get(i) instanceof Element))
						continue;
					localPtrNameElement = ((Element)attributeNameNodes.get(i));
					if(localPtrNameElement.getFirstChildElement("name", localPtrNameElement.getNamespaceURI()) != null)
						localPtrName = localPtrNameElement.getFirstChildElement("name", localPtrNameElement.getNamespaceURI()).getValue()
						.trim();
					else
						localPtrName = localPtrNameElement.getValue().trim();
					
					satisfied = isPtrParameterAssigned(parameterName, localPtrName, attributeNameNodes.get(i), functionNode, functionBodyExprXPath, context);
					if (satisfied) break;
				}
		
		}
		
		return satisfied;
	}
	
	private boolean isPtrParameterAssigned(String parameterName, String localPtrName, Node localVariableNode, Node functionNode, 
			String functionBodyExprXPath, XPathContext context){
		boolean satisfied = false;
		Nodes functionBodyExprNodes;
		String functionBodyExprElementName;
		//Assigned to pointer during declaration - initialization
		if (((Element)localVariableNode).getFirstChildElement("init", ((Element)localVariableNode).getNamespaceURI()) != null)
			if( ((Element)localVariableNode).getFirstChildElement("init", ((Element)localVariableNode).getNamespaceURI()).getValue().trim().equals(parameterName))
				satisfied = true;
		if (!satisfied){
			functionBodyExprNodes = functionNode.query(functionBodyExprXPath, context);
			if (functionBodyExprNodes != null){
				for (int j=0; j<functionBodyExprNodes.size(); j++){
					if (!(functionBodyExprNodes.get(j) instanceof Element))
						continue;
					
					if (((Element)functionBodyExprNodes.get(j).getParent()).getQualifiedName().equals("init") 
							|| ((Element)functionBodyExprNodes.get(j).getParent()).getQualifiedName().equals("condition"))
						continue;
					
					if (((Element)functionBodyExprNodes.get(j)).getFirstChildElement("name", 
							((Element)functionBodyExprNodes.get(j)).getNamespaceURI()) == null)
						continue;
					int assignmentFormat = 0; //0= initial, 1= 0 and localParamMatched, 2 =  1 and assignemnt(=) operator, and 3= 2 and param matched
					for (int k=0; k<functionBodyExprNodes.get(j).getChildCount(); k++){
						if ((functionBodyExprNodes.get(j).getChild(k) instanceof Element))
							if (((Element)functionBodyExprNodes.get(j).getChild(k)).getFirstChildElement("name", ((Element)functionBodyExprNodes
									.get(j).getChild(k)).getNamespaceURI())!=null)
								functionBodyExprElementName = ((Element)functionBodyExprNodes.get(j).getChild(k)).getFirstChildElement("name", 
										((Element)functionBodyExprNodes.get(j).getChild(k)).getNamespaceURI()).getValue().trim();
							else
								functionBodyExprElementName = functionBodyExprNodes.get(j).getChild(k).getValue().trim();
						else
							functionBodyExprElementName = functionBodyExprNodes.get(j).getChild(k).getValue().trim();
								
						if ((functionBodyExprNodes.get(j).getChild(k) instanceof Element) 
								&& assignmentFormat == 0 && functionBodyExprElementName.equals(localPtrName))
							assignmentFormat = 1;
						if ((functionBodyExprNodes.get(j).getChild(k) instanceof Text) 
								&& assignmentFormat == 1 && functionBodyExprElementName.equals("="))
							assignmentFormat = 2;
						if ((functionBodyExprNodes.get(j).getChild(k) instanceof Element) 
								&& assignmentFormat == 2 && functionBodyExprElementName.equals(parameterName))
							assignmentFormat = 3;
						else if ((functionBodyExprNodes.get(j).getChild(k) instanceof Element) 
								&& assignmentFormat == 2 && !functionBodyExprElementName.equals(parameterName))
							break;
					}
					if (assignmentFormat == 3){
						satisfied = true;
						break;
					}
				}
			}
		}
		return satisfied;
	}
	
	private boolean passedToFunction(String parameterNameToBeChecked, String memberOfClass, Node functionNode, 
			String functionCallXPath, XPathContext context, Element factRootElement, String factClassXPath, 
			String factSignatureXPath, String functionCallArgumentsXPath, String factFunctionParameterTypeXPath, 
			String parameterXPath, String localVariableXPath, boolean byReference){
		boolean satisfied = false;
		
		boolean parameterPassedToFunction;
		int passedParameterIndex=0;
		String calledFunctionName="";
		String className = memberOfClass;
		Element argumentNameElement ;
		String argumentName;
		boolean asAReference = byReference;
	
		Nodes functionCalls = functionNode.query(functionCallXPath, context);
		for (int i=0; i<functionCalls.size(); i++){
			if (functionCalls.get(i) instanceof Element) 
				if ( ((Element)functionCalls.get(i)).getFirstChildElement("name", 
						((Element)functionCalls.get(i)).getNamespaceURI()) !=  null ){
					
					calledFunctionName = ((Element)functionCalls.get(i)).getFirstChildElement("name", 
						((Element)functionCalls.get(i)).getNamespaceURI()).getValue().trim();
					
					Nodes functionCallArgumentNodes =  functionCalls.get(i).query(functionCallArgumentsXPath, context);
					if (functionCallArgumentNodes.size() == 0)
						continue;
					
					parameterPassedToFunction = false;
					for (int l=0; l<functionCallArgumentNodes.size(); l++)
						if (functionCallArgumentNodes.get(l) instanceof Element)
							if (((Element)functionCallArgumentNodes.get(l)).getFirstChildElement("name", 
									((Element)functionCallArgumentNodes.get(l)).getNamespaceURI())!= null){
								asAReference = byReference;
								argumentNameElement = ((Element)functionCallArgumentNodes.get(l)).getFirstChildElement("name", ((Element)functionCallArgumentNodes
										.get(l)).getNamespaceURI());
								if (argumentNameElement.getFirstChildElement("name", ((Element)functionCallArgumentNodes
										.get(l)).getNamespaceURI())!= null){
									argumentName = argumentNameElement.getFirstChildElement("name", ((Element)functionCallArgumentNodes
										.get(l)).getNamespaceURI()).getValue().trim();
									if (argumentNameElement.getValue().contains("[") && argumentNameElement.getValue().contains("]"))
										asAReference = true;
								}
								else
									argumentName = argumentNameElement.getValue().trim();
								
								if (argumentName.equals(parameterNameToBeChecked))
									if (!functionCallArgumentNodes.get(l).getValue().contains("*")){//is only value of the param which is passed?
										parameterPassedToFunction = true;
										passedParameterIndex = l;
										break;
									}
							}
					if (parameterPassedToFunction){
						//the called function may not be a member function so check ....
						if (!isMemberFunction(functionCalls.get(i)))
							className = objectType (functionCalls.get(i), (Element)functionNode, localVariableXPath, parameterXPath, context);
						//Now chk if the formal param is by ref or not and const or not ....
						satisfied = constReferencePtrFormalParameter(passedParameterIndex, factRootElement, factClassXPath, 
							className, factSignatureXPath, factFunctionParameterTypeXPath, calledFunctionName, 
							this.argumentSize(functionCallArgumentNodes), asAReference);
					}
				}
			if (satisfied) break;
		}
		
		return satisfied;
	}
	
	private boolean constReferencePtrFormalParameter (int passedParameterIndex, Element factRootElement, String factClassXPath, 
			String memberOfClass, String factSignatureXPath, String factFunctionParameterTypeXPath, 
			String calledFunctionName, int functionCallArgumentSize, boolean referenceCheck){
		boolean satisfied = false;
		boolean matchingMethodInFactFileFound = false;
		Nodes factClassNodes;
		Nodes factMethodNodes;
		Nodes factParameterTypeNodes;
		
		if (factRootElement != null){
			factClassNodes = factRootElement.query(factClassXPath + "[@name='" + memberOfClass.trim() + "']");
			
			if ( factClassNodes != null )
				for (int j=0; j<factClassNodes.size(); j++){
					factMethodNodes = factClassNodes.get(j).query(factSignatureXPath);
					
					if (factMethodNodes != null)
						for (int k=0; k<factMethodNodes.size(); k++)
							if (factMethodNodes.get(k) instanceof Element){
								if ( ((Element)factMethodNodes.get(k)).getFirstChildElement("name").getValue().trim()
										.equals(calledFunctionName) && this.getParameterCount(factMethodNodes.get(k)) 
										== functionCallArgumentSize ){
									matchingMethodInFactFileFound = true;
									
									factParameterTypeNodes = factMethodNodes.get(k).query(factFunctionParameterTypeXPath);
									
									for (int m=0; m<factParameterTypeNodes.get(passedParameterIndex).getChildCount(); m++ ){
										if (factParameterTypeNodes.get(passedParameterIndex).getChild(m) instanceof Text){
											if (factParameterTypeNodes.get(passedParameterIndex).getChild(m).getValue().trim().equals("&") 
													&& referenceCheck){//check the corresponding parameter for ref type??
												satisfied =  true;
												break;
											}
											if (factParameterTypeNodes.get(passedParameterIndex).getChild(m).getValue().trim().equals("*")
													&& !referenceCheck){
												satisfied =  true;
												break;
											}
										}
									}
									
									if (satisfied) {
										int constNumber = constNumber(factParameterTypeNodes.get(passedParameterIndex));
										if ( (constNumber== 1 || constNumber == 2 )&& !referenceCheck)
											satisfied = false; //say const to only const type & var and const type * const var
									}
									if (satisfied) break;
								}
								if (satisfied) 	break;
							}
					if (satisfied) break;
				}
		}
		if (!matchingMethodInFactFileFound)
			satisfied = true; //Assumes methods not found in fact file might use by ref or as ptr, without const
		return satisfied;
	}
	
	private int constNumber (Node factParameterTypeNode){
		int constCount = 0;
		boolean ptrSign = false;
		
		for (int i=0; i<factParameterTypeNode.getChildCount(); i++)
			if (factParameterTypeNode.getChild(i) instanceof Element){
				if (factParameterTypeNode.getChild(i).getValue().trim().equals("const"))
					if (!ptrSign)
						constCount = 1;
					else if (ptrSign && constCount == 1 )
						constCount = 3;
					else if (ptrSign && constCount == 0 )
						constCount = 2;
			}
			else if (factParameterTypeNode.getChild(i) instanceof Text)
				if (factParameterTypeNode.getChild(i).getValue().trim().equals("*") 
						|| factParameterTypeNode.getChild(i).getValue().trim().equals("&"))
					ptrSign = true;
		
		return constCount;
	}
	
	private boolean methodInvocation(Node functionNode, String functionBodyExprXPath, 
			XPathContext context, String parameterNameToBeChecked, String functionCallArgumentsXPath, Element factRootElement, 
			String factClassXPath, String factSignatureXPath, String className, boolean referenceParam ) {
		
		boolean satisfied = false;
		int indexOfCalledMethod;
		String pointerName;
		Nodes functionBodyExprNodes = functionNode.query(functionBodyExprXPath, context);
		int ptrRefFormat;
		
		for (int i=0; i<functionBodyExprNodes.size(); i++)
			if (functionBodyExprNodes.get(i) instanceof Element){
				
				if (((Element)functionBodyExprNodes.get(i)).getFirstChildElement("call", ((Element)functionBodyExprNodes.get(i)).getNamespaceURI()) == null)
					continue;
				
				for (int j = functionBodyExprNodes.get(i).getChildCount()-1; j >= 0; j--){
					ptrRefFormat = 0;
					indexOfCalledMethod = -2;
					if (functionBodyExprNodes.get(i).getChild(j) instanceof Element && ptrRefFormat == 0){
						if (((Element)functionBodyExprNodes.get(i).getChild(j)).getQualifiedName().equals("call")){
							ptrRefFormat = 1;
							indexOfCalledMethod = j;
						}
					}
					if (ptrRefFormat == 1 &&  indexOfCalledMethod > 1 ){
						if (functionBodyExprNodes.get(i).getChild(indexOfCalledMethod -1) instanceof Text ){
							if (referenceParam){
								if(functionBodyExprNodes.get(i).getChild(indexOfCalledMethod -1).getValue().trim().equals("."))
									ptrRefFormat = 2;
							}else
								if (functionBodyExprNodes.get(i).getChild(indexOfCalledMethod -1).getValue().trim().equals("->"))
									ptrRefFormat = 2;
						
						}
						if (functionBodyExprNodes.get(i).getChild(indexOfCalledMethod -2) instanceof Element && ptrRefFormat == 2){
							if (((Element)functionBodyExprNodes.get(i).getChild(indexOfCalledMethod -2)).getFirstChildElement("name",
									((Element)functionBodyExprNodes.get(i).getChild(indexOfCalledMethod -2)).getNamespaceURI()) != null)
								pointerName = ((Element)functionBodyExprNodes.get(i).getChild(indexOfCalledMethod -2)).getFirstChildElement("name",
									((Element)functionBodyExprNodes.get(i).getChild(indexOfCalledMethod -2)).getNamespaceURI()).getValue();
							else
								pointerName = functionBodyExprNodes.get(i).getChild(indexOfCalledMethod -2).getValue();
							if (pointerName.equals(parameterNameToBeChecked))
								ptrRefFormat = 3;
						}
					}
					if (ptrRefFormat == 3 ){
						satisfied = true;
						break;
					}
				}
				if (satisfied) break;
		}
		return satisfied;
	}

	
	private boolean refPtrModificationByAssignment(Node functionNode, String functionBodyExprXPath, 
			XPathContext context, String parameterNameToBeChecked, boolean dereferenced) {
		
		boolean satisfied = false;
		String leftSideOfExprName ="";
		Element leftSideOfExpr;
		Nodes functionBodyExprNodes = functionNode.query(functionBodyExprXPath, context);
		
		for (int i=0; i<functionBodyExprNodes.size(); i++){
			if (functionBodyExprNodes.get(i) instanceof Element){
				if (((Element)functionBodyExprNodes.get(i).getParent()).getQualifiedName().equals("init") 
						|| ((Element)functionBodyExprNodes.get(i).getParent()).getQualifiedName().equals("condition"))
					continue;
				
				leftSideOfExpr = leftSideOfExpr(functionBodyExprNodes.get(i));
				if (leftSideOfExpr == null)
					continue;
								
				if (leftSideOfExpr.getFirstChildElement("name", leftSideOfExpr.getNamespaceURI()) != null )
					leftSideOfExprName = leftSideOfExpr.getFirstChildElement("name", leftSideOfExpr.getNamespaceURI())
											.getValue().trim();
				else
					leftSideOfExprName = leftSideOfExpr.getValue().trim();
				
				if (!dereferenced){
					if (parameterNameToBeChecked.equals(leftSideOfExprName)){
						
						satisfied = true;
						break;
					}
				}else{
					if (leftSideOfExpr.getChildCount() > 0)
						if (leftSideOfExpr.getValue().trim().contains("*") 
								|| ( leftSideOfExpr.getValue().trim().contains("[") && leftSideOfExpr.getValue().trim().contains("]")) )
							if (parameterNameToBeChecked.equals(leftSideOfExprName)){
								satisfied = true;
								break;
							}
					}
				}
			}
		return satisfied;
	}
	
	private Element leftSideOfExpr(Node functionBodyExprNode){
		Element leftSideOfExpr = null;
		boolean nameTagFound = false;
		
		if (((Element)functionBodyExprNode).getFirstChildElement("name", ((Element)functionBodyExprNode).getNamespaceURI()) != null)
			
			for (int i=0; i<functionBodyExprNode.getChildCount(); i++){
				if (functionBodyExprNode.getChild(i) instanceof Element){
					if (((Element)functionBodyExprNode.getChild(i)).getQualifiedName().equals("name"))
						if (!((Element)functionBodyExprNode.getChild(i)).getValue().trim().equals("this")){
							nameTagFound = true;
							leftSideOfExpr = (Element)functionBodyExprNode.getChild(i);
						}
				}else if (functionBodyExprNode.getChild(i) instanceof Text && nameTagFound){
					if (functionBodyExprNode.getChild(i).getValue().trim().contains("=") 
							|| functionBodyExprNode.getChild(i).getValue().trim().contains("+")
							|| functionBodyExprNode.getChild(i).getValue().trim().contains("-")
							|| functionBodyExprNode.getChild(i).getValue().trim().equals("->")
							|| functionBodyExprNode.getChild(i).getValue().trim().equals("."))
							
						break;
				}
			}
			
		return leftSideOfExpr;
	}
	
	private boolean checkParameterType(Element factRootElement, String className, String calledMethodName,
			int parameterIndex, int numOfParameters, String factClassXPath, String factSignatureXPath, 
			String factParameterTypeNameXPath){
		boolean refOrPtrUsed = false;
		int matchingMethods = 0;
		Element parameterTypeElement;
		
		if (factRootElement != null){
			Nodes classNodes = factRootElement.query(factClassXPath + "[@name='" + className.trim() + "']");
			if ( classNodes != null )
				for (int i=0; i<classNodes.size(); i++){
					Nodes methodNodes = classNodes.get(i).query(factSignatureXPath);
					if (methodNodes != null)
						for (int j=0; j<methodNodes.size(); j++)
							if (methodNodes.get(j) instanceof Element){
								if ( ((Element)methodNodes.get(j)).getFirstChildElement("name")
										.getValue().trim().equals(calledMethodName.trim()) && 
										this.getParameterCount(methodNodes.get(j)) == numOfParameters){
									
									matchingMethods++;
									if (methodNodes.get(j).getChild(parameterIndex) != null)
										if (methodNodes.get(j).getChild(parameterIndex) instanceof Element){
											parameterTypeElement = ((Element)methodNodes.get(j).getChild(parameterIndex)).getFirstChildElement("type");
											if (parameterTypeElement.getValue().contains("&") || parameterTypeElement.getValue().contains("*")){
												refOrPtrUsed = true;
												break;
											}
										}
									}
										
								}
					if (refOrPtrUsed) break;
				}
		//2- options here: if (at least) one method's parameter contains ptr or ref type, we can assume 
		//the call uses it and return true or if there is at least one method which doesnt use ptr or ref type, 
		//we can return false assuming it uses this. Here we will take the first option.
						
		}
		return refOrPtrUsed;
	}
				
	
	private boolean isReferenceParameter (Node parameterNode){
		boolean referenceParameter = false;
		Element parameterTypeElement;
		
		if (parameterNode instanceof Element){
			parameterTypeElement = ((Element)parameterNode).getFirstChildElement("type",
					((Element)parameterNode).getNamespaceURI());
			if (parameterTypeElement != null)
				for (int j=0; j<parameterTypeElement.getChildCount(); j++)
					if (parameterTypeElement.getChild(j) instanceof Text )
						if (parameterTypeElement.getChild(j).getValue().trim().equals("&") ){
							referenceParameter = true;
							break;
						}
		}
		return referenceParameter;
	}
	
	private boolean isPointerParameter (Node parameterNode){
		boolean pointerParameter = false;
		Element parameterTypeElement;
		
		if (parameterNode instanceof Element){
			parameterTypeElement = ((Element)parameterNode).getFirstChildElement("type",
					((Element)parameterNode).getNamespaceURI());
			if (parameterTypeElement != null)
				for (int j=0; j<parameterTypeElement.getChildCount(); j++)
					if (parameterTypeElement.getChild(j) instanceof Text )
						if (parameterTypeElement.getChild(j).getValue().trim().equals("*") ){
							pointerParameter = true;
							break;
						}
		}
		return pointerParameter;
	}
	
	/**
	 * Checks if either of the operands of a condition (with operator == or !=) are floating numbers or of 
	 * type floating variables (can be local variables/parameters, the class' or its super-class attributes, 
	 * attributes of instance of another class) or return floating value (can be the class' or its super-class 
	 * methods, methods of instance of another class)
	 * 
	 * @param ifCondition
	 * @param functionBody
	 * @param localVariableXPath
	 * @param parameterXPath
	 * @param context
	 * @param factClassXPath
	 * @param factAttributeNameXPath
	 * @param factFunctionDeclXPath
	 * @param factRootElement
	 * @param className
	 * @param factSuperClassXPath
	 * @return
	 */
	public boolean floatingNumberEqualityComparison (Node ifCondition, Node functionBody,
			String localVariableXPath, String parameterXPath, XPathContext context, 
			String factClassXPath, String factAttributeNameXPath, String factFunctionDeclXPath, 
			Element factRootElement, String className, String factSuperClassXPath){
		
		boolean floatEqulaityComparison = false;
		Element ifConditionExpr;
		String conditionExpr;
		List<String> predicates = new ArrayList<String>();
		String predicate;
		int equalityOperatorIndex;
		String withoutExternalBrace;
		
		if (ifCondition != null ){
			if (ifCondition instanceof Element){
				
				ifConditionExpr = ((Element)ifCondition).getFirstChildElement("expr", 
						((Element)ifCondition).getNamespaceURI());
				if (ifConditionExpr == null )
					return floatEqulaityComparison;
				conditionExpr = ifConditionExpr.getValue();
				
				//Remove string/char arguments from method calls
				conditionExpr = conditionExpr.replaceAll("\\\\\"", " ");
				conditionExpr = conditionExpr.replaceAll("\"[^\"]*\"", "\" \"");
				
				conditionExpr = conditionExpr.replaceAll("\\\\\'", " ");
				conditionExpr = conditionExpr.replaceAll("\'[^\']?\'", "\' \'");
				
				//For comments in condition expressions
				conditionExpr = conditionExpr.replaceAll("\\/\\/.*\n", " ");
				conditionExpr = conditionExpr.replaceAll("\\\\\\*", " ");
				conditionExpr = conditionExpr.replaceAll("\\/\\*(.|\n)*\\*\\/", " ");
				
				identifyPredicates(predicates, conditionExpr, 0);
				for (int i=0; i<predicates.size(); i++)
					if (predicates.get(i).contains("==") || predicates.get(i).contains("!=")){
						if (predicates.get(i).contains("=="))
							equalityOperatorIndex = predicates.get(i).indexOf("==");
						else 
							equalityOperatorIndex = predicates.get(i).indexOf("!=");
						predicate = predicates.get(i);
						
						if (equalityOperatorIndex<predicate.length()-2)
							while (countBrace(predicate.substring(0, equalityOperatorIndex))>0 
									&& countBrace(predicate.substring(equalityOperatorIndex+2))<0){
								predicate = trimBraces(predicate, false);
								equalityOperatorIndex--;
							}
						String[] operands;
						if (countBrace(predicate.substring(0, equalityOperatorIndex))==0 
									&& countBrace(predicate.substring(equalityOperatorIndex+2))==0){
							operands = predicate.split("==|!=");
						}
						else{
							operands = new String[2];
							operands[0] = predicate;
							operands[1] = "";
						}
						
						withoutExternalBrace = trimBraces(operands[0], true);
						if (!withoutExternalBrace.matches(".*\\).*\\(.*"))
							operands[0] = withoutExternalBrace;
						
						withoutExternalBrace = trimBraces(operands[1], true);
						if (!withoutExternalBrace.matches(".*\\).*\\(.*"))
							operands[1] = withoutExternalBrace;
						
						if (isFloatOrDouble(operands[0]) || isFloatOrDouble(operands[1])){
							floatEqulaityComparison = true;
							break;
						}
						else {
							if (isLocalFloatOrDouble(operands[0], functionBody, localVariableXPath, 
									parameterXPath, context, factClassXPath, factAttributeNameXPath, 
									factFunctionDeclXPath, factRootElement, className, 
									factSuperClassXPath) 
								|| isLocalFloatOrDouble(operands[1], functionBody, localVariableXPath, 
									parameterXPath, context, factClassXPath, factAttributeNameXPath, 
									factFunctionDeclXPath, factRootElement, className, 
									factSuperClassXPath)){
								floatEqulaityComparison = true;
								break;
							}
						}
					}
			}
		}
		
		return floatEqulaityComparison;
	}
	
	/**
	 * Identifies the predicates before and after && or || and puts them in predicates array list. It assumes in 
	 * one predicate there are equal number of opening and closing braces
	 * 
	 * @param predicates
	 * @param conditionExpr
	 * @param prevLogicalOperator
	 */
	private void identifyPredicates(List<String> predicates, String conditionExpr, int prevLogicalOperator){
		int newLogicalOperatorPosition = -1;
		int newAndPos = -1;
		int newOrPos = -1;
		conditionExpr = removePrefixChars(conditionExpr);
		String partOfConditionExpr = conditionExpr.substring(prevLogicalOperator);
	
		if (partOfConditionExpr.contains("&&"))
			newAndPos = partOfConditionExpr.indexOf("&&") + prevLogicalOperator;
		
		if (partOfConditionExpr.contains("||"))
			newOrPos = partOfConditionExpr.indexOf("||") + prevLogicalOperator;
		
		if ( (newAndPos > 0 && newOrPos > 0 && newAndPos < newOrPos) || (newAndPos > 0 && newOrPos < 0))
			newLogicalOperatorPosition = newAndPos;
		else if ( (newOrPos > 0 && newAndPos > 0 && newOrPos < newAndPos) || (newOrPos > 0 && newAndPos < 0) )
			newLogicalOperatorPosition = newOrPos;
		
		if (newLogicalOperatorPosition > 0)
			if (countBrace(conditionExpr.substring(0, newLogicalOperatorPosition))==0 && countBrace(conditionExpr.substring(newLogicalOperatorPosition+2))==0){
				if (conditionExpr.substring(0, newLogicalOperatorPosition).contains("||") 
						|| conditionExpr.substring(0, newLogicalOperatorPosition).contains("&&"))
					identifyPredicates(predicates, conditionExpr.substring(0, newLogicalOperatorPosition).trim(), 0);
				else
					predicates.add(conditionExpr.substring(0, newLogicalOperatorPosition).trim());
				
				if (conditionExpr.substring(newLogicalOperatorPosition+2).contains("||") 
						|| conditionExpr.substring(newLogicalOperatorPosition+2).contains("&&"))
					identifyPredicates(predicates, conditionExpr.substring(newLogicalOperatorPosition+2).trim(), 0);
				else
					predicates.add(conditionExpr.substring(newLogicalOperatorPosition+2).trim());
			}
			else {
				identifyPredicates(predicates, conditionExpr, newLogicalOperatorPosition+2);
			}
		
		else if (newLogicalOperatorPosition < 0 && conditionExpr.trim().startsWith("(") && conditionExpr.trim().endsWith(")")
				&& (conditionExpr.contains("&&") ||conditionExpr.contains("||")) ) {
			conditionExpr = trimBraces(conditionExpr, false);
			identifyPredicates(predicates, conditionExpr, 0);
		}
		else
			predicates.add(conditionExpr);

	}
	
	private String removePrefixChars(String expression){
		if (expression.trim().startsWith("!"))
			expression = expression.trim().replaceAll("^\\!(\\!|\\p{Space})*", "");
		if (expression.trim().startsWith("-"))
			expression = expression.trim().replaceAll("^\\-(\\-|\\p{Space})*", "");
		return expression;
	}
	
	private int countBrace(String text){
		int curveCount=0;
		for(int i=0; i<text.length(); i++)
			switch (text.charAt(i)){
				case '(':
					curveCount++;
					break;
				case ')':
					curveCount--;
					break;
			}
			
		return curveCount;
	}
	
	private String trimBraces(String expression, boolean removeAll){
		
		do {
			expression = removePrefixChars(expression);
		
			if (expression.trim().startsWith("(") && expression.trim().endsWith(")")){
				expression = expression.trim().substring(1);
				expression = expression.trim().substring(0, expression.trim().lastIndexOf(')'));
			}
			else
				removeAll = false;
		}while (removeAll);
				
		return expression;
	}
	/**
	 * Identifies the variable/method and its object/class name when it has. Then it calls the 
	 * appropriate method to see if it is type of or returns float 
	 * 
	 * @param operand
	 * @param functionImplementation
	 * @param localVariableXPath
	 * @param parameterXPath
	 * @param context
	 * @param factClassXPath
	 * @param factAttributeNameXPath
	 * @param factFunctionDeclXPath
	 * @param factRootElement
	 * @param className
	 * @param factSuperClassXPath
	 * @return
	 */
	private boolean isLocalFloatOrDouble(String operand, Node functionImplementation, String localVariableXPath, 
			String parameterXPath, XPathContext context, String factClassXPath, String factAttributeNameXPath, 
			String factFunctionDeclXPath, Element factRootElement, String className, String factSuperClassXPath){
		boolean localFloat = false;
		String variableName;
		String objectName;
		
		if (!operand.contains("("))
			if (operand.contains(".")){ //object.memberVar
				variableName = operand.substring(operand.indexOf('.')+1);
				objectName = operand.substring(0, operand.indexOf('.'));
			}
			else if (operand.contains("->")){ //object->memberVar
				variableName = operand.substring(operand.indexOf("->")+2);
				objectName = operand.substring(0, operand.indexOf("->"));
			}
			else{ //var
				variableName = operand;
				objectName = "";
			}
		else
			if (operand.substring(0,operand.indexOf('(')).contains(".")){ //object.member()
				variableName = operand.substring(0,operand.indexOf('(')).substring(operand.indexOf('.')+1);
				objectName = operand.substring(0,operand.indexOf('(')).substring(0, operand.indexOf('.'));
			}
			else if (operand.substring(0,operand.indexOf('(')).contains("->")){ //object->member()
				variableName = operand.substring(0,operand.indexOf('(')).substring(operand.indexOf("->")+2);
				objectName = operand.substring(0,operand.indexOf('(')).substring(0, operand.indexOf("->"));
			}
			else{ //method()
				variableName = operand;
				objectName = "";
			}
		
		if (variableName.contains("["))
			variableName = variableName.substring(0,variableName.indexOf('['));
		
		if (objectName.contains("::")){//This actually is a static method or attribute call
			localFloat = isFloatMember(variableName, objectName.substring(0, objectName.indexOf("::")),
					factClassXPath, factAttributeNameXPath, factFunctionDeclXPath, factRootElement);
		}
		else{
			// with local variables  declarations
			Nodes localDeclarations = functionImplementation.query(localVariableXPath, context); 
			localFloat = declaredAsFloatOrDouble (variableName, objectName, localDeclarations, 
					factClassXPath, factAttributeNameXPath, factFunctionDeclXPath, factRootElement);
			if (!localFloat){
				// with parameter declarations
				Nodes parameterDeclarations = functionImplementation.query(parameterXPath, context);
				localFloat = declaredAsFloatOrDouble (variableName, objectName, parameterDeclarations, 
						factClassXPath, factAttributeNameXPath, factFunctionDeclXPath, factRootElement);
			}
			if (!localFloat){
				// is this class' attribute/method declaration, includes "this" objects
				localFloat = isFloatMember(variableName, className,
						factClassXPath, factAttributeNameXPath, factFunctionDeclXPath, factRootElement);
			}
			if (!localFloat){
				//with inherited attribute/method declarations
				localFloat = isInheritedAttributeFloat (variableName, className, factClassXPath, 
						factAttributeNameXPath, factFunctionDeclXPath, factRootElement, 
						factSuperClassXPath, false);
			}
		}
		
		return localFloat;
	}
	
	/**
	 * It calls isFloatMember() with member attributes' or methods' of a super-class (and does this up in the 
	 * hierarchy until no super-class is found or either of the two elements of a class are of type float
	 *  
	 * @param memberName
	 * @param className
	 * @param factClassXPath
	 * @param factAttributeNameXPath
	 * @param factFunctionDeclXPath
	 * @param factRootElement
	 * @param factSuperClassXPath
	 * @param inheritedFloat
	 * @return
	 */
	private boolean isInheritedAttributeFloat (String memberName, String className, String factClassXPath, 
			String factAttributeNameXPath, String factFunctionDeclXPath, Element factRootElement, 
			String factSuperClassXPath, boolean inheritedFloat) {
		
		String superClassName;
		Nodes inheritedClasses = factRootElement.query(factClassXPath + "[@name='" + className.trim() + "']/" 
								+ factSuperClassXPath);
		if (inheritedClasses != null)
			for (int i=0; i< inheritedClasses.size(); i++){
				if ( inheritedClasses.get(i) instanceof Element )
					if (((Element)inheritedClasses.get(i)).getChildElements("name")!= null)
						for (int j=0; j< ((Element)inheritedClasses.get(i)).getChildElements("name").size(); j++){
							superClassName = ((Element)inheritedClasses.get(i)).getChildElements("name").
												get(j).getValue();
							inheritedFloat = isFloatMember(memberName, superClassName, factClassXPath, 
									factAttributeNameXPath, factFunctionDeclXPath, factRootElement);
							if (inheritedFloat)
								break;
							else{
								inheritedFloat = isInheritedAttributeFloat (memberName, superClassName, factClassXPath, 
									factAttributeNameXPath, factFunctionDeclXPath, factRootElement, 
									factSuperClassXPath, inheritedFloat);
								if (inheritedFloat)
									break;
							}
						}
				if (inheritedFloat)
					break;
			}
		
		return inheritedFloat;
	}
	
	/**
	 * Checks if variableName is float or not. VariableName can be of basic type or a certain class method
	 * or attribute declared as local variable or method. 
	 * 
	 * @param variableName
	 * @param objectName
	 * @param declarations
	 * @param factClassXPath
	 * @param factAttributeNameXPath
	 * @param factFunctionDeclXPath
	 * @param factRootElement
	 * @return
	 */
	private boolean declaredAsFloatOrDouble (String variableName, String objectName, Nodes declarations, 
			String factClassXPath, String factAttributeNameXPath, String factFunctionDeclXPath, 
			Element factRootElement){
		boolean floatOrDouble = false;
		String declaredName;
		Element typeElt;
		String typeName;
		
		if (declarations != null){
			for (int i=0; i<declarations.size(); i++){
				if (declarations.get(i) instanceof Element){
					
					if (((Element)declarations.get(i)).getFirstChildElement("name", 
							((Element)declarations.get(i)).getNamespaceURI()) != null ){
						declaredName = ((Element)declarations.get(i)).getFirstChildElement("name", 
								((Element)declarations.get(i)).getNamespaceURI()).getValue();
					
						if ("".equals(objectName) && variableName.trim().equals(declaredName.trim())){
							typeElt = ((Element)declarations.get(i)).getFirstChildElement("type", 
									((Element)declarations.get(i)).getNamespaceURI());
							typeName = typeElt.getFirstChildElement("name", typeElt.getNamespaceURI()).getValue();
							typeName = typeName.replaceAll("\\p{Space}", "");
							if ("float".equals(typeName) || "double".equals(typeName) || "longdouble".equals(typeName) 
									|| "Float_t".equals(typeName) || "Double_t".equals(typeName)
									|| "Float16_t".equals(typeName) || "Double32_t".equals(typeName)){
								floatOrDouble = true;
								break;
							}
						}
						else if (!"".equals(objectName) && objectName.trim().equals(declaredName.trim())){
							typeElt = ((Element)declarations.get(i)).getFirstChildElement("type", 
									((Element)declarations.get(i)).getNamespaceURI());
							typeName = typeElt.getFirstChildElement("name", typeElt.getNamespaceURI()).getValue();
							floatOrDouble = isFloatMember(variableName, typeName, factClassXPath, factAttributeNameXPath, 
									factFunctionDeclXPath, factRootElement);
						}
					}
					
				}
			}
		}
		
		return floatOrDouble;
	}
	
	/**
	 * Checks if member function returns float or if an attribute is float. 
	 * 
	 * @param memberName
	 * @param className
	 * @param factClassXPath
	 * @param factAttributeNameXPath
	 * @param factFunctionDeclXPath
	 * @param factRootElement
	 * @return
	 */
	private boolean isFloatMember(String memberName, String className, String factClassXPath, 
			String factAttributeNameXPath, String factFunctionDeclXPath, Element factRootElement){
		boolean floatOrDouble = false;
		Nodes memberElements;
		String declaredName;
		Element typeElt;
		String typeName="";
		String onlyName="" ;
		
		if (factRootElement != null){
			if (memberName.trim().endsWith(")")){
				memberElements = factRootElement.query(factClassXPath + "[@name='" + className.trim() + "']/" + factFunctionDeclXPath);
				onlyName = memberName.substring(0, memberName.indexOf("(")).trim();
				if ("".equals(onlyName.trim()))
					onlyName = memberName;
			}
			else{
				memberElements = factRootElement.query(factClassXPath + "[@name='" + className.trim() + "']/" + factAttributeNameXPath);
				onlyName = memberName.trim();
			}
			if (memberElements != null){
				for (int i=0; i<memberElements.size(); i++){
					if (memberElements.get(i) instanceof Element){
						if (memberName.trim().endsWith(")")){
							declaredName = ((Element)memberElements.get(i)).getFirstChildElement("name").getValue().trim();
							typeElt = ((Element)memberElements.get(i)).getFirstChildElement("type");
							if ( typeElt != null)
								typeName = typeElt.getFirstChildElement("name").getValue();
							else
								typeName = "";
						}
						else{
							declaredName = memberElements.get(i).getValue();
							typeName = ((Element)memberElements.get(i)).getAttributeValue("type");
						}
						
						typeName = typeName.replaceAll("\\p{Space}", "");
						
						if (onlyName.trim().equals(declaredName) && ("float".equals(typeName) || "double".equals(typeName) 
								|| "longdouble".equals(typeName) || "Float_t".equals(typeName) || "Double_t".equals(typeName)
								|| "Float16_t".equals(typeName) || "Double32_t".equals(typeName)) ){
							floatOrDouble = true;
							break;
						}
					}
				}
			}
		}
		
		return floatOrDouble;
	}
	
	/**
	 * Checks if operand is a float number or not.
	 * @param operand
	 * @return
	 */
	private boolean isFloatOrDouble (String operand){
		boolean floatNumber = true;
		try {
			Float.parseFloat(operand);
			if (!(operand.contains(".") || operand.toLowerCase().contains("e")))
				floatNumber = false;
		}
		catch(NumberFormatException e){
			floatNumber = false;
		}
		return floatNumber;
	}
}
