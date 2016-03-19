#include<iostream>
#include<fstream>
#include<string>

using namespace std;

class Entry
{
private:

	Entry* next;
	Entry* prev;
	string email;
	int num;

	Entry()
	{
		next = NULL;
		prev = NULL;
		email = "";
		num = 0;
	}
	
};
class Database
{
private:

	Entry* head;
	Entry* tail;
	int size;
	
	Database(string fileName)
	{
		head = NULL;
		tail = NULL;
		size = 0;
		buildBase(fileName);
	}

	void insert(string line);
	void remove(string line);
	void buildBase(string fileName);
};

int main(int argc, char* argv[])
{
	
	return 0;
}
void Database::insert(string line)
{
	
	return;
}
void Database::remove(string line)
{

	return;
}
void Database::buildBase(string fileName)
{

	return;
}












