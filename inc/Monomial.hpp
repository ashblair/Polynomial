#pragma once
//#include "Variable.h"


typedef struct MonomialTermType * pMTType;

typedef struct MonomialTermType
{
	MonomialTermType(const Variable & vIn): v(vIn), exponent(0), pNextComponent(NULL) {}
	const Variable & v;
	long exponent;
	pMTType pNextComponent;
	Glib::ustring print(void) const;
} MonomialTerm;

class Monomial
{
public:
	Monomial(void);
	Monomial(double);
	Monomial(const Variable &);
	//Monomial(double, MonomialTermType &);
	Monomial(const Monomial &);
	~Monomial(void);
	Monomial operator *=(const double &);
	Monomial operator *=(const Monomial &); //{return *this * a;};
	friend Monomial operator *(const double &a, const Monomial &b) {Monomial c = b; return c*=a;}
	friend Monomial operator *(const Monomial & a, const double & b) {Monomial c = a; return c*=b;}
	friend Monomial operator *(const Monomial &a, const Monomial &b) {Monomial c = a; return c*=b;}
	// the next operators deal with the Monomial term's order and equality
	// they do not check the coefficient
	bool IsEQ(const Monomial&) const;
	bool IsLT(const Monomial&) const;
	friend bool operator <(const Monomial &a, const Monomial &b) {return a.IsLT(b);}
	friend bool operator ==(const Monomial &a, const Monomial &b) {return a.IsEQ(b);}

	static int iMonCount;
	double coefficient;
	pMTType pTerm;
	unsigned int degree;
	char * error;
	//Monomial & operator =(Monomial &);
	Monomial operator =(const Monomial &);
	//Monomial Eq(const Monomial &);
	//Monomial & operator =(const double &);
	void ErrorEncountered(const char *);
	Glib::ustring print(void);
};

class MonomialQuotient
{
public:
	MonomialQuotient(void);
	MonomialQuotient(const MonomialQuotient &);
	~MonomialQuotient(void);
	void divide(const Monomial &, const Monomial &);
	friend Monomial operator /(const Monomial &a, const Monomial &b) {MonomialQuotient mq; mq.divide(a, b); return mq.q;}
	friend Monomial operator %(const Monomial &a, const Monomial &b) {MonomialQuotient mq; mq.divide(a, b);	return mq.r;}
	Monomial GetQuotient(void) {return q;};
	Monomial GetRemainder(void) {return r;};
private:
	Monomial q, r;

};

