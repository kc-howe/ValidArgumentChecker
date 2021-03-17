// Valid Argument Checker
// Written by Kenneth Howe, March 2021

#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <math.h>
#include <stack>
#include <string>
#include <queue>
#include <vector>

using namespace std;

// Throw if mismatched parens
class parexception : public exception
{
public:
	virtual const char* what() const throw()
	{
		return "Mismatched parentheses.";
	}
} pex;

// Throw if mismatched brackets
class braexception : public exception
{
public:
	virtual const char* what() const throw()
	{
		return "Mismatched brackets.";
	}
} bex;

// Throw if invalid syntax
class synexception : public exception
{
public:
	virtual const char* what() const throw()
	{
		return "Invalid syntax.";
	}
} sex;

// Class for representing sentences as binary expression trees
class Sentence {

private:

	char symbol;
	Sentence* leftArg;
	Sentence* rightArg;

public:

	// CONSTRUCTOR
	Sentence(char rootSymbol) {
		this->symbol = rootSymbol;
		this->leftArg = NULL;
		this->rightArg = NULL;
	}

	Sentence() {
		this->symbol = '\0';
		this->leftArg = NULL;
		this->rightArg = NULL;
	}

	// GETTERS AND SETTERS
	Sentence* getLeftArg() {
		return this->leftArg;
	}

	Sentence* getRightArg() {
		return this->rightArg;
	}

	void setRootSymbol(char symbol) {
		this->symbol = symbol;
	}

	char getRootSymbol() {
		return this->symbol;
	}

	// TREE METHODS
	void insertLeft(Sentence* branch) {
		if (this->leftArg == NULL) {
			this->leftArg = branch;
		}
		else {
			Sentence* s = branch;
			s->leftArg = this->leftArg;
			this->leftArg = s;
		}
	}

	void insertRight(Sentence* branch) {
		if (this->rightArg == NULL) {
			this->rightArg = branch;
		}
		else {
			Sentence* s = branch;
			s->rightArg = this->rightArg;
			this->rightArg = s;
		}
	}

	// Recursive method for evaluating the sentence
	bool evaluate(map<char, bool>* atomValues) {
		switch (this->symbol) {
		case '&':
			if (!this->leftArg || !this->rightArg) {
				throw sex;
			}
			return (this->leftArg)->evaluate(atomValues) && (this->rightArg)->evaluate(atomValues);
		case '|':
			if (!this->leftArg || !this->rightArg) {
				throw sex;
			}
			return (this->leftArg)->evaluate(atomValues) || (this->rightArg)->evaluate(atomValues);
		case '~':
			if (!this->leftArg) {
				throw sex;
			}
			return !(this->leftArg)->evaluate(atomValues);
		case '>':
			if (!this->leftArg || !this->rightArg) {
				throw sex;
			}
			return !(this->leftArg)->evaluate(atomValues) || (this->rightArg)->evaluate(atomValues);
		case '=':
			if (!this->leftArg || !this->rightArg) {
				throw sex;
			}
			return ((this->leftArg)->evaluate(atomValues) && (this->rightArg)->evaluate(atomValues)) || !((this->leftArg)->evaluate(atomValues) || (this->rightArg)->evaluate(atomValues));
		case ',':
			if (!this->leftArg || !this->rightArg) {
				throw sex;
			}
			return (this->leftArg)->evaluate(atomValues) && (this->rightArg)->evaluate(atomValues);
		case '/':
			if (!this->leftArg || !this->rightArg) {
				throw sex;
			}
			return !(this->leftArg)->evaluate(atomValues) || (this->rightArg)->evaluate(atomValues);
		case '0':
			return false;
		case '1':
			return true;
		default:
			if (isalpha(this->symbol)) {
				return atomValues->at(this->symbol);
			}
			throw sex;
		}
	}

