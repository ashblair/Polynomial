#include "first.hpp"
#include "Variable.hpp"
#include "Monomial.hpp"
#include "Polynomial.hpp"
#include "PolyExceptions.hpp"

using namespace std;

VariableLU Polynomial::vl;

int Polynomial::iMemAccess = 0;

Polynomial::Polynomial()
{
	pmL = new MonomialListType;
	pmL->pNextMon = NULL;
	pmL->mon = new Monomial;
	degree = 0;
	error = NULL;
	iMemAccess++;
}

Polynomial::Polynomial(const Monomial & m)
{
	degree = m.degree;
	error = NULL;
	if (m.error != NULL) error = m.error; 
	pmL=new MonomialListType;
	pmL->pNextMon = NULL;
	pmL->mon = new Monomial(m);
	//*(pmL->mon) = m;
	iMemAccess++;
}

Polynomial::Polynomial(const Polynomial & a)
{
	degree = a.degree;
	error = NULL;
	if (a.error != NULL) error = a.error;
	pmL = NULL;
	pMonomialListType trail = NULL, traverse = a.pmL;
	while (traverse != NULL)
	{
		pMonomialListType p = new MonomialListType;
		p->pNextMon = NULL;
		p->mon = new Monomial(*(traverse->mon));
		//*(p->mon) = *(traverse->mon);
		if (trail == NULL) pmL=p;
		else trail->pNextMon = p;
		trail = p;
		traverse = traverse->pNextMon;
	}
	iMemAccess++;

}

Polynomial::~Polynomial()
{
	pMonomialListType pDoom, pNext;

	pDoom = pmL;
	while (pDoom != NULL)
	{
		pNext = pDoom->pNextMon;
		delete(pDoom->mon);
		delete pDoom;
		pDoom = pNext;
	}
	pmL = NULL;
	iMemAccess--;
}

Polynomial Polynomial::operator =(const Polynomial & a)
{
	pMonomialListType traverse, last = pmL, p, trail = NULL;
	degree = a.degree;
	error = NULL;
	if (a.error != NULL) error = a.error;
	traverse = a.pmL;
	while (traverse != NULL)
	{
		if (last == NULL)
		{
			p = new MonomialListType;
			p->pNextMon = NULL;
			p->mon = new Monomial;
			if (pmL == NULL)
			{
				pmL = p;
				last = pmL;
			}
			else if (trail == NULL)
			{
				trail = pmL;
				pmL->pNextMon = p;
				last = p;
			}
			else
			{
				trail->pNextMon = p;
				last = p;
			}
		}
		*(last->mon) = *(traverse->mon);
		trail = last;
		last = last->pNextMon;
		traverse = traverse->pNextMon;
	}
	if (trail == NULL)
	{
		if (last != NULL) last = last->pNextMon;
		pmL = NULL;
	}
	else trail->pNextMon = NULL;
	while (last != NULL)
	{
		trail = last;
		last = last->pNextMon;
		delete(trail->mon);
		delete(trail);
	}
	return *this;
}

Polynomial Polynomial::operator =(const Monomial & a)
{
	degree = a.degree;
	if (a.error != NULL) error = a.error;
	if (pmL == NULL)
	{
		pmL = new MonomialListType;
		pmL->mon = new Monomial(a);
		pmL->pNextMon = NULL;
	}
	else
	{
		delete(pmL->mon);
		pmL->mon = new Monomial(a);
		pMonomialListType trail = pmL->pNextMon;
		if (trail != NULL) do
		{
			pmL->pNextMon = trail->pNextMon;
			delete(trail->mon);
			delete(trail);
			trail = pmL->pNextMon;
		} while (pmL->pNextMon != NULL);
	}
	return *this;
}

