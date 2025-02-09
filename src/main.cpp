#include <iostream>
#include <sstream>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

// Environment variables
const string PATH = getenv("PATH");
int directoryCount = 1; 
char pathDelimiter = ':';

const string commands[4] = {
    "exit 0",
    "echo",
    "type",
    "pwd"
};

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
void type(string input, const std::string* arr, size_t size);
void checkCommand(string input, const std::string* arr, size_t size, char* argv[]);

int main(int argc, char* argv[])
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

    // forloop of built-in commands, then check PATH  
    if (input == commands[0])
    {
      return 0; // exit 0
    }
    else if (command == commands[1])
    {
      echo(input);
    }
    else if (command == commands[2])
    {
      type(input, directories, directoryCount);
    }
    else if (command == commands[3])
    {
      cout << filesystem::current_path().string() << endl;
    }
    else
    {
      checkCommand(input, directories, directoryCount, argv);
    }
  }
}

void echo(string input)
{
  string echoInput(input.substr(input.find(" ") + 1, input.length() - 1));
  cout << echoInput << endl;
}

// Only checks the first string up until the space
void type(string str, const std::string* arr, size_t size)
{
  // 1. Check if command is in commands[] (built-in)
  // 2. Check PATH, if found, print entire PATH
  // 3. Else command not fouund

  istringstream inputStream(str);
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

  // Find command in PATH
  // 1. loop through array 
  // 2. loop through directories
  for (int i = 0; i < size; i++) {
    
    for (const auto& entry : filesystem::directory_iterator(arr[i])) {

      // gets the path for the binary, and the binary is compared ot the command
      std::filesystem::path outfilename = entry.path();
      std::string outfilename_str = outfilename.string(); 
  
      // Gets the binary from the path i.e., 'ls' from /usr/bin/ls
      std::string last_element = outfilename_str.substr(outfilename_str.rfind("/") + 1); //binary
      
      if (secondWord == last_element) {
        cout << RED << secondWord << RESET << " is " << outfilename_str << endl;
        return;
      }

    }
    
  }

  cout << secondWord << typeError << endl; // Command does not exist
}

void checkCommand(string str, const std::string* arr, size_t size, char* argv[]) {

  // 1. Check if command is in PATH, and execute, 
  // 2. Must also accept command line arguments
  string firstWord = str.substr(0, str.find(" "));

  // Find command in PATH
  // 1. loop through array 
  // 2. loop through directories
  for (int i = 0; i < size; i++) {
    
    for (const auto& entry : filesystem::directory_iterator(arr[i])) {

      // gets the path for the binary, and the binary is compared ot the command
      filesystem::path outfilename = entry.path();
      string outfilename_str = outfilename.string(); 
  
      // Gets the binary from the path i.e., 'ls' from /usr/bin/ls
      std::string last_element = outfilename_str.substr(outfilename_str.rfind("/") + 1); //binary
      
      if (firstWord == last_element) {

        if (str.find(" ") != string::npos) {                  
          
          // gets the path of the binary, and the arguments, and combines them
          string commandAndArgumentsStr = firstWord + str.substr(str.find(" "), str.length() - 1);
          
          // converting string to char*
          const char* commandAndArguments = commandAndArgumentsStr.c_str();

          system(commandAndArguments);
        } else {
          // runs binary without arguments
          system(outfilename_str.c_str());

        }

        return; // binary executed
      }

    }
    
  }

  cout << firstWord << typeError << endl; // Command does not exist

}
