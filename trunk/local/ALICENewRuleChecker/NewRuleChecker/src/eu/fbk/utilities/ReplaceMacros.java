package eu.fbk.utilities;

import eu.fbk.checkRules.CheckRules;
import nu.xom.Attribute;
import nu.xom.Element;
import nu.xom.Elements;
import nu.xom.Node;
import nu.xom.Nodes;
import nu.xom.Text;
import nu.xom.XPathContext;

public class ReplaceMacros {
	
	private String xmlPrefix = "tmpPrefix";
	private XPathContext context= new XPathContext (xmlPrefix, "http://www.sdml.info/srcML/src");
	private String classXPath = xmlPrefix + ":class";
	private Element newRootElement;
	private Element rootElement;
	
	public ReplaceMacros(Element rootElement){
		context.addNamespace("cpp", "http://www.sdml.info/srcML/cpp");
		this.rootElement = rootElement;
		newRootElement = new Element (rootElement.getQualifiedName(), rootElement.getNamespaceURI());
		for(int i=0; i<rootElement.getAttributeCount(); i++){
			Attribute newAttribute = new Attribute(rootElement.getAttribute(i).getQualifiedName(), 
					rootElement.getAttribute(i).getValue());
			newRootElement.addAttribute(newAttribute);
		}
		newRootElement.addNamespaceDeclaration("cpp", "http://www.sdml.info/srcML/cpp");
	}
	
	public Element checkHeaderFileAndMergMacro (){
		Element rootElementChecked;
		Nodes classNodes;
		classNodes = rootElement.query(classXPath, context);
		if (classNodes != null){
			if (classNodes.size() <= 0){
				constructTree();
				rootElementChecked = this.newRootElement;
			}
			else 
				rootElementChecked = this.rootElement;
		}
		else
			rootElementChecked = this.rootElement;
		
		return rootElementChecked;
	}
	
	private void constructTree(){
		Element privateElement =null; //look for other options
		boolean classOpenedByMacro = false;
		Element argumentElement;
		Elements argumentElements;
		String argumentOne = "";
		String argumentTwo = "";
		for (int i=0; i<rootElement.getChildCount(); i++){
			if (rootElement.getChild(i) instanceof Element){
				if (((Element)rootElement.getChild(i)).getQualifiedName().equals("macro") ){
					if (((Element)rootElement.getChild(i)).getFirstChildElement("name",
							((Element)rootElement.getChild(i)).getNamespaceURI()).getValue().equals("START_EVENT_HEADER")){
						
						argumentElement = ((Element)rootElement.getChild(i)).getFirstChildElement("argument_list",
							((Element)rootElement.getChild(i)).getNamespaceURI());
						argumentElements = argumentElement.getChildElements("argument", argumentElement.getNamespaceURI());
						
						if (argumentElements.size() > 1){
							argumentOne = argumentElements.get(0).getValue();
							argumentTwo = argumentElements.get(1).getValue();
						}
						classOpenedByMacro = true;
						privateElement = this.privateElement();
					}
					else if (((Element)rootElement.getChild(i)).getFirstChildElement("name",
								((Element)rootElement.getChild(i)).getNamespaceURI()).getValue().equals("END_EVENT_HEADER")){
						classOpenedByMacro = false;
						newRootElement.appendChild((Node)constructClass(privateElement, argumentOne, argumentTwo));
						Element macroClassImpElement = constructImpMacro(argumentOne, argumentTwo);
						newRootElement.appendChild((Node) macroClassImpElement);
					}
				}
				else
					if (classOpenedByMacro)
						privateElement.appendChild((Node)this.copyElement((Element)rootElement.getChild(i)));
					else
						newRootElement.appendChild((Node)this.copyElement((Element)rootElement.getChild(i)));
			}
			else if (rootElement.getChild(i) instanceof Text)
				if (classOpenedByMacro)
					privateElement.appendChild(rootElement.getChild(i).getValue());
				else
					newRootElement.appendChild(rootElement.getChild(i).getValue());
				
		}
	}

	private Element constructImpMacro(String argumentOne, String argumentTwo) {
		Element macroClassImpElement = this.macroElement();
		Element macroClassImpArguemtnList = this.argumentList();
		
		macroClassImpArguemtnList.appendChild(" ( ");
		macroClassImpArguemtnList.appendChild((Node)this.argument("AliRawEventHeaderV" + argumentOne + "_" + argumentTwo));
		macroClassImpArguemtnList.appendChild(" ) ");
		
		macroClassImpElement.appendChild((Node)this.nameElement("ClassImp"));
		macroClassImpElement.appendChild((Node)macroClassImpArguemtnList);
		return macroClassImpElement;
	}
	
