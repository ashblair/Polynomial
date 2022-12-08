#pragma once

typedef struct MonomialListType * pMonomialListType;
typedef struct MonomialListType
{
	Monomial * mon;
	pMonomialListType pNextMon;
} MonomialListType;

typedef class Polynomial * pPolynomial;
/*
class PolynomialQuotient
{
public:
	PolynomialQuotient(void);
	PolynomialQuotient(const PolynomialQuotient &);
	~PolynomialQuotient(void);
	void divide(const pPolynomial &, const pPolynomial &);
	pPolynomial GetQuotient(void) {return q;};
	pPolynomial GetRemainder(void) {return r;};
private:
	pPolynomial q, r;
};
*/
class Polynomial
{
public:
	Polynomial(void);
	Polynomial(const Monomial &);
	Polynomial(const Polynomial &);
	~Polynomial(void);
	Polynomial operator =(const Polynomial &);
	Polynomial operator =(const Monomial &);
	Polynomial operator =(const double &);
	Polynomial operator +=(const Monomial &);
	Polynomial operator +=(const Polynomial &); 
	Polynomial operator -=(const Monomial &);
	Polynomial operator -=(const Polynomial &);
	Polynomial operator *=(const double &);
	Polynomial operator *=(const Monomial &);
	Polynomial operator *=(const Polynomial &); 
	Polynomial SquareMe(void); 
	Polynomial operator ^=(const Polynomial &); 
	Polynomial operator /=(const double &);
	void divide(const Polynomial &, pPolynomial); // does a /= w/ first parameter, returns remainder in second
	friend Polynomial operator +(const Monomial &a, const Monomial &b) {Polynomial c = a; return c+=b;}
	friend Polynomial operator +(const Monomial &a, const Polynomial &b) {Polynomial c = b; return c+=a;}
	friend Polynomial operator +(const Polynomial & a, const Monomial & b){return b + a;}
	friend Polynomial operator +(const Polynomial &a, const Polynomial &b) {Polynomial c = a; return c+=b;}
	friend Polynomial operator -(const Monomial &a, const Monomial &b) {Polynomial c = a; return c-=b;}
	friend Polynomial operator -(const Monomial &a, const Polynomial &b) {Polynomial c = a; return c-=b;}
	friend Polynomial operator -(const Polynomial &a, const Monomial &b) {Polynomial c = a; return c-=b;}
	friend Polynomial operator -(const Polynomial &a, const Polynomial &b) {Polynomial c = a; return c-=b;}
	friend Polynomial operator *(const double &a, const Polynomial &b) {Polynomial c = b; return c*=a;}
	friend Polynomial operator *(const Polynomial & a, const double & b){return b * a;}
	friend Polynomial operator *(const Monomial &a, const Polynomial &b) {Polynomial c = b; return c*=a;}
	friend Polynomial operator *(const Polynomial & a, const Monomial & b){return b * a;};
	friend Polynomial operator *(const Polynomial &a, const Polynomial &b) {Polynomial c = a; return c*=b;}
	friend Polynomial operator ^(const Polynomial &a, const Polynomial &b) {Polynomial c = a; return c^=b;}
	friend Polynomial operator /(const Polynomial &a, const double &b) {Polynomial c = a; return c/=b;}
	friend Polynomial operator /(const Polynomial &a, const Polynomial &b) {Polynomial A=a, r; A.divide(b, &r); return A;}
	friend Polynomial operator %(const Polynomial &a, const Polynomial &b) {Polynomial A=a, r; A.divide(b, &r); return r;}
	//Polynomial operator /=(const Polynomial &); 
	Glib::ustring print(void) const;

	static VariableLU vl;
	static int iMemAccess;
	pMonomialListType pmL;
	unsigned int degree;
	char  * error;
};

