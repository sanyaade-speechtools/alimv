package eu.fbk.utilities;

public class CamelCaseParsing {
	
	private String textToBeParsed;
	private String term;
	private int startIndex;
	private int lastIndex;

	public CamelCaseParsing(String textToBeParsed) {
		
		this.textToBeParsed = textToBeParsed;
		this.startIndex = 0;
		this.term = null;
		
	}
	
	/**
	 * This method returns the next possible term starting from the beginning of the
	 * the textToBeParsed or null if there are no more terms to be displayed
	 * 
	 * @return a string or null
	 */
	public String getNextTerm(){
		
		if (startIndex < textToBeParsed.length()){
			this.setLastIndexOfTerm();
			
			if (lastIndex == startIndex){
				term = Character.toString(textToBeParsed.charAt(startIndex));
				startIndex  = lastIndex + 1;
			}
			else if (lastIndex >= textToBeParsed.length()){
				term = textToBeParsed.substring(startIndex);
				startIndex = lastIndex;
			}
			else{
				term = textToBeParsed.substring(startIndex, lastIndex);
				startIndex = lastIndex;
			}
		}
		else
			term = null;
	
		return term;
	}
	
	/**
	 * Sets the lastIndex attribute by the index of the last character
	 * of the possible term identified using CamelCasing. The search of 
	 * the possible term starts from startIndex.
	 */
	private void setLastIndexOfTerm (){
		
		boolean allCaps = false;
		boolean smallLetterFound = false;
		boolean capitalStarts = false;
		boolean possibleWordFound = false;
		int i = startIndex;
		
		do {
			if (Character.isLetter(textToBeParsed.charAt(i))){
				if(Character.isUpperCase(textToBeParsed.charAt(i)) && !capitalStarts )
					if (smallLetterFound){
						possibleWordFound = true;
						lastIndex = i;
					}
					else
						capitalStarts = true;
				else if (Character.isUpperCase(textToBeParsed.charAt(i)) && capitalStarts)
					if (!smallLetterFound)
						allCaps = true;
					else{
						possibleWordFound = true;
						lastIndex = i;
					}
				else if (Character.isLowerCase(textToBeParsed.charAt(i)) && !allCaps)
					smallLetterFound = true;
				else if (Character.isLowerCase(textToBeParsed.charAt(i)) && allCaps){
					possibleWordFound = true;
					lastIndex = i-1;
				}
			}
			else {
				possibleWordFound = true;
				lastIndex = i;
			}
			if (!possibleWordFound)
				i++;
			if (i >= textToBeParsed.length()){
				possibleWordFound = true;
				lastIndex = i;
			}
		}while(!possibleWordFound);
		
	}
	
	public static void main(String args[]){
		
		if (args.length != 1){
			System.err.println("Usage:java fbk.eu.CamelCaseParsing word");
			System.exit(-1);
		}
		
		String possibleWord;
		CamelCaseParsing camelCaseParsing = new CamelCaseParsing (args[0]);
		
		while ((possibleWord = camelCaseParsing.getNextTerm()) != null){
			System.out.println(possibleWord);
		}
	}


}