Polynomial Polynomial::operator =(const double & a)
{
	degree = 0;
	error = NULL;
	if (pmL == NULL)
	{
		pmL = new MonomialListType;
		pmL->pNextMon = NULL;
		pmL->mon = new Monomial(a);
	}
	else
	{
		delete(pmL->mon);
		pmL->mon = new Monomial(a);
		pMonomialListType trail = pmL->pNextMon;
		if (trail != NULL) do
		{
			pmL->pNextMon = trail->pNextMon;
			delete(trail->mon);
			delete(trail);
			trail = pmL->pNextMon;
		} while (pmL->pNextMon != NULL);
	}
	return *this;
}
/*
Polynomial operator +(const Monomial & a, const Monomial & b)
{
	Polynomial r = Polynomial();
	// check for zero
	if (abs(a.coefficient) < DBL_EPSILON)
	{
		r = b;
		return r;
	}
	if (abs(b.coefficient) < DBL_EPSILON)
	{
		r = a;
		return r;
	}
	pMonomialListType p =(pMonomialListType)::operator new(sizeof(MonomialListType));
	p->pNextMon = NULL;
	p->mon = new Monomial();
	double sum;
	r.degree = (a.degree < b.degree) ? b.degree: a.degree;
	if (a == b) // this checks the terms not the coefficients
	{
		sum = a.coefficient + b.coefficient;
		if (fabs(sum) < DBL_EPSILON) // this is for zero or appox. zero
		{
			r.degree = 0;
			r.pmL = p;
		}
		else
		{
			*(p->mon) = a;
			p->mon->coefficient = sum;
			r.pmL = p;
		}
	}
	else
	{
		*(p->mon) = a;
		r.pmL = p;
		p = (pMonomialListType)::operator new(sizeof(MonomialListType));
		p->pNextMon = NULL;
		p->mon = new Monomial(b);
		//*(p->mon) = b;
		if (a < b)
		{
			r.pmL->pNextMon = p;
		}
		else
		{
			p->pNextMon = r.pmL;
			r.pmL = p;
		}
	}
	return r;
}

Polynomial operator +(const Monomial & a, const Polynomial & b)
{
	Polynomial r = Polynomial();
	// check for zero
	if (abs(a.coefficient) < DBL_EPSILON)
	{
		r = b;
		return r;
	}
	if ((b.degree == 0) && (abs(b.pmL->mon->coefficient) < DBL_EPSILON))
	{
		r = a;
		return r;
	}
	r = b;
	r += a;
	return r;
}
*/
Polynomial Polynomial::operator+=(const Monomial & a)
{
	// check for zero
	if (abs(a.coefficient) < DBL_EPSILON) // adding zero
	{
		return *this;
	}
	// pmL == NULL -> next line gives error
	if ((degree == 0) && (pmL->mon->coefficient < DBL_EPSILON))
	{
		*(pmL->mon) = a;
		degree = a.degree;
		return *this;
	}
	pMonomialListType p, trail, traverse;
	trail = NULL;
	traverse = pmL;
	bool monomial_in = false;
	while (traverse != NULL)
	{
		if (a == *(traverse->mon)) // terms are equal (not coefficients)
		{
			monomial_in = true;
			traverse->mon->coefficient += a.coefficient;
			if (fabs(traverse->mon->coefficient) < DBL_EPSILON) 
			// monomial now zero -- remove node
			{
				if (trail == NULL) pmL = traverse->pNextMon;
				else trail->pNextMon = traverse->pNextMon;
				delete(traverse->mon);
				delete traverse;
			}
			traverse = NULL; // short circuit loop
		}
		else if (a < *(traverse->mon))
		{
			monomial_in = true;
			p = new MonomialListType;
			p->pNextMon = traverse;
			p->mon = new Monomial(a);
			if (trail == NULL) pmL = p;
			else trail->pNextMon = p;
			traverse = NULL; // short circuit while loop
		}
		else
		{
			trail = traverse;
			traverse = traverse->pNextMon;
		}
	} // end of while loop

	if (!monomial_in) // insert node at end of pmL
	{
		p = new MonomialListType;
		p->pNextMon = NULL;
		p->mon = new Monomial(a);
		trail->pNextMon = p; // trail cannot be NULL because pmL!=NULL
	}

	//update degree
	degree = 0;
	traverse = pmL;
	while (traverse != NULL)
	{
		if (traverse->mon->degree > degree) degree = traverse->mon->degree;
		traverse = traverse->pNextMon;
	}
	if (pmL == NULL) //deleted the node
	{
		p = new MonomialListType;
		p->pNextMon = NULL;
		p->mon = new Monomial();
		pmL = p;
	}
	return *this;

}
/*
Polynomial operator +(const Polynomial & a, const Polynomial & b)
{
	Polynomial r = Polynomial();
	// check for zero
	if ((a.degree == 0) && (abs(a.pmL->mon->coefficient) < DBL_EPSILON))
	{
		r = b;
		return r;
	}
	if ((b.degree == 0) && (abs(b.pmL->mon->coefficient) < DBL_EPSILON))
	{
		r = a;
		return r;
	}
	r = b;
	r += a;
	return r;
}
*/
Polynomial Polynomial::operator +=(const Polynomial & a)
{
	// check for zero
	if ((a.degree == 0) && (abs(a.pmL->mon->coefficient) < DBL_EPSILON))
	{
		return *this;
	}
	pMonomialListType traverse = a.pmL;
	while (traverse != NULL)
	{
		*this += *(traverse->mon);
		traverse = traverse->pNextMon;
	}
	return *this;

}
/*
Polynomial operator -(const Monomial & a, const Monomial & b)
{
	Polynomial r = b;
	r *= -1.0;
	r += a;
	return r;
}

Polynomial operator -(const Monomial & a, const Polynomial & b)
{
	Polynomial r = b;
	r *= -1.0;
	r += a;
	return r;
}
*/
Polynomial Polynomial::operator -=(const Monomial & a)
{
	Monomial mNegA(a);
	mNegA *= -1.0;
	*this += mNegA;
	return *this;
}
/*
Polynomial operator -(const Polynomial & a, const Monomial & b)
{
	Polynomial r = b;
	r *= -1.0;
	r += a;
	return r;
}

Polynomial operator -(const Polynomial & a, const Polynomial & b)
{
	Polynomial r = a;
	r -= b;
	return r;
}
*/
Polynomial Polynomial::operator-=(const Polynomial & a)
{
	Polynomial pNegA(a);
	pNegA *= -1.0;
	*this += pNegA;
	return *this;
}
/*
Polynomial operator *(const double & a, const Polynomial & b)
{
	Polynomial r = b;
	r *= a;
	return r;
}
*/
Polynomial Polynomial::operator *=(const double & a)
{
	pMonomialListType traverse = pmL;
	if (fabs(a) > DBL_EPSILON) // check for non-zero parameter
	{
		while (traverse != NULL)
		{
			traverse->mon->coefficient *= a;
			traverse = traverse->pNextMon;
		}
	}
	else // multiplication by zero
	{
		while (pmL != NULL) // deleting monomial list
		{
			pmL = pmL->pNextMon;
			delete(traverse->mon);
			delete(traverse);
			traverse = pmL;
		}
		// re-initializing the zero poly
		pmL = new MonomialListType;
		pmL->pNextMon = NULL;
		pmL->mon = new Monomial();
		degree = 0;
	}
	return *this;

}
/*
Polynomial operator *(const Monomial & a, const Polynomial & b)
{
	Polynomial r = b;
	r *= a;
	return r;
}
*/
Polynomial Polynomial::operator *=(const Monomial & a)
{
	// next check for zero or no-monomial poly
	pMonomialListType traverse;
	if ((fabs(a.coefficient) < DBL_EPSILON) || (pmL == NULL))
	{
		traverse = pmL;
		while (pmL != NULL)
		{
			pmL = pmL->pNextMon;
			delete(traverse->mon);
			delete(traverse);
			traverse = pmL;
		}
		pmL = new MonomialListType;
		pmL->pNextMon = NULL;
		pmL->mon = new Monomial();
		degree = 0;
		return *this;
	}
	degree += a.degree;
	traverse = pmL;
	while (traverse != NULL)
	{
		*(traverse->mon) *= a;
		traverse = traverse->pNextMon;
	}


	return *this;
}
/*
Polynomial operator *(const Polynomial & a, const Polynomial & b)
{
	Polynomial r = b;
	r *= a;
	return r;
}
*/
Polynomial Polynomial::operator *=(const Polynomial & a)
{
	// next check for no-monomial poly
	if (a.pmL == NULL) return *this;
	if (a.degree == 0) 
	{
		*this *= a.pmL->mon->coefficient;
		return *this;
	}
	//Polynomial * monotimespoly, *polypluspoly;
	Polynomial oldthis = *this;
	//Making this the zero poly:
	pMonomialListType p = pmL;
	while (pmL != NULL)
	{
		pmL=pmL->pNextMon;
		delete(p->mon);
		delete(p);
		p = pmL;
	}
	pmL = new MonomialListType;
	pmL->pNextMon = NULL;
	pmL->mon = new Monomial();
	degree = 0;
	Polynomial monotimespoly;
	pMonomialListType traverse = a.pmL;
	while (traverse != NULL)
	{
		monotimespoly = oldthis; //leaks?
		monotimespoly *= *(traverse->mon);
		*this += monotimespoly;
		traverse = traverse->pNextMon;
	}
	return *this;

}

