/*
* Author: James Walter Erardi
* University of Massachusetts Lowell
*
* jerardi@cs.uml.edu
* © UMass Lowell 2015
*
* Created on October 11th, 2015 Using Tyler Bainbridge's Program Three Code
* My Third program made several things difficult going into this new assignment,
* so I decided to start with something a bit less convoluted.
* Edited on 10/12/2015
* Edited on 10/13/2015
* Edited on 10/14/2015
* Edited on 10/15/2015
* Edited on 10/25/2015
* Edited on 10/26/2015
* Edited on 10/28/2015
* Edited on 11/1/2015
* Edited on 11/3/2015
*/

#ifndef ELEMENT_H
#define ELEMENT_H
#include <cstdlib>						//the netbeans version of stdlib
#include <iostream>						//required lib for input/output stream functions
#include <cstring>						//required lib for c-strings
#include <string>						//required lib for strings
#include <fstream>						//required lib for reading in files
#include <vector>						//required lib for STL vector
#include <algorithm>					//supplys a few useful algorithms
#include "Element.h"					//header file containing functions 
#include <stack>						//required lib for STL stack
#include <map>							//required lib for STL map
#include <iomanip>						//required lib for input/output formatting
#include <iterator>                                             //required lib for next
using namespace std;					//standard namespace operation scope, so we don't need to put std:: before everything

/**
* @author		James Erardi <jerardi@cs.uml.edu>
* @version		1.4
* @since		9/2015
*
*/
class element{
public:
	/**
	* Default constructor, used to instantiate element objects
	*/
	element();

	/**
	* A constructor which takes in a tag name and element line nnumber
	* We use this to initialize elements which already have dedicated data
	* @param finalTag the cleaned up tag which will be the element's name
	* @param lineNumber the line number on which the element was found in the XML file
	*/
	element(string finalTag, int lineNumber) : elementName(finalTag), lineNum(lineNumber){};

	/**
	* A constructor which takes in a tag name, level, and element line number
	* We use this to initialize elements which already have dedicated data
	* @param finalTag the cleaned up tag which will be the element's name
	* @param lvl the current level of which you're in on the elementTree
	* @param lineNumber the line number on which the element was found in the XML file
	*/
	element(string finalTag, int lvl, int lineNumber) : elementName(finalTag), level(lvl), lineNum(lineNumber){};


	/**
	*
	* @return getter for integer level, used throughout file. usually in conditional statements
	*/
	int getLevel(){ return level; }

	/**
	*
	* @return getter for elementName, used throughout file. usually in conditional statements
	*/

	/**
	*
	* @return getter for getName, used throughout file. usually in conditional statements
	*/
	string getName(){ return elementName; }

	/**
	*
	* @return getter for line number variable, used throughout file. usually in conditional statements
	*/

	int getlineNum(){ return lineNum; }

	/**
	*
	* @return getter for the Child's size, used to figure out what's a child and what's not
	*/

	int getChildSize(void){ return children.size(); }

	/**
	*
	* @return returns the child vector of element pointer, used for iterators
	*/

	vector<element*> getVector(void){ return children; }

	/**
	* Used when we want to print a line of XML's content (if it has it)
	* @return getter for the content, used to access if we're
	* printing a line with content so we can print it's content
	*/
	string getContent(void){ return content; }

	/** 
	* Used when we want to get an attribute element's value. Usually when we print the attribute
	* @return returns attribute element's value
	*/
	string getValue(void){ return value; }
	
	/**
	* Used when we want to get an element's attribute. 
	* Also used to figure out if an element is an attribute
	* @return
	*/
	string getAttribute(void){ return attribute; }

	/**
	* Adds a child element into the element object. Used when we find a new child element. 
	* @param child     when we find a new child, pushes this child onto the element* stack
	*/
	void addChild(element* child);

	/**
	* This function is used when we add values into the AttributeMap
	* We add attributes into the AttributeMap so we can easily access/print/evaluate them
	* @param att Att is the name of the XML line which is an attribute
	* @param val Val is the(a) value of the XML attribute 
	*/
	void setMapValues(string att, string val){ attributeMap.insert(pair<string, string>(att, val)); }
	/**
	* We use this whenever we want to evaluate/print/use the AttributeMap
	* @return Returns element's private object AttributeMap
	*/
	map<string, string> getMap(){ return attributeMap; }

	/**
	* A quick getter function to find if the current element has an attribute
	* Used whenever an if loop attempts to evaluate if the current XMl file is an attribute,
	* so the program can parse specifically
	* @return returns true/false based on attribute presence
	*/
	bool isaAttribute(void);

	/**
	* Used whenever we want to create a new element with content
	* Function finds what's content and adds it to the private member variable "content"
	* @param line takes in current line of XML which is a content line
	*/
	void setContent(string line);

