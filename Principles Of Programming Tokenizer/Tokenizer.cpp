/*
Christopher Robertson
Principles of Programming Lanugages Lab 1
Tokenizer
02/20/2017
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

int procLine(ofstream &, string, int);
void symbol(ofstream &, char);
void op(ofstream &, char);
bool other(ofstream &, string);

int main()
{
	bool success = false;
	int status = 1;
	string inFileName,
		outFileName,
		lexime,
		str;
	ifstream infile;
	ofstream outfile;

	do
	{
		cout << "Enter the file name: ";
		cin >> inFileName;
		str = "";
		getline(cin, str);
		if (!cin)
		{
			cin.clear();
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please try again.\n\n";
			inFileName = "";
			success = false;
		}
		else if (str != "")
		{
			cout << "Invalid input. Please try again.\n\n";
			inFileName = "";
			success = false;
		}
		else
		{
			infile.open(inFileName);
			if (!infile)
			{
				cout << "Input file not found. Terminating.\n";
				system("pause");
				exit(EXIT_FAILURE);
			}
			int i = 0;
			while (inFileName[i] != '.')
			{
				outFileName += inFileName[i];
				i++;
			}
			outFileName += "T.xml";
			outfile.open(outFileName);
			success = true;
		}
		str = "";
	} while (!success);

	outfile << "<tokens>" << endl;

	while (!infile.eof())
	{
		str = "";
		getline(infile, str);
		status = procLine(outfile, str, status);
		if (status == 0)
		{
			cout << "Invalid lexime detected. Terminating now.\n";
			infile.close();
			outfile.close();
			system("pause");
			exit(EXIT_FAILURE);
		}
		str = "";
	}

	outfile << "</tokens>";

	infile.close();
	outfile.close();
	cout << "\nProcess complete.\n";
	system("pause");
	return 0;
}

int procLine(ofstream &outfile, string line, int status)
{
	string str = "";
	char current;

	if (status == 2)
	{
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] == '*' && line[i + 1] == '/')
			{
				return 1;
			}
		}
		return 2;
	}

	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] == '/')
		{
			if (line[i + 1] == '/')
			{
				return 1;
			}
			else if (line[i + 1] == '*')
			{
				for (; i < line.size(); i++)
				{
					if (line[i] == '*' && line[i + 1] == '/')
					{
						return 1;
					}
				}
				return 2;
			}
		}

		current = line[i];

		switch (current)
		{
			case '(':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				symbol(outfile, '(');
				break;
			}
			case ')':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				symbol(outfile, ')');
				break;
			}
			case '{':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				symbol(outfile, '{');
				break;
			}
			case '}':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				symbol(outfile, '}');
				break;
			}
			case '[':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				symbol(outfile, '[');
				break;
			}
			case ']':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				symbol(outfile, ']');
				break;
			}
			case ',':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				symbol(outfile, ',');
				break;
			}
			case ';':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				outfile << "<terminator> ; </terminator>" << endl;
				break;
			}
			case '.':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				op(outfile, '.');
				break;
			}
			case '+':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				op(outfile, '+');
				break;
			}
			case '-':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				op(outfile, '-');
				break;
			}
			case '&':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				op(outfile, '&');
				break;
			}
			case '|':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				op(outfile, '|');
				break;
			}
			case '~':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				op(outfile, '~');
				break;
			}
			case '<':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				op(outfile, '<');
				break;
			}
			case '>':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				op(outfile, '>');
				break;
			}
			case '=':
			{
				if (str != "")
				{
					i--;
					goto identSymbol;
				}
				if (line[i + 1] == '=')
				{
					outfile << "<comparisonOperator> == </comparisonOperator>" << endl;
					i++;
				}
				else
				{
					outfile << "<assignmentOperator> = </assignmentOperator>" << endl;
				}
				break;
			}
			case ' ':
			{
				identSymbol:
					if (str != "")
					{
						if (!other(outfile, str))
						{
							return 0;
						}
						str = "";
					}
					break;
			}
			case '\t':
			{
				if (str != "")
				{
					if (!other(outfile, str))
					{
						return 0;
					}
					str = "";
				}
				break;
			}
			case '"':
			{
				symbol(outfile, '"');
				if (str == "")
				{
					int j = i + 1;
					while (line[j] != '"')
					{
						if (line[j] == '\0')
						{
							return 0;
						}
						str += line[j];
						j++;
					}
					outfile << "<stringConstant> " << str << " </stringConstant>" << endl;
					str = "";
					symbol(outfile, '"');
					i = j;
				}
				break;
			}
			default:
			{
				str += line[i];
			}
		}
	}
	return 1;
}

void symbol(ofstream &outfile, char in)
{
	outfile << "<symbol> " << in << " </symbol>" << endl;
}

void op(ofstream &outfile, char in)
{
	outfile << "<operator> " << in << " </operator>" << endl;
}

bool other(ofstream &outfile, string str)
{
	if (str[0] > 47 && str[0] < 58)
	{
		outfile << "<integerConstant> ";
		for (int i = 0; i < str.size(); i++)
		{
			if (str[i] < 48 || str[i] > 57)
			{
				return 0;
			}
			outfile << str[i];
		}
		outfile << " </integerConstant>" << endl;
		return true;
	}
	if (str == "class" || str == "constructor" || str == "method" || str == "function" || str == "int" || str == "boolean" || str == "char" || str == "void" || str == "var" || str == "static" || str == "field" || str == "let" || str == "do" || str == "if" || str == "else" || str == "while" || str == "return" || str == "true" || str == "false" || str == "null" || str == "this")
	{
		outfile << "<keyword> " << str << " </keyword>" << endl;
		return true;
	}
	outfile << "<identifier> " << str << " </identifier>" << endl;
	return true;
}