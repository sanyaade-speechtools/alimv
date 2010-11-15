package eu.fbk.rules;

import nu.xom.Document;
import nu.xom.Element;
import nu.xom.Node;
import nu.xom.Nodes;
import nu.xom.Text;
import nu.xom.XPathContext;

public class Guidelines {

	public boolean nonPrimitiveTemplateUse(Node declarationTemplateType){
		boolean nonPrimitiveTemplateUsed = false;
		String[] primitiveTypes ={"int", "char", "short", "long", "double", "shortint", "longint", "longdouble", "float", "bool", "wchar_t"};
		String typeGiven;
		String[] types;
		boolean matchingTypeFound = true;
		
		if (declarationTemplateType != null){
			typeGiven = declarationTemplateType.getValue();
			if (typeGiven.contains("<") && typeGiven.contains(">")){
				typeGiven = typeGiven.substring(typeGiven.indexOf("<")+1, typeGiven.lastIndexOf(">"));
				types = typeGiven.split(",");
				for (int i=0; i<types.length; i++)
					if (!types[i].trim().matches("\\d+")){
						matchingTypeFound = false;
						for (int j=0; j<primitiveTypes.length; j++)
							if ( primitiveTypes[j].equals(types[i].replace(" ", "")) ){
								matchingTypeFound = true;
								break;
							}
						if (!matchingTypeFound) break;
					}
				if (!matchingTypeFound) nonPrimitiveTemplateUsed = true;
			}	
		}
		
		return nonPrimitiveTemplateUsed;
	}
/*	
	public boolean templates (Element xmlFileRoot, String templateXPath, XPathContext context){
		//Obsolate; This check is no more used.
		boolean templateFound = false;
		
		Nodes templateNodes = xmlFileRoot.query(templateXPath, context);
		
		for (int i=0; i<templateNodes.size(); i++)
			if (templateNodes.get(i) instanceof Element){
				templateFound = true;
				break;
			}
		
		return templateFound;
	}
*/
	public boolean exceptionTry (Element xmlFileRoot, String exceptionTryXPath, XPathContext context){
		boolean tryBlockFound = false;
		
		Nodes exceptionTryNodes = xmlFileRoot.query(exceptionTryXPath, context);
		
		for (int i=0; i<exceptionTryNodes.size(); i++)
			if (exceptionTryNodes.get(i) instanceof Element){
				tryBlockFound = true;
				break;
			}
		
		return tryBlockFound;
	}
	
/*	
	public String castingType (Node functionCall ){
	//Obsolate; This check is no more used. Casting is allowed.	
		String functionCallName;
		
		if (functionCall instanceof Element)
			if (!((Element)functionCall).getQualifiedName().equals("constructor_decl"))
				if (((Element)functionCall).getFirstChildElement("name", ((Element)functionCall)
						.getNamespaceURI()) != null){
					functionCallName = ((Element)functionCall).getFirstChildElement("name"
							, ((Element)functionCall).getNamespaceURI()).getValue().trim();
					if (functionCallName.contains("dynamic_cast") 
							&& functionCallName.contains("<")
							&& functionCallName.contains(">"))
						return "dynamic_cast";
					else if (functionCallName.contains("static_cast") 
							&& functionCallName.contains("<")
							&& functionCallName.contains(">"))
						return "static_cast";
				}
		return null;
	}
*/	
	public boolean useForwardDeclaration (Nodes declarationTypeNodes, Nodes superClassNameNodes, 
			String includeClass, String exprOnlyXPath, String nameXPath, Element xmlFileRoot, 
			XPathContext context, Nodes functionCallNodes, String castTypeNameXPath, Nodes exprWithNewNodes){
		
		boolean useForwardDecl = true;
		
		for (int i=0; i<declarationTypeNodes.size(); i++)
			if (declarationTypeNodes.get(i) instanceof Element){
				Element typeElement = (Element)declarationTypeNodes.get(i);
				String typeName = "";
				for (int j=0; j< typeElement.getChildCount(); j++){
					if (typeElement.getChild(j).getValue().trim().equals("const") 
							|| typeElement.getChild(j).getValue().trim().equals("static"))
						continue;
					else {
						typeName = typeElement.getChild(j).getValue().trim();
						break;
					}
				}
				
				if (typeName.equals(includeClass.trim())){
					if(!(declarationTypeNodes.get(i).getValue().contains("&") 
							|| declarationTypeNodes.get(i).getValue().contains("*"))){
						useForwardDecl = false;
						break;
					}
					else{ //check the variable declaration (type *x or type & x)and its usage in the file
						useForwardDecl = includeClassUseInDecl(declarationTypeNodes.get(i), 
								exprOnlyXPath, nameXPath,xmlFileRoot, context);
						if (!useForwardDecl)
							break;
					}
				}
			}
		if (useForwardDecl)
			useForwardDecl = includeClassAsSuperClass(superClassNameNodes, includeClass);
		
		if(useForwardDecl)
			useForwardDecl = includeClassStaticUse(includeClass, nameXPath, xmlFileRoot, context);
		
		if (useForwardDecl)
			useForwardDecl = includeClassUsedInNew(includeClass, exprOnlyXPath, xmlFileRoot, context);
		
		if (useForwardDecl)
			useForwardDecl = includeClassUsedInCasting(includeClass, functionCallNodes, castTypeNameXPath, context);
		
		if (useForwardDecl)
			useForwardDecl = includeClassUsedInObjCreation(includeClass, exprWithNewNodes, context); //constructor use
		
		return useForwardDecl;
	}
	
