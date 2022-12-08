#include "first.hpp"
#include "Variable.hpp"
#include "PolyExceptions.hpp"

//static initializations:
const Glib::ustring Variable::subs = "₀₁₂₃₄₅₆₇₈₉", Variable::exps = "⁰¹²³⁴⁵⁶⁷⁸⁹"; 

// static function:
bool Variable::IsVariable(char c)
{
	//returns true iff c is alphabetic
	if (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')))
		return true;
	else
		return false;
}

// static function:
Glib::ustring Variable::printSubs(const std::string &s)
{
	Glib::ustring r = "";
	for (size_t i = 0; i < s.size(); ++i)
	{
		char c = s[i];
		assert (isdigit(c));
		r += subs[c - '0'];
	}
	return r;
}

// static function:
Glib::ustring Variable::printExps(const std::string &s)
{
	Glib::ustring r = "";
	for (size_t i = 0; i < s.size(); ++i)
	{
		char c = s[i];
		assert (isdigit(c));
		r += exps[c - '0'];
	}
	return r;
	
}

//static function:
double Variable::GetDbl(char * szReal, int & length)
{  
	double r = 0;
	char * p = szReal;

	r = strtod(szReal, &p);
	length = p - szReal;
	if (errno == ERANGE)
	{
		if (r == HUGE_VAL) throw(overflow());
		else throw(underflow());
	}
	return r;

}

long Variable::GetLng(char * w_in, int & length)
{
	int r = 0;
	char * p = w_in;

	r = strtol(w_in, &p, 10);
	length = p - w_in;
	if (errno == ERANGE)
	{
		if (r == LONG_MAX) throw(overflow());
		else throw(underflow());
	}
	return r;
}


Variable::Variable(void)
{
	varV = '?';
	varSub = -1;
}
Variable::Variable(const Variable & vCpy)
{
	varV = vCpy.varV;
	varSub = vCpy.varSub;
}
Variable::Variable(char v)
{
	varV = v;
	varSub = -1;
}

Variable::Variable(char v, long s)
{
	varV = v;
	varSub = s;
}

Variable::Variable(char *& p)
{
	char s;
	int i;

	varSub = -1;
	char c = *p;
	assert (IsVariable(c));

	varV = c;
	s = *(++p);
	if (isdigit(s))
	{
		varSub = GetLng(p, i);
		p += i;
	}

}


Variable::~Variable(void)
{
	varV = ' ';
	varSub = -2;
}

Variable & Variable::operator =(Variable & v) 
{
	varV = v.varV;
	varSub = v.varSub;
	return *this;
}

bool Variable::IsEQ(const Variable & b) const 
{
	return ((varSub == b.varSub) && (varV == b.varV));
}

bool Variable::IsLT(const Variable & b) const 
{
	if (varV == b.varV) 
	{
		return (varSub < b.varSub);
	}
	else
	{
		return (varV < b.varV);
	}
}

Glib::ustring Variable::print(void) const
{
	Glib::ustring r = "";
	r += varV;
	if (varSub == -1) return r;
	std::string s = std::to_string(varSub);
	r += printSubs(s);
	return r;

}

VariableLU::VariableLU(void)
{}

VariableLU::~VariableLU(void)
{
	vLU.clear();
}

void VariableLU::DeleteVariable(Variable & v)
{
	std::map<Variable, int>::iterator vit = vLU.find(v);
	assert (vit != vLU.end());
	vLU.erase(vit);
}

void VariableLU::AddAllVariables(const std::string &s)
{
	char * top = const_cast<char *>(s.c_str()), * p = top;
	while (*p != '\0')
	{
		char c = *p, *t = p;
		if (Variable::IsVariable(c)) 
		{
			Variable v = Variable(p); // constructor advances pointer p
			std::map<Variable, int>::iterator vit = vLU.find(v);
			if (vit == vLU.end()) vLU[v] = t - top; // index of first occurrence
		}
		else ++p;
	}
}

const Variable & VariableLU::RetrieveVariable(char vc, long ss)
{
	Variable v = Variable(vc, ss);
	std::map<Variable, int>::iterator vit = vLU.find(v);
	assert (vit != vLU.end());
	return vit->first;
}

Glib::ustring VariableLU::print(void) const 
{
	Glib::ustring r = "VLU: ";
	std::map<Variable, int>::const_iterator vit = vLU.begin();
	while (vit != vLU.end())
	{
		Variable v = vit->first;
		r += v.print() + " ";
		++vit;
	}
	r += "\n";
	return r;
}



/*
VariableList::VariableList(void)
{
	vList = NULL;
}

void VariableList::DeleteVariable(Variable & d)
{
	pvarListType traverse = vList;
	pvarListType trailer = NULL;
	bool fNotFound = true;
	while (fNotFound)
	{
		if (traverse == NULL)
		{
			fNotFound = false;
		}
		else
		{
			if (*(traverse->var) == d)
			{
				if (trailer == NULL)
				{
					vList = traverse->pNextVar;
				}
				else
				{
					trailer->pNextVar = traverse->pNextVar;
				}
				traverse->var->~Variable();
				delete traverse->var;
				delete traverse;
				fNotFound = false;
			}
			else
			{
				trailer = traverse;
				traverse = traverse->pNextVar;
			}
		}
	}
}

void VariableList::AddVariable(const char * szPolyString)
// this method will search through szPolyString and add all variables
// that are in the string and not yet in the VariableList
// will return NULL unless there's an error 
{
	while (*szPolyString != '\0')
	{
		if (Variable::IsVariable(*szPolyString))
		{
			char v = *szPolyString++;
			int sub = -1;
			if (isdigit(*szPolyString))
			{
				sub = 0;
				while ((sub < (INT_MAX / 10)) && isdigit(*szPolyString))
				{
					sub = 10 * sub + (*szPolyString++ - '0');
				}
				if (sub > (INT_MAX / 10))
				{
					throw(overflow());
				}
			}
			Variable * pVar = new Variable(v, sub);
			pvarListType traverse = vList, trail = NULL;
			bool listAction = false;
			while (traverse != NULL)
			{
				if (*pVar == *(traverse->var))
				{
					//pVar->~Variable();
					delete pVar;
					listAction = true;
					traverse = NULL;
				}
				else if (*pVar < *(traverse->var))
				{
					pvarListType insertion = (pvarListType) ::operator new (sizeof(varListType));
					insertion->pNextVar = traverse;
					insertion->var = pVar;
					if (trail == NULL)
					{
						vList = insertion;
					}
					else
					{
						trail->pNextVar = insertion;
					}
					listAction = true;
					traverse = NULL;
				}
				else
				{
					trail = traverse;
					traverse = traverse->pNextVar;
				}

			}
			if (!listAction)
			{
				pvarListType insertion = (pvarListType) ::operator new (sizeof(varListType));
				insertion->pNextVar = NULL;
				insertion->var = pVar;
				if (trail == NULL)
				{
					vList = insertion;
				}
				else
				{
					trail->pNextVar = insertion;
				}

			}
		}
		else
		{
			++szPolyString;
		}

	}

}


// this method will retrieve a single reference for each variable in a polynomial
// it handles insertions into the vaiable list when a variable isn't already there
Variable & VariableList::RetrieveVariable(char vc, int ss)
	//the char vc needs to be a valid variable character
	//the int ss is the subscript and is an optional input
{
	pvarListType traverse = vList;
	pvarListType trailer = NULL;
	pvarListType insertion = NULL;
	bool fLoop;
	// 1st return the variable if it's there
	while (traverse != NULL)
	{
		if ((traverse->var->varSub == ss) && (traverse->var->varV == vc)) 
			return *(traverse->var);
		traverse = traverse->pNextVar;
	}
	// At this point the variable is not in the list so we'll add it:
	Variable * v = new Variable(vc, ss); // we'll return this when done
	traverse = vList;  // reset traverse so we can traverse again
	insertion = (pvarListType) ::operator new (sizeof(varListType));
	insertion->var = v;
	insertion->pNextVar = NULL;
	if (traverse == NULL)
	{
		vList = insertion;
		return *v;
	}
	if (*v < *(traverse->var))
	{
		insertion->pNextVar = traverse;
		vList = insertion;
		return *v;
	}
	// this next loop will set trailer & traverse so that v between them
	// will maintain an ordered list
	do
	{
		trailer = traverse;
		traverse = traverse->pNextVar;
		fLoop = (traverse != NULL);
		if (fLoop) fLoop = (*(traverse->var) < *v);
	} while (fLoop);

	if (traverse == NULL)
	{
		trailer->pNextVar = insertion;
		return *v;
	}
	trailer->pNextVar = insertion;
	insertion->pNextVar = traverse;
	return *v;
}

VariableList & VariableList::operator= (const VariableList & varL)
{
	if (vList != NULL) this->~VariableList();
	vList = NULL;
	pvarListType traverse = varL.vList;
	pvarListType add_node = NULL;
	while (traverse != NULL)
	{
		pvarListType insertion = (pvarListType)::operator new (sizeof(varListType));
		insertion->var = new Variable(*(traverse->var));
		insertion->pNextVar = NULL;
		if (add_node == NULL)
		{
			add_node = insertion;
			vList = insertion;
		}
		else
		{
			add_node->pNextVar = insertion;
			add_node = insertion;
		}
		traverse = traverse->pNextVar;
	}
	return *this;
}

void VariableList::Merge(const VariableList & mvlst)
{
	pvarListType traverse1 = vList;
	pvarListType previous1 = NULL;
	pvarListType traverse2 = mvlst.vList;
	pvarListType insertion;
	while (traverse2 != NULL)
	{
		if (traverse1 == NULL)
		{
			insertion = (pvarListType)::operator new(sizeof(varListType));
			insertion->var = new Variable(*(traverse2->var));
			insertion->pNextVar = NULL;
			if (previous1 == NULL)
			{
				vList = insertion;
				previous1 = insertion;
			}
			else
			{
				previous1->pNextVar = insertion;
				previous1 = insertion;
			}
			traverse2 = traverse2->pNextVar;
		}
		else
		{
			if (traverse1->var == traverse2->var)
			{
				previous1 = traverse1;
				traverse1 = traverse1->pNextVar;
				traverse2 = traverse2->pNextVar;
			}
			else if (traverse2->var < traverse1->var)
			{
				insertion = (pvarListType)::operator new(sizeof(varListType));
				insertion->var = new Variable(*(traverse2->var));
				insertion->pNextVar = traverse1;
				if (previous1 == NULL)
				{
					vList = insertion;
					previous1 = insertion;
				}
				else
				{
					previous1->pNextVar = insertion;
					previous1 = insertion;
				}
				traverse2 = traverse2->pNextVar;
			}
			else
			{
				previous1 = traverse1;
				traverse1 = traverse1->pNextVar;
			}
		}
	}
}

VariableList::~VariableList(void)
{
	while (vList != NULL)
	{
		pvarListType leader = vList->pNextVar;
		vList->var->~Variable();
		delete vList->var;
		delete vList;
		vList = leader;
	}
}
*/
