#include <iostream>

using namespace std;

string commands[3] = {
  "exit 0",
  "echo",
  "type"
};

//functions
void echo(string input);

int main() {

  // Common String outputs
  const string invalidCommand(": command not found");

  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  // Read-Eval-Print Loop
  while(true) {

    std::cout << "$ ";

    string input;
    getline(cin, input);

    string command = input.substr(0, input.find(" "));

    if (input == commands[0]) {
        return 0;
    }
    else if (command == commands[1]) {
        echo(input);
    }
    else {
        cout << input << invalidCommand << endl;
    }

  }

}

void echo(string str) {
   string echoInput(str.substr(str.find(" ") + 1, str.length() - 1));
   cout << echoInput << endl;
}