	/**
	* A quick getter function to find if the current element has content
	* Used whenever we want to evaluate if an XML line has content for specific parsing
	* @return returns true/false based on content presence
	*/
	bool hasContent(void);

	/**
	* Member function which prints the element's attributeMap in a nice way
	* We use this whenever we want to print XML elements which have attributes
	*/
	void printAttributes();

	/**
	* This function is the function prints the attribute XML lines into JSON
	* The purpose of this function is to make the program more modular and cleaner.
	* Attribute syntax is specific in JSON, so we cannot use conventional XML to JSON conversion
	* @param jsonFile Passes in the output json file so we can parse through modular means 
	*/
	void printAttributesToJson(ofstream& jsonFile);
	/**
	* This function is the function prints the attribute XML lines into JSON
	* The purpose of this function is to make the program more modular and cleaner.
	* Attribute syntax is specific in JSON, so we cannot use conventional XML to JSON conversion
	* This function is nearly identical to the printAttributesToJson function, however 
	* attributes which end the JSON document need to be parsed differently. 
	* @param jsonFile Passes in the output json file so we can parse through modular means
	*/
	void printAttributesToJson_end(ofstream& jsonFile);
private:
	string content;							//string for name of XML line's content
	string elementName;						//string for name of the XML Tag
	string attribute;						//string for XML attribute's name
	string value;							//string for the value of the attribute
	int level;								//level of which the XML element exists on
	bool isAttribute;						//for tags that are attributes 
	int lineNum;							//line number the XML is found on
	vector<element*> children;				//Vector of element children for easy storing/parsing
	map<string, string> attributeMap;		//map which contains the attribute/value key pair for XML lines which are attributes
};

/**
* Function which parses open tags and returns them cleanly
* Used whenever we want to find the tag of an open line
* @param line current dirty line being parsed
* @return returns a nice and clean line, free of spaces and other stuff we don't want
*/
string getOpen(string line); //get the name of the opening tag

/**
* This function is used to find the tag which is between a closed XML tag
* @param line current XML line to be parsed
* @return returns a formatted closed XML line
*/
string getClosed(string line); 

/**
* This function contains almost everything related to parsing for this, and the last project.
* Admittedly, this function could be a bit smaller, but I'm worried about results being effected
* if I modulate the function. I tried to document my thought processes well as I could so it isn't
* too difficult to understand.
*
* @param fileName takes in a dynamic fileName specified in main
*/
void parseFile(string fileName); 

/**
* this function handles the process of printing a opening XML tag
* Used whenever we want to print the open Element, namely in earlier assignments
* @param tag the opening tag
*/
void printOpenElement(element tag); 
/**
* this function handles the process of printing a closing XML tag
* Used whenever we want to print the closed element, namely in earlier assignments
* @param tag closed element tag
*/
void printClosedElement(element tag);

/**
* this function handles the process of printing a complete element
* Used whenever we want to print a complete XML element, namely in earlier assignments
* @param line passes in the current line that's being parsed
* @param lineNumber passes in the current line number for pretty formatting
*/
void printCompleteElement(string line, int lineNumber); 

/**
* standard STL stack printing function
* Used when we want to print a stack, which was in Assingment #4.
* Will empty the songstack after the end of the function via popping
* @param songstack takes in a the stack which user wants to print
*/
void printStack(stack<element> songstack); 

/**
* This function which handles the correct parsing of XML lines which contain XML attributes
* This function also puts attribute information into the STL map
* This function is called whenever we want to figure out an XML line's attributes
* Handles all of the attribute parsing
* @param line current XML line which is being parsed
* @param ele passes in the pointer to the current element being parsed
*/
void getAtrribute(string line, element* ele);

/**
* This is the big papa print statement which will print everything inside of the elementTree
* in a nice and clean manner
* Used after we do the parsing for Assignment #4
* @param elementTree Takes in a vector (which is technically a tree, even though it's a vector)
*/
void printEverything(vector<element> elementTree); 

/**
* Basic print map function which checks if the map is empty before it prints it, so it won't print empty maps
* Used whenever we want to print a map for error checking
* @param mappy whatever map is currently being printed
*/
void printMap(map<string, string> mappy); 
/**
* This function patches a few problems I had with my output. It's a bit convoluted, but it works.
* @param temp takes the vector aspect of the child node
* @param tag takes in the current tag which is being parsed
* @return returns a true or false value based on the fact if the child vector is equal to current tag
* this will fix the fact the last child tag was being added twice, because it didn't know that it was the last child tag
*/
bool searchChildVec(vector<element*> temp, element tag); 
/**
* This function is the powerhouse of Assignment Five
* It'll go through the parsed elementTree, element by element,
* decide what the element is, and convert accordingly. 
* This could be smaller and more modular 
* @param elementTree takes in parsed XML tree for JSON conversion
*/
void printJson(vector<element> elementTree);	

#endif