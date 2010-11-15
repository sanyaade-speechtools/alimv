package eu.fbk.utilities;

import nu.xom.Element;
import nu.xom.Elements;
import nu.xom.Node;
import nu.xom.Nodes;
import nu.xom.ParentNode;
import nu.xom.Text;
import nu.xom.XPathContext;

/**
 * This method does the fix for the cpp files with -ve initialization values for the parameters of a method/functions
 * And also for java native methods
 * @author surafel
 *
 */
public class XmlCheckAndFix {
	
	private String xmlPrefix = "tmpPrefix";
	private Element rootElement;
	
	XPathContext context = new XPathContext (xmlPrefix, "http://www.sdml.info/srcML/src");	
	Element xmlFileRoot;
	
	final String methodAsDeclCppXPath = "//"+ xmlPrefix + ":decl_stmt/" + xmlPrefix + ":decl/" + 
									 xmlPrefix + ":argument_list/" + xmlPrefix + ":argument/" + xmlPrefix + ":expr";

	final String incorrectMethodDeclJavaXPath = "//"+ xmlPrefix + ":decl_stmt/" + xmlPrefix + ":decl/" + 
	 								 xmlPrefix + ":argument_list";

	final String incorrectClassDeclJavaXPath = "//" + xmlPrefix + ":decl_stmt/" + xmlPrefix + ":decl/" + 
									 xmlPrefix + ":type"; 
	
	public XmlCheckAndFix() {
		context.addNamespace("cpp", "http://www.sdml.info/srcML/cpp");
	}

