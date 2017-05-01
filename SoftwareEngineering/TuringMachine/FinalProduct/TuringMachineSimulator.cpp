/*
Title:		TuringMachineSimulator
University:	Washington State University Tri-Cities
Class:		Computer Science 322
Instructor:	Niel Corrigan
Notes:

	I have elected to write the entirety of this program in a single .cpp file
	while I understand that this is bad practice, it has been useful due to the
	design evolution that has occurred throughout the implementation process.
*/

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<ctype.h>

#define DBG cout << "Here" << BUGERATOR << endl; BUGERATOR++;

#define STATENUM 8
#define STATES_ID "STATES:"
#define INPUT_ALPHABET_ID "INPUT_ALPHABET:"
#define TAPE_ALPHABET_ID "TAPE_ALPHABET:"
#define TRANSITION_FUNCTION_ID "TRANSITION_FUNCTION:"
#define INITIAL_STATE_ID "INITIAL_STATE:"
#define BLANK_CHARACTER_ID "BLANK_CHARACTER:"
#define FINAL_STATES_ID "FINAL_STATES:"

#define TM_EXIT_STR "Q"
#define TM_FAIL_STR "F"; cout << "Error! invalid command" << endl;
#define COMMAND_NUM 11

using namespace std;

enum DIRECTION {R, L};

enum STATE {	START,
		Expect_States,
		Expect_Input_Alphabet,
		Expect_Tape_Alphabet,
		Expect_Transition_Function,
		Expect_Initial_State,
		Expect_Blank_Character,
		Expect_Final_States};

string COMMAND_LIST[11] = {"D", "X", "H", "I", "L", "Q", "R", "E", "W", "T", "V"};

ostream &operator <<(ostream &os, DIRECTION &dir)
{
	if(dir == R)
	{
		cout << "R";
	}
	if(dir == L)
	{
		cout << "L";
	}

	return os;
}

class TM_STATE
{
public:
	string name;
	//vector<TM_STATE*> edges;
	//vector<Instr> edge_instr;

	TM_STATE()
	{

	}
};
class TM_TransitionFunction
{
public:
	vector<TM_STATE> ObjectStates;
	vector<TM_STATE> TransTo;
	vector<char> TransOn;
	vector<char> WriteToTape;
	vector<DIRECTION> MoveTape;

	TM_TransitionFunction()
	{
		
	}

	int GetObjectID(TM_STATE* alpha)
	{
		int i;
		bool complete = false;

		for(i = 0; (i < ObjectStates.size()) && (complete == false); i++)
		{
			if(ObjectStates[i].name == alpha->name)
			{
				complete = true;
				i--;
			}
		}

		if(complete == false)
		{
			i = -1;
		}

		return i;
	}

	TM_STATE* GetTransTo(int id)
	{
		return &TransTo[id];
	}
	char GetTransOn(int id)
	{
		return TransOn[id];
	}
	char GetWriteToTape(int id)
	{
		return WriteToTape[id];
	}
	DIRECTION GetDirection(int id)
	{
		return MoveTape[id];
	}
};

class TM_Tape
{
public:
	vector<char> cells;
	vector<char>* tapeAlphabet;
	int tapeHead;
	char* blankCharacter;

	TM_Tape(vector<char>* inTapeAlphabet, char* inBlankCharacter)
	{
		tapeHead = 1;
		tapeAlphabet = inTapeAlphabet;
		blankCharacter = inBlankCharacter;
	}

	bool loadString(vector<char> inputString);
	bool isInTapeAlphabet(char input);
	bool moveRight();
	bool moveLeft();
	bool end()
	{
		bool bVal;

		if(curChar() == *blankCharacter)
		{
			bVal = true;
		}
		else
		{
			bVal = false;
		}
		
		return bVal;
	}
	char curChar();
	void writeHere(char chr);
	void print()
	{
		//cout << "Printing Tape: " << endl;
		for(int i = 0; i < cells.size(); i++)
		{
			cout << cells[i] << " ";
		}
		cout << endl;
	}
};

