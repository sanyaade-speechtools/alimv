package eu.fbk.factExtractor;

import java.io.File;

import eu.fbk.utilities.ReplaceMacros;
import eu.fbk.utilities.SrcMLRoot;
import eu.fbk.utilities.XmlCheckAndFix;

import nu.xom.Element;
import nu.xom.Node;
import nu.xom.Nodes;
import nu.xom.Text;
import nu.xom.XPathContext;

public class ExtractFacts {

	private String xmlPrefix = "tmpPrefix";
	private StringBuffer factClasses;
	
	XPathContext context = new XPathContext (xmlPrefix, "http://www.sdml.info/srcML/src");	
	Element xmlFileRoot;
	String actualFileName = "";
	
	//Common for XPath queries
	final String nameXPath = xmlPrefix + ":name";	
	final String nameInInit = xmlPrefix + ":init/" + xmlPrefix + ":expr/" + xmlPrefix + ":name";
	//relative from root
	final String classXPath = "//" + xmlPrefix + ":class";
	//Relative from class element
	final String methodXPath = xmlPrefix + ":block/"+ xmlPrefix + ":public/" + xmlPrefix + ":constructor_decl|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":public/" + xmlPrefix + ":destructor_decl|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":public/" + xmlPrefix + ":function_decl|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":public/" + xmlPrefix + ":function|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":public/" + xmlPrefix + ":destructor|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":public/" + xmlPrefix + ":constructor|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":protected/" + xmlPrefix + ":constructor_decl|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":protected/" + xmlPrefix + ":destructor_decl|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":protected/" + xmlPrefix + ":function_decl|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":protected/" + xmlPrefix + ":function|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":protected/" + xmlPrefix + ":destructor|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":protected/" + xmlPrefix + ":constructor|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":private/" + xmlPrefix + ":constructor_decl|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":private/" + xmlPrefix + ":destructor_decl|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":private/" + xmlPrefix + ":function_decl|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":private/" + xmlPrefix + ":destructor|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":private/" + xmlPrefix + ":constructor|" +
							   xmlPrefix + ":block/"+ xmlPrefix + ":private/" + xmlPrefix + ":function";
	
	//Relative from class element
	final String attributeXPath = xmlPrefix + ":block/"+ xmlPrefix + ":public/"+ xmlPrefix + ":decl_stmt/"+ xmlPrefix + ":decl|" +
								  xmlPrefix + ":block/"+ xmlPrefix + ":protected/"+ xmlPrefix + ":decl_stmt/"+ xmlPrefix + ":decl|" +
								  xmlPrefix + ":block/"+ xmlPrefix + ":private/"+ xmlPrefix + ":decl_stmt/"+ xmlPrefix + ":decl";
	//relative from class
	final String superClassXPath = xmlPrefix + ":super";
	
	public ExtractFacts (){
		context.addNamespace("cpp", "http://www.sdml.info/srcML/cpp");
		
		factClasses = new StringBuffer();
	}
	
	public void initXMLTag(){
		factClasses.append("<classes>");
	}
	
	public void closeXMLTag(){
		factClasses.append("</classes>");
	}

	public void getClassFacts (String filePath){
		
		SrcMLRoot getXmlRoot = new SrcMLRoot();
		Element fileRoot = getXmlRoot.getXmlFileRootElt(filePath, false);
		
		if ( fileRoot != null ){
			ReplaceMacros fileWithMacro = new ReplaceMacros(fileRoot);
			fileRoot = fileWithMacro.checkHeaderFileAndMergMacro();
			XmlCheckAndFix checkXml = new XmlCheckAndFix();
			fileRoot = checkXml.checkAndFix(fileRoot, false);
			Nodes classNodes = fileRoot.query(classXPath, context);
			if (classNodes != null) {
				for (int i=0; i < classNodes.size(); i++)
					if (classNodes.get(i) instanceof Element){
						Element classElement = (Element)classNodes.get(i);
						StringBuffer factClassElement = new StringBuffer();
						Nodes classNameNode = classElement.query(nameXPath, context);
						
						for (int j=0; j<classNameNode.size(); j++)
							if (classNameNode.get(j) instanceof Element){
								boolean nameFound = false;
								for (int k=0; k<((Element)classNameNode.get(j)).getChildCount(); k++)
									if (((Element)classNameNode.get(j)).getChild(k) instanceof Text ){
										factClassElement.append("\n<class name=\"" + ((Element)classNameNode.get(j)).getChild(k).getValue().trim() + "\" " +
												"filePath=\"" + fileRoot.getAttributeValue("dir") + File.separatorChar +fileRoot.getAttributeValue("filename")  +"\"> ");
												//"filePath=\"" + fileRoot.getBaseURI() +"\"> ");
										nameFound = true;
										break;
									}
								if (nameFound)
									break;
							}
						addFactElements(factClassElement, classElement, "superClasses",superClassXPath, true);
						addFactElements(factClassElement, classElement, "attributes", attributeXPath, true);
						addFactElements(factClassElement, classElement, "signatures", methodXPath, false);
						factClassElement.append("</class>");
						factClasses.append(factClassElement);
					}
			}
		}
		//else
		//	System.err.println("Err:- Well-formedness error in file in :- " + filePath);
		
	}