	public Element checkAndFix (Element xmlFileRoot, boolean isJava){
		this.rootElement = xmlFileRoot;
		if (!isJava)
			while(checkMisRepresentationInCPP(xmlFileRoot));
		else{
			checkNativeMethodMisRepresentationInJava (xmlFileRoot);
			checkStaticClassMisRepresentationInJava(xmlFileRoot);
		}
		
		return xmlFileRoot;
	}
	
//JAVA
	private void checkStaticClassMisRepresentationInJava(Element xmlFileRoot){
		Nodes incorrectDeclNodes = xmlFileRoot.query(incorrectClassDeclJavaXPath, context);
		Node classNode;
		int indexOfNodeToBeModified;
		ParentNode parentNode;
		
		for (int i=0; i< incorrectDeclNodes.size(); i++){
			for (int j=0; j<incorrectDeclNodes.get(i).getChildCount(); j++){
				if (incorrectDeclNodes.get(i).getChild(j) instanceof Text)
					if (incorrectDeclNodes.get(i).getChild(j).getValue().trim().equals("class")){
						Element newClass = new Element("class", ((Element)incorrectDeclNodes.get(i)).getNamespaceURI());
						
						for (int k=0; k<incorrectDeclNodes.get(i).getChildCount(); k++){//append elts in type as text
							if (incorrectDeclNodes.get(i).getChild(k).getValue().trim().equals("class"))
								newClass.appendChild(incorrectDeclNodes.get(i).getChild(k).getValue());
							else{
								Element newSpecifier = new Element("specifier", ((Element)incorrectDeclNodes.get(i)).getNamespaceURI() );
								newSpecifier.appendChild(incorrectDeclNodes.get(i).getChild(k).getValue());
								newClass.appendChild((Node)newSpecifier);
							}
						}
						
						classNode = incorrectDeclNodes.get(i).getParent();
								
						for (int l=1; l<classNode.getChildCount(); l++) //class name and block
							newClass.appendChild(classNode.getChild(l).copy());
						
						parentNode = classNode.getParent().getParent();
						indexOfNodeToBeModified = parentNode.indexOf(classNode.getParent());
						parentNode.removeChild(indexOfNodeToBeModified);
						parentNode.insertChild(newClass, indexOfNodeToBeModified);
						
					}
			}
			
		}
	}
	
//JAVA	
	private void checkNativeMethodMisRepresentationInJava (Element xmlFileRoot){
		//boolean misRepresentationFound = false;
		
		//for native method declarations
		
		Nodes incorrectDeclNodes = xmlFileRoot.query(incorrectMethodDeclJavaXPath, context);
		Node methodNode;
		int indexOfNodeToBeModified;
		
		for (int i=0; i< incorrectDeclNodes.size(); i++){
			methodNode = incorrectDeclNodes.get(i).getParent();
			if (isNative(methodNode)){
				//misRepresentationFound = true;
				ParentNode parentNode = methodNode.getParent().getParent();
				
				Element newMethodElt = new Element("function_decl", ((Element)methodNode).getNamespaceURI());
				newMethodElt.appendChild(constructTypeNode(methodNode));
				newMethodElt.appendChild(getMethodName(methodNode));
				newMethodElt.appendChild(getParameterList(methodNode));
				newMethodElt.appendChild(";");
				
				indexOfNodeToBeModified = parentNode.indexOf(methodNode.getParent());
				parentNode.removeChild(indexOfNodeToBeModified);
				parentNode.insertChild((Node)newMethodElt, indexOfNodeToBeModified);
			}
		}
		
		//return misRepresentationFound;
	}
	
//JAVA using cpp function to get parameter list
	private Node getParameterList(Node methodNode){
		Node parameterList;
		Element argumentListNode = ((Element)methodNode).getFirstChildElement("argument_list", ((Element)methodNode).getNamespaceURI());
		
		parameterList = (Node)parameterListElement(argumentListNode);
		
		return parameterList;
	}
	
//JAVA
	private Node getMethodName (Node methodNode){
		
		Node nameNode;
		Elements nameElts = ((Element)methodNode).getChildElements("name", ((Element)methodNode).getNamespaceURI());
		nameNode = (Node)nameElts.get(nameElts.size()-1);
		nameNode.detach();
		
		return nameNode;
	}
	
//JAVA
	private Node constructTypeNode (Node methodNode){
		Node typeNode;
		Node childNode;
		Node childCopy;
		int childNodesSize;
		Element typeElement = ((Element)methodNode).getFirstChildElement("type", ((Element)methodNode).getNamespaceURI());
		typeNode = (Node)typeElement;
		typeNode.detach();
		childNodesSize = methodNode.getChildCount();
		int nextElementIndex;
		
		boolean nextElementIsArgument = false;

		for (int i=0; i<childNodesSize-2; i++){
			nextElementIndex = i+1;
			while (! (methodNode.getChild(nextElementIndex) instanceof Element) && nextElementIndex < childNodesSize){
				
				nextElementIndex ++;
			}
			if (methodNode.getChild(nextElementIndex) instanceof Element)
				if (((Element)methodNode.getChild(nextElementIndex)).getQualifiedName().equals("argument_list"))
					nextElementIsArgument = true;
			childNode = methodNode.getChild(i);
			
			if (!nextElementIsArgument) //the current node is method name
				if (childNode instanceof Text)
					((Element)typeNode).appendChild(childNode.getValue());
				else {
					childCopy = childNode.copy();
					((Element)typeNode).appendChild(childCopy);
				}
		}
		
		return typeNode;
	}

//JAVA
	private boolean isNative (Node methodDecl){
		boolean nativeMethod = false;
		
		Elements nameElements = ((Element)methodDecl).getChildElements("name", ((Element)methodDecl).getNamespaceURI());
		if (nameElements != null)	
			for (int i=0; i<nameElements.size(); i++)
				if (nameElements.get(i).getValue().trim().equals("native")){
					nativeMethod = true;
					break;
				}
		
		return nativeMethod;
	}
	
