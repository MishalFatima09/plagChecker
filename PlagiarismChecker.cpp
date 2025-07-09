#include <iostream>
#include <fstream>
#include <cmath>
#include<cstring>
#include<string>
using namespace std;

//Mishal Fatima
//23i-0776
//Section C

const int MAX_WORDS = 100;
const int MAX_LINE_LENGTH = 1000;
const int MAX_LETTERS = 100;
const int MAX_WORD_LENGTH = 1000;
const int MAX_DOCS = 10;

class PlagiarismChecker
{
private:
	

public:
	//moved to public for testing in main
	//i feel like everything should be in public since the functions are global
	char stopWords[MAX_WORDS][MAX_WORD_LENGTH];
	int stopWordCount;

	char documentPaths[MAX_DOCS][MAX_LINE_LENGTH];
	int numberOfDocuments;

	char documents[MAX_DOCS][MAX_WORDS];

	char** uniqueWords; 
	int** documentFrequency; 
	int uniqueCount;

	double ** similarities; 


	//Constructor
	PlagiarismChecker()
	{
		this->stopWordCount = 0;
		this->numberOfDocuments = 0;
		this->uniqueCount = 0;
		this->uniqueWords = new char* [MAX_WORDS];
		for (int i = 0; i < MAX_WORDS; i++) {
			uniqueWords[i] = new char[MAX_WORD_LENGTH];
		}

		documentFrequency = new int* [MAX_WORDS];
		for (int i = 0; i < MAX_WORDS; i++) {
			documentFrequency[i] = new int[MAX_DOCS];

			for (int j = 0; j < MAX_DOCS; j++) {
				documentFrequency[i][j] = 0;
			}
		}

		similarities = new double* [MAX_WORDS];
		for (int i = 0; i < MAX_WORDS; i++) {
			similarities[i] = new double[MAX_DOCS];

			for (int j = 0; j < MAX_DOCS; j++) {
				similarities[i][j] = 0;
			}
		}
		//cout<<this->uniqueCount<<endl;
	}
	//
	~PlagiarismChecker()
	{
		//wow shes so responsible she frees up the memory
		for (int i = 0; i < MAX_WORDS; i++) {
			delete[] uniqueWords[i];
		}
		delete[] uniqueWords;

		for (int i = 0; i < MAX_WORDS; i++) {
			delete[] documentFrequency[i];
		}
		delete[] documentFrequency;
	}
}checker;

void readInput(const char* pathofInputFile)
{
	ifstream file(pathofInputFile);
	if (!file) {
		cout << "error" << endl;
	}
	if (file.peek() == EOF) {
		checker.numberOfDocuments = 0;
		file.close();
		return;
	}

	char line[MAX_LINE_LENGTH];
	if (file.getline(line, MAX_LINE_LENGTH)) {
		int i = 0, j = 0;

		//idk why they added the words  "Stop Words: " these 
		// shouldnt be added to the array of stop words
		//pls dont remove that in the next test cases T-T
		while (line[i] != '\0' && (line[i] != ':' || line[i + 1] != ' ')) {
			i++;
		}

		// iterate over the ": " to the start of the actual stop words
		i += 2;


		while (line[i] != '\0')
		{
			if (line[i] == ' ')
			{
				checker.stopWords[checker.stopWordCount][j] = '\0';
				if (j > 0)
				{
					checker.stopWordCount++;
				}
				j = 0;
			}
			else
			{
				checker.stopWords[checker.stopWordCount][j++] = line[i];
			}
			i++;
		}
		if (j > 0)
		{
			checker.stopWords[checker.stopWordCount][j] = '\0';
			checker.stopWordCount++;
		}
	}
	//read no of docs then add to attribute
	if (file.getline(line, MAX_LINE_LENGTH)) {
		checker.numberOfDocuments = 0;
		for (int i = 0; line[i] != '\0'; i++) {
			checker.numberOfDocuments = line[i] - '0';
		}
	}

	//read the document paths 
	int docIndex = 0;
	while (docIndex < checker.numberOfDocuments && file.getline(line, MAX_LINE_LENGTH)) {
		int i = 0;
		while (line[i] != '\0') {
			checker.documentPaths[docIndex][i] = line[i];
			i++;
		}
		checker.documentPaths[docIndex][i] = '\0';
		docIndex++;
	}

	//now use these document paths to put the documents in my class attribute
	for (int i = 0; i < checker.numberOfDocuments; i++)
	{
		ifstream documentfile(checker.documentPaths[i]);
		if (!documentfile) {
			cout << "error" << endl;
		}
		else
		{
			int j = 0;
			while (documentfile.getline(line, MAX_LINE_LENGTH, '\0'))
			{
				for (int m = 0; line[m] != '\0'; m++)
				{
					checker.documents[i][j++] = line[m];
				}

				checker.documents[i][j++] = ' ';
			}
			checker.documents[i][j] = '\0';
			//for when there is an extra space at the end of the doc
			int length = 0;
			while (checker.documents[i][length] != '\0')
			{
				length++;
			}
			int m = length - 1;
			while (m >= 0 && checker.documents[i][m] == ' ') {
				checker.documents[i][m] = '\0';  
				m--;
			}

			documentfile.close();
		}
		/////////////////////////j
		//cout << "------" << checker.documents[0] << endl;
	}

	file.close();
}