class InputMenu
/*This function should be extended to accomodate command + modifier(s) in one line*/
{
public:
	string com;
	string tmp;

	InputMenu()
	{

	}
	void prompt()
	{
		com = "";
		cout << "Enter Command: ";
		cin >> tmp;
		for(int i = 0; i < tmp.length(); i++)
		{
			com += toupper(tmp.c_str()[i]);
		}
		/*
		if(com.length() != 1) // Finish this later!
		{
			com = "Invalid";
		}
		*/
	}
};

class TuringMachine
{
public:

	string EXIT_STR;

	// Static information from parser (part of machine definition)
	string name;
	string notes;	// Loaded by think()
	vector<TM_STATE> allStates;	// Loaded by think()
	vector<TM_STATE> finalStates;	// Loaded by think()
	vector<char> inputAlphabet;	// Loaded by think()
	vector<char> tapeAlphabet;	// Loaded by think()
	vector<string> inputStrings;	// Loaded by loadIS();
	TM_STATE initialState;	// Loaded by think()
	char blankCharacter;	// Loaded by think()
	TM_TransitionFunction tFunction; // Loaded by think()

	// Attributes of TM (composition I think?)
	TM_Tape* tape;
	InputMenu inputMenu;

	// Dynamic state info
	TM_STATE* currentState;
	int maxTrans;
	int tAmt;
	bool running;
	vector<string> errors;

	TuringMachine() // Populate this, maybe leave TM_Trans as it is...
	{
		tAmt = 20;
		maxTrans = 100;
		running = false;
	}

	bool validateTF();
	TM_STATE* findStateByValue(TM_STATE alpha);
	void start()
	{
		/*
		EXIT_STR = TM_EXIT_STR;
		notes = "";
		*/

		tape = new TM_Tape(&tapeAlphabet, &blankCharacter);

		currentState = findStateByValue(initialState);
		do
		{
			inputMenu.prompt();
			comJunc(inputMenu.com);
		}
		while(inputMenu.com != "X");

		return;
	}
	void print();
	void loadIS(string is);

	void comJunc(string com);
	void remove(); // delete
	void exit();
	void help();
	void insert();
	void list();
	void quit();
	int run();
	void set();
	void show();
	void truncate();
	void view();
	void printID();
};
class Parser
{
public:
	enum SUB_STATE {Expect_Object_State,
			Expect_Trans_On,
			Expect_Trans_To,
			Expect_Write_To_Tape,
			Expect_Move_Tape};

	SUB_STATE subState;
	ifstream defFile;
	fstream strFile;
	string tmpFileName;
	STATE state;
	char ch;
	bool doneReading;
	bool complete;
	string word;
	string tempWord;
	string stuff;
	vector<string> errors;
	vector<TM_STATE> allStates;
	TuringMachine* tmPtr;
	TM_TransitionFunction tFunction;

	Parser(string fileName, TuringMachine* inputMachine)
	{
		complete = false;
		tmPtr = inputMachine;

		tmPtr->name = fileName;
		subState = Expect_Object_State;
		tmpFileName = fileName + ".def";
		defFile.open(tmpFileName.c_str());
		tmpFileName = fileName + ".str";
		strFile.open(tmpFileName.c_str());

		state = START;
		doneReading = false;
		word = "";
		tempWord = "";

		if(defFile.is_open() == true && strFile.is_open() == true)
		{
			think();
			loadInputStrings();
			
			//tmPtr->print();
			loadInputStrings();
			complete = true;
			cout << "Turing Machine \"" << tmPtr->name;
			cout << "\" was loaded successfully" << endl;
		}
		else
		{
			if(defFile.is_open() == false)
			{
				errors.push_back("Error! No .def file could be found");
			}
			if(strFile.is_open() == false)
			{
				errors.push_back("Error! No .str file could be found");
			}
		}
	}