/*
Polynomial & operator ^(const Polynomial & a, const unsigned int & b)
{
	Polynomial * r = new Polynomial(1.0);
	//*r = 1.0;
	r->Check4Error(a.error);
	if (r->error != NULL) return *r;
	Polynomial * polytimespoly;
	for (unsigned int i=0; i < b; ++i)
	{
		polytimespoly = &(*r * a);
		*r = *polytimespoly;
		delete(polytimespoly);
	}
	return *r;
}
*/
/*
Polynomial operator ^(const Polynomial & a, const Polynomial & b)
{
	Polynomial r = a;
	r ^= b;
	return r;
}
*/
Polynomial Polynomial::SquareMe(void)
{
	Polynomial oldthis(*this);
	degree *= 2;
	pMonomialListType p = pmL;
	unsigned int uiMonCount = 0;
	while (p != NULL)
	{
		p->mon->degree *= 2;
		p->mon->coefficient *= p->mon->coefficient;
		pMTType t = p->mon->pTerm;
		while (t != NULL)
		{
			t->exponent *= 2;
			t = t->pNextComponent;
		}
		p = p->pNextMon;
		++uiMonCount;
	}
	p = oldthis.pmL;
	Monomial m;
	pMonomialListType traverse = p;
	for (unsigned int i = 0; i < (uiMonCount - 1); ++i)
	{
		for (unsigned int j = i + 1; j < uiMonCount; ++j)
		{
			m = *(p->mon);  //leaks?
			traverse = traverse->pNextMon;
			m *= *(traverse->mon);
			m *= 2.0;
			*this += m;
		}
		p = p->pNextMon;
		traverse = p;

	}

	return *this;
}


