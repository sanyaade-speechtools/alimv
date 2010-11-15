package eu.fbk.checkRules;

import nu.xom.Element;
import nu.xom.Nodes;
import eu.fbk.rules.StyleRules;

public class CheckStyleRules extends CheckRules {

	private final static int hFileDescriptionLines = 3;
	private final static int cxxFileDescriptionLines = 5;
	private final static int maxInlineFunctionSize = 4;
	private StyleRules satisfyStyleRules = new StyleRules();
	private String specialMessage;
	
	public CheckStyleRules( String configFile) {
		super("StyleRules", configFile);
	}

	public void verifyStyleRules (String fileType){
		
		if (fileType.equals("HeaderFile")){
			checkHeaderFileDescription();
			checkDataMemberComment();
			checkFunctionInHeaderFile();
			checkInlineFunctionInHeaderFileClass();
		}
		else{
			checkImplementationFileDescription();
			checkMemberFunctionComment();
		}
	}
	
	private void checkHeaderFileDescription(){
		
		if (this.setRuleNameAndContent("RS1")){
			String headerFileContent = xmlFileRoot.getValue();
			if (!satisfyStyleRules.minFileDescription(headerFileContent, hFileDescriptionLines)){
				specialMessage = "the header file:\n       " 
					+ actualFileName 
			      	+ "\n    has a comment less than " 
			      	+ hFileDescriptionLines + " lines at the beginning\n";
				this.setViolationMessage(specialMessage);
			}
			
		}
	}
	
	private void checkImplementationFileDescription(){
		
		if (this.setRuleNameAndContent("RS1")){
			String implmentationFileContent = xmlFileRoot.getValue();
			if (!satisfyStyleRules.minFileDescription(implmentationFileContent, cxxFileDescriptionLines)){
				specialMessage = "the implmentation file:\n       " 
					+ actualFileName 
			      	+ "\n    has a comment less than " 
			      	+ cxxFileDescriptionLines + " lines at the beginning\n";
				this.setViolationMessage(specialMessage);
			}
			
		}
	}
	
	private void checkDataMemberComment(){
		
		if (this.setRuleNameAndContent("RS2")){
			String headerFileContent = xmlFileRoot.getValue();
			Nodes attributeNodes = xmlFileRoot.query(attributeXPath, context);
			for (int i=0; i< attributeNodes.size(); i++)
				if (attributeNodes.get(i) instanceof Element){
					int dataMemberNoCommentLine = satisfyStyleRules.dataMemberComment(attributeNodes.get(i).getParent().getValue(), headerFileContent);
					if (0 < dataMemberNoCommentLine){
						specialMessage = "the data member declaration\n       " 
							+ attributeNodes.get(i).getParent().getValue()
							+ "\nin [file: "+ actualFileName +"		line: " + dataMemberNoCommentLine + " ]\n"
					      	+ "    has no comment\n";
						this.setViolationMessage(specialMessage);
					}
				}
		}
	}
	
	private void checkMemberFunctionComment (){
		
		if (this.setRuleNameAndContent("RS3")){
			Nodes functionNodes = xmlFileRoot.query(functionImplementationXPath, context);
			for (int i=0; i<functionNodes.size(); i++)
				if (functionNodes.get(i) instanceof Element)
					if (!satisfyStyleRules.memberFunctionComment(functionNodes.get(i).getValue())){
						int endIndex;
						if (functionNodes.get(i).getValue().indexOf('{') > 0)
							endIndex = functionNodes.get(i).getValue().indexOf('{');
						else
							endIndex = functionNodes.get(i).getValue().indexOf(')') + 1;
							
						specialMessage = "the member function\n       " 
							+ functionNodes.get(i).getValue().substring(0, endIndex)
							+ "\nin [file: "+ actualFileName +"		line: " + this.getLineNumber(functionNodes.get(i).getValue()) + " ]\n"
					      	+ "    has no comment\n";
						this.setViolationMessage(specialMessage);
					}
		}
	}
	
	private void checkFunctionInHeaderFile (){
		
		if (this.setRuleNameAndContent("RS4")){
			Nodes classNodes = xmlFileRoot.query("//" + classXPath, context);
			for (int j=0; j<classNodes.size(); j++)
				if (classNodes.get(j) instanceof Element){
					Nodes functionNodes = classNodes.get(j).query(functionImplementationInClassXPath, context);
					for (int i=0; i<functionNodes.size(); i++)
						if (functionNodes.get(i) instanceof Element)
							if (!satisfyStyleRules.inlineFunctionSize(functionNodes.get(i).getValue(), maxInlineFunctionSize)){
								int endIndex;
								if (functionNodes.get(i).getValue().indexOf('{') > 0)
									endIndex = functionNodes.get(i).getValue().indexOf('{');
								else
									endIndex = functionNodes.get(i).getValue().indexOf(')') + 1;
									
								specialMessage = "the member function\n       " 
									+ functionNodes.get(i).getValue().substring(0, endIndex)
									+ "\nin [file: "+ actualFileName +"		line: " + this.getLineNumber(functionNodes.get(i).getValue()) + " ]\n"
							      	+ "    is implemented inside the class and has more than "
							      	+ maxInlineFunctionSize + " lines \n";
								this.setViolationMessage(specialMessage);
							}
				}
		}
	}
	
	private void checkInlineFunctionInHeaderFileClass (){
		
		if (this.setRuleNameAndContent("RS5")){
			Nodes classNodes = xmlFileRoot.query(classXPath, context);
			for (int j=0; j<classNodes.size(); j++)
				if (classNodes.get(j) instanceof Element){
					Nodes functionNodes = classNodes.get(j).query(functionImplementationInClassXPath, context);
					for (int i=0; i<functionNodes.size(); i++)
						if (functionNodes.get(i) instanceof Element)
							if (satisfyStyleRules.usesInlineKeyword((Element)functionNodes.get(i), typeXPath, context)){
								int endIndex;
								endIndex = functionNodes.get(i).getValue().indexOf(')') + 1;
								specialMessage = "the member function\n       " 
									+ functionNodes.get(i).getValue().substring(0, endIndex)
									+ "\nin [file: "+ actualFileName +"		line: " + this.getLineNumber(functionNodes.get(i).getValue()) + " ]\n"
							      	+ "    uses \"inline\" \n";
								this.setViolationMessage(specialMessage);
							}
				}
		}
	}
	
}