	void loadInputStrings();
	void inputWordFromFile();
	void think();
	void commitWord(bool stateChange);
	void loadTM();
	void spillErrors()
	{
		for(int i = 0; i < errors.size(); i++)
		{
			cout << errors[i] << endl;
		}
		errors.clear();
	}
	bool isCommand();
	string details();
};

STATE STATELIST[STATENUM] = {START, Expect_States};
int BUGERATOR = 1;

int main(int argc, char* argv[])
{
	Parser* pPtr;
	TuringMachine tm;

	int rVal = 0;

	if(argc == 2)
	{
		pPtr = new Parser(argv[1], &tm);

		if(pPtr->complete == true)
		{
			//tm.print();
			tm.start();
		}
		else
		{
			pPtr->spillErrors();
		}
	}
	else
	{
		cout << "Error! Invalid program arguments" << endl;
		rVal = -1;
	}

	return rVal;
}

void Parser::loadInputStrings()
{
	while(getline(strFile, word))
	{
		tmPtr->loadIS(word);
	}

	return;
}

bool Parser::isCommand()
{
	bool bVal = false;
	string str1 = "";
	string str2 = "";

	for(int i = 0; i < word.length(); i++)
	{
		if(bVal == false)
		{
			str1 += word.c_str()[i];
		}
		else
		{
			str2 += word.c_str()[i];
		}
		if(word.c_str()[i] == ':')
		{
			bVal = true;
		}
	}
	word = str1;
	tempWord = str2;

	return bVal;
}
string Parser::details()
{
	string dString;
	int lineNum = 0;
	int posInLine = 0;
	int posInFile = 0;
	char inChar;
	char buff[20];

	posInFile = defFile.tellg();
	defFile.seekg(0, defFile.beg);

	for(int i = 0; i < posInFile; i++)
	{
		defFile.get(inChar);
		if(inChar == '\n')
		{
			lineNum++;
			posInLine = 0;
		}
		else
		{
			posInLine++;
		}
	}

	dString = "\nError occurred at word: \"" + word + "\"\nline number: ";
	sprintf(buff, "%d\n", lineNum);
	dString += buff;
	dString += "Position in line: ";
	sprintf(buff, "%d\n", posInLine);
	dString += buff;

	return dString;
}
void Parser::inputWordFromFile()
{
	if(tempWord == "")
	{
		if(defFile.eof() == false)
		{
				defFile >> word;
		}
		else
		{
			doneReading = true;
		}
	}
	else
	{
		word = tempWord;
		tempWord = "";
	}
	return;
}
void Parser::think()
{
	while(doneReading == false)
	{
		inputWordFromFile();
		if(isCommand())
		{
			if(word == STATES_ID)
			{
				state = Expect_States;
			}
			else if(word == INPUT_ALPHABET_ID)
			{
				state = Expect_Input_Alphabet;
			}
			else if(word == TAPE_ALPHABET_ID)
			{
				state = Expect_Tape_Alphabet;
			}
			else if(word == TRANSITION_FUNCTION_ID)
			{
				state = Expect_Transition_Function;
			}
			else if(word == INITIAL_STATE_ID)
			{
				state = Expect_Initial_State;
			}
			else if(word == BLANK_CHARACTER_ID)
			{
				state = Expect_Blank_Character;
			}
			else if(word == FINAL_STATES_ID)
			{
				state = Expect_Final_States;
			}
			else
			{
				cout << "Error! Invalid turing machine argument (specified by \':\')" << details();
			}

			word = "";
		}
		else
		{
			if(word != "")
			{
				commitWord(false);
			}
		}
	}

	return;
}
void Parser::commitWord(bool stateChange)
{
	TM_STATE tmState;

		tmState.name = word;
		switch(state)
		{
		case START:
			tmPtr->notes += word;
			tmPtr->notes += " ";
			break;
		case Expect_States:
			tmPtr->allStates.push_back(tmState);
			break;
		case Expect_Input_Alphabet:
			tmPtr->inputAlphabet.push_back(word.c_str()[0]);
			break;
		case Expect_Tape_Alphabet:
			tmPtr->tapeAlphabet.push_back(word.c_str()[0]);
			break;
		case Expect_Transition_Function:

			switch(subState)
			{
			case Expect_Object_State:
				tFunction.ObjectStates.push_back(tmState);
				subState = Expect_Trans_On;
				break;
			case Expect_Trans_On:
				tFunction.TransOn.push_back(word.c_str()[0]);
				subState = Expect_Trans_To;
				break;
			case Expect_Trans_To:
				tFunction.TransTo.push_back(tmState);
				subState = Expect_Write_To_Tape;
				break;
			case Expect_Write_To_Tape:
				tFunction.WriteToTape.push_back(word.c_str()[0]);
				subState = Expect_Move_Tape;
				break;
			case Expect_Move_Tape:
				if(word == "R")
				{
					tFunction.MoveTape.push_back(R);
				}
				else if(word == "L")
				{
					tFunction.MoveTape.push_back(L);
				}
				else
				{
					cout << word << " is an invalid direction!" << endl;
				}
				subState = Expect_Object_State;
				break;
			}
			tmPtr->tFunction = tFunction;
			break;
		case Expect_Initial_State:
			tmPtr->initialState = tmState;
			break;
		case Expect_Blank_Character:
			tmPtr->blankCharacter = word.c_str()[0];
			break;
		case Expect_Final_States:
			tmPtr->finalStates.push_back(tmState);
			break;
		default:
			cout << "Error! Invalid state!" << endl;
		}

	word = "";

	return;
}

