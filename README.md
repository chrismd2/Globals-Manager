# Globals-Manager

Name:		Christenson, Mark
File:		README.md

Rules:
    - Changing UNCOMPILED to COMPILED in GLOBALS.cpp implies the user will recompile before use
    - First line of Header.txt must be CURRENT
    - Newly introduced variables can not be last

Description:	GLOBALS.cpp manages global variables that may be used by other applications and
main.cpp tests and provides an interface for globals
		Variables can be changed during run time
      - Modify Header.txt variable to equal values
      - Modify CURRENT in Header.txt to equal false
      - Call FetchGlobalVariables to use new values from Header.txt
		New variables can be defined:
      - Change CURRENT to equal false
			- Adding a new variable name to Header.txt
			- New variable can't be the first or last variable in Header.txt
			- Set new variable to equal to some value
      - Call FetchGlobalVariables and rewrite
			- Automatic types include double, string, or bool
		Recompile the code
			- To use new variables
      - After Calling rewrite
		Steps to recompile globals
			- Change the first line UNCOMPILED coment to COMPILED
			- Compile program normally
		Additional user options can be defined in main
      - The main function is the last one
    Developer test function
      - Named devTest
      - Second to last function
      - Called in developer mode in CLI with x
      - Can safely be edited to change its behavior for testing

Behavior:
    Automatic Definitions are restricted to double, float, and bool
      - Can redefine variable types if needed
      - Variables may be reordered in Header.txt after compiling


Bugs:
    - User currently can't define dependencies
    - Dependencies must be defined in additional functionality
    - Dependencies haven't been tested

Proposed New Features:
    Replace "CURRENT = bool" line with a hashing function and compare the hashes
    	+ User won't have to change bool value
	- The program won't be able to terminate reading early if Header.txt is current
