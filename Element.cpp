/*
* Author: James Walter Erardi
* University of Massachusetts Lowell
*
* jerardi@cs.uml.edu
* © UMass Lowell 2015
*
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


#include <iterator>     // std::next
#include "element.h"

using namespace std;
/**
* Adds a child element into the element object. Used when we find a new child element. 
* @param child     when we find a new child, pushes this child onto the element* stack
*/
void element::addChild(element* child)
{
	children.push_back(child);
}

/**
* This function contains almost everything related to parsing for this, and the last project.
* Admittedly, this function could be a bit smaller, but I'm worried about results being effected
* if I modulate the function. I tried to document my thought processes well as I could so it isn't
* too difficult to understand.
*
* @param fileName takes in a dynamic fileName specified in main
*/

void parseFile(string fileName)
{
	ifstream inFile(fileName.c_str());							//ifstream object to read in dynamic file name 
	string line, finalTag, initialTag;							//various strings for parsing 
	element* tag;												//element pointer for creating new tags
	element* closeTag;											//for new closing elements
	stack<element> songstack;									//stack for opening tags
	vector<element> elementTree;								//vector which will be used to pop the songstack onto
	int startPos, endPos, lineNumber = 1;						//various ints that will be used in the STL find function
	string tempstring;											//various uses throughout the file

	while (getline(inFile, line))
	{
		if (line.at(0) == '<' && line.at(1) == '?')
		{
			//cout << lineNumber << "    " << getOpen(line) << " is an attribute, not an open tag." << endl;
		}

		if (line.find("<!--") != string::npos) //looking comments
		{
			cout << lineNumber << ": Parser State - Starting comment: " << line << endl; //shows the first part of the comment
			lineNumber++; //increment for the starting comment 
			getline(inFile, line); //gets next line
			while (line.find("-->") == string::npos) //searching for the end of the comment
			{
				lineNumber++; //important to increment the linenum now, because we will use getline() to move to the next line soon
				//and we would not count for this complete element if we don't increment an extra time
				cout << lineNumber << "    Comment: " << line << endl;
				getline(inFile, line); //gets next line
			}
			cout << lineNumber << "    Ending comment: " << line << endl << endl;
			getline(inFile, line); //gets next line
			lineNumber++; //counts the line again
		}

		//if loop that attempts to find complete lines, and move past them

		if (line.at((line.find("<")) + 1) != '/') //if it is not a closing tag, then proceed to see if its closed inline
		{
			int backSlash = line.find('/'); //checks if there is a slash to then see if it is a closing tag
			if (backSlash != string::npos&&line.at(backSlash - 1) == '<') //checking for complete elements
			{
				//attribute = getAtrribute(line);
				tempstring = getOpen(line);

				tag = new element(tempstring, songstack.size(), lineNumber);
				//cout << tag->getName() << endl;
				//cout << tag->getLevel() << songstack.top().getLevel() << endl;
				if (tag->getLevel()>(songstack.top().getLevel()))
				{
					tag->setContent(line); //setting the content between the tags
					getAtrribute(line, tag);
					songstack.top().addChild(tag);
					
				}
				//printCompleteElement(line, lineNumber); //prints the full line, and the content between tags
			}
		}

		if (line.at((line.find("<")) + 1) != '/'&&!ispunct(line.at((line.find("<")) + 1)) && line.at((line.find(">")) - 1) != '/') //if the character proceeding the opening bracket is not a backslash or punct (to get rid of closing tags and other tags) and there is no slash before the closing tags(self closing)
		{
			//Since we've found a new element, we must dynamically allocate a new element assigned to the tag pointer
			tag = new element(getOpen(line), songstack.size(), lineNumber); //creates a new Element with the line number and 

			int backSlash = line.find('/'); //checks if there is a slash to then see if it is a closing tag

			if (backSlash != string::npos&&line.at(backSlash - 1) != '<') //safeguard for tags that have forward slashes but are not for closing tags
			{
				//runs the get attribute function to figure out if the current line we're reading in is an attribute
				//if it isn't an attribute, nothing will happen to the tag pointer. if it is, the map living inside
				//the tag pointer will be filled with information
				getAtrribute(line, tag);
				songstack.push(*tag); //pushes the tag onto the stack
			}

			if (backSlash == string::npos)//if there is no slash aka it is an open tag
			{
				//see above
				getAtrribute(line, tag);
				songstack.push(*tag);
			}
		}

		if (line.at((line.find("<")) + 1) == '/') //if the character proceeding the opening bracket is a backslash (to find closing tags)
		{


			closeTag = new element(getClosed(line), songstack.size(), lineNumber); //creates a new Element with the line number and tagName
			if (songstack.top().getName() == closeTag->getName()){
				//checks if the top of the stack is equal to the closing tag found first
				//printClosedElement(*tag); //prints info



				if (!songstack.empty()) // if songstack isnt empty
				{
					if (!searchChildVec(songstack.top().getVector(), *tag)) //if tag is not in the songstack's child vector
					{
						songstack.top().addChild(tag); //add the child
					}
				}
				elementTree.push_back(songstack.top()); //pushing the elements on the vector for printing and organizing
				songstack.pop(); //pops the stack



				//printStack(songstack); //displays the stack's contents
			}

		}
		lineNumber++; //counting lines
	}

	//printMap(elementTree);	
	//printEverything(elementTree);
	printJson(elementTree);


}
/**
* This function patches a few problems I had with my output. It's a bit convoluted, but it works.
* @param temp takes the vector aspect of the child node
* @param tag takes in the current tag which is being parsed
* @return returns a true or false value based on the fact if the child vector is equal to current tag
* this will fix the fact the last child tag was being added twice, because it didn't know that it was the last child tag
*/

