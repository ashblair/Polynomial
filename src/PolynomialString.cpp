#include "first.hpp"
#include "Variable.hpp"
#include "Monomial.hpp"
#include "Polynomial.hpp"
#include "PolynomialString.hpp"
#include "PolyExceptions.hpp"


PolynomialString::PolynomialString()
{
	polystring = "";
	outputpolystring = "";
	inputpolystring = "";
	pPLY = NULL;
}


PolynomialString::PolynomialString(const Glib::ustring & pString)
{ 
	try
	{
		outputpolystring = "";
		inputpolystring = pString;
		polystring = makeASC(inputpolystring);
		pPLY = NULL;
		Polynomial::vl.AddAllVariables(polystring); // setting up the variable list
		GetFromString(); 
		outputpolystring += pPLY->print();
		//PrintPoly();
	}
	catch (std::bad_alloc & ba)//(std::bad_alloc& ba)
	{
		pPLY->error = const_cast<char *>(ba.what());
	}
	catch (polyexception & e)
	{
		pPLY->error = const_cast<char *>(e.what());
	}
}

PolynomialString::PolynomialString(const PolynomialString & a)
{ 
	//if (richoutputpolystring != NULL) delete [] richoutputpolystring;
	/*
	richoutputpolystring = NULL;
	if (a.richoutputpolystring != NULL)
	{
		size_t l = strlen(a.richoutputpolystring) + 1;
		richoutputpolystring = new char [l];
		memcpy(richoutputpolystring, a.richoutputpolystring, l);
	}
	//if (outputpolystring != NULL) delete [] outputpolystring;
	outputpolystring = NULL;
	if (a.outputpolystring != NULL)
	{
		size_t l = strlen(a.outputpolystring) + 1;
		outputpolystring = new char [l];
		memcpy(outputpolystring, a.outputpolystring, l);
	}
	//if (polystring != NULL) delete [] polystring;
	polystring = NULL;
	if (a.polystring != NULL)
	{
		size_t l = strlen(a.polystring) + 1;
		polystring = new char [l];
		memcpy(polystring, a.polystring, l);
	}
	*/
	outputpolystring = a.outputpolystring;
	inputpolystring = a.inputpolystring;
	polystring = a.polystring;
	pPLY = a.pPLY;
}