Polynomial Polynomial::operator ^=(const Polynomial & a)
{
	if (a.degree > 0)
	{
		throw(variablesinexponent());
	}
	//saving old this and making this = 1.0:
	Polynomial oldthis(*this);
	pMonomialListType p = pmL;
	while (pmL != NULL)
	{
		pmL = pmL->pNextMon;
		delete(p->mon);
		delete(p);
		p = pmL;
	}
	pmL = new MonomialListType;
	pmL->pNextMon = NULL;
	pmL->mon = new Monomial(1.0);
	degree = 0;

	double expo = a.pmL->mon->coefficient;
	double expo_l;
	bool ExpisInteger = (fabs(modf(expo, &expo_l)) < DBL_EPSILON);
	if (oldthis.degree > 0)
	{
		if (ExpisInteger)
		{
			if (expo_l < 0)
			{
				throw(negativeexponent());
			}
			else
			{
				const unsigned int cui = (const unsigned int) expo_l; // exponent
				if (cui == 0) return *this;
				int monomialcount = 0;
				pMonomialListType p = oldthis.pmL;
				while (p != NULL) 
				{
					++monomialcount;
					p = p->pNextMon;
					if (monomialcount > 1) break;
				}
				if (monomialcount == 1)
				{
					*(pmL->mon) = *(oldthis.pmL->mon);
					pmL->mon->coefficient = pow(pmL->mon->coefficient, (int) cui);
					pMTType trm = pmL->mon->pTerm;
					while (trm != NULL)
					{
						trm->exponent *= cui;
						trm = trm->pNextComponent;
					}
					pmL->mon->degree = oldthis.pmL->mon->degree * cui;
					degree = oldthis.degree * cui;
				}
				else
				{
					// m to equal log base 2 of exponent:
					unsigned int m = (unsigned int)(log((double)cui)/log((double)2));
					unsigned int bcheck = 1;
					for (unsigned int i = 0; i <= m; ++i)
					{
						if((bcheck & cui) == bcheck) *this *= oldthis;
						if (i < m) oldthis.SquareMe();
						bcheck <<= 1;
					}
				}
			}
		}
		else
		{
			throw(nonwholeexponent());
		}
	}
	else //a.degree == 0
	{
		double significand = oldthis.pmL->mon->coefficient;
		if (ExpisInteger)
		{
			*this *= pow(significand, (int) expo_l);
			if ((errno == EDOM) || (errno == ERANGE))
				throw(exponenterror());
		}
		else if (significand < 0)
		{
			throw(unreal());
		}
		else
		{
			*this *= pow(significand, expo);
			if ((errno == EDOM) || (errno == ERANGE))
				throw(exponenterror());
		}
	}
	return *this;

}
/*
Polynomial operator /(const Polynomial & a, const double & b)
{
	Polynomial r = a;
	r /= b;
	return r;
}
*/
Polynomial Polynomial::operator /=(const double & b)
{
	// check for zero
	if (fabs(b) < DBL_EPSILON) throw(zerodivide());
	else *this *= (double)(1.0 / b);

	return *this;
}

