#include <iostream>

using namespace std;

// commands
const string commands[3] = {
  "exit 0",
  "echo",
  "type"
};

// Common string outputs
const string shellStart("$ ");
const string inputError(": command not found");
const string typeError(": not found");
const string typeSuccess(" is a shell builtin");

//functions
void echo(string input);
void type(string input);

int main() {


  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  // Read-Eval-Print Loop
  while(true) {

    std::cout << shellStart;

    string input;
    getline(cin, input);

    string command = input.substr(0, input.find(" "));

    if (input == commands[0]) {
        return 0;
    }
    else if (command == commands[1]) {
        echo(input);
    }
    else if (command == commands[2]) {
        type(input);
    }
    else {
        cout << input << inputError << endl;
    }

  }

}

void echo(string input) {
   string echoInput(input.substr(input.find(" ") + 1, input.length() - 1));
   cout << echoInput << endl;
}

// Only checks the first string up until the space
void type(string input) {
   string secondString(input.substr(input.find(" ") + 1, input.length() - 1));
   string trimmedSecondString(secondString.substr(0, secondString.find(" ")));
  for (int i = 0; i < sizeof(commands); i++) {
    string trimmedCommand(commands[i].substr(0, commands[i].find(" ")));
    if (trimmedSecondString == trimmedCommand) {
        cout << trimmedCommand << typeSuccess << endl;
        return;
    }
  }
  cout << trimmedSecondString << typeError << endl;
}