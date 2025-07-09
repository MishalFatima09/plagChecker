#include "pch.h"
#include "PlagiarismChecker.cpp"
#include <fstream>
#include <string>
#include <math.h>

TEST(PC, FileOpen) {

	ifstream is("input.txt", std::ifstream::in);

	if (!is.is_open())
		ASSERT_TRUE(false);
	else
		ASSERT_TRUE(true);

	is.close();
}

TEST(PC, FileReading) {

	readInput("input.txt");	
	char expected[] = "John likes to ,     watch movies. Mary: likes movies too.";
	EXPECT_EQ(0, strcmp(getText(1), expected));		//This should return the content of doucment 1
	EXPECT_EQ(2, getNumberOfDocument());
}


TEST(PC, RemovePunctucationMarks)
{
	readInput("input.txt");
	removePunctuationMarks();
	char expected[] = "Mary also likes to watch football games";
	EXPECT_EQ(0, strcmp(getText(2), expected));
}

TEST(PC, CaseChange) {

	readInput("input.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	char expected[] = "john likes to watch movies mary likes movies too";
	EXPECT_EQ(0, strcmp(getText(1), expected));		//This should return the content of doucment 1
	EXPECT_EQ(2, getNumberOfDocument());
}

TEST(PC, RemoveStopWords) {

	readInput("input.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	removeStopWords();
	char expected1[] = "john likes watch movies mary likes movies";
	char expected2[] = "mary likes watch football games";
	EXPECT_EQ(0, strcmp(getText(1), expected1));		//This should return the content of doucment 1
	EXPECT_EQ(0, strcmp(getText(2), expected2));
	EXPECT_EQ(2, getNumberOfDocument());
}

TEST(PC, FrequencyCheck) {

	readInput("input.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	removeStopWords();
	char** uniqueWords;
	int uniqueCount;
	int** documentFrequency;
	generateFrequencies(uniqueWords, uniqueCount, documentFrequency);
	char name1[] = "movies";
	char name2[] = "football";
	char name3[] = "mary";
	EXPECT_EQ(getFrequency(name1,1),2);
	EXPECT_EQ(getFrequency(name2, 1), 0);
	EXPECT_EQ(getFrequency(name3, 2), 1);
	EXPECT_EQ(uniqueCount, 7);
}

TEST(PC, PlagCheck) {

	readInput("input.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	removeStopWords();
	char** uniqueWords;
	int uniqueCount;
	int** documentFrequency;
	generateFrequencies(uniqueWords, uniqueCount, documentFrequency);
	double** similarities ;
	calculateAllCosineSimilarities(similarities, documentFrequency);

	EXPECT_EQ(similarityIn(1, 1), 100);
	EXPECT_NEAR(similarityIn(2, 1), 53.94, 0.3);
	EXPECT_NEAR(similarityIn(1, 2), 53.94, 0.3);
}

TEST(PC, DocumentWithOnlyStopWords) {
	readInput("input1.txt");
	removeStopWords();
	EXPECT_EQ(0, strcmp(getText(1), ""));	
}

TEST(PC, EmptyInputFile) {
	readInput("input2.txt");  // This file is intentionally left empty
	EXPECT_EQ(0, getNumberOfDocument());
}

TEST(PC, NoCommonWords) {
	readInput("input3.txt");  
	removePunctuationMarks();
	convertUpperToLowerCase();
	removeStopWords();
	char** uniqueWords;
	int uniqueCount;
	int** documentFrequency;
	generateFrequencies(uniqueWords, uniqueCount, documentFrequency);
	double** similarities;
	calculateAllCosineSimilarities(similarities, documentFrequency);

	EXPECT_EQ(similarityIn(1, 2), 0); 
}