bool searchChildVec(vector<element*> temp, element tag)
{
	bool found = false;
	vector<element*>::iterator jt;

	for (jt = temp.begin(); jt != temp.end(); ++jt)
	{
		if ((*jt)->getName() == tag.getName()) //if tag is located in temp (child vec)
		{
			found = true;
		}
	}

	return found;
}
/**
* Basic print map function which checks if the map is empty before it prints it, so it won't print empty maps
* Used whenever we want to print a map for error checking
* @param mappy whatever map is currently being printed
*/
void printMap(map<string, string> mappy){
	map<string, string>::iterator itr;
	if (!mappy.empty()){
		for (itr = mappy.begin(); itr != mappy.end(); ++itr){
			cout << "Attribute: " << itr->first << " has value: " << itr->second << endl;
		}
	}
}
/*
void printMap(map<string, string> mappy, string& value, string& attribute){
	map<string, string>::iterator itr;
	if (!mappy.empty()){
		for (itr = mappy.begin(); itr != mappy.end(); ++itr){
			cout << "Attribute: " << itr->first << " has value: " << itr->second << endl;
			value = itr->second;
			attribute = itr->first;

		}
	}
}*/
/**
* this function handles the process of printing a opening XML tag
* Used whenever we want to print the open Element, namely in earlier assignments
* @param tag the opening tag
*/
void printOpenElement(element tag)
{
	int nameWidth = 30; //width of the name
	int numWidth = 5; //width of the number
	char separator = ' '; //using the i/o manipulation library adding spaces to fill the rest of the width

	cout << left << setw(numWidth) << setfill(separator) << tag.getlineNum(); //line number
	cout << left << setw(nameWidth) << setfill(separator) << "Opened Element: " + tag.getName() << endl; //prints the element opened
}
/**
* this function handles the process of printing a closing XML tag
* Used whenever we want to print the closed element, namely in earlier assignments
* @param tag closed element tag
*/
void printClosedElement(element tag)
{
	int nameWidth = 30; //width of the name
	int numWidth = 5; //width of the number
	char separator = ' '; //using the i/o manipulation library adding spaces to fill the rest of the width

	cout << left << setw(numWidth) << setfill(separator) << tag.getlineNum(); //line number
	cout << left << setw(nameWidth) << setfill(separator) << "Closed Element: " + tag.getName() << endl; //prints the element closed
	cout << endl;
}
/**
* this function handles the process of printing a complete element
* Used whenever we want to print a complete XML element, namely in earlier assignments
* @param line passes in the current line that's being parsed
* @param lineNumber passes in the current line number for pretty formatting
*/

