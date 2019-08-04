/**

Jinxiu Liu
Date: 4/4/2019
Professor: Ioannis Stamos
This is a cc file called create_and_test_hash.cc

    This file will test double_hashing, linear_probing, and quadratic probing:
		Part 1:	Read all words from a given text file: words.txt and insert them into a hash table.
				After the table	is created, print out the total number of elements in the table	(N), the size of table (T),	the	load factor	(N/T),
				the	total number of	collisions (C),	and	the	average	number of collisions (C/N).		
		Part 2: Check whether each word	in another given file query_words.txt is in	the	hash table or not.
				For	each word that is found	print the word, the string "Found", and	the	number of probes used.
				For	each word not found, print the word, the string	"Not Found"	and	the	number of probes used.	
	
**/

#include "quadratic_probing.h"
#include "linear_probing.h"
#include "double_hashing.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, const string &query_filename) {
	cout << "TestFunctionForHashTables..." << endl;
	cout << "Words filename: " << words_filename << endl;
	cout << "Query filename: " << query_filename << endl;
	hash_table.MakeEmpty();  
	//..Insert your own code
	ifstream file;
	string getStr;

	file.open(words_filename);
	int countColli = 0;
	while (file >> getStr && getStr != "")
	{
		hash_table.Insert(getStr,countColli);
		getStr = "";
	}

	file.close();

	cout << "Collisions: " <<countColli << endl;                                 //print # of collisions
	 int getSize = hash_table.GetItemSize();
	 cout << "Number of items: " << getSize << endl;                            //print # of items in table
	 cout << "Size of hash table: "<< hash_table.GetTableSize() << endl;       //print the size of table
	 cout << "Load factor: " << (float)getSize / hash_table.GetTableSize() << endl;  //print the load factor
	 cout << "Avg. number of collisions: "<< (float)countColli/ getSize << endl;       //print average number of collisions
//---------------------------------------------------------------------------------
	ifstream fileQ;
	getStr="";
	fileQ.open(query_filename);
	int probes = 1;
	while (fileQ >> getStr && getStr != "")
	{
		if (!hash_table.Contains(getStr, probes))
			cout << getStr << ": Not Found, " << "Number of probes: " << probes << endl;      //print not found if string is not found
		else
			cout << getStr << ": Found, " << "Number of probes: " << probes << endl;          //print found if string is found
		probes = 1;
	}
}

// Sample main for program create_and_test_hash
int
main(int argc, char **argv) {
	if (argc != 4) {
		cout << "Usage: " << argv[0] << " <wordsfilename> <queryfilename> <flag>" << endl;
    return 0;
}
	const string words_filename(argv[1]);
	const string query_filename(argv[2]);
	const string param_flag(argv[3]);

	if (param_flag == "linear") {
		HashTableLinear<string> linear_probing_table;
		TestFunctionForHashTable(linear_probing_table, words_filename, query_filename);    
	} else if (param_flag == "quadratic") {
		HashTable<string> quadratic_probing_table;
		TestFunctionForHashTable(quadratic_probing_table, words_filename, query_filename);    
	} else if (param_flag == "double") {
		HashTableDouble<string> double_probing_table;
		TestFunctionForHashTable(double_probing_table, words_filename, query_filename);    
	} else {
		cout << "Uknown tree type " << param_flag << " (User should provide linear, quadratic, or double)" << endl;
	}
	return 0;
}




