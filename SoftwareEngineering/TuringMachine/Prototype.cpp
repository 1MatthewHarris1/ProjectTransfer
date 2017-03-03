#include<iostream>
#include<string>
#include<vector>
#include<stdlib.h>

#define VERBOSE false
#define COMNUM 11

using namespace std;

class Command
{
private:
	const string* VALID_COMMANDS;
	bool goodCommand;
	string exitStr;
	string com;
	vector<string> errors;
	vector<string> notes;

	void inCom();
	void comJunc();
	void isCom();
	void spillErrors();
	void spillNotes();
	void outputBlock();

public:
	Command()
	{
		string V_C[COMNUM] = {"D", "X", "H", "I", "L", "Q", "R", "E", "W", "T", "V"};
		VALID_COMMANDS = V_C;
		exitStr = "X";
		com = "";
		goodCommand = false;

		while(com != exitStr)
		{
			inCom();
			isCom();
			if(goodCommand == true)
			{
				comJunc();	
			}
			if(VERBOSE == true)
			{
				outputBlock();
			}
			cout << endl;
		}
	}
};

int main(int argc, char* argv[])
{
	Command command;

	return 0;
}
void Command::inCom()
{
	string temp;
	cout << "Command: ";
	cin >> temp;
	com = "";

	for(int i = 0; i < temp.length(); i++)
	{
		com += toupper(temp.c_str()[i]);
	}

	return;
}
void Command::isCom()
{
	goodCommand = false;
	for(int i = 0; i < COMNUM; i++)
	{
		if(com == VALID_COMMANDS[i])
		{
			goodCommand = true;
		}
	}
	if(com == "CLEAR")
	{
		goodCommand = true;
	}

	if(goodCommand == false)
	{
		errors.push_back("Invalid command \"" + com  + "\"");
	}

	return;
}
void Command::comJunc()
{
	cout << endl;
	if(com == "D")
	{
		cout << "Delete" << endl;
		cout << "Input string number: 2" << endl;
		notes.push_back("Input string number 2 deleted");
	}
	else if(com == "X")
	{
		cout << "Exit" << endl;
		notes.push_back("Input string file was written");
		notes.push_back("Program will now exit");
	}
	else if(com == "H")
	{
		cout << "Help" << endl;
		cout << "Command\t\tName\t\tDescription" << endl;
		cout << "-------------------------------------------" << endl;
		cout << "D\t\tDelete\t\tDelete input string from list" << endl;
		cout << "E\t\tExit\t\tExit Application" << endl;
		cout << "H\t\tHelp\t\tHelp user (displays this list)" << endl;
		cout << "I\t\tInsert\t\tInsert input string into list" << endl;
		cout << "L\t\tList\t\tList input strings" << endl;
		cout << "Q\t\tQuit\t\tQuit application of Turing machine on input string" << endl;
		cout << "R\t\tRun\t\tRun Turing machine on input string" << endl;
		cout << "E\t\tSet\t\tSet maximum number of transitions to perform" << endl;
		cout << "W\t\tShow\t\tShow status of application" << endl;
		cout << "T\t\tTruncate\tTruncate instantaneous descriptions" << endl;
		cout << "V\t\tView\t\tView the Turing machine" << endl;
	}
	else if(com == "I")
	{
		cout << "Insert" << endl;
		cout << "Input string: aabba" << endl;
		notes.push_back("Input string \"aabba\" added to list");
	}
	else if(com == "L")
	{
		cout << "List" << endl;
		cout << "1: \\" << endl << "2: abba" << endl << "3: aaabbb" << endl;
	}
	else if(com == "Q")
	{
		cout << "Quit" << endl;
		cout << "Turing machine has halted and rejected string \"abbab\"" << endl;
		cout << "13:\tab[s2]ab" << endl;
	}
	else if(com == "R")
	{
		cout << "Run" << endl;
		cout << "Input string number: 2" << endl;
		cout << "0:\t[s0]abba" << endl;
		cout << "10:\ta[s1]bx" << endl;
	}
	else if(com == "E")
	{
		cout << "Set" << endl;
		cout << "Input maximum number of transitions: 100" << endl;
		notes.push_back("Maximum number of transitions changed to '100'");
	}
	else if(com == "W")
	{
		cout << "Show" << endl;
		cout << "Program: Turing Machine Simulator" << endl;
		cout << "Version: 0.0.1" << endl;
		cout << "Author: Matthew Harris" << endl;
		//notes.push_back("Not done with this command");
		// Don't forget this
	}
	else if(com == "T")
	{
		cout << "Truncate" << endl;
		cout << "Input truncation ammount: 3" << endl;
		cout << "The machine will now truncate after 3 spaces" << endl;
	}
	else if(com == "V")
	{
		cout << "View" << endl;
		notes.push_back("TM = {}");
		notes.push_back("States = {s1, s2, s3}");
	}/*
	else if(com == "CLEAR")
	{
		system("clear");
		notes.push_back("Screen cleared");
	}*/
	else
	{
		errors.push_back("Error! Invalid command passed to comJunc. How did you do that?");
	}

	if(VERBOSE == false)
	{
		for(int i = 0; i < notes.size(); i++) // Should only be size 1, but just in case
		{
			cout << notes[i] << endl;
		}
		notes.clear();
	}

	return;
}
void Command::spillErrors()
{
	if(errors.size() == 0)
	{
		errors.push_back("None");
	}
	cout << "Errors:" << endl;
	for(int i = 0; i < errors.size(); i++)
	{
		cout << "\t" << errors[i] << endl;
	}
	errors.clear();

	return;
}
void Command::spillNotes()
{
	if(notes.size() == 0)
	{
		notes.push_back("None");
	}
	cout << "Notes:" << endl;
	for(int i = 0; i < notes.size(); i++)
	{
		cout << "\t" << notes[i] << endl;
	}
	notes.clear();

	return;
}
void Command::outputBlock()
{
	int maxLen = 0;

	for(int i = 0; i < notes.size(); i++)
	{
		if(maxLen < notes[i].length())
		{
			maxLen = notes[i].length();
		}
	}
	for(int i = 0; i < errors.size(); i++)
	{
		if(maxLen < errors[i].length())
		{
			maxLen = errors[i].length();
		}
	}

	if(maxLen == 0)
	{
		maxLen = 4;
	}
	maxLen += 8;

	for(int i = 0; i < maxLen; i++)
	{
		cout << "=";
	}
	cout << endl;
	spillNotes();
	spillErrors();
	cout << endl;
	for(int i = 0; i < maxLen; i++)
	{
		cout << "=";
	}
	cout << endl;

	return;
}


