void printCompleteElement(string line, int lineNumber)
{
	int nameWidth = 30; //width of the name
	int numWidth = 5; //width of the number
	int startPos, endPos; //start and end pos for substr
	string fullLine; //for the full line without XML indentation
	string elementContent; //for what is between the tags

	char separator = ' '; //using the i/o manipulation library adding spaces to fill the rest of the width

	startPos = line.find("<");
	endPos = line.find("/n ", startPos + 1); //getting the full line without spacing from XML
	fullLine = line.substr(startPos, (endPos - startPos) - 1); //getting the line with no spacing

	startPos = line.find(">");
	endPos = line.find("<", startPos + 1); //getthing what's between the tags aka Content
	elementContent = line.substr(startPos + 1, (endPos - startPos) - 1); //getting content

	cout << left << setw(numWidth) << setfill(separator) << lineNumber; //prints the name
	cout << left << setw(nameWidth) << setfill(separator) << "Complete Element: " + fullLine << endl; //prints the complete line
	cout << left << setw(numWidth) << setfill(separator) << " "; //spacing for next line
	cout << left << setw(nameWidth) << setfill(separator) << "Element Name: " + getOpen(fullLine) << endl; //prints what is between the tags
	cout << left << setw(numWidth) << setfill(separator) << " "; //spacing for next line
	cout << left << setw(nameWidth) << setfill(separator) << "Element Content: " + elementContent << endl; //prints what is between the tags
	cout << endl;
}

/**
* standard STL stack printing function
* Used when we want to print a stack, which was in assignment #4.
* Will empty the songstack after the end of the function via popping
* @param songstack takes in a the stack which user wants to print
*/
void printStack(stack<element> songstack)
{
	cout << "     Stick contains: ";
	while (songstack.size() != 0) //if the stack isn't empty pop and print
	{
		cout << songstack.top().getName() << " ";
		songstack.pop();
	}
	cout << endl; //spacing for neatness
	cout << endl;
}

/**
* This function is used to find the tag which is between a closed XML tag
* @param line current XML line to be parsed
* @return returns a formatted closed XML line
*/
string getClosed(string line)
{
	int startPos; //where to start the substr
	int endPos; //where to end the substr
	string finalTag;

	startPos = line.find("/"); //to get tagname without slash
	endPos = line.find(">", startPos + 1); //sets end pos to the closing bracket
	finalTag = line.substr(startPos + 1, (endPos - startPos) - 1); //creates a new string that is the closing tag

	return finalTag;
}