int getNumberOfDocument() 
{ return checker.numberOfDocuments; }

char* getText(int documentNumber) 
{
	//seedha seedha document number dedetay tou jaan jaani thi?
	//cout << checker.documents[documentNumber - 1] << endl;
	return checker.documents[documentNumber -1]; }

bool checkLetter(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool checkSpace(char c)
{
	return c == ' '; // \n?
}

bool CompareWords(char* word1, char* word2)
{
	//maybe i should have just made a friggin string class
	for (int i = 0; word1[i] != '\0' || word2[i] != '\0'; i++)
	{
		if (word1[i] != word2[i])
		{
			return false;
		}
	}
	return true;
}

double roundOff(double value) 
{
	return round(value * 100.0) / 100.0;
}

void removePunctuationMarks()
{
	for (int m = 0; m < checker.numberOfDocuments; m++)
	{
		//m iterates through number of documents
		int cleanIndex = 0;
		bool word = false;

		for (int i = 0; checker.documents[m][i] != '\0'; i++)
		{
			//i iterates through letters characters in the document
			if (checkLetter(checker.documents[m][i])) //if it is a letter na, then add it to the documents, simple
			{
				checker.documents[m][cleanIndex++] = checker.documents[m][i];
				//make word true to show we are inside the word!
				word = true;
			}
			else if (checkSpace(checker.documents[m][i]))
			{
				if (word)
				{
					//if we are already in the word and we encounter a space add it then make the bool 
					checker.documents[m][cleanIndex++] = ' ';
					//then make the bool false to show that aik word poora hougaya
					word = false;
				}
			}
		}
		//this will remove the last space to MAKE THE FKN TEST CASE PASS *eyeroll*
		if (cleanIndex > 0 && checker.documents[m][cleanIndex - 1] == ' ') {
			cleanIndex -= 1;
		}

		checker.documents[m][cleanIndex] = '\0';

	}
	
}

void convertUpperToLowerCase()
{
	for (int i = 0; i < checker.numberOfDocuments; i++)
	{
		for (int j = 0; checker.documents[i][j] != '\0'; j++)
		{
			if (checker.documents[i][j] >= 'A' && checker.documents[i][j] <= 'Z')
			{
				//easy sa formula
				checker.documents[i][j] = (checker.documents[i][j] + ('a' - 'A'));
			}
		}
	}
}

void removeStopWords()
{
	//if docs words == stopwords    || 11.37pm
	//remove						||
	// 
	//damn mishal slow down u done solved the entire issue		|| 10.14 am


	for (int m = 0; m < checker.numberOfDocuments; m++)
	{
		//m iterates through number of documents

		char cleanedDocument[MAX_WORDS] = "";
		char word[MAX_LETTERS] = "";
		int index = 0;
		int cleanIndex = 0;

		for (int i = 0; checker.documents[m][i] != '\0'; i++)
		{

			//i iterates through characters in the document
			if (checker.documents[m][i] != ' ' && checker.documents[m][i] != '\n')
			{
				word[index++] = checker.documents[m][i];
			}
			else
			{
				word[index] = '\0';
				bool matches = false;

				for (int j = 0; j < checker.stopWordCount; j++)
				{
					//j iterates through number of stop words
					if (CompareWords(checker.stopWords[j], word))
					{
						matches = true;
						break;
					}
					
				}

				if (!matches)
				{
					for (int l = 0; word[l] != '\0'; l++)
					{
						//l iterates through the characters in our temporary word
						cleanedDocument[cleanIndex++] = word[l];
					}
					cleanedDocument[cleanIndex++] = ' ';
				}
				
				index = 0;

				//line break?
				if (checker.documents[m][i] == '\n') {
					cleanedDocument[cleanIndex++] = '\n';
				}


			}
		}

		//to add the last word since we checked the ending of each word on space 
		word[index] = '\0';  

		bool matches = false;
		for (int j = 0; j < checker.stopWordCount; j++) 
		{
			if (CompareWords(checker.stopWords[j], word)) 
			{
				matches = true;
				break;
			}
		}
		if (!matches) 
		{
			for (int p = 0; word[p] != '\0'; p++) {
				cleanedDocument[cleanIndex++] = word[p];
			}
		}
		//
		cleanedDocument[cleanIndex] = '\0';

		int k = 0;
		for (k = 0; cleanedDocument[k] != '\0'; k++)
		{
			checker.documents[m][k] = cleanedDocument[k];
			
		}
		
		checker.documents[m][k] = '\0';
		/*for (int i = 0; checker.documents[m][i] != '\0'; i++)
		{
			cout << checker.documents[m][i];
		}
		cout << endl;*/
		////
		int length = 0;
		while (checker.documents[m][length] != '\0')
		{
			length++;
		}
		int p = length - 1;
		while (p >= 0 && checker.documents[m][p] == ' ') {
			checker.documents[m][p] = '\0';
			p--;
		}
		//////////
	}
	


}

void generateFrequencies(char**& uniqueWords, int& uniqueCount, int**& documentFrequency) {
	
	uniqueCount = 0;

	uniqueWords = new char* [MAX_WORDS];
	for (int i = 0; i < MAX_WORDS; i++) {
		uniqueWords[i] = new char[MAX_WORD_LENGTH];
	}

	documentFrequency = new int* [MAX_WORDS];
	for (int i = 0; i < MAX_WORDS; i++) {
		documentFrequency[i] = new int[MAX_DOCS]();
	}

	for (int m = 0; m < checker.numberOfDocuments; m++) 
	{
		for (int i = 0; checker.documents[m][i] != '\0';) 
		{
			char word[MAX_WORD_LENGTH] = ""; ///gonna make like a buffer word
			int wordIndex = 0;

			while (checker.documents[m][i] != '\0' && checker.documents[m][i] != ' ' && checker.documents[m][i] != '\n') {
				//freaking backslash n gonna be the death of me
				word[wordIndex++] = checker.documents[m][i++];
			}

			//ez enough, right? WRONG!!!!

			word[wordIndex] = '\0';

			if (wordIndex > 0) 
			{
				bool matches = false;
				for (int j = 0; j < uniqueCount; j++) {
					if (CompareWords(word, uniqueWords[j])) {
						matches = true;
						documentFrequency[j][m]++;  // add to frequency if u find
						break;
					}
				}

				if (!matches) 
				{
					//if it doesnt match its a new word!
					//common sense type shi
					//NAHIN BANAAUN GI COPY WORD KA FUNCTION HARR DAFA LOOP LAGAAUN GI
					int p = 0;
					for (p = 0; word[p] != '\0'; p++) 
					{
						uniqueWords[uniqueCount][p] = word[p];
					}
					uniqueWords[uniqueCount][p] = '\0';
					//first time baby! so u put 1
					documentFrequency[uniqueCount][m] = 1; 
					uniqueCount++;
				}
			}

			while (checker.documents[m][i] != '\0' && (checker.documents[m][i] == ' ' 
				|| checker.documents[m][i] == '\n'))
			{
				i++;
			}
		}
	}

	// GOD IT TOOK ME SO LONG TO GET THIS
	checker.uniqueWords = uniqueWords;
	checker.uniqueCount = uniqueCount;
	checker.documentFrequency = documentFrequency;

}

int getFrequency(char* word, int doucmentNum)
{
	for (int i = 0; i < checker.uniqueCount; i++)
	{
		if (CompareWords(checker.uniqueWords[i], word)) 
		{
			return checker.documentFrequency[i][doucmentNum-1]; 
		}

	}
	return -1;
}

void calculateAllCosineSimilarities(double**& similarities, int** documentFrequency)
{

	//copy paste unique words shi
	similarities = new double* [MAX_WORDS];
	for (int i = 0; i < MAX_WORDS; i++) {
		similarities[i] = new double[MAX_WORD_LENGTH]; ////size????????????????????
	}

	double* magnitudes = new double[MAX_DOCS];
	double denominator = 1;
	//magnitude calculation should be done first
	for (int m = 0; m < checker.numberOfDocuments; m++)
	{
		double magnitude = 0;
		double sum = 0;
		for (int i = 0; i < checker.uniqueCount; i++)
		{
			sum += (checker.documentFrequency[i][m] * checker.documentFrequency[i][m]);
		}
		magnitude = sqrt(sum);
		magnitudes[m] = magnitude;
	}

	for (int m = 0; m < checker.numberOfDocuments; m++) 
	{
		for (int n = 0; n < checker.numberOfDocuments; n++) 
		{
			if (m == n) 
			{
				similarities[m][n] = 100;
			}
			else 
			{
			double dot = 0;
			for (int i = 0; i < checker.uniqueCount; i++) 
			{
				//i goes through rows and m & n go throught the different documents
				dot += documentFrequency[i][m] * documentFrequency[i][n];
			}
			//for two decimal place precision and allat
			similarities[m][n] = roundOff( (dot / (magnitudes[m] * magnitudes[n])) * 100);
			}
		}
	}
	//i forgot to put it back into the attributes and almost cried hehe :D
	checker.similarities = similarities;
	delete[] magnitudes;
	
}

double similarityIn(int doucmentNum1, int documentNum2)
{
	return checker.similarities[doucmentNum1 - 1][documentNum2 - 1];
}

//int main() {
//
//	// Path to the input file (modify this path as needed)
//	const char* inputFilePath = "input.txt";
//
//	// Call readInput to process the input file
//	readInput(inputFilePath);
//
//	cout << "this is the get text funxtion running" << endl;
//	cout << getText(0);
//	cout << endl;
//	cout << "fin";
//	cout << endl;
//	cout << "number of docs " << endl;
//	cout << getNumberOfDocument();
//	cout << endl;
//
//	// Output stop words
//	for (int i = 0; i < checker.stopWordCount; i++) {
//		
//		cout << "stop" << endl;
//		cout << checker.stopWords[i] << " ";
//	}
//	cout << endl;
//	cout << checker.numberOfDocuments << endl;
//
//	cout << endl;
//
//	for (int i = 0; i < checker.numberOfDocuments; i++)
//	{
//		cout << checker.documentPaths[i] << endl;
//	}
//
//
//	cout << endl;
//	for (int i = 0; i < checker.numberOfDocuments; i++)
//	{
//		cout << checker.documents[i] << endl;
//	}
//	
//	cout << endl;
//
//	removePunctuationMarks();
//	for (int i = 0; i < checker.numberOfDocuments; i++)
//	{
//		cout << checker.documents[i] << endl;
//	}
//
//	cout << endl;
//	convertUpperToLowerCase();
//	for (int i = 0; i < checker.numberOfDocuments; i++)
//	{
//		cout << checker.documents[i] << endl;
//	}
//	cout << endl;
//	cout << endl;
//	removeStopWords();
//	for (int i = 0; i < checker.numberOfDocuments; i++)
//	{
//		cout << checker.documents[i] << endl;
//	}
//	char** uniqueWords;
//	int uniqueCount;
//	int** documentFrequency;
//	generateFrequencies(uniqueWords, uniqueCount, documentFrequency);
//
//	cout << "In frequency check?" << endl;
//	char name1[] = "movies";
//	char name2[] = "football";
//	char name3[] = "mary";
//
//	/*std::cout << "Unique Words Count: " << uniqueCount << std::endl;
//	for (int i = 0; i < uniqueCount; i++) {
//		std::cout << "Word[" << i << "]: " << checker.uniqueWords[i] << std::endl;
//	}
//
//	std::cout << "Document Frequencies:" << std::endl;
//	for (int i = 0; i < uniqueCount; i++) {
//		std::cout << "Document[" << i << "]: ";
//		for (int j = 0; j < checker.numberOfDocuments; j++) {
//			std::cout << checker.documentFrequency[i][j] << " ";
//		}
//		std::cout << std::endl;
//	}*/
//
//	cout << endl;
//	cout << endl;
//	cout << "done!" << endl;
//	cout << getFrequency(name1, 1) << "----" << endl;
//	cout << getFrequency(name2, 1) << "----" << endl;
//	cout << getFrequency(name3, 2) << "----" << endl;
//	cout << "lalalla" << endl;
//	//for (int i = 0; i < checker.numberOfDocuments; i++)
//	//{
//	//	cout << checker.documents[i] << endl;
//	//}
//	//generateFrequencies(checker.uniqueWords, checker.uniqueCount, checker.documentFrequency);
//	
//	//hmmm
//	cout << "Term Frequencies:" << endl;
//	cout << "Term\tDocument1\tDocument2" << endl;
//	for (int i = 0; i < checker.uniqueCount; i++) {
//		cout << checker.uniqueWords[i] << "\t" << "\t" << checker.documentFrequency[i][0] << "\t\t" << checker.documentFrequency[i][1] << endl;
//	}
//	//calculateAllCosineSimilarities(checker.similarities, checker.documentFrequency);
//
//}
