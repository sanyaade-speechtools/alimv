package eu.fbk.rules;

public class NamingRules {
	
	public boolean generalName (String name){
		boolean satisfied = true;
		
		if ( name.contains("_") )
			satisfied = false;
		
		return satisfied;
	}
	
	public boolean headerFileName (String name, String className){
		boolean satisfied = true;
		int lastDotIndex = name.lastIndexOf('.');
		
		if ((lastDotIndex < name.length()-1) && (lastDotIndex > 0)){
			if (!(name.substring(lastDotIndex+1).equalsIgnoreCase("h")) || !(name.substring(0, lastDotIndex).equalsIgnoreCase(className)))
				satisfied = false;
		}
		else
			satisfied = false;
		
		return satisfied;
	}
	
	public boolean implmentationFileName (String name, String className){
		boolean satisfied = true;
		int lastDotIndex = name.lastIndexOf('.');
		
		if ((lastDotIndex < name.length()-1) && (lastDotIndex > 0)){
			if (!(name.substring(lastDotIndex+1).equalsIgnoreCase("cxx")) || !(name.substring(0, lastDotIndex).equalsIgnoreCase(className)))
				satisfied = false;
		}
		else
			satisfied = false;
		
		return satisfied;
	}
	
	public boolean className (String name){
		boolean satisfied = true;
		
		if ( !name.trim().startsWith("Ali") )
			satisfied = false;

		return satisfied;
	}
	
	public boolean memberFunction (String name){
		boolean satisfied = true;
		
		if ( !name.trim().matches("\\p{Upper}.*") )
			satisfied = false;
		
		return satisfied;
	}
	
	public boolean dataMember (String name){
		boolean satisfied = true;
		
		if ( !name.trim().startsWith("f") )
			satisfied = false;
		
		return satisfied;
	}
	
	public boolean localVariable (String name){
		boolean satisfied = true;
		
		if ( !name.trim().matches("\\p{Lower}.*") )
			satisfied = false;
		
		return satisfied;
	}
	
	public boolean globalNonConstVariable (String name){
		boolean satisfied = true;
		
		if ( !name.trim().startsWith("gAli") )
			satisfied = false;
		
		return satisfied;
	}
	
	public boolean globalConstVariable (String name){
		boolean satisfied = true;
		
		if ( !name.trim().startsWith("k") )
			satisfied = false;
		
		return satisfied;
	}

	public boolean constants (String name){
		boolean satisfied = true;
		
		if ( !name.trim().startsWith("k") )
			satisfied = false;
		
		return satisfied;
	}
	
	public boolean constantDataMember (String name){
		boolean satisfied = true;
		
		if ( !name.trim().startsWith("fk") )
			satisfied = false;
		
		return satisfied;
	}
	
	public boolean staticDataMember (String name){
		boolean satisfied = true;
		
		if ( !name.trim().startsWith("fg") )
			satisfied = false;
		
		return satisfied;
	}

	public boolean staticConstantDataMember (String name){
		boolean satisfied = true;
		
		if ( !name.trim().startsWith("fgk") )
			satisfied = false;
		
		return satisfied;
	}

}
