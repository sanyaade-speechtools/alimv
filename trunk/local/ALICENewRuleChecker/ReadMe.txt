NewRuleChecker v1.4.01 
August 04, 2009

This tool is developed to automatically verify the compliance of a C++
source file to the ALICE coding conventions
(http://aliceinfo.cern.ch/Offline/AliRoot/Coding-Conventions.html). 

Prerequisites
--------------
To use this tool, you need to:
1.Install and configure Java on your machine.
2.Download srcML toolkit, which includes src2srcml, from
http://www.sdml.info/projects/srcml/

Steps to use this tool:
-----------------------
1.Translate the source files (".cxx" and ".h") into XML using
src2srcml: 
	   ./src2srcml file.cxx check/file.cxx.xml
	   ./src2srcml file.h check/file.h.xml

2.Run the fact extractor. This will create a file, factFile.xml, under output_directory.

	java -jar FactExtractor.jar xml_files_home_directory output_directory 

3.Execute NewRuleChecker.jar with the XML files as arguments. If there is only the header file or the implementation file, give a place holder name to the missing file. The tool handles this case by automatically generating a dummy file.

	java -jar NewRuleChecker.jar check/file.cxx.xml check/file.h.xml factFile.xml config/AliceCodingConventions.xml

To know the version of this tool use the -version switch.
	java -jar NewRuleChecker.jar -version
	java -jar NewRuleChecker.jar -v
	
Configuration:
---------------
It is possible to decide which rules must be checked by changing the true values for the check attributes in the file config/AliceCodingConventions.xml.
The forbidden strings checked for RC18 can be added or deleted by inserting or deleting a <String> tag which are found under <ForbiddenStrings> tag.

Example
1.	./src2srcml ./MUON/AliMUON.h ./MUON/check/AliMUON.h.xml
	./src2srcml ./MUON/AliMUON.cxx ./MUON/check/AliMUON.cxx.xml
	./src2srcml ./STEER/AliSTEER.h ./STEER/check/AliSTEER.h.xml
	./src2srcml ./STEER/AliSTEER.cxx ./STEER/check/AliSTEER.cxx.xml

2.	java -jar FactExtractor.jar ./ ./
	
3. 	java -jar NewRuleChecker.jar ./MUON/check/AliMUON.cxx.xml ./MUON/check/AliMUON.h.xml ./factFile.xml config/AliceCodingConventions.xml
	java -jar NewRuleChecker.jar ./STEER/check/AliSTEER.cxx.xml ./STEER/check/AliSTEER.h.xml ./factFile.xml config/AliceCodingConventions.xml

=================================================================================
Changes and bug fixes
=================================================================================
The current version (v1.4.01, August 04, 2009) incorporates the following bug fixes and changes.
-Minor fix to collect attribute names
-Fact extractor version changed to version 1.2
-Fact extractor 
	-Bug fix: Fixed a bug caused due to name space 
	-Signals the user about xml files which are not well-formed and continues to the next file w/o exiting
	-Changes made to super class and attribute name collection
	
Changes made in v1.4 (July 08, 2009)
-RC19: added.
-Fact extractor version changed to version 1.1 (July 08, 2009)
-Fact extractor is modified to include attribute type information for use in RC19.

Changes made in v1.3.02 (June 13, 2009)
-RC17: Bug fix. Enumeration element name check corrected.
-RC6: Message error. The message upon violation of this rule is corrected.
-RC15: Bug fix. Internal structure (embedded to class declaration) are not anymore reported as violations.

Changes made in v1.3.01 (March 20, 2009)
-RS2: Error message correction. The field name is now correctly printed in the error. 

Changes made in v1.3 (March 12, 2009)
-RC18: Added. Displays a warning message when a set of forbidden strings are used in the code.
-RC11: Bug fix. Methods that return by reference are now excluded from the const check. Attribute xpath is modified to also include nested classes attributes.
	   
Changes made in v1.2 (February 25, 2009)
-RC11: Bug fix. RC11 is no more reported if 'this' is passed as an argument to other functions.
-RC11: Bug fix. Its modified to also consider attributes used as file stream inputs.
-RC3: The message shown to the user now includes both formats possible to be used as multiple inclusion protection
-GC2: Include files are also now checked if their class is use in casting and new object creation 
 	  (use of constructors)
-Attribute query expression is modified.

Changes made in v1.1 (February 14, 2009)
-RC11: Bug fix. The rule is checked only if there is class definition in the header file.
-RC3: Modified to also consider multiple inclusion protection forms of ALI_GEOM_ST1_V2_H (in addition to 
	  ALI_GEOM_ST_1_V_2_H and ALIGEOMST1V2_H)

Changes made in v1.0 (February 09, 2009)
-Display version information functionality is added.
-A functionality which checks the xml file for wrong function definition (caused due to use of
 negative values while initializing parameters) and its fix is added
-RS1: Modified to check comments even after include statements at the beginning
-RC4: Modified to give warning only when there is more than one class in a header file
-RC6: Bug fix. Order of access specifiers and friend declaration check is corrected.
-RN11: Bug fix. Friend classes are no more considered as member attributes.

Changes made in v0.2 (November 24, 2008)
-RC3: Bug fix. Only the first define is checked for multiple inclusion protection.
-GC1: Static and dynamic casts are excluded from the check.

Changes made in v0.1 (November 20, 2008)
-Macros (START_EVENT_HEADER(AA,BB) and END_EVENT_HEADER(AA,BB)) are expanded for any header file that uses them to create a class definition.
-GC1: is Changed. Now this rule checks for non-primitive type use in templates rather than template definition.
-Dummy file is embeded. This file is used when there is a missing header or implementation file.
-RC11: Modification on the checks for suggesting a method to be const
 	Bug fix. (Make all functions const if they don't modify attributes) is no more reported for virtual methods.
-RC7: Bug fix. The tool checks global variables not const or static global variables.
-RN15: Bug fix for global constant variables.
-RC12: Bug fix in the way the tool checks and suggests dummy arguments. 
-RC11: Bug fix. Static functions are filtered out from the check 
-RC17: is added. It checks possible const pointer or reference parameters (const type*ptr, type*const ptr, const type * const ptr and const type & ptr).

August 29, 2008
-Preliminary version