	private Element constructClass(Element privateElement, String argumentOne, String argumentTwo){
		Element classElement = this.classElement();
		Element classBlock = this.openBlockElement();
		Element constructorElement = this.constructorElement();
		Element destructorElement = this.destructorElement();
		Element publicElement = this.publicElement();
		Element macroClassDefElement = this.macroElement();
		Element macroClassDefArgumentList = this.argumentList();
		

		constructorElement.appendChild((Node)this.nameElement("AliRawEventHeaderV" + argumentOne + "_" + argumentTwo ));
		constructorElement.appendChild((Node)this.parameterListElement());
		constructorElement.appendChild((Node)this.openBlockElement());
		this.closeBlockElement(constructorElement);
		
		destructorElement.appendChild((Node)this.specifierElement(" virtual "));
		destructorElement.appendChild((Node)this.nameElement("~AliRawEventHeaderV" + argumentOne + "_" + argumentTwo ));
		destructorElement.appendChild((Node)this.parameterListElement());
		destructorElement.appendChild((Node)this.openBlockElement());
		this.closeBlockElement(destructorElement);
		
		publicElement.appendChild((Node)constructorElement);
		publicElement.appendChild(";\n");
		publicElement.appendChild((Node)destructorElement);
		publicElement.appendChild(";\n");
		
		macroClassDefArgumentList.appendChild(" ( ");
		macroClassDefArgumentList.appendChild((Node)this.argument("AliRawEventHeaderV" + argumentOne + "_" + argumentTwo));
		macroClassDefArgumentList.appendChild(", ");
		macroClassDefArgumentList.appendChild((Node)this.argument("1"));
		macroClassDefArgumentList.appendChild(" ) ");
		
		macroClassDefElement.appendChild((Node)this.nameElement("ClassDef"));
		macroClassDefElement.appendChild((Node)macroClassDefArgumentList);
		
		classBlock.appendChild((Node) this.privateDefaultElement());
		classBlock.appendChild((Node) publicElement);
		classBlock.appendChild((Node) privateElement);
		classBlock.appendChild((Node) macroClassDefElement);
		
		this.closeBlockElement(classBlock);
		classBlock.appendChild(";\n");
		
		classElement.appendChild((Node)this.nameElement("AliRawEventHeaderV" + argumentOne + "_" + argumentTwo ));
		classElement.appendChild((Node) classBlock);
		
		return classElement;
	}
	
	private Element classElement(){
		Element newElement = new Element ("class", rootElement.getNamespaceURI());
		newElement.appendChild("class");
		newElement.appendChild(" ");
		return newElement;
	}
	private Element privateDefaultElement(){
		Element newElement = new Element ("private", rootElement.getNamespaceURI());
		Attribute typeAttrib = new Attribute("type", "default");
		newElement.addAttribute(typeAttrib);
		newElement.appendChild(" ");
		return newElement;
	}
	private Element privateElement(){
		Element newElement = new Element ("private", rootElement.getNamespaceURI());
		newElement.appendChild("private:\n");
		newElement.appendChild(" ");
		return newElement;
	}
	private Element publicElement(){
		Element newElement = new Element ("public", rootElement.getNamespaceURI());
		newElement.appendChild("public:\n");
		newElement.appendChild(" ");
		return newElement;
	}
	private Element openBlockElement(){
		Element newElement = new Element ("block", rootElement.getNamespaceURI());
		newElement.appendChild("{\n");
		return newElement;
	}
	private void closeBlockElement(Element openedBlockElement){
		openedBlockElement.appendChild("\n}");
	}
	private Element parameterListElement(){
		Element newElement = new Element ("parameter_list", rootElement.getNamespaceURI());
		newElement.appendChild("()");
		newElement.appendChild(" ");
		return newElement;
	}
	private Element nameElement(String name){
		Element newElement = new Element ("name", rootElement.getNamespaceURI());
		newElement.appendChild(name);
		newElement.appendChild(" ");
		return newElement;
	}
	private Element specifierElement(String specifier){
		Element newElement = new Element ("specifier", rootElement.getNamespaceURI());
		newElement.appendChild(specifier);
		newElement.appendChild(" ");
		return newElement;
	}
	private Element destructorElement(){
		Element newElement = new Element ("destructor", rootElement.getNamespaceURI());
		return newElement;
	}
	private Element constructorElement(){
		Element newElement = new Element ("constructor", rootElement.getNamespaceURI());
		return newElement;
	}
	private Element macroElement(){
		Element newElement = new Element ("macro", rootElement.getNamespaceURI());
		return newElement;
	}
	private Element argumentList(){
		Element newElement = new Element ("argument_list", rootElement.getNamespaceURI());
		return newElement;
	}
	private Element argument(String argument){
		Element newElement = new Element ("argument", rootElement.getNamespaceURI());
		newElement.appendChild(argument);
		newElement.appendChild(" ");
		return newElement;
	}

	private Element copyElement(Element nodeToBeCopied){
		Element newElement;
		newElement = new Element (nodeToBeCopied.getQualifiedName(), nodeToBeCopied.getNamespaceURI());
			
		for(int i=0; i<nodeToBeCopied.getAttributeCount(); i++){
			Attribute newAttribute = new Attribute(nodeToBeCopied.getAttribute(i).getQualifiedName(), 
					nodeToBeCopied.getAttribute(i).getValue());
			newElement.addAttribute(newAttribute);
		}
		
		for (int j=0; j<nodeToBeCopied.getChildCount(); j++)
			if (nodeToBeCopied.getChild(j) instanceof Element)
				newElement.appendChild((Node)copyElement((Element)nodeToBeCopied.getChild(j)));
			else if (nodeToBeCopied.getChild(j) instanceof Text)
				newElement.appendChild(nodeToBeCopied.getChild(j).getValue());
			else
				newElement.appendChild(nodeToBeCopied.getChild(j).getValue());
		return newElement;
	}
	
	public static void main (String[] args){
		if (args.length>0){
			CheckRules fileToBeChecked = new CheckRules();
			Element rootElement = fileToBeChecked.getXmlFileRootElt(args[0]);
			ReplaceMacros fileWithMacro = new ReplaceMacros(rootElement);
			rootElement = fileWithMacro.checkHeaderFileAndMergMacro();
			System.out.println(rootElement.toXML().toString());
		}
	}

}