	private boolean checkMisRepresentationInCPP (Element xmlFileRoot){
		
		Nodes methodParameterExprNodes = xmlFileRoot.query(methodAsDeclCppXPath, context);
		Node exprNode;
		boolean nameTagFound = false;
		boolean oprTagFound = false;
		boolean misRepresentationFound = false;
		
		for (int i=0; i< methodParameterExprNodes.size(); i++)
			if (methodParameterExprNodes.get(i) instanceof Element){
				nameTagFound = false;
				oprTagFound = false;
				exprNode = methodParameterExprNodes.get(i);
				for (int j=0; j<exprNode.getChildCount(); j++)
					if (exprNode.getChild(j) instanceof Element){
						if (((Element)exprNode.getChild(j)).getQualifiedName().equals("name"))
							if (nameTagFound && !oprTagFound){
								misRepresentationFound = true;
								
								ParentNode parentNode;
								int indexOfNodeToBeModified;
								Node nodeToBeModified;
								Node methodParameterExprNode = methodParameterExprNodes.get(i);
								Node newFunctionNode;
								newFunctionNode = fix(methodParameterExprNode);
								parentNode = methodParameterExprNode.getParent().getParent().getParent().getParent().getParent();
								nodeToBeModified = methodParameterExprNode.getParent().getParent().getParent().getParent();
								indexOfNodeToBeModified = parentNode.indexOf(nodeToBeModified);
								parentNode.removeChild(indexOfNodeToBeModified);
								parentNode.insertChild(newFunctionNode, indexOfNodeToBeModified);
								break;
							}
							else if (!nameTagFound)
								nameTagFound = true;
					}
					else if (exprNode.getChild(j) instanceof Text)
						if (!((Text)exprNode.getChild(j)).getValue().trim().equals(""))
							oprTagFound = true;
				if (misRepresentationFound)
					break;
			}
		
		
		return misRepresentationFound;
	}
	
