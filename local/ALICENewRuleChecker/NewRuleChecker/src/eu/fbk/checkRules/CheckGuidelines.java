package eu.fbk.checkRules;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import eu.fbk.rules.Guidelines;
import nu.xom.Element;
import nu.xom.Nodes;

public class CheckGuidelines extends CheckRules {
	
	private String specialMessage;
	private Guidelines satisfyGuidelines; 
	private List<String> headerFileNameSet;
	
	public CheckGuidelines(String configFile) {
		super("Guidelines", configFile);
		headerFileNameSet = new ArrayList<String>();
		satisfyGuidelines =  new Guidelines();
	}
	
	public void verifyGuidelineConventions(String fileType){
		
		checkTemplateUseGC();
		//checkTemplateGC();
		checkExceptionGC();
		//checkCastingGC();
		checkIncludeFilesGC(xmlFileRoot.getAttributeValue("filename"));
		
	}

	private void checkTemplateUseGC(){
		if (this.setRuleNameAndContent("GC1")){
			Nodes templateUseNodes;
			Nodes parentNodes;
			
			parentNodes =xmlFileRoot.query(functionImplementationXPath, context);
			for (int i=0; i<parentNodes.size(); i++){
				if (parentNodes.get(i) instanceof Element){
					templateUseNodes = parentNodes.get(i).query(localVariableXPath + "/" +templateUseXPath, context);
					isTemplateUsedInDeclaration(templateUseNodes, false);
					templateUseNodes = parentNodes.get(i).query(parameterXPath + "/" +templateUseXPath, context);
					isTemplateUsedInDeclaration(templateUseNodes, true);
				}
			}
			parentNodes = xmlFileRoot.query(classXPath, context);
			Nodes attributeNodes;
			for (int i=0; i<parentNodes.size(); i++){
				if (parentNodes.get(i) instanceof Element){
					attributeNodes = parentNodes.get(i).query(attributeXPath, context);
					for (int j=0; j<attributeNodes.size(); j++){
						if (attributeNodes.get(j) instanceof Element){
							templateUseNodes = attributeNodes.get(j).query(templateUseXPath, context);
							isTemplateUsedInDeclaration(templateUseNodes, false);
						}
					}
				}
			}
		}
	}

	private void isTemplateUsedInDeclaration(Nodes templateUseNodes, boolean isParameter) {
		String declarationText;
		String declaredInLineText;
		for (int j=0; j<templateUseNodes.size(); j++){
			if (satisfyGuidelines.nonPrimitiveTemplateUse(templateUseNodes.get(j))){
				if (isParameter){
					declaredInLineText =templateUseNodes.get(j).getParent().getParent().getParent().getParent().getParent().getValue();
					declarationText = templateUseNodes.get(j).getParent().getParent().getParent().getParent().getValue();
				}
				else{
					declaredInLineText =templateUseNodes.get(j).getParent().getParent().getParent().getValue();
					declarationText =templateUseNodes.get(j).getParent().getParent().getParent().getValue();
				}
				specialMessage = "the declaration \n         "
					+ "		" + declarationText
					+ "\n in  [file:" + actualFileName + " line: "
					+ this.getLineNumber(declaredInLineText)+" ]\n"
					+ "    uses non-primitive type for the template";
				this.setViolationMessage(specialMessage);
				break;
			}
		}
	}
/*	private void checkTemplateGC(){
		
		if (this.setRuleNameAndContent("GC1"))
			if (satisfyGuidelines.templates(xmlFileRoot, templateXPath, context)){
				specialMessage = "[file:" + actualFileName + " ]\n"
					+ "      contains template";
				this.setViolationMessage(specialMessage);
			}
	}
*/	
	private void checkExceptionGC(){
		
		if (this.setRuleNameAndContent("GC1"))
			if (satisfyGuidelines.exceptionTry(xmlFileRoot, exceptionTryXPath, context)){
				specialMessage = "the file\n         "
					+ "  [file:" + actualFileName + " ]\n"
					+ "    uses exception";
				this.setViolationMessage(specialMessage);
			}
	}
	
/*	private void checkCastingGC(){
		
		if (this.setRuleNameAndContent("GC1")){
			Nodes castingCallNodes = xmlFileRoot.query(functionCallXPath, context);
			
			for (int i=0; i<castingCallNodes.size(); i++)
				if (castingCallNodes.get(i) instanceof Element){
					String castType = satisfyGuidelines.castingType(castingCallNodes.get(i));
					if ( castType != null){
						specialMessage = "the file\n         "
							+ "  [file:" + actualFileName + " line: "
							+ this.getLineNumber(castingCallNodes.get(i).getValue())+" ]\n"
							+ "    contains " + castType;
						this.setViolationMessage(specialMessage);
					}
				}
		}
	}
*/
	