/*
void TuringMachine::validateTF()
{
	string erStr;
	int errorState = 0;
	int it;

	for(int i = 0; i < ObjectStates.size(); i++)
	{
		
	}

	return;
}
*/

void TuringMachine::printID()
{
	for(int i = 1; i < tape->cells.size() + 1; i++)
	{
		if(i == tape->tapeHead)
		{
			cout << "[" << currentState->name << "]";
			cout << " ";
		}
		else
		{
			//if(i > (tape->tapeHead - tAmt) && i < (tape->tapeHead + tAmt));
			if(i >= (tape->tapeHead - tAmt))
			{
				//cout << i << " > " << (tape->tapeHead - tAmt) << endl;
				if(i <= (tape->tapeHead + tAmt))
				{
				//cout << i << " < " << (tape->tapeHead + tAmt) << endl;
				//cout << endl << "i: " << i << "\tlow: " << (tape->tapeHead - tAmt) << "high: " << (tape->tapeHead + tAmt) << endl;
				//cout << "tape->tapeHead: " << tape->tapeHead << endl;
					cout << tape->cells[i - 1];
					cout << " ";
				}
			}
		}
	}
	cout << endl;

	return;
}
TM_STATE* TuringMachine::findStateByValue(TM_STATE alpha)
{
	TM_STATE* rVal = NULL;

	for(int i = 0; i < allStates.size(); i++)
	{
		if(allStates[i].name == alpha.name)
		{
			rVal = &allStates[i];
		}
	}
	
	return rVal;
}
void TuringMachine::comJunc(string com)
{
	int a;
	fstream file;
	string str;

	if(com == "D")
	{
		cout << "Input string number: ";
		cin >> a;

		if(a > 0 && a <= inputStrings.size())
		{
			inputStrings.erase(inputStrings.begin() + (a - 1));
			str = name + ".str";
			file.open(str.c_str(), ofstream::out | ofstream::trunc);
			for(int i = 0; i < inputStrings.size(); i++)
			{
				file << inputStrings[i] << endl;
			}
			cout << "Input string was deleted successfully" << endl;
		}
		else
		{
			// Error message
			cout << "Error! Invalid input. No strings were deleted" << endl;
		}
	}
	else if(com == "X")
	{
		/*
		cout << "Exit" << endl;
		notes.push_back("Input string file was written");
		notes.push_back("Program will now exit");
		*/
		cout << "Program will now exit" << endl;
	}
	else if(com == "H")
	{
		help();
	}
	else if(com == "I")
	{
		/*
		cout << "Insert" << endl;
		cout << "Input string: aabba" << endl;
		notes.push_back("Input string \"aabba\" added to list");
		*/

		cout << "Input a string: ";
		cin >> str;

		// Search for duplicates
		inputStrings.push_back(str);
		str = name + ".str";
		file.open(str.c_str());
		for(int i = 0; i < inputStrings.size(); i++)
		{
			file << inputStrings[i] << endl;
		}
		cout << "String was loaded successfully" << endl;
	}
	else if(com == "L")
	{
		/*
		cout << "List" << endl;
		cout << "1: \\" << endl << "2: abba" << endl << "3: aaabbb" << endl;
		*/
		list();
	}
	else if(com == "Q")
	{
		/*
		cout << "Quit" << endl;
		cout << "Turing machine has halted and rejected string \"abbab\"" << endl;
		cout << "13:\tab[s2]ab" << endl;
		*/
		if(running == true)
		{
			cout << "Turing machine has halted operation on string" << endl;
			printID();
			quit();
		}
		else
		{
			cout << "Error! Machine is not currently running" << endl;
		}
	}
	else if(com == "R") // comRun
	{
		vector<char> cv;
		bool goodRun = true;

		if(running == false)
		{
			cout << "Input string number: ";
			cin >> a;
		}

		
		if(a >= 0 && a <= inputStrings.size() || running == true)
		{
			if(running == false)
			{
				
				for(int i = 0; i < inputStrings[a - 1].length(); i++)
				{
					cv.push_back(inputStrings[a - 1].c_str()[i]);
				}
	
				//cin.get();
				goodRun = tape->loadString(cv);
			}
			if(goodRun == true)
			{
				run();
			}
		}
		if(a < 0 || a > inputStrings.size())
		{
			cout << "Error! Invalid string selection" << endl;
		}

		/*
		cout << "Run" << endl;
		cout << "Input string number: 2" << endl;
		cout << "0:\t[s0]abba" << endl;
		cout << "10:\ta[s1]bx" << endl;
		*/
	}
	else if(com == "E")
	{
		/*
		cout << "Set" << endl;
		cout << "Input maximum number of transitions: 100" << endl;
		notes.push_back("Maximum number of transitions changed to '100'");
		*/
		cout << "Input maximum number of transitions: ";
		cin >> a;
		if(a > 0)
		{
			maxTrans = a;
			cout << "The maximum number of transitions has been updated" << endl;
		}
		else
		{
			cout << "Error! Invalid input. Maximum transitions not changed" << endl;
		}
	}
	else if(com == "W")
	{
		cout << "Program: Turing Machine Simulator" << endl;
		cout << "Version: 0.0.1" << endl;
		cout << "Instructor: Neil Corrigan" << endl;
		cout << "Author: Matthew Harris" << endl;
		cout << "Maximum number of transitions: " << maxTrans << endl;
		cout << "Instantaneous description truncation amount: " << tAmt << endl;
		cout << "Turing Machine Name: " << name << endl;
		if(running == true)
		{
			cout << "Machine is currently running" << endl;
		}
		else
		{
			cout << "Machine is not running" << endl;
		}

		//notes.push_back("Not done with this command");
		// Don't forget this

	}
	else if(com == "T")
	{
		/*
		cout << "Truncate" << endl;
		cout << "Input truncation ammount: 3" << endl;
		cout << "The machine will now truncate after 3 spaces" << endl;
		*/
		cout << "Input truncation amount: ";
		cin >> a;

		if(a > 0)
		{
			tAmt = a;
			cout << "The truncation amount has been updated" << endl;
		}
		else
		{
			cout << "Error! Invalid input. Truncation amount not changed" << endl;
		}
	}
	else if(com == "V")
	{
		/*
		cout << "View" << endl;
		notes.push_back("TM = {}");
		notes.push_back("States = {s1, s2, s3}");
		*/
		print();
	}
	else
	{
		//errors.push_back("Error! Invalid command passed to comJunc. How did you do that?");
		cout << "Error! Unknown command \"" << com << "\"" << endl;
	}

	
/*
Delete:	Delete input string from list
Exit:		Exit application
Help:		Help user
Insert:	Insert input string into list
List:		List input strings
Quit:		Quit application of TM on input string
Run:		Run TM on input string
Set:		Set maximum number of transitions to perform
Show:		Show status of application
Truncate:	Truncate instantaneous descriptions
View:		View TM
*/

	return;
}
void TuringMachine::quit()
{
	tape->cells.clear();
	tape->tapeHead = 1;
	running = false;
	currentState = NULL;

	return;
}
int TuringMachine::run()
{
	int curID;
	bool detectedErrors = false;
	bool complete = false;
	bool accept;

	if(running == false)
	{
		currentState = findStateByValue(initialState);
		running = true;
		tape->tapeHead = 1; // This should be set when the machine stops as well, but just to be safe I've set it here as well
		accept = false;
	}

	for(int i = 0; (i < maxTrans) && (complete == false); i++)
	{
		//cout << "i: " << i << endl;
		curID = tFunction.GetObjectID(currentState);
		//cout << "Starting here: " << currentState->name << endl;
		//cout << "This state has ID: " << curID << " In this transition function index" << endl;
		//cout << "The current char in the tape is: " << tape->curChar() << endl;
		//tape->print();
		//printID();
		// Use curID with other tFunction methods to get info for transitions
		while(tape->curChar() != tFunction.GetTransOn(curID) && detectedErrors == false)
		{
			//cout << "Looking for matching TransOn character from tFunction. Current ID: " << curID << endl;
			//cout << "We got: " << tFunction.GetTransOn(curID) << endl;
			if(tFunction.ObjectStates[curID].name != tFunction.ObjectStates[curID + 1].name)
			{
				//cout << "And now we've detected an error" << endl;
				//cout << "Apparently: " << endl;
				//cout << tFunction.ObjectStates[curID].name << " is not the same as ";
				//cout << tFunction.ObjectStates[curID + 1].name << endl; 

				detectedErrors = true;
			}
			else
			{
				curID++;
			}
		}
		if(detectedErrors == true)
		{
			// Put a real error message here later
			//cout << "Something bad happened" << endl;
		}
		else
		{
			tape->writeHere(tFunction.GetWriteToTape(curID));
			//cout << "Writing to tape: " << tFunction.GetWriteToTape(curID) << endl;
			if(tFunction.GetDirection(curID) == L)
			{
				tape->moveLeft();
				//cout << "Tape has moved left" << endl;
			}
			else
			{
				tape->moveRight();
				//cout << "Tape has moved right" << endl;
			}
			currentState = tFunction.GetTransTo(curID);
			//cout << "Ending in state: " << currentState->name << endl;
			//tape->print();
		}


		if(tape->end() == true || detectedErrors == true)
		{
			//cout << "All done!" << endl;
			complete = true;

			if(detectedErrors == false)
			{
				for(int j = 0; j < finalStates.size(); j++)
				{
					if(currentState->name == finalStates[j].name)
					{
						accept = true;
					}
				}
			}

			if(accept == true)
			{
				cout << "The string was accepted!" << endl;
			}
			else
			{
				cout << "The string was rejected!" << endl;
			}

			/*
			tape->cells.clear();
			tape->tapeHead = 1;
			running = false;
			*/
			
			printID();
			quit();
		}
		else
		{
			//cout << "tapeHead: " << tape->tapeHead << endl;
			//cout << "cells->size: " << tape->cells.size() << endl;
		}

		if(i == maxTrans - 1)
		{
			printID();
		}


		//cout << "loop: " << i << endl;
		//cin.get();
	}

	return 0;
}
void TuringMachine::help()
{
	string inString;
	ifstream inFile;
	inFile.open("./commands.txt");

	while(getline(inFile, inString))
	{
		cout << inString << endl;
	}

	inFile.close();

	return;
}
void TuringMachine::list()
{
	for(int i = 0; i < inputStrings.size(); i++)
	{
		cout << i + 1 << ": " << inputStrings[i] << endl;
	}

	return;
}
void TuringMachine::loadIS(string is)
{
	//if(validInputString(is) == true)
	{
		inputStrings.push_back(is);
	}

	return;
}
void TuringMachine::print()
{
	cout << "Notes:" << endl << notes << endl;

	cout << "All States:" << endl;
	for(int i = 0; i < allStates.size(); i++)
	{
		cout << allStates[i].name << endl;
	}
	cout << "Input Alphabet:" << endl;
	for(int i = 0; i < inputAlphabet.size(); i++)
	{
		cout << inputAlphabet[i] << endl;;
	}
	cout << "Tape Alphabet:" << endl;
	for(int i = 0; i < tapeAlphabet.size(); i++)
	{
		cout << tapeAlphabet[i] << endl;
	}
	cout << "Transition Function:" << endl;
	for(int i = 0; i < tFunction.ObjectStates.size(); i++)
	{
		cout << tFunction.ObjectStates[i].name << " ";
		cout << tFunction.TransOn[i] << "\t";
		cout << tFunction.TransTo[i].name << " ";
		cout << tFunction.WriteToTape[i] << " ";
		cout << tFunction.MoveTape[i] << endl;
	}
	cout << "Initial State:" << endl;
	cout << initialState.name << endl;

	cout << "Blank Character:" << endl;
	cout << blankCharacter << endl;

	cout << "Final States: " << endl;
	for(int i = 0; i < finalStates.size(); i++)
	{
		cout << finalStates[i].name << endl;
	}

	return;
}
bool TM_Tape::moveRight() // This should be modified to handle infinite blank characters
{
	bool bVal;

	if(tapeHead < cells.size())
	{
		tapeHead++;
		bVal = true;
	}
	else
	{
		bVal = false;
	}

	return bVal;
}
bool TM_Tape::moveLeft()
{
	bool bVal;

	if(tapeHead > 1)
	{
		tapeHead--;
		bVal = true;
	}
	else
	{
		bVal = false;
	}

	return bVal;
}
char TM_Tape::curChar()
{
	char chr;
	chr = cells[tapeHead - 1];

	return chr;
}
void TM_Tape::writeHere(char chr)
{
	cells[tapeHead - 1] = chr;

	return;
}

bool TM_Tape::loadString(vector<char> inputString)
{
	bool bFlag = true;

	for(int i = 0; i < inputString.size(); i++)
	{
		if(isInTapeAlphabet(inputString[i]) == false)
		{
			cout << "Error! Tried to run an erroneous string!" << endl;
			bFlag = false;
			i = inputString.size() + 1;
		}
		else
		{
			cells.push_back(inputString[i]);
		}
	}
	cells.push_back(*blankCharacter);

	return bFlag;
}

bool TM_Tape::isInTapeAlphabet(char input)
{
	// Turns out I never initialize tape with new TM_Tape();
	bool bVal = false;

	for(int i = 0; i < tapeAlphabet->size(); i++)
	{
		if(input == (*tapeAlphabet)[i])
		{
			bVal = true;
		}
	}

	return bVal;
}