	void print(string prefix = "", bool isLeft = true) {
		cout << prefix << "`-(" << this->getRootSymbol() << ")" << endl;

		if (isLeft) {
			prefix += "   ";
		}
		else {
			prefix += "|  ";
		}

		if (this->getRightArg()) {
			this->getRightArg()->print(prefix, false);
		}
		if (this->getLeftArg()) {
			this->getLeftArg()->print(prefix, true);
		}
	}
};

// Checks if statement is atomic
bool isAtomic(char c) {
	return isalpha(c) || c == '0' || c == '1';
}

// Just returns a map of op precedences
map<char, int>* getOperatorPrecedence() {
	static map<char, int> p;
	p['~'] = 7;
	p['&'] = 6;
	p['|'] = 5;
	p['>'] = 4;
	p['='] = 3;
	p[','] = 2;
	p['/'] = 1;

	return &p;
}

// Converts infix expressions to postfix
queue<char>* polishParse(string inputSentence) {

	// Quick syntax check, looking for duplicate/misplaced '{' or '/' symbols
	int quotientIndex = -1;
	int bracketIndex = -1;
	for (int i = 0; i < inputSentence.length(); i++) {
		if (inputSentence[i] == '/') {
			if (quotientIndex > -1) {
				throw sex;
			}
			else {
				quotientIndex = i;
			}
		}
		else if (inputSentence[i] == '{') {
			if (bracketIndex > -1 || bracketIndex > quotientIndex) {
				throw sex;
			}
			else {
				bracketIndex = i;
			}
		}
	}

	// Using Shunting-Yard for the conversion

	static queue<char> output;

	char token;
	stack<char> operators;

	int ind = 0;
	char nextOperator = '\0';
	int nextOperatorPrec = 0;

	map<char, int> precedence = *getOperatorPrecedence();

	for (int i = 0; i < inputSentence.length(); i++) {
		token = inputSentence.at(i);

		switch (token) {

			// Push unary operator ~ to operator stack
		case '~':
			operators.push(token);
			break;

			// If token is left paren, push to stack
		case '(':
			operators.push(token);
			break;

			// If token is right paren, push operators until left paren is reached
		case ')':
			while (!operators.empty() && operators.top() != '(') {
				if (operators.top() == ',') {
					throw sex;
				}
				output.push(operators.top());
				operators.pop();
			}

			// CHECK FOR MISMATCHED PARANTHESES
			// (i.e. operators run out before left paren reached)
			if (operators.empty() || operators.top() != '(') {
				throw pex;
			}
			operators.pop();
			break;

			// Similar approach for left and right brackets
		case '{':
			operators.push(token);
			break;

		case '}':
			while (!operators.empty() && operators.top() != '{') {
				if (operators.top() == '}') {
					throw sex;
				}
				output.push(operators.top());
				operators.pop();
			}

			// CHECK FOR MISMATCHED BRACKETS
			if (operators.empty() || operators.top() != '{') {
				throw bex;
			}
			operators.pop();
			break;

			// If space, ignore
		case ' ':
			break;

		default:
			// If token is atomic, push it to output queue
			if (isAtomic(token)) {
				if (i < inputSentence.length() - 1 && isAtomic(inputSentence[i + 1])) {
					throw sex;
				}
				output.push(token);
			}

			// If the next token is an operator. . .
			else if (precedence.find(token) != precedence.end()) {

				// Pop operators from stack onto the output queue if they take precedence
				while ((!operators.empty()) && (precedence[operators.top()] >= precedence[token]) && token != '(') {
					output.push(operators.top());
					operators.pop();
				}

				// Push the current operator onto the stack
				operators.push(token);

			}

			// Syntax includes unhandled character
			else {
				throw sex;
			}

			break;
		}
	}

	// If no more tokens to read, dump leftover operators
	while (!operators.empty()) {
		if (operators.top() != '(' && operators.top() != ')') {
			output.push(operators.top());
		}
		else {
			throw pex;
		}
		operators.pop();
	}

	return &output;
}