	private void checkIncludeFilesGC(String fileName){
		
		if (this.setRuleNameAndContent("GC2")){
			String headerFileNames = "";
			Nodes declarationTypeNodes = xmlFileRoot.query(declarationTypeXPath, context);
			Nodes superClassNameNodes = xmlFileRoot.query(classXPath + "/" + superClassXPath + "/" + nameXPath, context);
			Nodes functionCallNodes = xmlFileRoot.query(functionCallXPath, context);
			Nodes exprWithNewNodes = xmlFileRoot.query(exprOnlyXPath, context);
			
			collectHeaderFileNames();
			for (int i=headerFileNameSet.size()-1; i>=0; i--){
				if (satisfyGuidelines.useForwardDeclaration(declarationTypeNodes, superClassNameNodes, headerFileNameSet.get(i), 
						exprOnlyXPath, nameXPath, xmlFileRoot, context, functionCallNodes, castTypeNameXPath, exprWithNewNodes)){
					headerFileNames = headerFileNames + headerFileNameSet.get(i) + "\n\t";
				}
				headerFileNameSet.remove(i);
			}
					
			if (!headerFileNames.equals("")){
				specialMessage = "the include file(s)\n\t"
							+ headerFileNames
							+ "\n in [file:" + fileName +" ]"
							+ "\n can be declared using forward declaration";
					
					this.setViolationMessage(specialMessage);
			}
		}
	}

	private void collectHeaderFileNames() {
		Nodes includeFileNodes = xmlFileRoot.query(includeFileXPath, context);
		
		for (int i=0; i<includeFileNodes.size(); i++)
			if (includeFileNodes.get(i) instanceof Element){
				int beginIndex = 0;
				int endIndex = includeFileNodes.get(i).getValue().lastIndexOf('.');
				
				if (endIndex <= 0)
					if (includeFileNodes.get(i).getValue().contains(">"))
						endIndex = includeFileNodes.get(i).getValue().lastIndexOf('>');
					else if (includeFileNodes.get(i).getValue().contains("\""))
						endIndex = includeFileNodes.get(i).getValue().lastIndexOf('"');
					else
						endIndex = includeFileNodes.get(i).getValue().length();
				
				if (includeFileNodes.get(i).getValue().contains("<"))
					beginIndex = includeFileNodes.get(i).getValue().indexOf('<') + 1;
				else if (includeFileNodes.get(i).getValue().contains("\""))
					beginIndex = includeFileNodes.get(i).getValue().indexOf('"') + 1;
			
				String includeClassName = includeFileNodes.get(i).getValue().substring(beginIndex, endIndex).trim();
				if (xmlFileRoot.getAttributeValue("filename") != null)
					if (xmlFileRoot.getAttributeValue("filename").substring(0, xmlFileRoot.getAttributeValue("filename").lastIndexOf('.'))
						.trim().equalsIgnoreCase(includeClassName) )
						continue;
				if (includeClassName.indexOf(File.separatorChar) >= 0)
					includeClassName = includeClassName.substring(includeClassName.lastIndexOf(File.separatorChar) + 1);
				
				headerFileNameSet.add(includeClassName);
				
			}
	}
	
}