	private boolean includeClassUsedInObjCreation(String includeClass, Nodes exprWithNewNodes, XPathContext context){
		boolean useForwardDecl = true;
		Element exprElt;
		boolean newFound = false;
		
		for (int i=0; i<exprWithNewNodes.size(); i++)
			if (exprWithNewNodes.get(i) instanceof Element){
				exprElt = (Element)exprWithNewNodes.get(i);
				for (int j=0; j<exprElt.getChildCount(); j++){
					if (exprElt.getChild(j) instanceof Text)
						if (exprElt.getChild(j).getValue().trim().equals("new"))
							newFound = true;
					if (newFound && (exprElt.getChild(j) instanceof Element))
						if (((Element)exprElt.getChild(j)).getQualifiedName().equals("name"))
							if (exprElt.getChild(j).getValue().trim().equals(includeClass)){
								useForwardDecl = false;
								break;
							}
				}
				if (!useForwardDecl)
					break;
			}
		return useForwardDecl;
	}
	
	private boolean includeClassUsedInCasting(String includeClass, Nodes functionCallNodes, String 
			castTypeNameXPath, XPathContext context){

		boolean useForwardDecl = true;
		Node functionCall;
		Element nameElt;
		boolean castFound = false;
		Nodes castTypeNameNodes;
		
		for (int i=0; i<functionCallNodes.size(); i++)
			if (functionCallNodes.get(i) instanceof Element){
				functionCall = functionCallNodes.get(i);
				castFound = false;
			if (!((Element)functionCall).getQualifiedName().equals("constructor_decl"))
				if (((Element)functionCall).getFirstChildElement("name", ((Element)functionCall)
						.getNamespaceURI()) != null){
					nameElt = ((Element)functionCall).getFirstChildElement("name", ((Element)functionCall)
						.getNamespaceURI());
					for (int j=0; j<nameElt.getChildCount(); j++){
						if (nameElt.getChild(j) instanceof Text)
							if (nameElt.getChild(j).getValue().trim().equals("dynamic_cast") ||
									nameElt.getChild(j).getValue().trim().equals("static_cast") ||
									nameElt.getChild(j).getValue().trim().equals("const_cast") ||
									nameElt.getChild(j).getValue().trim().equals("reinterpret_cast")){
								castFound = true;
							}
						
						if ( (nameElt.getChild(j) instanceof Element) && castFound ){
							castTypeNameNodes = nameElt.query(castTypeNameXPath, context);
							if (castTypeNameNodes != null)
								if(castTypeNameNodes.size() >0){
									
									if (castTypeNameNodes.get(0).getValue().trim().equals(includeClass)){
										useForwardDecl = false;
										break;
									}
								}
						}
					}
					if (!useForwardDecl)
						break;
				}
			}
		
		return useForwardDecl;
	}
	
	private boolean includeClassUsedInNew(String includeClass, String exprOnlyXPath, Element xmlFileRoot, 
			XPathContext context){
		boolean useForwardDecl = true;
		
		Nodes exprNodes = xmlFileRoot.query(exprOnlyXPath, context);
		
		for (int i=0; i<exprNodes.size(); i++)
			if (exprNodes.get(i) instanceof Element){
				for (int j=0; j<exprNodes.get(i).getChildCount(); j++)
					if (exprNodes.get(i).getChild(j) instanceof Text)
						if (exprNodes.get(i).getChild(j).getValue().trim().endsWith("new"))
							if (exprNodes.get(i).getChild(j+1) != null)
								if (exprNodes.get(i).getChild(j+1) instanceof Element)
									if (((Element)exprNodes.get(i).getChild(j+1)).getQualifiedName().equals("call"))
										if (((Element)exprNodes.get(i).getChild(j+1)).getFirstChildElement("name"
												, ((Element)exprNodes.get(i).getChild(j+1)).getNamespaceURI())
												.getValue().trim().equals(includeClass.trim())){
											useForwardDecl = false;
											break;
										}
				if (!useForwardDecl)
					break;
			}
		return useForwardDecl;
	}

