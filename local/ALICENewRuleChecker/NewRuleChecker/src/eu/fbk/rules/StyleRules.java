package eu.fbk.rules;

import nu.xom.Element;
import nu.xom.Nodes;
import nu.xom.XPathContext;


public class StyleRules {
	
	public boolean minFileDescription (String headerFileContent, int hFileDescriptionLines){
		boolean satisfied = true;
		int descriptionLines = 0;
		int lineBegin = 0;
		int lineEnd = headerFileContent.indexOf('\n');
		String codeTextLine;
		boolean blockCommentStarted = false;
		
		while(lineEnd >= 0 && lineEnd <= headerFileContent.length()){
			codeTextLine = headerFileContent.substring(lineBegin, lineEnd);
			
			if (codeTextLine.contains("/*") 
					&& !codeTextLine.substring(codeTextLine.indexOf("/*") + 2).contains("*/"))
				blockCommentStarted = true;
			else if (codeTextLine.contains("*/") && blockCommentStarted)
				blockCommentStarted = false;
			
			if (! (codeTextLine.trim().equals("") 
					|| codeTextLine.trim().startsWith("#") 
					|| codeTextLine.trim().startsWith("/*") 
					|| codeTextLine.trim().startsWith("*")) )
				
				if  ( codeTextLine.trim().startsWith("//")  && !blockCommentStarted) {
					if ( !codeTextLine.trim().equals("//") ) 
						descriptionLines++;
				}
				else
					if (!blockCommentStarted)
						break;
			
			lineBegin = lineEnd + 1;
			lineEnd = headerFileContent.indexOf('\n', lineBegin);
			if (lineEnd < 0 && lineBegin < headerFileContent.length())
				lineEnd = headerFileContent.length();
		}
		
		if (descriptionLines < hFileDescriptionLines)
			satisfied =false;
		
		return satisfied;
	}
	
	public int dataMemberComment( String dataMemberText, String headerFileContent){
		int lineNumber = 0;
		boolean commentFound = false;
		int lineBegin = 0;
		int lineEnd = headerFileContent.indexOf('\n');
		String lineText;
		
		while(lineEnd >= 0 && lineEnd < headerFileContent.length()){
			lineText = headerFileContent.substring(lineBegin, lineEnd);
			lineBegin = lineEnd;
			lineEnd = headerFileContent.indexOf('\n', lineEnd + 1);
			lineNumber++;
			
			if (lineText.contains(dataMemberText)){
				String commentText ="";
				if (lineText.contains("//")){
					if (lineText.indexOf("//")+2 < lineText.length())
						commentText = lineText.substring(lineText.indexOf("//")+2).trim();
					else
						commentText ="";
				}
				else if (lineText.contains("/*")){
					if (lineText.contains("*/"))
						commentText = lineText.substring(lineText.indexOf("/*")+2, lineText.indexOf("*/", lineText.indexOf("/*")+2)).trim();
					else
						commentText = lineText.substring(lineText.indexOf("/*")+2).trim();
				}
				
				if (!commentText.equals(""))
						commentFound = true;
				break;
			}
		}
		if (commentFound)
			lineNumber = -1;
		
		return lineNumber;
	}
	
	public boolean memberFunctionComment (String functionBody){
		
		final int minContentLines = 3;
		int textLineNumber = 0;
		boolean commentFound = false;
		boolean statmentFound = false;
		int lineBegin = 0; 
		int lineEnd;
		String lineText;
		String commentText = "";
		
		functionBody = functionBody.substring(functionBody.indexOf('{') + 1);
		lineEnd = functionBody.indexOf('\n', lineBegin);
		
		while(lineEnd >= 0 && lineEnd < functionBody.length()){
			lineText = functionBody.substring(lineBegin, lineEnd);
			
			if (!lineText.trim().equals("")){
				textLineNumber++;
				if (lineText.trim().startsWith("//") && !statmentFound){
					if (lineText.indexOf("//")+2 < lineText.length())
						commentText = lineText.substring(lineText.indexOf("//")+2).trim();
					else
						commentText ="";
				}
				else if (lineText.trim().startsWith("/*") && !statmentFound)
					commentText = functionBody.substring(functionBody.indexOf("/*", lineBegin)+2, 
														 functionBody.indexOf("*/", functionBody.indexOf("/*", lineBegin)+2)).trim();
				else
					statmentFound = true;
				
				if (!commentText.equals("") && !statmentFound){
					commentFound = true;
					commentText = "";
				}
				
			}
			
			lineBegin = lineEnd + 1;
			lineEnd = functionBody.indexOf('\n', lineBegin);
			if (lineEnd < 0 && lineBegin < functionBody.length())
				lineEnd = functionBody.length();
			
		}
		
		if (!commentFound && textLineNumber <= minContentLines)
			commentFound = true;
		
		return commentFound;
	}

	
	public boolean inlineFunctionSize (String functionBody, int maxInlineFunctionSize){
		
		int textLineNumber = 0;
		boolean satisfied = true;
		int lineBegin = 0;
		int lineEnd; 
		String lineText;
		
		functionBody = functionBody.substring(functionBody.indexOf('{') + 1);
		
		lineEnd = functionBody.indexOf('\n', lineBegin);
		
		while(lineEnd >= 0 && lineEnd <= functionBody.length()){
			lineText = functionBody.substring(lineBegin, lineEnd);
			
			if (!lineText.trim().equals(""))
				textLineNumber++;
			
			lineBegin = lineEnd + 1;
			lineEnd = functionBody.indexOf('\n', lineBegin);
			if (lineEnd < 0 && lineBegin < functionBody.length())
				lineEnd = functionBody.length();
		}
		
		if (textLineNumber > maxInlineFunctionSize)
			satisfied = false;
		
		return satisfied;
	}
	
	
	public boolean usesInlineKeyword(Element functionElement, String typeXPath, XPathContext context){
		
		boolean satisfied = false;
		Nodes functionTypeNodes = functionElement.query(typeXPath, context);
		
		for (int i=0; i < functionTypeNodes.size(); i++)
			if (functionTypeNodes.get(i) instanceof Element)
				if (functionTypeNodes.get(i).getValue().contains("inline")){
					satisfied = true;
					break;
				}

		return satisfied;
	}
}