/**
* This function which handles the correct parsing of XML lines which contain XML attributes
* This function also puts attribute information into the STL map
* This function is called whenever we want to figure out an XML line's attributes
* Handles all of the attribute parsing
* @param line current XML line which is being parsed
* @param ele passes in the pointer to the current element being parsed
*/
void getAtrribute(string line, element* tag)
{
	int startPos;                                                                   //where to start the substr
	int endPos;
	int idstartPos;                                                                   //where to start the substr
	int idendPos;
	int valuestartPos;                                                                   //where to start the substr
	int valueendPos = 0;
	string insideBrackets, attribute, value;                                    //strings which we will use to parse data
	int equalSign;
	startPos = line.find("<");
	endPos = line.find(">", startPos + 1);                  //goes through "line" and finds what's in the brackets

	insideBrackets = line.substr(startPos + 1, (endPos - startPos) - 2); //creates a new string that is just what's in the brackets

	while (insideBrackets.find('=', valueendPos) != string::npos)//while there is an equal sign ahead of the previos attribute valueendpos starts at zero
	{
		idstartPos = insideBrackets.find(' ', valueendPos); // looks for the space in between ex.) idstartPos-id-idendPos
		idendPos = insideBrackets.find("=", idstartPos); //gets the equal sign 
		attribute = insideBrackets.substr(idstartPos + 1, (idendPos - idstartPos) - 1);

		valuestartPos = idendPos + 1;  //valuestartPos starts right after the equal sign
		if (insideBrackets.find(' ', idendPos) != string::npos) //if there is a space ahead, end at the space
		{
			valueendPos = insideBrackets.find(' ', idendPos);
			value = insideBrackets.substr(valuestartPos + 1, ((valueendPos)-valuestartPos) - 2);
		}
		else //if there is a bracket at the end end right before that
		{
			valueendPos = insideBrackets.find('\"', valuestartPos); //looks for the last 
			value = insideBrackets.substr(valuestartPos + 1, (valueendPos - valuestartPos) - 2);
		}

		tag->setMapValues(attribute,value);
	}

}
/*
void getAtrribute(string line, element* ele)
{
	int startPos;									//where to start the substr
	int endPos;										//where to end the substr
	string initialTag, finalTag;					//strings which we will use to parse data 
	//map<string, string> mappy;						//map which will store the attribute and it's value
	startPos = line.find("<");
	endPos = line.find(">", startPos + 1);			//goes through "line" and finds what's in the brackets

	initialTag = line.substr(startPos + 1, (endPos - startPos) - 1); //creates a new string that is just what's in the brackets
	startPos = initialTag.find("<");
	endPos = initialTag.find(">", startPos + 1);	//resets end positions to correspond with the new string

	if (initialTag.find(" ") != string::npos){

		startPos = initialTag.find(" ");
		endPos = initialTag.find(">");
		finalTag = initialTag.substr(startPos + 1, (endPos - startPos) - 1);
	}

	string attribute;							//string for the attribute
	string value;							//string for the attribute's value
	if (finalTag.find('=') != string::npos){
		//Code which finds the attribute's name, which lives before the equals sign
		cout << "Found attribute with = " << finalTag << endl;
		//this find function is an overloaded version of find,
		//where the 1 represents the beginning of the string 
		startPos = finalTag.find("", 1);
		endPos = finalTag.find('=');
		//cout << "Startpos is:" << startPos << "Endpos is: " << endPos << endl;
		attribute = finalTag.substr(startPos - 1, endPos);

		//Erases what's in between the value's quotation marks for easy parsing because using find with "s is difficult
		finalTag.erase(remove(finalTag.begin(), finalTag.end(), '\"'), finalTag.end());
		//cout << "FinalTag after erase: " << finalTag << endl;

		//Code which finds the attribute's value, which lives after the = and before the newline
		startPos = finalTag.find('=');
		endPos = finalTag.find('\n');
		value = finalTag.substr(startPos + 1, endPos);
		ele->setAttributeStatus(true);
		cout << "attribute is: " << attribute << " value is: " << value << endl;
		cout << ele->isaAttribute() << endl;

		if (value.find('=') != string::npos){

			startPos = finalTag.find(" ");
			endPos = finalTag.find('=');
			attribute = finalTag.substr(startPos, endPos);

			startPos = finalTag.find('=');
			endPos = finalTag.find('\n');
			value = finalTag.substr(startPos + 1, endPos);
			cout << "attribute is: " << attribute << " value is: " << value << endl;

		}
		//set to map, then do this for the value
		ele->setMapValues(attribute, value);
	}
	//ele->setAttribute(attribute);
	//ele->setValue(value);
	//mappy.insert(pair<string, string>(attribute, value));
	//return finalTag;
}*/
/**
* Function which parses open tags and returns them cleanly
* Used whenever we want to find the tag of an open line
* @param line current dirty line being parsed
* @return returns a nice and clean line, free of spaces and other stuff we don't want
*/
string getOpen(string line)
{

	int startPos; //where to start the substr
	int endPos; //where to end the substr
	string initialTag, finalTag;

	startPos = line.find("<");
	endPos = line.find(">", startPos + 1); //goes through "line" and finds what's in the brackets

	initialTag = line.substr(startPos + 1, (endPos - startPos) - 1); //creates a new string that is just what's in the brackets
	startPos = initialTag.find("<");
	endPos = initialTag.find(">", startPos + 1); //resets end positions to correspond with the new string

	if (initialTag.find(" ", startPos + 1) != string::npos) //checks if there is a space in the string (ids, etc)
	{
		if (ispunct(initialTag.at(startPos + 1))) //to avoid random punct.
		{
			startPos++;
		}
		endPos = initialTag.find(" ", startPos + 1); //sets the endPos to where the first instance of the space is

		//cout << "initial tag is: " << initialTag << endl;
		finalTag = initialTag.substr(startPos + 1, (endPos - startPos) - 1); //get's rid of what is after the space and stores in finalTag
	}

	if (initialTag.find(" ", startPos + 1) == string::npos) //if there is no space in the tag
	{
		if (ispunct(initialTag.at(startPos + 1))) //to avoud random punct.
		{
			startPos++;
		}
		finalTag = initialTag.substr(startPos + 1, (endPos - startPos) - 1); //getting the finalTag
	}

	return finalTag;
}
/**
* This is the big papa print statement which will print everything inside of the elementTree
* in a nice and clean manner
* Used after we do the parsing for Assignment #4
* @param elementTree Takes in a vector (which is technically a tree, even though it's a vector)
*/