	private Node fix(Node methodParameterExprNode){
		
		String functionTagType;
		String signature = methodParameterExprNode.getParent().getParent().getParent().getParent().getValue();
		
		//System.out.println(signature);
		//System.out.println(methodParameterExprNode.getParent().getParent().getParent().getParent().toXML())	;	
		
		if (signature.trim().endsWith(";"))
			functionTagType = "function_decl";
		else
			functionTagType = "function";
		
		Element newFunctionElement = new Element (functionTagType, rootElement.getNamespaceURI());
		Element newTypeElement = new Element ("type", rootElement.getNamespaceURI());
		
		Node declNode = methodParameterExprNode.getParent().getParent().getParent();
		Node elementNode;
		for (int i=0; i<declNode.getChildCount(); i++)
			if (declNode.getChild(i) instanceof Element){
				elementNode = declNode.getChild(i);
				if (((Element)elementNode).getQualifiedName().trim().equals("type")){
					for (int j=0; j<elementNode.getChildCount(); j++)
						if (elementNode.getChild(j) instanceof Element)
							newTypeElement.appendChild((Node)nameElement(elementNode.getChild(j).getValue()));
						else if (elementNode.getChild(j) instanceof Text)
							newTypeElement.appendChild(elementNode.getChild(j).getValue());
					newFunctionElement.appendChild((Node)newTypeElement);
				}
				else if (((Element)elementNode).getQualifiedName().trim().equals("name")){
					newFunctionElement.appendChild((Node)nameElement(elementNode.getValue()));
				}
				else if (((Element)elementNode).getQualifiedName().trim().equals("argument_list")){
					newFunctionElement.appendChild((Node)parameterListElement(methodParameterExprNode.getParent().getParent()));
				}
				else{
					elementNode.detach();
					newFunctionElement.appendChild(elementNode);
				}
			}
			else if (declNode.getChild(i) instanceof Text)
				newFunctionElement.appendChild(declNode.getChild(i).getValue());
		
		if (functionTagType.equals("function_decl"))
			newFunctionElement.appendChild(";");
					
		//System.out.println("New represented content => " + newFunctionElement.getValue());
		//System.out.println("New xml => " + newFunctionElement.toXML());
		return (Node)newFunctionElement;
	}
	
	
	private Element parameterListElement(Node argumentListNode){
		Element newParameterListElement = new Element ("parameter_list", rootElement.getNamespaceURI());
		newParameterListElement.appendChild("(");
		
		//Node argumentListNode = methodParameterExprNode.getParent().getParent();
		boolean firstArgument = false;
		for (int i=0; i< argumentListNode.getChildCount(); i++)
			if (argumentListNode.getChild(i) instanceof Element){
				if (((Element)argumentListNode.getChild(i)).getQualifiedName().trim().equals("argument")){
					if (firstArgument)
						newParameterListElement.appendChild(",");
					newParameterListElement.appendChild((Node)paramElement(argumentListNode.getChild(i)));
					firstArgument = true;
				}
			}
		newParameterListElement.appendChild(")");
		return newParameterListElement;
	}
	private Element paramElement(Node argumentNode){
		Element newParamElement = new Element ("param", rootElement.getNamespaceURI());
		
		Node nameNode;
		Node exprNode;
		boolean typeName = false;
		
		for (int i=0; i< argumentNode.getChildCount(); i++)
			if (argumentNode.getChild(i) instanceof Element){
				Element newTypeElement = new Element ("type", rootElement.getNamespaceURI());
				Element newDeclElement = new Element ("decl", rootElement.getNamespaceURI());
				int constCount = 0;
				int nameElementCount = 0;
				boolean bracketOpened = false;
				exprNode = argumentNode.getChild(i);
				for (int j=0; j<exprNode.getChildCount(); j++){
					Element newExprElement = new Element ("expr", rootElement.getNamespaceURI());
					Element newInitElement = new Element ("init", rootElement.getNamespaceURI());
					
					if (exprNode.getChild(j) instanceof Element){
						
						if (((Element)exprNode.getChild(j)).getQualifiedName().trim().equals("name")){
							nameElementCount ++;
							nameNode = exprNode.getChild(j);
							if (nameNode.getValue().trim().equals("const"))
								constCount++;
							if (nameElementCount == ((Element)exprNode).getChildElements("name",((Element)exprNode).getNamespaceURI()).size()){
									
								if (((Element)exprNode).getChildElements("name",((Element)exprNode).getNamespaceURI()).size() - constCount == 1
										|| bracketOpened){
									newTypeElement.appendChild((Node)nameElement(nameNode.getValue()));
									newDeclElement.appendChild((Node)newTypeElement);
								}
								else {
									newDeclElement.appendChild((Node)newTypeElement);
									newDeclElement.appendChild((Node)nameElement(nameNode.getValue()));
									typeName = true;
								}
							}
							else
								if (!typeName)
									newTypeElement.appendChild((Node)nameElement(nameNode.getValue()));
								else
									newDeclElement.appendChild((Node)nameElement(nameNode.getValue()));
						}
						else{
							Node tmpNode = exprNode.getChild(j).copy();
							newDeclElement.appendChild(tmpNode);
						}
					}
					else if (exprNode.getChild(j) instanceof Text){
						if (exprNode.getChild(j).getValue().contains("=")){
							newDeclElement.appendChild("=");
							newExprElement.appendChild(exprNode.getChild(j).getValue().substring(exprNode.getChild(j).getValue().indexOf("=")+1));
							newInitElement.appendChild((Node)newExprElement);
							newDeclElement.appendChild((Node)newInitElement);
						}
						else{
								newTypeElement.appendChild(exprNode.getChild(j).getValue());
								if (exprNode.getChild(j).getValue().trim().equals("("))
									bracketOpened = true;
								else if (exprNode.getChild(j).getValue().trim().equals(")"))
									bracketOpened = false;
						}
					}
					
				}
				newParamElement.appendChild((Node)newDeclElement);
			}
		return newParamElement;
	}
	private Element nameElement(String name){
		Element newElement = new Element ("name", rootElement.getNamespaceURI());
		newElement.appendChild(name);
		return newElement;
	}
	
	public static void main (String[] args){
		XmlCheckAndFix chkFile = new XmlCheckAndFix();
		SrcMLRoot getXmlRoot = new SrcMLRoot();
		Element rootElt = getXmlRoot.getXmlFileRootElt(args[0], true);
		chkFile.checkAndFix(rootElt, true);//Now working only for java
		System.out.println("Fixed \n\n" + rootElt.toXML().toString());
			
	}

}