// Builds an expression tree from postfix notation
Sentence* constructSentence(queue<char>* polish) {

	stack<Sentence*> components;

	while (!polish->empty()) {

		// If next token is atomic, push to stack
		if (isAtomic(polish->front())) {
			Sentence* atom = new Sentence(polish->front());
			components.push(atom);
			polish->pop();
		}

		// If next token is a unary operator, pop a value from the stack and push result onto stack
		else if (polish->front() == '~') {

			Sentence* unarySentence = new Sentence(polish->front());
			polish->pop();

			if (!components.empty()) {
				unarySentence->insertLeft(components.top());
				components.pop();
			}

			components.push(unarySentence);

		}

		// If next token is a binary operator, pop two components from the stack and push the result onto stack
		else {

			Sentence* binarySentence = new Sentence(polish->front());
			polish->pop();

			if (!components.empty()) {
				binarySentence->insertRight(components.top());
				components.pop();
			}

			if (!components.empty()) {
				binarySentence->insertLeft(components.top());
				components.pop();
			}

			components.push(binarySentence);

		}

	}

	return components.top();

}

// Computes next "line" in "truth table" ordered from all false to all true
bool incrementAtomValues(vector<char>* atoms, map<char, bool>* atomValues) {

	int firstFalse = -1;
	int firstTrue = -1;
	int lastFalse = atoms->size() - 1;
	int lastTrue = atoms->size();

	for (int i = 0; i < atoms->size(); i++) {
		if (!atomValues->at(atoms->at(i)) && i < lastFalse) {
			lastFalse = i;
		}
		if (!atomValues->at(atoms->at(i)) && i > firstFalse) {
			firstFalse = i;
		}
		if (!atomValues->at(atoms->at(i)) && i < lastTrue) {
			lastFalse = i;
		}
		if (!atomValues->at(atoms->at(i)) && i > firstTrue) {
			firstFalse = i;
		}
	}
	if (firstFalse < 0) {
		return false;
	}
	if (lastFalse < firstTrue) {
		for (int i = 0; i < atoms->size(); i++) {
			atomValues->at(atoms->at(i)) = false;
		}
		atomValues->at(atoms->at(lastFalse)) = true;
	}
	else if (lastFalse < lastTrue) {
		for (int i = lastFalse; i < atoms->size(); i++) {
			atomValues->at(atoms->at(i)) = false;
		}
		atomValues->at(atoms->at(lastFalse)) = true;
	}
	else {
		atomValues->at(atoms->at(lastFalse)) = true;
	}

	return true;
}

// Checks the argumentative validity of a sentence
bool evaluateSentence(string input) {
	queue<char>* polish = polishParse(input);
	Sentence* sentence = constructSentence(polish);

	vector<char> atoms;
	static map<char, bool> atomValues;

	// Find all atoms in sentence, initialize their truth values
	for (int i = 0; i < input.length(); i++) {
		if (isalpha(input[i]) && find(atoms.begin(), atoms.end(), input[i]) == atoms.end()) {
			atoms.push_back(input[i]);
			atomValues[input[i]] = false;
		}
	}

	// Cycle through all possible truth values and evaluate
	bool isValid = true;
	bool incrementAgain = true;

	while (isValid && incrementAgain) {
		isValid = sentence->evaluate(&atomValues);
		incrementAgain = incrementAtomValues(&atoms, &atomValues);
	}

	return isValid;
}