	private boolean includeClassAsSuperClass(Nodes superClassNameNodes,
			String includeClass ) {
		boolean useForwardDecl = true;
		for (int j=0; j<superClassNameNodes.size(); j++)
			if (superClassNameNodes.get(j) instanceof Element)
				if (superClassNameNodes.get(j).getValue().toLowerCase().trim()
						.equals(includeClass.toLowerCase().trim())){
					useForwardDecl = false;
					break;
				}
		return useForwardDecl;
	}

	private boolean includeClassUseInDecl(Node declarationTypeNode,
			String exprOnlyXPath, String nameXPath, Element xmlFileRoot,
			XPathContext context ) {
		
		boolean useForwardDecl = true;
		Node declNode = declarationTypeNode.getParent();
		Element nameElementInDecl = ((Element)declNode).getFirstChildElement("name", 
				((Element)declNode).getNamespaceURI());
		
		if (nameElementInDecl == null)//Decl types of the form - int * - in parameters
			return useForwardDecl;
		
		String identifierName;
		if (nameElementInDecl.getFirstChildElement("name", nameElementInDecl.getNamespaceURI()) != null)
			identifierName = nameElementInDecl.getFirstChildElement("name", nameElementInDecl
					.getNamespaceURI()).getValue().trim();
		else
			identifierName = nameElementInDecl.getValue().trim();
		
		
		Node surroundingBlockNode;
		boolean parentBlockFound= false;
		
		do{
			surroundingBlockNode = declNode.getParent();
			if (surroundingBlockNode == null){
				surroundingBlockNode = xmlFileRoot;
				parentBlockFound = true;
			}
			else if (surroundingBlockNode instanceof Document){
				surroundingBlockNode = xmlFileRoot;
				parentBlockFound = true;
			}
			else if (((Element)surroundingBlockNode).getQualifiedName().equals("block"))
				parentBlockFound = true;
			else
				declNode = surroundingBlockNode;
		}while (!parentBlockFound);
		
		Nodes exprNameNodes = surroundingBlockNode.query(exprOnlyXPath + "/" + nameXPath, context);
		for (int k = 0; k<exprNameNodes.size(); k++){
			if (exprNameNodes.get(k) instanceof Element){
				String identifierInExprName;
				if (((Element)exprNameNodes.get(k)).getFirstChildElement("name", ((Element)exprNameNodes.get(k)).getNamespaceURI()) != null)
					identifierInExprName = ((Element)exprNameNodes.get(k)).getFirstChildElement("name", ((Element)exprNameNodes.get(k)).getNamespaceURI()).getValue().trim();
				else
					identifierInExprName = exprNameNodes.get(k).getValue().trim();
				
				if (identifierInExprName.equals(identifierName) 
						&& (((Element)exprNameNodes.get(k)).getParent().getValue().contains("->")
							|| ((Element)exprNameNodes.get(k)).getParent().getValue().contains("."))){
					useForwardDecl = false;
					break;
				}
			}
		}
		return useForwardDecl;
	}

	private boolean includeClassStaticUse(String includeClass,
			String nameXPath, Element xmlFileRoot, XPathContext context) {
		boolean useForwardDecl = true;
		Nodes allNameNodes = xmlFileRoot.query("//" + nameXPath, context);
		for (int l=0; l<allNameNodes.size(); l++)
			if (allNameNodes.get(l) instanceof Element){
				Element nameElement = (Element)allNameNodes.get(l);
				
				if (!allNameNodes.get(l).getValue().contains("::") )
					continue;
				
				if (((Element)nameElement.getParent()).getQualifiedName().equals("function") 
						|| ((Element)nameElement.getParent()).getQualifiedName().equals("function_decl") 
						|| ((Element)nameElement.getParent()).getQualifiedName().equals("constructor") 
						|| ((Element)nameElement.getParent()).getQualifiedName().equals("constructor_decl") 
						|| ((Element)nameElement.getParent()).getQualifiedName().equals("destructor") 
						|| ((Element)nameElement.getParent()).getQualifiedName().equals("destructor_decl"))
					continue;
						
				if (nameElement.getFirstChildElement("name", nameElement.getNamespaceURI()) == null)
					continue;
				
				String staticVariableClassName = nameElement.getFirstChildElement("name", nameElement.getNamespaceURI()).getValue().trim();
				
				if (staticVariableClassName.equals(includeClass)){
					useForwardDecl = false;
					break;
				}
			}
		return useForwardDecl;
	}
}
