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
string englishListPath = "NewEnglishList.txt";
string varNamesFile = "VariableNames.txt";

vector <string> parseAndToVec(char firstStartTag, char secondStartTag, char endTag, char charConstraint, vector<string> &outputVec, string &filePath, string &outputFileName)
{
	//Character
	char ch;

	//File stream variable 
	ifstream xmlInput;

	//Tells the ifstream to not ignore white space
	xmlInput >> skipws;

	//Open the original resource file 
	xmlInput.open(filePath);

	//This provides a way to look at the last couple of characters to determine
	//if a sequence of characters is a part of a word 
	vector<char> charBuffer;

	//Roughly counts number of characters 
	size_t counter = 0;

	//Counts the number of strings taken in
	size_t stringCount = 0;

	while (xmlInput >> ch)
	{
		//pushes the current read character on the buffer
		charBuffer.push_back(ch);

		//Checks to make sure the charBuffer size is greater than two to prevent
		//an indexing error; Checks the conditions of the tags
		//essentially it is looking for "> to be present
		if (charBuffer.size() > 2  && charBuffer[counter - 2] == firstStartTag && charBuffer[counter - 1] == secondStartTag)
		{
			//Being of the loop for reading the whole word 
			do
			{
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
	//Character
	char ch;

	//File stream variable 
	ifstream xmlInput;

	//Tells the ifstream to not ignore white space
	xmlInput >> noskipws;

	//Open the original resource file 
	xmlInput.open(filePath);

	//This provides a way to look at the last couple of characters to determine
	//if a sequence of characters is a part of a word 
	vector<char> charBuffer;

	//Roughly counts number of characters 
	size_t counter = 0;

	//Counts the number of strings taken in
	size_t stringCount = 0;

	while (xmlInput >> ch)
	{
		//pushes the current read character on the buffer
		charBuffer.push_back(ch);

		//Checks to make sure the charBuffer size is greater than two to prevent
		//an indexing error; Checks the conditions of the tags
		//essentially it is looking for "> to be present
		if (charBuffer.size() > 2 && charBuffer[counter - 1] == secondStartTag && charBuffer[counter - 2] == firstStartTag)
		{
			//Being of the loop for reading the whole word 
			do
			{
				//Break out of the loop if the character is '<' which indicates the end of what we want
				if (ch == endTag)
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

	//output stream variable 
	ofstream parsedOutput;

	//Open the parsed output file
	parsedOutput.open(outputFileName);

	//Start to iterate through the parsedWords vector 
	for (size_t i = 0; i <= stringCount; i++)
	{
		//Output each string in the vector and then create a newline 
		parsedOutput << outputVec[i] + "\n";
	}

	//Close the fstreams 
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

void introCouts()
{
	cout << "This is a quick utility to parse an XML language resource file.";
	cout << "\n";
	cout << "This file also generates the content of the XML for a new language file.";
	cout << "\n";
	cout << "For example a file in the following format works: ";
	cout << "\n";
	cout << "Ich habe";
	cout << "\n";
	cout << "Ja";
	cout << "\n";
	cout << "Nein";
	cout << "\n";
	cout << "etc. \n";
	cout << "Please enter the file path for the orinial xml that you would like to be parsed. Use \\ instead of just \. e.g. C:\\Desktop\\myfile.txt: ";
	cout << "\n";

	getline(cin, orignalXMLFilePath);

	cout << "This is the file path you have selected: ";
	cout << orignalXMLFilePath;
	cout << "\n";
	cout << "Please enter the file path for the language file you would like an xml made for: ";
	cout << "\n";

	getline(cin, langFilePath);

	cout << "This is the file path you have selected: ";
	cout << "\n";
	cout << langFilePath;
	cout << "\n";

	cout << "Please enter the name you would like for the new language xml file with the file path included. e.g. C:\\Desktop\\myNEWfile.txt: ";
	cout << "\n";

	getline(cin, outputFile);

	cout << "\n";
	cout << "This is the file name you have entered: ";
	cout << "\n";

	cout << outputFile;

	cout << "\n";

	cout << "Working...";
	cout << "\n";
}

int main()
{
	introCouts();
	parseAndToVecEng('"', '>', '<', englishParsed, orignalXMLFilePath, englishListPath);
	parseAndToVec('=', '\"', '\"', '=', varNames, orignalXMLFilePath, varNamesFile);
	langFileToVec(langFilePath, parsedNewLangVec, varNames);
	writeNewXML(parsedNewLangVec, varNames, outputFile);

	cout << "Everthing has now been parsed. Your files are now available.";
	cout << "\n";

	cin.get();

	return 0;
}