/*
Polynomial operator /(const Polynomial & a, const Polynomial & b)
{
	Polynomial r = a;
	r /= b;
	return r;
}

Polynomial Polynomial::operator /=(const Polynomial & b)
{
	//check for degree zero in denominator
	if (b.degree != 0)
	{
		throw(dpolydegree());
		//Polynomial remainder = *this;
		//Polynomial quotient = 0;

	}
	else if (b.pmL == NULL)
	{
		throw(dpolyempty());
	}
	else if (fabs(b.pmL->mon->coefficient) < DBL_EPSILON)
	{
		throw(zerodivide());
	}
	else
	{
		*this *= (double)(1 / b.pmL->mon->coefficient);
	}


	return *this;
}


PolynomialQuotient::PolynomialQuotient(void)
{
	q = new Polynomial(0.0);
	r = new Polynomial(0.0);
}

PolynomialQuotient::PolynomialQuotient(const PolynomialQuotient & a)
{
	q = a.q;
	r = a.r;
}

PolynomialQuotient::~PolynomialQuotient(void)
{
	delete q;
	delete r;
	//q.~Polynomial();
	//r.~Polynomial();
}
*/
void Polynomial::divide(const Polynomial &b, pPolynomial r)
{ // this is operator /= with poly b as second operand and pointer to poly r as remainder
	
	Polynomial a = *this;
	*this = 0.0;
	*r = 0.0;
	if (fabs(b.pmL->mon->coefficient) < DBL_EPSILON)  // divide by zero
	{
		throw(zerodivide());
	}
	else if (b.pmL == NULL)  // empty poly in denominator
	{
		throw(dpolyempty());
	}
	else if (b.degree == 0)  //check for degree zero in denominator
	{
		*this = a / b.pmL->mon->coefficient;
	}
	else
	{ // b.degree is greater than zero
		pMonomialListType ML_N = a.pmL, ML_D = b.pmL, ML_r;
		MonomialQuotient mq;
		Monomial buff;
		bool found_one, is_zero;
		while (ML_N != NULL)
		{
			ML_D = b.pmL;
			ML_r = r->pmL;
			found_one = false;
			is_zero = false;
			while (ML_D != NULL)
			{
				buff = *(ML_N->mon);
				//possibly modifying buffered numerator monomial with remainder monomial: 
				while (ML_r != NULL)
				{
					if (*(ML_r->mon) == buff)  // this compares variables and exponents not coefficients
					{
						buff.coefficient += ML_r->mon->coefficient;
						if (fabs(buff.coefficient) < DBL_EPSILON)
						{
							is_zero = true;
							buff = 0.0;
						}
						*r -= *(ML_r->mon);
						ML_r = NULL;
					}
					else
					{
						ML_r = ML_r->pNextMon;
					}
				}
				mq.divide(buff, *(ML_D->mon));
				if ((is_zero) || (fabs(mq.GetQuotient().coefficient) > DBL_EPSILON))
				{
					found_one = true;
					ML_D = NULL;
				}
				else
				{
					ML_D = ML_D->pNextMon;
					if (ML_D != NULL)
					{
						if (ML_D->mon->degree < b.degree)
						{
							ML_D = NULL;
						}
					}
				}
			}
			if (found_one)
			{
				*this += mq.GetQuotient();
				*r += buff - (mq.GetQuotient() * b);
			}
			else
			{
				*r += mq.GetRemainder();
			}
		
			ML_N = ML_N->pNextMon;

		}
		if (r->degree >= b.degree)
		{
			*this = 0.0;
			*r = a;
		}
		//throw(dpolydegree());
		//Polynomial remainder = *this;
		//Polynomial quotient = 0;

	}

}