PolynomialString::PolynomialString(const pPolynomial & a)
{
	try
	{
		pPLY = a;
		outputpolystring = pPLY->print();
		polystring = makeASC(outputpolystring);
		inputpolystring = polystring;

	}
	catch (polyexception & e)
	{
		pPLY->error = const_cast<char *>(e.what());
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	

}

PolynomialString::~PolynomialString()
{
	/*
	delete[] polystring;
	polystring = NULL;
	delete[] outputpolystring;
	outputpolystring = NULL;
	delete[] richoutputpolystring;
	richoutputpolystring = NULL;
	*/

	if (pPLY != NULL) delete pPLY;
	pPLY = NULL;

}

Glib::ustring PolynomialString::GetOutputPolyString(void) {return outputpolystring;}
Glib::ustring PolynomialString::GetInputPolyString(void) {return inputpolystring;}
	//char * GetOutputPolyString1(void) {return outputpolystring;};
	//char * GetOutputPolyString2(void) {return richoutputpolystring;};
char * PolynomialString::GetOutputError(void) {return pPLY->error;};
pPolynomial PolynomialString::GetPolynomial(void) {return pPLY;};

std::string PolynomialString::makeASC(const Glib::ustring & gStr)
{
	std::string r = "";
	bool expFound = false;
	for (size_t i = 0; i < gStr.size(); ++i)
	{
		Glib::ustring uc = gStr.substr(i, 1);
		if (uc.bytes() == 1) 
		{
			r += uc; 
			expFound = false;
		}
		else 
		{
			size_t subsPos = Variable::subs.find(uc);
			if (subsPos != Glib::ustring::npos)
			{
				r += static_cast<char>('0' + subsPos);
				expFound = false;
			}
			else
			{
				size_t expsPos = Variable::exps.find(uc);
				if (expsPos != Glib::ustring::npos)
				{
					if (!expFound) r += '^';
					r += static_cast<char>('0' + expsPos);
					expFound = true;
				}
				else throw(invalid_char());
			}
		}
	}
	return r;
}

Glib::ustring PolynomialString::makeUST(const std::string & s)
{
	Glib::ustring r = "";
	bool expFound = false, varFound = false;
	char * buff = const_cast<char *>(s.c_str());
	for (size_t i = 0; i < s.size(); ++i)
	{
		char c = s[i];
		if (c == '^') 
		{ // exponent:
			expFound = true; varFound = false;
		}
		else if (Variable::IsVariable(c))
		{ // variable:
			varFound = true; expFound = false;
			r += c;
		}
		else if ((isdigit(c)) || (c == '.'))
		{ // numerics can be complicated with internal signs (+ or -), dots and letters like x, X, e or E.
		  // they depend on calls to functions GetNumber and GetWholeNumber.
		  // they may need to go subscript or exponent (superscript).
			int ld = 0, ll = 0;
			double dBuff = Variable::GetDbl(&buff[i], ld);
			long lBuff = c == '.'? 0: Variable::GetLng(&buff[i], ll);
			if (c == '.')
			{
				if (expFound) throw(nonwholeexponent());
				if (varFound) 
				{
					r += "*";
					varFound = false;
				}
				r += s.substr(i, ld);
				i += ld - 1;
			}
			else
			{
				if (expFound || varFound) 
				{
					size_t j = i;
					do
					{
						int actualNum = c - '0';
						r += expFound? Variable::exps[actualNum]: Variable::subs[actualNum];
						c = s[++i];
					} while ((i - j) < ll);
					--i;
				}
				else 
				{
					r += s.substr(i, ld);
					i += ld - 1;
				}
			}
		}
		else
		{ // other:
			varFound = false; expFound = false;
			r += c;
		}

	}
	return r;

}

Glib::ustring PolynomialString::makeUST(const Polynomial &p)
{
	return p.print();
}

size_t PolynomialString::GetEndParentheses(const size_t idx, const std::string & s)
{
	size_t edx = idx, sSz = s.size();
	assert (s[edx] == '(');
	int i = 0;
	do
	{
		if (s[edx] == '(') ++i;
		if (s[edx] == ')') --i;
		++edx;
	} while ((i > 0) && (edx < sSz));
	if ((i != 0) && (edx == sSz)) 
		throw(parenthesesunmatched());
	return --edx; 

}
/*
char * PolynomialString::GetEndParentheses(char * bp_in)
{
	char * ep = bp_in;
	//if (*ep != '(') 
	//{
	//	throw(incorrectGetEndParentheses());
	//}
	assert (*ep == '(');
	int i = 0;
	do
	{
		if (*ep == '(') ++i;
		if (*ep == ')') --i;
		++ep;
	} while ((i > 0) && (*ep != '\0'));
	if ((i != 0) && (*ep == '\0')) 
		throw(parenthesesunmatched());
	return ep;  // ep points one char past ')' in string
}
*/
void  PolynomialString::CheckParentheses(void)
{
	if (polystring.size() == 0) return; //empty string
	char * ep = const_cast<char *>(polystring.c_str());
	int i = 0;
	bool BelowZero = false;
	//if (ep == NULL) 
	//{
	//	throw(incorrectCheckParentheses());
	//}
	//else if (strlen(ep) == 0)
	//{
	//	throw(CheckParenthesesEmpty());
	//}
	//else
	//{
		do
		{
			if (*ep == '(') ++i;
			if (*ep == ')') --i;
			++ep;
			BelowZero = BelowZero || (i < 0);
		} while (*ep != '\0');
		if ((BelowZero) || (i != 0)) 
			throw(parenthesesunmatched());
	//}
}

void PolynomialString::RemoveSuperfluousParentheses(void)
// basically if there are two sets of parentheses right next to each other
// and the 1st left parenthesis )) closes the 2nd right parenthesis ((
// one of each can be removed by this method
{
	size_t pSz = polystring.size(), iL = 0, iR = 0, jL = 0, jR = 0;
	if (pSz == 0) return;
	while (iL < pSz)
	{
		if (polystring[iL] == '(')
		{
			iR = GetEndParentheses(iL, polystring);
			jL = iL + 1;
			while (polystring[jL] == '(')
			{
				jR = GetEndParentheses(jL, polystring);
				if ((jR + 1) == iR)
				{
					polystring.erase(jR, 1);
					polystring.erase(jL, 1);
					pSz -= 2;
					iR -= 2;
				}
			}
		}
		++iL;
	}

}

void PolynomialString::RemoveSpaces(void)
{
	size_t pSz = polystring.size(), idx = 0;
	if (pSz == 0) return;
	while (idx < pSz)
	{
		char c = polystring[idx];
		bool iswhitespace = (c == ' ') || (c == '\n') || (c == '\t') || (c == '\r') || (c == '\v');
		if (iswhitespace) 
		{
			polystring.erase(idx, 1);
			--pSz;
		}
		else ++idx;
	}

}

bool PolynomialString::IsOperator(char o) 
{
	return ((o == '+') || (o == '-') || (o == '*') || (o == '/') || (o == '%') ||(o == '^'));
}

void PolynomialString::CheckOperators(void)
{
	size_t pSz = polystring.size(), idx = 0;
	if (pSz == 0) return;
	while (idx < pSz)
	{
		char c = polystring[idx];
		if (IsOperator(c))
		{
			if (idx == (pSz - 1)) throw(opsatend());
			char n = polystring[idx + 1];
			if (IsOperator(n)) throw(adjacentops());
			if (n == ')') throw(opsendingq());
			bool b_quan = (idx == 0) || (polystring[idx - 1] == '(');
			if (b_quan && ((c == '*') || (c == '/') || (c == '%') || (c == '^'))) throw(opsbeginningq());
		}
		++idx;
	}
}

void PolynomialString::CheckCharacters(void)
{
	size_t pSz = polystring.size(), idx = 0;
	if (pSz == 0) return;
	while (idx < pSz)
	{
		char c = polystring[idx];
		bool fGoodChar = (isalnum(c) || IsOperator(c) || (c == '.')
			|| (c == '(') || (c == ')'));
		if (!fGoodChar) throw(invalid_char());
		++idx;
	}
}

void PolynomialString::GetFromString()
{
	RemoveSpaces();
	if (polystring.begin() == polystring.end()) 
		throw(improperGetFromString());
	CheckParentheses();
	RemoveSuperfluousParentheses();
	CheckOperators();
	CheckCharacters();
	pUnitTreeNode pTree = NULL;
	pTree = BuildUnitTree(polystring);
	pPLY = GetPoly(pTree);
	DeleteTree(pTree);
}

int PolynomialString::GetPrecedence(char o)
{
	return (o == '^') ? 2 : ((o == '*') || (o == '/') || (o == '%')) ? 1 : 
		((o == '+') || (o == '-')) ? 0 : -1;
}

std::string PolynomialString::GetUnit(const std::string & s, size_t & idx)
{
	// returns string of number variable quantity or operation pointed at by szTxtIn
	//char * r = NULL, * r_e = NULL;
	//size_t l = 0;//, b_offset = 0;

	std::string r = "";
	size_t sSz = s.size(), l = 0, x = 0;
	if (sSz == 0) return r;
	char c = s[idx];

	if (c == '(')
	{
		x = GetEndParentheses(idx, s);
	}
	else if (IsOperator(c))
	{
		x = idx;
	}
	else if ((c == '.') || isdigit(c))
	{
		x = idx;
		char * cBuff = const_cast<char *>(s.c_str());
		cBuff += idx;
		int i = 0;
		double dBuff = Variable::GetDbl(cBuff, i);
		x += i - 1;
	}
	else if (Variable::IsVariable(c))
	{
		x = idx;
		// check for a subscript immediately following the variable letter:
		char * cBuff = const_cast<char *>(s.c_str());
		cBuff += idx + 1;
		int i = 0;
		if (isdigit(*cBuff))
		{
			long lBuff = Variable::GetLng(cBuff, i);
			x += i;
		}
	}
	else
	{
		throw(GetUnitError());
	}
	
	l = x - idx + 1;
	r = s.substr(idx, l);
	idx += l;
	return r;
}

pUnitTreeNode PolynomialString::BuildUnitTree(const std::string & s)
{
	pUnitTreeNode pTop = NULL, pCursor = NULL;
	size_t l = 0, sSz = s.size(), idx = 0;
	if (sSz == 0) return NULL;
	//char * p = szTxtIn;
	// c_u: current unit, l_u: last unit i.e. q, n, v, o
	char c = s[0], c_u = '\0', l_u = '\0';
	while (idx < sSz)
	{
		c_u = (c == '(') ? 'q': Variable::IsVariable(c) ? 'v' : ((c == '.') || isdigit(c)) ? 'n' : 
			IsOperator(c) ? 'o' : '\0';
		if (c_u == '\0') return NULL;
		bool OnImpliedOp = ((idx > 0) && ((l_u != 'o') && (c_u != 'o')));
		if (OnImpliedOp) 
		{
			c_u = 'o';
			c = '*';
		}
		pUnitTreeNode pInsert = new UnitTreeNode;
		if (pTop == NULL) pTop = pInsert;
		pInsert->up = pCursor;
		pCursor = pInsert;

		switch (c_u) {
		case 'q':
			pInsert->id = 'q';
			pInsert->right = NULL; //unary unit left only
			pInsert->txtIn = GetUnit(s, idx); //GetUnit increases idx before returning
			// removing the parentheses:
			l = pInsert->txtIn.size();
			pInsert->txtIn.erase(l - 1, 1);
			pInsert->txtIn.erase(0, 1);
			
			pInsert->left = BuildUnitTree(pInsert->txtIn);
			pInsert->left->up = pInsert;
			//p += 2 + l; // advancing our pointer
			c = s[idx]; // setting our control character
			l_u = 'q'; // setting the last unit
			break;
		case 'v':
			pInsert->id = 'v';
			pInsert->left = NULL;  //tree has an end here
			pInsert->right = NULL;
			pInsert->txtIn = GetUnit(s, idx);
			//p += strlen(pInsert->szEnteredText);
			c = s[idx];
			l_u = 'v';
			break;
		case 'n':
			pInsert->id = 'n';
			pInsert->left = NULL;  //tree has an end here
			pInsert->right = NULL;
			pInsert->txtIn = GetUnit(s, idx);
			//p += strlen(pInsert->szEnteredText);
			c = s[idx];
			l_u = 'n';
			break;
		case 'o':
			pInsert->id = c;
			pUnitTreeNode pPrev = pInsert->up, pStemPoint = NULL;
			if (pPrev != NULL)
			{
				int c_p = GetPrecedence(c);
				if (c_p > 0)
				{
					pStemPoint = pPrev->up;
					while (pStemPoint != NULL)
					{
						if (c_p > GetPrecedence(pStemPoint->id)) break;
						pStemPoint = pStemPoint->up;
					}

				}
			}
			pInsert->up = pStemPoint;
			if (pPrev == NULL)   // iff + or - begins quantity
			{
				pUnitTreeNode pImplied = new UnitTreeNode;

				pInsert->up = NULL;
				pImplied->up = pInsert;
				pInsert->left = pImplied;
				pImplied->left = NULL;
				pImplied->right = NULL;
				pImplied->id = 'n';
				pImplied->txtIn = "\0\0";
				//memcpy(pImplied->szEnteredText, "0\0", 2);
				pTop = pInsert;
			}
			else 
			{
				if (pStemPoint != NULL) // pointing at lower precedence op
				{ // replace right branch
					pStemPoint->right->up = pInsert;
					pInsert->left = pStemPoint->right;
					pStemPoint->right = pInsert;
					pInsert->up = pStemPoint;
				}
				else //Stem is NULL
				{
					pInsert->left = pTop;
					pInsert->left->up = pInsert;
					pInsert->up = NULL;
				}
			}
			if (pInsert->left == pTop)
			{
				pTop = pInsert;
			}
			pInsert->txtIn = "";
			if (!OnImpliedOp) ++idx; 
			std::string strRight = GetUnit(s, idx);
			pInsert->right = BuildUnitTree(strRight);
			//p += strlen(szRight);
			pInsert->right->up = pInsert;
			pCursor = pInsert->right;
			l_u = pCursor->id; // this will not be an op so: q, v or n
			c = s[idx];
			break;
		}
	}
	return pTop;
}

pPolynomial PolynomialString::GetPoly(pUnitTreeNode pNode)
{
	char cTypeNode = pNode->id, * txt = const_cast<char *>(pNode->txtIn.c_str());
	int l = 0;

	switch (cTypeNode)
	{
	case 'n': 
		return new Polynomial(Monomial(Variable::GetDbl(txt, l)));
		break;
	case 'v':
		if (pNode->txtIn.size() == 1) 
			return new Polynomial(Monomial(Polynomial::vl.
				RetrieveVariable(pNode->txtIn[0])));
		else
			return new Polynomial(Monomial(Polynomial::vl.
				RetrieveVariable(pNode->txtIn[0], 
				Variable::GetLng(txt + 1, l))));
		break;
	case 'q':
		return GetPoly(pNode->left);
		break;
	case '+': {pPolynomial lp = GetPoly(pNode->left), rp = GetPoly(pNode->right);(*lp)+=(*rp);delete rp;return lp;}break;
	case '-': {pPolynomial lp = GetPoly(pNode->left), rp = GetPoly(pNode->right);(*lp)-=(*rp);delete rp;return lp;}break;
	case '*': {pPolynomial lp = GetPoly(pNode->left), rp = GetPoly(pNode->right);(*lp)*=(*rp);delete rp;return lp;}break; //return GetPoly(pNode->left) * GetPoly(pNode->right); break;
	case '/': {pPolynomial lp = GetPoly(pNode->left), rp = GetPoly(pNode->right);Polynomial p=(*lp)/(*rp);delete lp;delete rp;return new Polynomial(p);}break; //return GetPoly(pNode->left) / GetPoly(pNode->right); break;
	case '%': {pPolynomial lp = GetPoly(pNode->left), rp = GetPoly(pNode->right);Polynomial p=(*lp)%(*rp);delete lp;delete rp;return new Polynomial(p);}break; //return GetPoly(pNode->left) % GetPoly(pNode->right); break;
	case '^': {pPolynomial lp = GetPoly(pNode->left), rp = GetPoly(pNode->right);Polynomial p=(*lp)^(*rp);delete lp;delete rp;return new Polynomial(p);}break; // return GetPoly(pNode->left) ^ GetPoly(pNode->right); break;
	default: 
		throw(GetPolyError()); 
		break;
	}
}

void PolynomialString::DeleteTree(pUnitTreeNode pNode)
{
	if (pNode != NULL)
	{
		if (pNode->left != NULL) DeleteTree(pNode->left);
		if (pNode->right != NULL) DeleteTree(pNode->right);
		pNode->txtIn.clear();
		delete (pNode);
	}
}
/*
void PolynomialString::PrintPoly(void)
{
	if (outputpolystring != NULL)
	{
		delete [] outputpolystring;
		outputpolystring = NULL;
	}
	if (richoutputpolystring != NULL)
	{
		delete [] richoutputpolystring;
		richoutputpolystring = NULL;
	}
	char szRichEditProlog1[] = "{\\rtf1\\ansi\\deff0{\\fonttbl{\\f0 Times New Roman;}";
	char szRichEditProlog2[] = "{\\colortbl\\red75\\green0\\blue0;\\red75\\green75\\blue255;\\red50\\green200\\blue50;}}";
	char szRichEditDF1[] = "\\deflang1033\\widowctrl {\\pard\\plain\\fs30\\cf0 ";
	char szRichEditSuffix[] = "\\par}}";
	char Sub[] = "\\sub\\fs16\\cf2 "; int iSub = strlen(Sub); 
	char Sup[] = "\\super\\fs20\\cf1 "; int iSup = strlen(Sup); 
	char Norm[] = "\\plain\\fs30\\cf0 "; int iNorm = strlen(Norm);
	//char C0[] = "\\cf0 "; int iC0 = strlen(C0);
	//char C1[] = "\\cf1 "; int iC1 = strlen(C1);
	//char C2[] = "\\cf2 "; int iC2 = strlen(C2);
	size_t rwidth=strlen(szRichEditProlog1)+strlen(szRichEditProlog2)+
		strlen(szRichEditDF1)+strlen(szRichEditSuffix) + 1;

	if (pPLY->error != NULL) 
	{
		size_t l = strlen(pPLY->error) + 1;
		outputpolystring = new char[l];
		memcpy(outputpolystring, pPLY->error, l);
		l += rwidth - 1;
		richoutputpolystring = new char[l];
		richoutputpolystring[l - 1] = '\0';
		l = strlen(szRichEditProlog1);
		memcpy(richoutputpolystring, szRichEditProlog1, l);
		rwidth = l;
		l = strlen(szRichEditProlog2);
		memcpy(richoutputpolystring + rwidth, szRichEditProlog2, l);
		rwidth += l;
		l = strlen(szRichEditDF1);
		memcpy(richoutputpolystring + rwidth, szRichEditDF1, l);
		rwidth += l;
		l = strlen(pPLY->error);
		memcpy(richoutputpolystring + rwidth, pPLY->error, l);
		rwidth += l;
		l = strlen(szRichEditSuffix);
		memcpy(richoutputpolystring + rwidth, szRichEditSuffix, l);
		rwidth += l;
	}
	else
	{
		// 1st get number of characters needed
		pMonomialListType p = pPLY->pmL;
		//double buffer length: DBL_DIG, point, sign, \0, 
		// exponent length, exponent sign, 'e'
		int maxExpl = (int)log10((double) ((DBL_MAX_10_EXP > -DBL_MIN_10_EXP) ?
			DBL_MAX_10_EXP : -DBL_MIN_10_EXP)) + 1 + 1 + 1;
		int width = 0, maxDbl = DBL_DIG + 3 + maxExpl; // digits + '.' + '-' + '\0'
		int itmp;
		char * buffer = new char[maxDbl];
		double c;
		pMTType tl;
		while (p != NULL)
		{
			c = p->mon->coefficient;
			if ((p != pPLY->pmL) && (c >= 0))   // '+' sign not included in printf
			{ ++width; ++rwidth;}
			if ((abs(abs(c) - 1.0) > DBL_EPSILON) || (p->mon->degree == 0)) 
			{	itmp = std::sprintf(buffer, "%.*g", DBL_DIG, c);
				width += itmp; rwidth += itmp;}
			if ((abs(c + 1.0) < DBL_EPSILON) && (p->mon->degree > 0)) // '-' sign
			{	++width; ++rwidth; }
			tl = p->mon->pTerm;
			while (tl != NULL)
			{
				if (tl->exponent > 1)
				{
					width++; // for '^'
					itmp = (int)log10((double)tl->exponent) + 1;
					width += itmp; rwidth += iSup + itmp + iNorm;
				}
				rwidth++; width++; // for variable char
				if (tl->v->varSub >= 0)
				{
					rwidth += iSub;
					if (tl->v->varSub == 0) 
					{	++width; ++rwidth;}
					else 
					{
						itmp = (int)log10((double)tl->v->varSub) + 1;
						width += itmp; rwidth += itmp;
					}
					rwidth += iNorm;
				}
				tl = tl->pNextComponent;
			}
			p = p->pNextMon;
		}

		// :::LENGTHS ARE NOW DETERMINED - ON TO THE ALLOCATING & SETTING:::
		outputpolystring = new char[width + 1]; // for final \0
		outputpolystring[width] = '\0';

		richoutputpolystring = new char[rwidth];
		richoutputpolystring[rwidth - 1] = '\0';
		itmp = strlen(szRichEditProlog1);
		memcpy(richoutputpolystring, szRichEditProlog1, itmp);
		int rindex = itmp;
		itmp = strlen(szRichEditProlog2);
		memcpy(richoutputpolystring + rindex, szRichEditProlog2, itmp);
		rindex += itmp;
		itmp = strlen(szRichEditDF1);
		memcpy(richoutputpolystring + rindex, szRichEditDF1, itmp);
		rindex += itmp;

		p = pPLY->pmL;
		int index = 0, w;
		while (p != NULL)
		{
			c = p->mon->coefficient;
			if ((p != pPLY->pmL) && (c >= 0)) 
			{
				outputpolystring[index++] = '+';
				richoutputpolystring[rindex++] = '+';
			}
			if ((abs(abs(c) - 1.0) > DBL_EPSILON) || (p->mon->degree == 0))
			{
				w = std::sprintf(buffer, "%.*g", DBL_DIG, c);
				memcpy(outputpolystring + index, buffer, w);
				index += w;
				memcpy(richoutputpolystring + rindex, buffer, w);
				rindex += w;
			}
			if ((abs(c + 1.0) < DBL_EPSILON) && (p->mon->degree > 0)) 
			{
				outputpolystring[index++] = '-';
				richoutputpolystring[rindex++] = '-';
			}
			tl = p->mon->pTerm;
			while (tl != NULL)
			{
				outputpolystring[index++] = tl->v->varV;
				richoutputpolystring[rindex++] = tl->v->varV;
				if (tl->v->varSub >= 0)
				{
					memcpy(richoutputpolystring + rindex, Sub, iSub); 
					rindex += iSub;
					if (tl->v->varSub < 10) 
					{
						outputpolystring[index++] = (char)((int)'0' + tl->v->varSub);
						richoutputpolystring[rindex++] = 
							(char)((int)'0' + tl->v->varSub);
					}
					else
					{
						w = std::sprintf(buffer, "%u", tl->v->varSub);
						memcpy(outputpolystring + index, buffer, w);
						index += w;
						memcpy(richoutputpolystring + rindex, buffer, w);
						rindex += w;
					}
					memcpy(richoutputpolystring + rindex, Norm, iNorm); 
					rindex += iNorm;
				}
				if (tl->exponent > 1)
				{
					memcpy(richoutputpolystring + rindex, Sup, iSup); 
					rindex += iSup;
					outputpolystring[index++] = '^';
					w = std::sprintf(buffer, /*(int)log10((double)tl->exponent) + 2, ~/ 
						"%u", tl->exponent);
					memcpy(outputpolystring + index, buffer, w);
					index += w;
					memcpy(richoutputpolystring + rindex, buffer, w);
					rindex += w;
					memcpy(richoutputpolystring + rindex, Norm, iNorm);
					rindex += iNorm;
				}
				tl = tl->pNextComponent;
			}
			p = p->pNextMon;

		}
		itmp = strlen(szRichEditSuffix);
		memcpy(richoutputpolystring + rindex, szRichEditSuffix, itmp);
		rindex += itmp;
		delete [] buffer;
	}
}
*/