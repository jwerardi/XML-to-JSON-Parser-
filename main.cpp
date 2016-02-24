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


#include "element.h"



int main(int argc, char** argv) {
	//parseFile will parse the file, and then call the JSON conversion at the end of it. 
	parseFile("test.xml");
	cout << "\nXML to JSON File Processing complete. Terminating program.\n";
	return 0;
}

