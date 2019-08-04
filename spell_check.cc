/**

Jinxiu Liu
Date: 4/4/2019
Professor: Ioannis Stamos
This is a cc file called spell_check.cc

Part3:  
	Use a hash_table to	output all of the misspelled words in a document.
	For	each misspelled	word provide a	list of corrections	from the dictionary	that can be	formed by:
		a. Adding one character in each possible position	
		b. Removing	one	character from the word	
		c. Swapping	adjacent characters	in the word
**/
#include "double_hashing.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
/**
   AddOnceChar() function:
		try to insert all the char from a-z, in all positions, to check whether
		there is a word for the corrected word
**/
void AddOneChar(string x, HashTableDouble<string> &hash_table)
{
	string newS = "";
	size_t index = 0;
	while (index <= x.length())                                        //try each position
	{
		int start = 97;                                                // start= 97 -'a'
		int junk = 0;
		while (start <= 122)                                           //try each char from a-z 
		{
			newS = x.substr(0, index) + char(start) + x.substr(index);
			if (hash_table.Contains(newS, junk) == true)
				cout << x << " -> " << newS << " (case a) " << endl;    // print the word, if there is candidate
			start++;
			newS = "";
		}
		start = 97;
		index++;
	}
}
/**
	RemoveOneChar():
		Swap all possible adjacent characters in the word, and check whether 
		there is a word for the corrected word
**/
void RemoveOneChar(string x, HashTableDouble<string> &hash_table)
{
	size_t index = 0;
	string newStr = "";
	while (index < x.length() )
	{
		newStr = x.substr(0, index) + x.substr(index + 1);         //delete one char in a string
		int keeptract = 0;
		if (hash_table.Contains(newStr, keeptract))         
			cout << x<< " -> " << newStr<< " (case b) " << endl;   // print the word, if there is candidate
		index++;
		newStr="";
	}
}

/**
	swapT():
		remove all possible chars and check whether
		there is a word for the corrected word
**/
void swapT(string x, HashTableDouble<string> &hash_table)
{
	size_t count = 0;
	while (count < x.length() - 1)
	{
		string xR = x;
		int junk = 0;
		std::swap(xR[count], xR[count + 1]);                     //swap possible positions to check, whether this is a corrected word in the table
		if (hash_table.Contains(xR, junk) == true)
			cout << x<< " -> " << xR<< " (case c) " << endl;     // print the word, if there is candidate
		count++;
	}
}
// Sample main for program spell_check
int main(int argc, char **argv) {
	if (argc != 3) {
		cout << "Usage: " << argv[0] << " <document-file> <dictionary-file>" << endl;
    return 0;
}
  
	const string document_filename(argv[1]);
	const string dictionary_filename(argv[2]);
	HashTableDouble<string> double_probing_table;                      //create a table
	cout << "Input document filename is " << document_filename << endl;
	cout << "Input dictionary filename is " << dictionary_filename << endl;

	// Call functions implementing the assignment requirements.
	ifstream file;
	string getStr;
	file.open(dictionary_filename);
	int countColli = 0;
	while (file >>getStr && getStr != "")
	{
		double_probing_table.Insert(getStr, countColli);                 //Insert all the words from dictionary into table
		getStr = "";
	}
  
	ifstream document;
	string getStrD;
	document.open(document_filename);                                   //open document file
	while (document >> getStrD && getStrD != "")    
	{
		bool skip = false;
		int junk = 0;
		for (size_t i = 0; i < getStrD.length(); i++)                  //for each word
		{
			getStrD[i] = tolower(getStrD[i]);                         // change each word to lower case
			if (!isalpha(getStrD[i]) )                                //if a char is not a alphabet and if 
			{                                                         //that char is not the last char in the string:
				if (i != getStrD.length() - 1) {                      //we gonna skip the correction for that word because it is
					skip = true;                                      // correct to have situations like: it's.  
					break;
				}
				else
					getStrD = getStrD.substr(0, getStrD.length() - 1);    // else delete the last char
			}
		  
		}

		if (double_probing_table.Contains(getStrD, junk) == false&&skip==false)   //if we can't find the word in table, check following procedures:
		{
			AddOneChar(getStrD, double_probing_table);                        
			RemoveOneChar(getStrD, double_probing_table);
			swapT(getStrD, double_probing_table);
		}
	}
	return 0;
}