void printEverything(vector<element> elementTree)
{
	vector<element>::reverse_iterator it;				//iterator used to fill up a map of elements
	vector<element*>::iterator jt;						//second iterator used to fill children into the elementmap
	vector<element*> childrenVec;						//vector of pointers to store children for parsing in
	char separator = '.'; //using the i/o manipulation library adding spaces to fill the rest of the width
	map<int, element> elementMap;						//map of elements used for print iteration 
	map<int, element>::iterator itr;					//map itr used to print 
	string to_be_used;									//string that evaluates which child/children to be used via ? operator

	cout << endl;

	/*string to_be_used;
	put on first level
	to_be_used = it->getChildSize() == 1 ? " child " : " children ";
	put on second level
	to_be_used = (*jt)->getChildSize() == 1 ? " child " : " children ";

	*/
	//this function fills up the element map with information from the element tree vector
	for (it = elementTree.rbegin(); it<elementTree.rend(); ++it)
	{
		elementMap.insert(pair<int, element>(it->getlineNum(), (*it)));

		if (it->getChildSize() != 0)
		{

			childrenVec = it->getVector();
			for (jt = childrenVec.begin(); jt<childrenVec.end(); ++jt)
			{
				elementMap.insert(pair<int, element>((*jt)->getlineNum(), *(*jt)));
			}

		}

	}
	//big papa print statement for the entire element tree
	for (itr = elementMap.begin(); itr != elementMap.end(); ++itr) //printing the map using io manipulators that are based off the level
	{
		//figures out if the current iteration is a content tag
		if (itr->second.hasContent()){
			cout << left << setw(itr->second.getLevel()) << setfill(separator) << "";
			cout << itr->second.getName() << " has " << itr->second.getChildSize() << " children and contains \"" << itr->second.getContent() << "\"" << endl;
			if (itr->second.isaAttribute())
			{
				cout << left << setw(itr->second.getLevel()) << setfill(separator) << "";
				cout << "";
				itr->second.printAttributes();
			}
		}
		//else if is not a content tag
		else
		{
			cout << left << setw(itr->second.getLevel()) << setfill(separator) << "";
			cout << itr->second.getName() << " has " << itr->second.getChildSize() << " children and has no content" << endl;
			if (itr->second.isaAttribute())
			{
				cout << left << setw(itr->second.getLevel()) << setfill(separator) << "";
				cout << "";
				itr->second.printAttributes();
			}
		}
	}


	//the whole loop structure below got way too confusing/ridiculous looking with the ? functionality, so I removed it

	//	//I was playing around the ? operator, but what I've done seems to be completely
	//	//obsolete due to the loop it requires to work. oh well
	//	//to_be_used = itr->second.getChildSize() == 1 ? "a child " : " children ";
	//	//if (to_be_used.find("a child") != string::npos){
	//	//	cout << left << setw(itr->second.getLevel()) << setfill(separator) << "";
	//	//	cout << itr->second.getName() << " has " << to_be_used << endl;
	//	//}
	//	//else{
	//	//	cout << left << setw(itr->second.getLevel()) << setfill(separator) << "";
	//	//	cout << itr->second.getName() << " has " << itr->second.getChildSize() << " children" << endl;
	//	//}
	//}
}

