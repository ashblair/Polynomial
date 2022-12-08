#pragma once


typedef class Variable
{
public:
	Variable(void);  // constructs a question mark variable
	Variable(const Variable &);
	Variable(char);  // A variable like x, y or z
	Variable(char, long);  // A variable w/ a subscript like x sub 1
	Variable(char *&);  // A variable embedded in a string 
	Variable & operator =(Variable &);
	bool IsEQ(const Variable &) const;
	bool IsLT(const Variable &) const;
	friend bool operator ==(const Variable&a, const Variable&b) {return a.IsEQ(b);}
	friend bool operator <(const Variable&a, const Variable&b) {return a.IsLT(b);}
	~Variable(void);

	char varV; // the identifying character
	long varSub; // the subscript, positive for all user input, -1 indicates empty
	static bool IsVariable(char);
	static const Glib::ustring subs, exps;
	static Glib::ustring printSubs(const std::string &);
	static Glib::ustring printExps(const std::string &);
	static double GetDbl(char *, int &);
	static long GetLng(char *, int &);


	Glib::ustring print(void) const;

} *pVariable;

/*
typedef struct varListType * pvarListType;
typedef struct varListType
{
	Variable * var;
	pvarListType pNextVar;
} varListType;
*/

class VariableLU
{
public:
	VariableLU(void);
	~VariableLU(void);

	void DeleteVariable(Variable &);
	void AddAllVariables(const std::string &);
	const Variable & RetrieveVariable(char, long = -1);
	Glib::ustring print(void) const;
	//pvarListType vList;
	//VariableList & operator =(const VariableList &);
	//void Merge(const VariableList&);

	std::map<Variable, int> vLU;
};
