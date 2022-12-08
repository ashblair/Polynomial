#pragma once


class polyexception: public std::exception
{ 
public:
	static const char * p_Exception[];
	//char * pExc;
};

class invalid_char: public polyexception
{
public:
    virtual const char* what() const throw() {return p_Exception[0];};
};

class variablesinexponent: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[1];};
};

class negativeexponent: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[2];};
};

class nonwholeexponent: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[3];};
};

class unreal: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[4];};
};

class exponenterror: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[5];};
};

class zerodivide: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[6];};
};

class dpolydegree: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[7];};
};

class dpolyempty: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[8];};
};

class incorrectGetEndParentheses: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[9];};
};

class parenthesesunmatched: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[10];};
};

class incorrectCheckParentheses: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[11];};
};

class CheckParenthesesEmpty: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[12];};
};

class incorrectRemoveSuperfluous: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[13];};
};

class incorrectRemoveSpaces: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[14];};
};

class incorrectCheckOperators: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[15];};
};

class adjacentops: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[16];};
};

class opsatend: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[17];};
};

class opsendingq: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[18];};
};

class opsbeginningq: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[19];};
};

class improperCheckCharacters: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[20];};
};

class GetNumberEmpty: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[21];};
};

class overflow: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[22];};
};

class underflow: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[23];};
};

class overdigits: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[24];};
};

class n_a_n: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[25];};
};

class GetWholeEmpty: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[26];};
};

class improperGetFromString: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[27];};
};

class GetUnitError: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[28];};
};

class GetPolyError: public polyexception
{
public:
	virtual const char * what() const throw() {return p_Exception[29];};
};

