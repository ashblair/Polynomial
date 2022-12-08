#include "first.hpp"
#include "Variable.hpp"
#include "Monomial.hpp"
#include "PolyExceptions.hpp"

int Monomial::iMonCount = 0;

Glib::ustring MonomialTermType::print(void) const
{
	Glib::ustring r = v.print();
	if (exponent > 1)
	{
		std::string s = std::to_string(exponent);
		r += Variable::printExps(s);
	}
	return r;
}

Monomial::Monomial(void)
{
	degree = 0;
	pTerm = NULL;
	coefficient = 0;
	error = NULL;
	++iMonCount;
}

Monomial::Monomial(double c)
{
	degree = 0;
	pTerm = NULL;
	coefficient = c;
	error = NULL;
	++iMonCount;
}

Monomial::Monomial(const Variable & vM)
{
	degree = 1;
	coefficient = 1.0;
	error = NULL;
	pTerm = new MonomialTerm(vM);
	pTerm->exponent = 1;
	pTerm->pNextComponent = NULL;
	++iMonCount;
}


Monomial::Monomial(const Monomial & a)
{
	degree = a.degree;
	coefficient = a.coefficient;
	error = NULL;
	if (a.error != NULL)
	{
		size_t l = strlen(a.error) + 1;
		error = new char[l];
		memcpy(error, a.error, l);
	}
	pTerm = NULL;
	pMTType traverse = a.pTerm, trail = NULL;
	while (traverse != NULL)
	{
		pMTType p = new MonomialTerm(traverse->v);
		p->pNextComponent = NULL;
		p->exponent = traverse->exponent;
		if (trail == NULL) pTerm = p;
		else trail->pNextComponent = p;
		trail = p;
		traverse = traverse->pNextComponent;
	}
	++iMonCount;
}

Monomial::~Monomial(void)
{
	pMTType pDoom, pNext;

	pDoom = pTerm;
	while (pDoom != NULL)
	{
		pNext = pDoom->pNextComponent;
		delete pDoom;
		pDoom = pNext;
	}
	pTerm = NULL;
	if (error != NULL)
	{
		delete[] error; //to allocate-error = new char[N]; where N is enough for message
	}
	--iMonCount;
}

Monomial Monomial::operator =(const Monomial & m)
{
	this->~Monomial();
	this->coefficient = m.coefficient;
	this->degree = m.degree;

	// copying the error
	if (m.error == NULL) 
	{
		this->error = NULL;
	}
	else
	{
		size_t len = strlen(m.error) + 1; // for end /0
		this->error = new char[len];
		memcpy(this->error, m.error, len);
	}

	// copying the term
	this->pTerm = NULL;
	pMTType traverse = m.pTerm;
	pMTType trail = NULL;
	while (traverse != NULL)
	{
		pMTType p = new MonomialTerm(traverse->v);
		p->exponent = traverse->exponent;
		//Variable VTmp(*(traverse->v));
		//*(p->v) = VTmp;
		p->pNextComponent = NULL;
		if (trail == NULL) 
		{
			this->pTerm = p;
		}
		else
		{
			trail->pNextComponent = p;
		}
		trail = p;
		traverse = traverse->pNextComponent;

	}
	++iMonCount;
	return *this;
}

Monomial Monomial::operator *=(const double & a)
{
	coefficient *= a;
	return *this;

}

Monomial Monomial::operator*=(const Monomial & a)
{
	pMTType p = NULL;
	coefficient *= a.coefficient;
	if (fabs(this->coefficient) < DBL_EPSILON) // check for zero 
	{
		degree = 0;
		while (pTerm != NULL)
		{
			p = pTerm;
			pTerm = pTerm->pNextComponent;
			delete(p);
		}
		return *this;  
	}
	degree += a.degree;
	pMTType atraverse = a.pTerm, traverse = pTerm, trail = NULL;
	while (atraverse != NULL)
	{
		if (traverse == NULL)
		{
			p = new MonomialTerm(atraverse->v);
			p->pNextComponent = NULL;
			p->exponent = atraverse->exponent;
			if (trail == NULL) pTerm = p;
			else trail->pNextComponent = p;
			trail = p;
			atraverse = atraverse->pNextComponent;
		}
		else
		{
			if (atraverse->v == traverse->v)
			{
				traverse->exponent += atraverse->exponent;
				trail = traverse;
				traverse = traverse->pNextComponent;
				atraverse = atraverse->pNextComponent;
			}
			else if (atraverse->v < traverse->v)
			{
				p = new MonomialTerm(atraverse->v);
				p->pNextComponent = traverse;
				p->exponent = atraverse->exponent;
				if (trail == NULL) pTerm = p;
				else trail->pNextComponent = p;
				trail = p;
				atraverse = atraverse->pNextComponent;
			}
			else
			{
				trail = traverse;
				traverse = traverse->pNextComponent;
			}
		}

	}
	return *this;

}


void Monomial::ErrorEncountered(const char * msg)
{
	size_t len = strlen(msg) + 1;
	this->error = new char[len];
	memcpy(this->error, msg, len);
}

