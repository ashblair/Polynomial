#pragma once


typedef struct UnitTreeNodeType * pUnitTreeNode;
typedef struct UnitTreeNodeType
{
	UnitTreeNodeType(void): id('\0'), txtIn(""), up(NULL), left(NULL), right(NULL) {}
	char id; //^ * / + - q v n i.e. quantity variable or number
	std::string txtIn; 
	pUnitTreeNode up, left, right;

} UnitTreeNode;

//class Polynomial;
typedef class Polynomial *pPolynomial;
class PolynomialString
{
public:
	PolynomialString(void);
	PolynomialString(const Glib::ustring &);
	PolynomialString(const PolynomialString &);
	PolynomialString(const pPolynomial &);
	~PolynomialString(void);

	Glib::ustring GetOutputPolyString(void); // {return outputpolystring;}
	Glib::ustring GetInputPolyString(void);
	//char * GetOutputPolyString1(void) {return outputpolystring;};
	//char * GetOutputPolyString2(void) {return richoutputpolystring;};
	char * GetOutputError(void); // {return PLY.error;};
	pPolynomial GetPolynomial(void); // {return PLY;};

private:

	std::string polystring; 
	//, * outputpolystring, * richoutputpolystring;
	Glib::ustring outputpolystring, inputpolystring;
	pPolynomial pPLY;

	std::string makeASC(const Glib::ustring &);
	Glib::ustring makeUST(const std::string &);
	Glib::ustring makeUST(const Polynomial &);
	size_t GetEndParentheses(const size_t, const std::string &);
	//char * GetEndParentheses(char *);
	void CheckParentheses(void);
	void RemoveSuperfluousParentheses(void);
	void RemoveSpaces(void);
	bool IsOperator(char);
	void CheckOperators(void);  // error if adjacent or end
	void CheckCharacters(void);  // error if other than {A to Z,a to z,0 to 9,.+-*/^()}
	//double GetNumber(char *, int &);
	//long GetWholeNumber(char *, int &);
	void GetFromString(void);
	int GetPrecedence(char);
	std::string GetUnit(const std::string &, size_t &);
	pUnitTreeNode BuildUnitTree(const std::string &);
	pPolynomial GetPoly(pUnitTreeNode);
	void DeleteTree(pUnitTreeNode);
	//void PrintPoly(void); // outputs polynomial into polystring


};




