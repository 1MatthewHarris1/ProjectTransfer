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

class Instr
{
public:
	string transChar;
	string transTo;
	string tapeWriteChar;
	DIRECTION tapeDir;

	Instr()
	{

	}
};

class TM_STATE
{
public:
	string name;
	vector<TM_STATE*> edges;
	vector<Instr> edge_instr;

	TM_STATE()
	{

	}
};
class TM_TransitionFunction
{
public:
	vector<TM_STATE> ObjectStates;
	vector<string> TransOn;
	vector<TM_STATE> TransTo;
	vector<string> WriteToTape;
	vector<DIRECTION> MoveTape;

	TM_TransitionFunction()
	{
		
	}
	int assign();
};
class TuringMachine
{
public:
	
	/*
	class Command
	{
	public:
		string name;
	
		Command()
		{
			
		}
	
		string input();
		//bool checkValidity(string command);
		bool isCommand(string str);
		void comJunc();
	};
	*/

	string EXIT_STR;
	string notes;
	//Command com;
	vector<TM_STATE> allStates;
	vector<string> inputAlphabet;
	vector<string> tapeAlphabet;
	vector<TM_STATE> finalStates;
	TM_STATE initialState;
	char blankCharacter;

	TM_TransitionFunction tFunction; // Delete this!!!

	TuringMachine() // Populate this, maybe leave TM_Trans as it is...
	{
	}

	void start()
	{
		EXIT_STR = TM_EXIT_STR;
		notes = "";
		/*
		do
		{
			com.input();
		}
		while(com.name != EXIT_STR);
		*/
		return;
	}
	void print();
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
	ifstream file;
	STATE state;
	char ch;
	bool doneReading;
	string word;
	string tempWord;
	//vector<string> stuff;
	string stuff;
	vector<TM_STATE> allStates;
	TuringMachine* tmPtr;
	TM_TransitionFunction tFunction;

	Parser(string fileName, TuringMachine* inputMachine)
	{
		subState = Expect_Object_State;
		tmPtr = inputMachine;
		file.open(fileName.c_str());
		state = START;
		doneReading = false;
		word = "";
		tempWord = "";

		think();
		tmPtr->print();
		tmPtr->start();
	}
	//char inputCharFromFile();
	void inputWordFromFile();
	void think();
	void commitWord(bool stateChange);
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
	}
	else
	{
		cout << "Error! Invalid program arguments" << endl;
		rVal = -1;
	}

	return rVal;
}
/*
string Command::input()
{
	string str;

	cout << "command: ";
	cin >> str;

	if(isCommand(str))
	{
		name = str;
	}
	else
	{
		name = TM_FAIL_STR;
	}

	str;
}
bool Command::isCommand(string input)
{
	bool bFlag = false;

	if(input.length() == 1)
	{
		for(int i = 0; i < COMMAND_NUM; i++)
		{
			if(input == COMMAND_LIST[i])
			{
				bFlag = true;
			}
		}
	}

	return bFlag;
}
void Command::comJunc()
{
	if(name == "D")
	{
		
	}
	else if(name == "X")
	{
		
	}
	else if(name == "H")
	{
		
	}
	else if(name == "I")
	{
		
	}
	else if(name == "L")
	{
		
	}
	else if(name == "Q")
	{
		
	}
	else if(name == "R")
	{
		
	}
	else if(name == "E")
	{
		
	}
	else if(name == "W")
	{
		
	}
	else if(name == "T")
	{
		
	}
	else if(name == "V")
	{
		
	}

	return;
}
*/
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

	posInFile = file.tellg();
	file.seekg(0, file.beg);

	for(int i = 0; i < posInFile; i++)
	{
		file.get(inChar);
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
		if(file.eof() == false)
		{
				file >> word;
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
/*
char Parser::inputCharFromFile()
{
	if(file.eof() == false)
	{
		file >> ch;
		if(ch == '\n')
		{
			curLine++;
			posInLine = 0;
		}
		else
		{
			posInLine++;
		}
	}
	else
	{
		doneReading = true;
	}

	return ch;
}
*/
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
			tmPtr->inputAlphabet.push_back(word);
			break;
		case Expect_Tape_Alphabet:
			tmPtr->tapeAlphabet.push_back(word);
			break;
		case Expect_Transition_Function:

			switch(subState)
			{
			case Expect_Object_State:
				tFunction.ObjectStates.push_back(tmState);
				subState = Expect_Trans_On;
				break;
			case Expect_Trans_On:
				tFunction.TransOn.push_back(word);
				subState = Expect_Trans_To;
				break;
			case Expect_Trans_To:
				tFunction.TransTo.push_back(tmState);
				subState = Expect_Write_To_Tape;
				break;
			case Expect_Write_To_Tape:
				tFunction.WriteToTape.push_back(word);
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
int TM_TransitionFunction::assign()
{

	return 0;
}