Glib::ustring Monomial::print(void)
{
	Glib::ustring r = "";
	if (error == NULL)
	{
		bool skipNum = fabs(fabs(coefficient) - 1.0) < 1e-7,
			addSign = skipNum && (coefficient < 0.0);
		if ((degree > 0) && addSign) r += "-"; 
		if ((degree == 0) || !skipNum) r += Glib::ustring::format(std::defaultfloat, coefficient);
		pMTType p = pTerm;
		while (p != NULL)
		{
			r += p->print();
			p = p->pNextComponent;
		}
	}
	else
	{
		r += error;
	}
	return r;
}

bool Monomial::IsEQ(const Monomial & b) const
{
	pMTType atraverse = pTerm, btraverse = b.pTerm;
	bool IsEqual = true;
	// enter loop if there remain terms in either a or b
	while ((atraverse != NULL) || (btraverse != NULL))
	{
		// if either a or b has no remaining terms than monomials are not equal
		IsEqual = !((atraverse == NULL) || (btraverse == NULL));
		if (IsEqual) 
		{
			IsEqual = (atraverse->v == btraverse->v) && 
				((atraverse->exponent) == (btraverse->exponent));
		}
		// provide short circuit to exit loop or get next terms to continue
		if (!IsEqual)
		{
			atraverse = NULL;
			btraverse = NULL;
		}
		else
		{
			atraverse = atraverse ->pNextComponent;
			btraverse = btraverse ->pNextComponent;
		}
	}
	return IsEqual;
}

bool Monomial::IsLT(const Monomial & b) const
{
	if (IsEQ(b)) return false;
	if (degree == b.degree)
	{
		pMTType atraverse = pTerm, btraverse = b.pTerm;
		bool IsLess = false;
		// enter loop if there remain terms in either a or b
		while ((atraverse != NULL) || (btraverse != NULL))
		{
			if ((atraverse->v == btraverse->v) && 
					((atraverse->exponent) == (btraverse->exponent)))
			{
				atraverse = atraverse->pNextComponent;
				btraverse = btraverse->pNextComponent;
			}
			else
			{
				if (atraverse->v == btraverse->v)
				{
					// higher exponents will go first
					IsLess = (atraverse->exponent) > (btraverse->exponent);
					atraverse = NULL;
					btraverse = NULL;
				}
				else
				{
					IsLess = (atraverse->v) < (btraverse->v);
					atraverse = NULL;
					btraverse = NULL;
				}
			}
		}
		return IsLess;
	}
	else return (degree > b.degree); // should put greater degree Monomials first
}

MonomialQuotient::MonomialQuotient(void)
{
	q = 1.0;
	r = 1.0;
}

MonomialQuotient::MonomialQuotient(const MonomialQuotient & a)
{
	q = a.q;
	r = a.r;
}

MonomialQuotient::~MonomialQuotient(void)
{
	//q.~Monomial();
	//r.~Monomial();
}

void MonomialQuotient::divide(const Monomial & a, const Monomial & b)
{// this is a/b = qb + r where q is the quotient and r is the remainder
	if (b.coefficient < DBL_EPSILON) throw(zerodivide());
	
	//PART 1:
	q = 0.0;
	r = a;
	bool V_In = false, D_OK = false;
	pMTType MTT_N = a.pTerm, MTT_D = b.pTerm, p = NULL, trail = NULL;
	while (MTT_D != NULL) // looping through terms of the denomenator
	{// checking to see if there's a match in the numerator
		V_In = false;
		D_OK = false;
		MTT_N = a.pTerm;
		while (MTT_N != NULL)
		{
			if ((MTT_N->v) == (MTT_D->v)) // variables match
			{
				V_In = true;
				D_OK = (MTT_D->exponent <= MTT_N->exponent);
				MTT_N = NULL;
			}
			else
			{
				MTT_N = MTT_N->pNextComponent;
			}
		}
		if ((V_In) && (D_OK))
		{
			MTT_D = MTT_D->pNextComponent;
		}
		else
		{
			MTT_D = NULL;
		}
	}
	// conditions that make q = 0 & r = numerator:
	if ((!V_In) || (!D_OK) || (fabs(a.coefficient) < DBL_EPSILON)) 
	{
		return;
	}

	//PART 2:
	// At this point we know b divides a:
	q = a; // quotient initialized to numerator
	q.coefficient = a.coefficient / b.coefficient;  //neither is 0
	r = 0.0;  //remainder will be 0
	MTT_D = b.pTerm; // denomenator term
	while (MTT_D != NULL)
	{
		p = q.pTerm;
		trail = NULL;
		while (p != NULL)
		{
			if ((p->v) == (MTT_D->v))
			{
				p->exponent -= MTT_D->exponent;
				q.degree -= MTT_D->exponent;
				if (p->exponent == 0)
				{
					if (trail == NULL) 
					{
						q.pTerm = p->pNextComponent;
					}
					else
					{
						trail->pNextComponent = p->pNextComponent;
					}
					delete p;
				}
				p = NULL;  //short circuit loop after found variable process
			}
			else
			{
				trail = p;
				p = p->pNextComponent;
			}
		}
		MTT_D = MTT_D->pNextComponent;
	}
}
