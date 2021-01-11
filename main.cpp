/*
 *  main.cpp
 *
 *  Created on: Mar 26, 2020
 *  Author: Christopher Valerio
 * 
 * 
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

class runLength
{

	int numRows;
	int numCols;
	int minVal;
	int maxVal;

	int whichMethod;
	string nameEncodeFile;
	string nameDecodeFile;

	int startRow;
	int startCol;
	int color;
	int length;

public:
	runLength();
	runLength(int rows, int cols, int min, int max, int method, string encodeFile, string decodeFile);
	int leftEndPrint();
	int rightEndPrint();
	int distApart();
	void deCodeMethod1(ifstream &encode, ofstream &decode);
	void deCodeMethod4(ifstream &encode, ofstream &decode);
};

// controls flow/execution of program
int main(int argc, char *argv[])
{

	cout << argc << " " << argv[0] << " " << argv[1] << endl;
	string nameEncodeFile = argv[1];
	ifstream encodeFile;
	encodeFile.open(nameEncodeFile.c_str());
	string rows;
	string cols;
	string min;
	string max;
	string method;
	encodeFile >> rows;
	encodeFile >> cols;
	encodeFile >> min;
	encodeFile >> max;
	encodeFile >> method;
	cout << rows << " " << cols << " " << min << " " << max << endl
		 << method << endl;

	string encode = nameEncodeFile;
	nameEncodeFile = nameEncodeFile.substr(0, nameEncodeFile.find_first_of(".", 0));
	nameEncodeFile += "_Decoded.txt";
	ofstream decodeFile;
	decodeFile.open(nameEncodeFile.c_str());
	decodeFile << rows << " " << cols << " " << min << " " << max;
	runLength *code = new runLength(stoi(rows), stoi(cols), stoi(min), stoi(max), stoi(method), encode, nameEncodeFile);

	switch (stoi(method))
	{
	case 1:
		cout << "method1" << endl;
		code->deCodeMethod1(encodeFile, decodeFile);
		break;

	case 4:
		cout << "method4" << endl;
		code->deCodeMethod4(encodeFile, decodeFile);
		break;

	default:
		cout << "invalid input";
	}

	encodeFile.close();
	decodeFile.close();

	return 0;
}

void runLength::deCodeMethod1(ifstream &encode, ofstream &decode)
{ // encode zero no wrap around
	string startRow, startCol, color, length;
	int rowCnt = 0;
	int colCnt = 0;
	decode << endl;
	while (rowCnt < this->numRows && !encode.eof())
	{

		encode >> startRow;
		encode >> startCol;
		encode >> color;
		encode >> length;
		for (int i = 0; i < stoi(length); i++)
		{
			decode << color << " ";
			colCnt++;
		}

		if (colCnt >= this->numCols)
		{
			decode << endl;
			colCnt = 0;
			rowCnt++;
		}
	}
}

void runLength::deCodeMethod4(ifstream &encode, ofstream &decode)
{ //dont encode zero with wrap around

	string startRow, startCol, color, length;
	int rowCnt = 0;
	int colCnt = 0;
	decode << endl;
	int preVal, curVal;
	int preCol, curCol;
	int preRow, curRow;
	int preLen, curLen;
	bool first = true;
	int preEnd;
	int preColEnd, preRowEnd;
	if (!encode.eof())
	{

		encode >> startRow;
		encode >> startCol;
		encode >> color;
		encode >> length;
		preRow = curRow = stoi(startRow);
		preCol = curCol = stoi(startCol);
		preVal = curVal = stoi(color);
		preLen = curLen = stoi(length);
		cout << startRow << "-" << startCol << "-" << color << "-" << length << endl;
		cout.flush();

		while (rowCnt < this->numRows /*&& !encode.eof()*/)
		{

			if (stoi(startRow) == 0 && curCol != 0 && first == true /*curCol != 0 && ((preRow != curRow) || first) */)
			{
				for (int i = 0; i < curCol; i++)
				{
					decode << 0 << " ";
					colCnt++;
					if (colCnt >= this->numCols)
					{ // if first non zero value does not begin at zero
						decode << endl;
						colCnt = 0;
						rowCnt++;
					}
				}
				first = false;
			}

			for (int i = 0; i < curLen; i++)
			{
				decode << curVal << " ";
				colCnt++;
				if (colCnt >= this->numCols)
				{
					decode << endl;
					colCnt = 0;
					rowCnt++;
				}
			}

			if (encode.eof())
				break;

			preVal = curVal;
			preCol = curCol;
			preLen = curLen;
			preRow = curRow;
			encode >> startRow;
			encode >> startCol;
			encode >> color;
			encode >> length;
			curVal = stoi(color);
			curCol = stoi(startCol);
			curLen = stoi(length);
			curRow = stoi(startRow);

			cout << startRow << "-" << startCol << "-" << color << "-" << length << endl;
			cout.flush();

			preEnd = preCol;
			preRowEnd = preRow;
			preColEnd = preCol;
			for (int i = 0; i < preLen - 1; i++)
			{

				preEnd++;
				preColEnd++;
				if (preEnd >= this->numCols)
				{
					preEnd = 0;
					preColEnd = 0;
					preRowEnd++;
				}
			}
			cout << preVal << "_" << preRowEnd << "_" << preColEnd << endl;
			cout.flush();

			int rowsApart = abs(curRow - preRowEnd);
			int colsApart;
			if (rowsApart == 0)
			{
				cout << "sameRow" << endl;
				colsApart = abs(preEnd - curCol);
				for (int i = 0; i < colsApart - 1; i++)
				{
					decode << 0 << " ";
					colCnt++;
					if (colCnt >= this->numCols)
					{
						decode << endl;
						colCnt = 0;
						rowCnt++;
					}
				}
			}
			else
			{ // A row or more between two runs

				for (int i = preColEnd; i < this->numCols - 1; i++)
				{ // handles ending zeros after current row
					decode << 0 << " ";
					colCnt++;
					if (colCnt >= this->numCols)
					{
						decode << endl;
						colCnt = 0;
						rowCnt++;
					}
				}

				while (rowCnt != curRow)
				{
					for (int i = 0; i < this->numCols; i++)
					{ // handles zeros between rows
						decode << 0 << " ";
						colCnt++;
						if (colCnt >= this->numCols)
						{
							decode << endl;
							colCnt = 0;
							rowCnt++;
						}
					}
				}

				if (rowCnt == curRow)
				{
					for (int i = 0; i < curCol; i++)
					{ // handles leading zeros before start of a new run
						decode << 0 << " ";
						colCnt++;
						if (colCnt >= this->numCols)
						{
							decode << endl;
							colCnt = 0;
							rowCnt++;
						}
					}
				}
			}

		} // end of while loop
		if (encode.eof())
		{
			for (int i = rowCnt; i < this->numRows; i++)
			{ // handles zeros at end of file after last non zero run
				for (int j = colCnt; j < this->numCols; j++)
				{
					decode << 0 << " ";
					colCnt++;
					if (colCnt >= this->numCols)
					{
						decode << endl;
						colCnt = 0;
						rowCnt++;
					}
				}
			}
		}
	}
}

int runLength::leftEndPrint()
{
	return 0;
}
int runLength::rightEndPrint()
{
	return 0;
}

runLength::runLength(int rows, int cols, int min, int max, int method, string encodeFile, string decodeFile)
{
	numRows = rows;
	numCols = cols;
	minVal = min;
	maxVal = max;
	whichMethod = method;
	nameEncodeFile = encodeFile;
	nameDecodeFile = decodeFile;

	startRow = 0;
	startCol = 0;
	color = 0;
	length = 0;
}

runLength::runLength()
{
	numRows = 0;
	numCols = 0;
	minVal = 0;
	maxVal = 0;

	whichMethod = 0;
	nameEncodeFile = "";
	nameDecodeFile = "";

	startRow = 0;
	startCol = 0;
	color = 0;
	length = 0;
}