void printTable(string input) {
	queue<char>* polish = polishParse(input);
	Sentence* sentence = constructSentence(polish);

	vector<char> atoms;
	static map<char, bool> atomValues;

	// Find all atoms in sentence, initialize their truth values
	for (int i = 0; i < input.length(); i++) {
		if (isalpha(input[i]) && find(atoms.begin(), atoms.end(), input[i]) == atoms.end()) {
			atoms.push_back(input[i]);
			atomValues[input[i]] = false;
		}
	}

	// Print table header
	for (int i = 0; i < atoms.size(); i++) {
		cout << " " << atoms[i] << " |";
	}
	cout << " ARG " << endl;
	for (int i = 0; i < atoms.size(); i++) {
		cout << "----";
	}
	cout << "-----" << endl;

	// Cycle through all possible truth values and evaluate, print to row
	bool isValid;
	for (int i = 0; i < pow(2, atoms.size()); i++) {
		for (int j = 0; j < atoms.size(); j++) {
			cout << " " << (atomValues[atoms[j]] ? 1 : 0) << "  ";
		}
		isValid = sentence->evaluate(&atomValues);
		cout << "  " << (isValid ? 1 : 0) << endl;

		incrementAtomValues(&atoms, &atomValues);
	}
}

int main() {

	string prevArg = "";
	string argument;
	bool isValid;

	cout << "Type an argument to get started (type 'help' for help or 'quit' to quit):" << endl;

	while (1) {

		cout << "> ";
		getline(cin, argument);

		if (argument == "help") {
			cout << "\nINSTRUCTIONS:\n" <<
				"Type a logical argument or expression at the prompt and press ENTER. " <<
				"The argument will be evaluated for logical validity (i.e. tautology via a truth table) " <<
				"and the result will be printed to the console.\n" << endl;
			cout << "SYNTAX:\n" <<
				" a-Z - propositions\n" <<
				"  0  - contradiction\n" <<
				"  1  - tautology\n" <<
				" { } - brackets (optional, group premises)\n" <<
				" ( ) - parentheses\n" <<
				"  ~  - not\n" <<
				"  &  - and\n" <<
				"  |  - or\n" <<
				"  >  - if-then\n" <<
				"  =  - if and only if\n" <<
				"  ,  - separator\n" <<
				"  /  - quotient (deductive transition)\n" << endl;
			cout << "COMMANDS:\n" <<
				"help         Prints this page.\n" <<
				"show-table   Prints an abbreviated truth table for the previous argument.\n" <<
				"show-tree    Prints the syntax tree for the previous argument.\n"
				"quit         Ends the program.\n" << endl;
			cout << "NOTES:\n" <<
				" - Operator precedence is in the same order as shown above.\n" <<
				" - Operators of equal precedence are read from left to right.\n" <<
				"    - e.g. (P > Q > R) = ((P > Q) > R)\n" <<
				" - Spaces can be used freely to improve readability.\n" << endl;
			cout << "EXAMPLE:\n\n" <<
				"Premise 1:  Either it is cloudy outside, or it is not raining.\n" <<
				"Premise 2:  It is raining.\n" <<
				"Conclusion: Therefore, it is cloudy outside.\n\n" <<
				"Using \',\' to separate our premises, '{ }' to group them, and \'/\' to draw our conclusion, this argument can be written:\n\n" <<
				"{C | ~R, R} / C\n\n" <<
				"We can also use the '=' operator to validate the logical equivalence of two expressions:\n\n" <<
				"(~P | Q) = (P > Q)\n" <<
				"(P & ~P) = 0\n" << endl;
		}
		else if (argument == "quit") {
			break;
		}
		else if (argument == "show-tree") {
			if (prevArg != "") {
				constructSentence(polishParse(prevArg))->print();
			}
			else {
				cout << "No argument found." << endl;
			}
		}
		else if (argument == "show-table") {
			if (prevArg != "") {
				printTable(prevArg);
			}
			else {
				cout << "No argument found." << endl;
			}
		}
		else if (argument == "") {
			continue;
		}
		else {
			try {
				isValid = evaluateSentence(argument);
			}
			catch (synexception s) {
				cout << "ERROR: " << s.what() << endl;
				continue;
			}
			catch (parexception p) {
				cout << "ERROR: " << p.what() << endl;
				continue;
			}
			catch (exception e) {
				cout << "ERROR: " << e.what() << endl;
				continue;
			}
			if (!isValid) {
				cout << "in";
			}
			cout << "valid" << endl;

			// Store argument for use in print commands
			prevArg = argument;
		}
	}
}