Glib::ustring Polynomial::print(void) const
{
	Glib::ustring r = "";
	if (error == NULL)
	{
		pMonomialListType p = pmL;
		while (p != NULL)
		{
			Glib::ustring m = p->mon->print(), m0 = m.substr(0, 1);
			assert (m0.bytes() == 1);
			char c0 = m0[0];
			if ((p != pmL) && (c0 != '-')) r += '+';
			r += m;
			p = p->pNextMon;
		}
	}
	else
	{
		r += error;
	}
	return r;
}
/*
void PolynomialQuotient::divide(const pPolynomial & a, const pPolynomial & b)
{
	
	if (fabs(b->pmL->mon->coefficient) < DBL_EPSILON)  // divide by zero
	{
		throw(zerodivide());
	}
	else if (b->pmL == NULL)  // empty poly in denominator
	{
		throw(dpolyempty());
	}
	else if (b->degree != 0)  //check for degree zero in denominator
	{
		if (a->degree == 0)
		{
			*q = 0.0;
			*r = *a;
			return;
		}
		pMonomialListType ML_N = a->pmL, ML_D = b->pmL, ML_r;
		MonomialQuotient mq;
		Monomial buff;
		bool found_one, is_zero;
		while (ML_N != NULL)
		{
			ML_D = b->pmL;
			ML_r = r->pmL;
			found_one = false;
			is_zero = false;
			while (ML_D != NULL)
			{
				buff = *(ML_N->mon);
				//possibly modifying buffered numerator monomial with remainder monomial: 
				while (ML_r != NULL)
				{
					if (*(ML_r->mon) == buff)  // this compares variables and exponents not coefficients
					{
						buff.coefficient += ML_r->mon->coefficient;
						if (fabs(buff.coefficient) < DBL_EPSILON)
						{
							is_zero = true;
							buff = 0.0;
						}
						*r -= *(ML_r->mon);
						ML_r = NULL;
					}
					else
					{
						ML_r = ML_r->pNextMon;
					}
				}
				mq.divide(buff, *(ML_D->mon));
				if ((is_zero) || (fabs(mq.GetQuotient().coefficient) > DBL_EPSILON))
				{
					found_one = true;
					ML_D = NULL;
				}
				else
				{
					ML_D = ML_D->pNextMon;
					if (ML_D != NULL)
					{
						if (ML_D->mon->degree < b->degree)
						{
							ML_D = NULL;
						}
					}
				}
			}
			if (found_one)
			{
				*q += mq.GetQuotient();
				*r += buff - (mq.GetQuotient() * *b);
			}
			else
			{
				*r += mq.GetRemainder();
			}
		
			ML_N = ML_N->pNextMon;

		}
		if (r->degree >= b->degree)
		{
			*q=0.0;
			*r = *a;
		}
		//throw(dpolydegree());
		//Polynomial remainder = *this;
		//Polynomial quotient = 0;

	}
	else  // denominator polynomial has one degree 0 monomial in it
	{
		*q = (double)(1.0 / b->pmL->mon->coefficient) * *a;
		*r = 0.0;
	}



}

Polynomial operator /(const Polynomial & a, const Polynomial & b)
{
	PolynomialQuotient pq;
	pq.divide(a, b);
	return pq.GetQuotient();
}

Polynomial operator %(const Polynomial & a, const Polynomial & b)
{
	PolynomialQuotient pq;
	pq.divide(a, b);
	return pq.GetRemainder();
}
*/