/**
* This function is the powerhouse of Assignment Five
* It'll go through the parsed elementTree, element by element,
* decide what the element is, and convert accordingly.
* This could be smaller and more modular
* @param elementTree takes in parsed XML tree for JSON conversion
*/
void printJson(vector<element> elementTree){
	vector<element>::reverse_iterator it;				//iterator used to fill up a map of elements
	vector<element*>::iterator jt;						//second iterator used to fill children into the elementmap
	vector<element*> childrenVec;						//vector of pointers to store children for parsing in
	vector<element*>::iterator ij;						//for stuff
	char separator = '.'; //using the i/o manipulation library adding spaces to fill the rest of the width
	map<int, element> elementMap;						//map of elements used for print iteration 
	map<int, element>::iterator itr;					//map itr used to print 
	string to_be_used;									//string that evaluates which child/children to be used via ? operator
	ofstream jsonFile;									//ofstream file object used for json output
	string value, attribute;							//strings for attribute name and value
	
	
	//instantiates jsonFile to the outputjson.json file local to our file
	jsonFile.open("outputjson.json");

	//this function fills up the element map with information from the element tree vector
	for (it = elementTree.rbegin(); it<elementTree.rend(); ++it)
	{
		elementMap.insert(pair<int, element>(it->getlineNum(), (*it)));

		if (it->getChildSize() != 0)
		{

			childrenVec = it->getVector();
			for (jt = childrenVec.begin(); jt<childrenVec.end(); ++jt)
			{
				elementMap.insert(pair<int, element>((*jt)->getlineNum(), *(*jt)));
			}

		}

	}
	//big papa print statement for the entire json file
	jsonFile << "{\n";
	for (itr = elementMap.begin(); itr != elementMap.end(); ++itr) //printing the map using io manipulators that are based off the level
	{
		
		//figures out if the current iteration is a content tag
		if (itr->second.hasContent()){
			if (!itr->second.isaAttribute()){
				//if the current line of XML is a child element w/ no children
				//and the next line is not an attribute (ie where you don't want a comma)
				//add in a comma
				if (itr->second.getChildSize() == 0 && (next(itr)->second.isaAttribute()==false)){
					jsonFile << "\"" << itr->second.getName() << "\":   " << "\"" << itr->second.getContent() << "\"";
					
					if (next(itr)->second.getChildSize() > 1){
						//cout << "next(itr)->second.getName() :" << next(itr)->second.getName() << endl;
						jsonFile << "\n}";
					}
					jsonFile << ",\n";


					//cout << "1) itr->second.getContent is: " << itr->second.getName() << " adding comma" << endl;
				}
				//if the current line of XML is a child w/ no children
				//and the next line IS an attribute, ie you want a } with a comma
				else if (itr->second.getChildSize() == 0 && (next(itr)->second.isaAttribute() == true)){
					jsonFile << "\"" << itr->second.getName() << "\":   " << "\"" << itr->second.getContent() << "\"";
					jsonFile << "\n},\n";
					//cout << "2) itr->second.getContent is: " << itr->second.getName() << " adding comma and curly bracket" << endl;
				}
				//if the current line is not a child element, it is a parent element
				else{
					jsonFile << "\"" << itr->second.getName() << "\":   " << "\"" << itr->second.getContent() << "\"\n";
					//cout << "3) itr->second.getContent is: " << itr->second.getName() << 
						//"adding neither comma and curly bracket" << endl;
				}
				
				
				

			}
			/*childrenVec = itr->second.getVector();
			for (ij = childrenVec.begin(); ij != childrenVec.end(); ++ij){*/
				//handling for all elements that are attributes
				if (itr->second.isaAttribute())
				{
					//tempMap = itr->second.getMap();
				
						jsonFile << "\"" << itr->second.getName() << "\":   ";
					
					//big if loop which evaluates on the basis that the next element isn't the end
					if (next(itr) != elementMap.end()){
						if (next(itr)->second.getName() == itr->second.getName()){
							jsonFile << "[";
						}
						//prints the current element to json file in json format
						itr->second.printAttributesToJson(jsonFile);
						//if the next element is not the end, print a comma
						if (next(itr) != elementMap.end()){
								jsonFile << ",\n";
							
						}
						//if the next is equal to the current, ie it's the end of the file but not the end of the children
						//convoluted, and I think unused in most cases.
						if (next(itr)->second.getName() == itr->second.getName()){
							next(itr)->second.printAttributesToJson_end(jsonFile);
							itr++;
						}
						//if it's an array element, and it's not the last one
						if (!next(itr)->second.isaAttribute()){
							jsonFile << "],";
						}
						//else if we need a ] ie ends an array and it's the last array
						else if (next(itr)->second.getName() == itr->second.getName()){
							jsonFile << "]";
						}
					}
					else{
						//else, none of these special cases, just print the JSON
						itr->second.printAttributesToJson(jsonFile);
					}

					//itr->second.printAttributes();

				}
			}
		//else if is not a content tag
		else
		{
			//if the current element's child size is 0, ie it is a child (ie not a parent)
			//print a comma
			if (itr->second.getChildSize() == 0){
				jsonFile << ",\n";
			}
			//prints the opening bracket for the current json object, along with it's name 
			jsonFile << "\"" << itr->second.getName() << "\"" << "   :\n{" << endl;
			if (itr->second.isaAttribute()){
				itr->second.printAttributesToJson(jsonFile);
				//cout << "value is: " << itr->second.getValue() << endl;
			}
		}
		
	}

	//prints the last closing bracket to end the json file
	jsonFile << "\n}" << endl;

}

