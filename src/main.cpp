#include <iostream>
#include <sstream>

using namespace std;

// Environment variables
const string PATH = getenv("PATH");
int directoryCount = 1; 
char pathDelimiter = ':';

const string commands[3] = {
    "exit 0",
    "echo",
    "type"};

// Common string outputs
const string shellStart("$ ");
const string inputError(": \033[31mcommand\033[0m not found");
const string typeError(": not found");
const string typeSuccess(" is a shell \033[31mbuiltin\033[0m");

// Colors
const string RED = "\033[31m";
const string RESET = "\033[0m";

// functions
void echo(string input);
void type(string input);

int main()
{
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  // Makes a string array containing all the PATH directories
  // Traverse through the string and count the colins and save in directoryCount
  for (int i = 0; i < PATH.length(); i++) {
    if (PATH[i] == pathDelimiter) { directoryCount++; }
  }
  // Use COUNT to make array of directories 
  string directories[directoryCount];
  stringstream ss(PATH);
  string temp;
  int count = 0;
  while(getline(ss, temp, pathDelimiter)) {
    directories[count] = temp;
    count++;
  }

  // Read-Eval-Print Loop
  while (true)
  {

    std::cout << shellStart;

    string input;
    getline(cin, input);

    string command = input.substr(0, input.find(" "));

    if (input == commands[0])
    {
      return 0;
    }
    else if (command == commands[1])
    {
      echo(input);
    }
    else if (command == commands[2])
    {
      type(input);
    }
    else
    {
      cout << input << inputError << endl;
    }
  }
}

void echo(string input)
{
  string echoInput(input.substr(input.find(" ") + 1, input.length() - 1));
  cout << echoInput << endl;
}

// Only checks the first string up until the space
void type(string input)
{
  // 1. Check if command is in commands[] (built-in)
  // 2. Check PATH, if found, print entire PATH
  // 3. Else command not fouund

  istringstream inputStream(input);
  string firstWord, secondWord;

  inputStream >> firstWord >> secondWord; // gets the word following "type"

  for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++)
  {
    string trimmedCommand(commands[i].substr(0, commands[i].find(" "))); // for commands with spaces like "exit 0"
    if (secondWord == trimmedCommand)
    {
      cout << RED << trimmedCommand << RESET << typeSuccess << endl; // Built-in
      return;
    }
  }

  // Found in PATH

  cout << secondWord << typeError << endl; // Command does not exist
}