	private void addFactElements(StringBuffer factClassElement, Element classElement, String factType, 
			String xPath, boolean nameOnly) {
		StringBuffer factElementType = new StringBuffer();
		Nodes classElementNodes = classElement.query(xPath, context);
		Element typeElement;
		String typeName ="";
		String superClassName;
		
		factElementType.append("<" + factType + ">");
		
		for (int j=0; j<classElementNodes.size(); j++)
			if (classElementNodes.get(j) instanceof Element){
				if (!nameOnly ){
					factElementType.append(this.copyNode(classElementNodes.get(j)));
				}
				else{ //This is for attributes and super-class names
					Element elementName = (Element) classElementNodes.get(j);
					Nodes elementNameNodes;
					if ("attributes".equals(factType)){//New Added
						elementNameNodes = elementName.query(nameXPath + "|" + nameInInit, context);
						typeName ="";
						typeElement = elementName.getFirstChildElement("type", elementName.getNamespaceURI());
						if (typeElement != null){
							
							if (typeElement.getChildElements("name", typeElement.getNamespaceURI()) != null)
								if (typeElement.getChildElements("name", typeElement.getNamespaceURI()).size() > 0)
									typeName = typeElement.getChildElements("name", typeElement.getNamespaceURI())
											.get(typeElement.getChildElements("name", typeElement.getNamespaceURI()).size()-1).getValue();
							if ("".equals(typeName))
								if (typeElement.getChildElements() != null)
									if (typeElement.getChildElements().size() > 0)
										typeName = typeElement.getChildElements().get(
												typeElement.getChildElements().size()-1).getValue();
							if (typeName.contains("<"))
								typeName = typeName.replaceAll("<", "&lt;");
							if (typeName.contains(">"))
								typeName = typeName.replaceAll(">", "&gt;");
						}
					}
					else
						elementNameNodes = elementName.query(nameXPath, context);
					
					for (int k=0; k<elementNameNodes.size(); k++)
						if (elementNameNodes.get(k) instanceof Element){
							//boolean nameFound = false;
							for (int l=0; l<((Element)elementNameNodes.get(k)).getChildCount(); l++)
								if (((Element)elementNameNodes.get(k)).getChild(l) instanceof Text ){
									if ("attributes".equals(factType))
										factElementType.append("<name type=\"" + String.valueOf(typeName) + "\" >" + ((Element)elementNameNodes.get(k)).getChild(l).getValue()+ "</name>");
									else{
										superClassName = elementNameNodes.get(k).getValue();
										while (superClassName.contains("<") && superClassName.contains(">"))
											superClassName = superClassName.replaceAll("<[[^\\<]&&[^\\>]]*>", " ");
										if (superClassName.contains("::"))
											superClassName = superClassName.substring(superClassName.lastIndexOf("::") + 2);
										factElementType.append("<name>" + superClassName.trim() + "</name>");
										break;
									}
								}
						}
				}
			}
		factElementType.append("</" + factType + ">");
		factClassElement.append(factElementType);
	}


	private String copyNode (Node nodeToBeCopied){
		
		String qualifiedName = ((Element)nodeToBeCopied).getQualifiedName();
		if ("function".equals(qualifiedName))
			qualifiedName = qualifiedName + "_decl";
		String newElement = "<" + qualifiedName + ">";

		if (nodeToBeCopied instanceof Element)
			this.filterIfDef((Element)nodeToBeCopied, false);
		
		for (int i=0; i<nodeToBeCopied.getChildCount(); i++)
			if ( nodeToBeCopied.getChild(i) instanceof Element ){
				if (!((Element)nodeToBeCopied.getChild(i)).getQualifiedName().equals("block")){
					this.filterIfDef((Element)nodeToBeCopied.getChild(i), true);
					newElement = newElement + nodeToBeCopied.getChild(i).toXML();
				}
				else
					newElement = newElement + ";";
			}
			else if (nodeToBeCopied.getChild(i) instanceof Text)
				newElement = newElement + nodeToBeCopied.getChild(i).getValue();

		return newElement + "</" + qualifiedName + ">";
	}
	
	private void filterIfDef (Node xmlNode, boolean recurse){
		
		for (int i=0; i<((Element)xmlNode).getChildCount(); i++ ) {
			if (((Element)xmlNode).getChild(i) instanceof Element){
				if (recurse)
					filterIfDef (((Element)xmlNode).getChild(i), recurse);
				if ( ((Element)((Element)xmlNode).getChild(i)).getQualifiedName().contains("cpp:"))
					((Element)((Element)xmlNode).getChild(i)).detach();
			}
		}
	}

	public StringBuffer getFactClasses() {
		return factClasses;
	}

}
