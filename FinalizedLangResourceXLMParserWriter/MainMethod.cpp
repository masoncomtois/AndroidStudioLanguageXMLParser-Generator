//@Author: Mason N. Comtois 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector <string> varNames;
vector <string> englishParsed;
vector <string> parsedNewLangVec;
string orignalXMLFilePath;
string langFilePath;
string outputFile;
string englishListPath;
string varNamesFile;

vector <string> parseAndToVec(char firstStartTag, char secondStartTag, char endTag, char charConstraint, vector<string> &outputVec, string &filePath, string &outputFileName)
{
	char ch;

	//File stream variable that will take in XML
	ifstream xmlInput;

	//Tells the ifstream to not ignore white space
	xmlInput >> skipws;

	xmlInput.open(filePath);

	//This provides a way to look at the last couple of characters to determine
	//if a sequence of characters is a part of a word 
	vector<char> charBuffer;

	//Keeps track of characters 
	size_t counter = 0;

	//Tracks index for the string array
	size_t stringCount = 0;

	while (xmlInput >> ch)
	{
		//pushes the current read character on the buffer
		charBuffer.push_back(ch);

		//Checks to make sure the charBuffer size is greater than two to prevent
		//an indexing error; Checks the conditions of the tags
		//looks for start tags to start putting elements in the current index
		if (charBuffer.size() > 2  && charBuffer[counter - 2] == firstStartTag && charBuffer[counter - 1] == secondStartTag)
		{
			//Beggining of the loop for reading the whole word 
			do
			{
				//If end tag is found, stop reading the word
				if (ch == endTag && charBuffer[counter - 1] != charConstraint)
				{
					break;
				}

				//Pushes an empty string back on the vector 
				//If this wasn't here, there would be an out of bounds error
				outputVec.push_back("");

				//appends the current character to the string 
				outputVec[stringCount] += ch;

				//Continue to read in characters 
			} while (xmlInput >> ch);

			//increments the stringCount since it is done reading that string
			stringCount++;
		}

		//increment counter
		counter++;
	}

	xmlInput.close();

	ofstream fileOut;

	fileOut.open(outputFileName);

	for (size_t i = 0; i < varNames.size(); i++)
	{
		fileOut << outputVec[i] + "\n";
	}

	fileOut.close();

	return outputVec;
}
//No charConstraint
vector <string> parseAndToVecEng(char firstStartTag, char secondStartTag, char endTag, vector<string> &outputVec, string &filePath, string &outputFileName)
{
	char ch;

	ifstream xmlInput;

	xmlInput >> noskipws;

	xmlInput.open(filePath);

	vector<char> charBuffer;

	size_t counter = 0;

	size_t stringCount = 0;

	while (xmlInput >> ch)
	{
		charBuffer.push_back(ch);

		if (charBuffer.size() > 2 && charBuffer[counter - 1] == secondStartTag && charBuffer[counter - 2] == firstStartTag)
		{ 
			do
			{
				if (ch == endTag)
				{
					break;
				}
				outputVec.push_back("");

				outputVec[stringCount] += ch;

			} while (xmlInput >> ch);
			stringCount++;
		}
		counter++;
	}
	ofstream parsedOutput;

	parsedOutput.open(outputFileName);

	for (size_t i = 0; i <= stringCount; i++)
	{
		parsedOutput << outputVec[i] + "\n";
	}
	xmlInput.close();
	parsedOutput.close();

	return outputVec;
}

vector<string> langFileToVec(string &langFileName, vector<string> &outputVec, vector<string> &matchedVec)
{
	size_t stringCount = 0;
	char char2;
	for (size_t i = 0; i < matchedVec.size(); i++)
	{
		parsedNewLangVec.push_back("");
	}

	ifstream langFile;

	langFile >> noskipws;

	langFile.open(langFileName);

	while (langFile >> char2)
	{
		if (char2 == '\n')
		{
			stringCount++;
			continue;
		}
		outputVec[stringCount] += char2;
	}
	return outputVec;
}

void writeNewXML(vector<string> &inputVec, vector<string> &matchedVec, string &outputFileName)
{
	ofstream writeLangXML;

	writeLangXML.open(outputFileName);

	writeLangXML;

	for (size_t i = 0; i < matchedVec.size(); i++)
	{
		if (matchedVec[i] != "")
		{
			writeLangXML << "<string name=";
			writeLangXML << "\"";
			writeLangXML << matchedVec[i];
			writeLangXML << "\"";
			writeLangXML << ">";
			writeLangXML << inputVec[i];
			writeLangXML << "</string>";
			writeLangXML << "\n";
		}
	}
}

//The function to manage file name inquiry. A loop that only breaks until user types in 'y'
void promptLoop(string &stringInQ, string &filePath)
{
	int counter = 0;
	while (true)
	{
		cout << stringInQ;

		getline(cin, filePath);

		cout << "This is the file path you have selected. If you would like to retype type 'n', otherwise type 'y'. \n";
		string resStr;
		getline(cin, resStr);
		if (resStr == "y")
		{
			break;
		}
		else if (resStr == "n")
		{
			cout << "Looping back... \n";
			continue;
		}
		else
		{
			cout << "You typed an invalid response. Looping you back... \n";
			continue;
		}
	
	}
}

//Manages the dialog flow with the user
void introCouts()
{
	string promptOne = "Please enter the file path for the orinial XML that you would like to be parsed. Use \\ instead of just \. e.g. C:\\Desktop\\myfile.txt: \n";
	string promptTwo = "\nPlease enter the file path for the language file you would like an xml made for: \n";
	string promptThree = "\nPlease enter the file path for the language file you would like an xml made for: \n";
	string promptFour = "\nPlease specify where you would like words used (The english words associated with the variables in the original XML) to be located: \n";
	string promptFive = "\nPlease specify where you would like the variable names file to be located and named: \n";

	cout << "This is a quick utility to parse an XML language resource file. \n";
	cout << "This file also generates the content of the XML for a new language file. \n";
	cout << "For example a file in the following format works: \n";
	cout << "Ich habe \n";
	cout << "Ja \n";
	cout << "Nein \n";
	cout << "etc. \n";

	//Prompt loop calls for user
	promptLoop(promptOne, orignalXMLFilePath);
	promptLoop(promptTwo, langFilePath);
	promptLoop(promptThree, outputFile);
	promptLoop(promptFour, englishListPath);
	promptLoop(promptFive, varNamesFile);

	cout << "Working...  \n";
}

int main()
{
	//Call intro prompts
	introCouts();

	//Parse initial XML to the english list of variables
	parseAndToVecEng('"', '>', '<', englishParsed, orignalXMLFilePath, englishListPath);

	//Parse variables names from original XML
	parseAndToVec('=', '\"', '\"', '=', varNames, orignalXMLFilePath, varNamesFile);

	//Parse language file provided by the user
	langFileToVec(langFilePath, parsedNewLangVec, varNames);

	//Write the new XML document
	writeNewXML(parsedNewLangVec, varNames, outputFile);

	cout << "Everthing has now been parsed. Your files are now available where you specified them to be. \n";

	cin.get();

	return 0;
}