/**
* Used whenever we want to create a new element with content
* Function finds what's content and adds it to the private member variable "content"
* @param line takes in current line of XML which is a content line
*/
void element::setContent(string line) //sets the content
{
	int startPos, endPos; //start and end position of line substr
	string elementContent; //after editing line

	startPos = line.find(">");
	endPos = line.find("<", startPos + 1); //getting what's between the tags, ie content
	elementContent = line.substr(startPos + 1, (endPos - startPos) - 1); //getting content

	content = elementContent;
}
/**
* A quick getter function to find if the current element has content
* Used whenever we want to evaluate if an XML line has content for specific parsing
* @return returns true/false based on content presence
*/

bool element::hasContent(void)
{
	if (!content.empty())
	{
		return true;
	}

	return false;
}

/**
* A quick getter function to find if the current element has an attribute
* Used whenever an if loop attempts to evaluate if the current XMl file is an attribute,
* so the program can parse specifically
* @return returns true/false based on attribute presence
*/
bool element::isaAttribute(void){
	if (attributeMap.empty())
	{
		return false;
	}
	if (!attributeMap.empty()){
		return true;
	}
}
/**
* Member function which prints the element's attributeMap in a nice way
* We use this whenever we want to print XML elements which have attributes
*/
void element::printAttributes(){
	map<string, string>::iterator itr;
	for (itr = attributeMap.begin(); itr != attributeMap.end(); ++itr){
		cout << "Attribute: " << itr->first << " has value: " << itr->second << endl;
	}

}
/**
* This function is the function prints the attribute XML lines into JSON
* The purpose of this function is to make the program more modular and cleaner.
* Attribute syntax is specific in JSON, so we cannot use conventional XML to JSON conversion
* @param jsonFile Passes in the output json file so we can parse through modular means 
*/

void element::printAttributesToJson(ofstream& jsonFile){
	map<string, string>::iterator itj;
	
	for (itj = attributeMap.begin(); itj != attributeMap.end(); ++itj){
		if (itj == attributeMap.begin()){
			jsonFile << "\n{\n";
		}
		jsonFile << "\"" << itj->first << "\": " << "\"" << itj->second << "\", " << endl;
		jsonFile << "\"" << "content" << "\": " << "\"" <<content << "\"";
		if (next(itj) != attributeMap.end()){
			jsonFile << ",\n";
		}
		else{
			jsonFile << "\n";
		}
		/*jsonFile << "},\n";*/
		//if (next(itj) == attributeMap.end()){
		//	cout << "mememem" << endl;
		//	jsonFile << "\n}\n";

		//}
		//jsonFile << "]";
		//jsonFile << "]";
	}	
	jsonFile << "}";
	//jsonFile << "\n}\n";
	//jsonFile << "]";
}
	/**
	* This function is the function prints the attribute XML lines into JSON
	* The purpose of this function is to make the program more modular and cleaner.
	* Attribute syntax is specific in JSON, so we cannot use conventional XML to JSON conversion
	* This function is nearly identical to the printAttributesToJson function, however 
	* attributes which end the JSON document need to be parsed differently. 
	* @param jsonFile Passes in the output json file so we can parse through modular means
	*/

void element::printAttributesToJson_end(ofstream& jsonFile){
	map<string, string>::iterator itj;

	for (itj = attributeMap.begin(); itj != attributeMap.end(); ++itj){
		if (itj == attributeMap.begin()){
			jsonFile << "\n{\n";
		}
		jsonFile << "\"" << itj->first << "\": " << "\"" << itj->second << "\", " << endl;
		jsonFile << "\"" << "content" << "\": " << "\"" <<  content;
		
		if (std::next(itj) != attributeMap.end()){
			jsonFile << "\"," << endl;
		}
		else{
			jsonFile << "\"" << endl;
		}

		/*jsonFile << "},\n";*/
		if (next(itj) == attributeMap.end()){
			//cout << "mememem" << endl;
			jsonFile << "\n},\n";

		}
		//jsonFile << "]";
		//jsonFile << "]";
	}
	//jsonFile << "\n}\n";
	//jsonFile << "]";
}