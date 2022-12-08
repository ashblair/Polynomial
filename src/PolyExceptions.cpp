#include "first.hpp"
#include <stdexcept>
#include <exception>
#include "PolyExceptions.hpp"

//errors:
const char * polyexception::p_Exception[] = {
	"Error: Invalid Character",
	"Error: Not a Polynomial- variables in the exponent",
	"Error: Not a Polynomial- negative exponent",
	"Error: Not a Polynomial- exponent not a whole number",
	"Error: Coefficient Probably not a real number",
	"Error: Domain or Range Problem During Exponentiation",
	"Error: Division by 0",
	"Error: Denominator Polynomial must be degree 0",
	"Error: Denominator Polynomial is empty",
	"Error: GetEndParentheses called incorrectly",
	"Error: Unmatched Parentheses",
	"Error: CheckParentheses called incorrectly",
	"Error: CheckParentheses called with empty string",
	"Error: RemoveSuperfluousParentheses called incorrectly",
	"Error: RemoveSpaces called incorrectly",
	"Error: CheckOperators called incorrectly",
	"Error: Adjacent Operators",
	"Error: Operator at end",
	"Error: Operator cannot end quantity",
	"Error: Operators *, / and ^ cannot begin quantity",
	"Error: CheckCharacters called improperly",
	"Error: Empty String in GetNumber",
	"Error: Overflow while Getting Number",
	"Error: Underflow while Getting Number",
	"Error: Too Many Digits while Getting Number",
	"Error: Not a Number",
	"Error: Empty String in GetWholeNumber",
	"Error: GetFromString called improperly",
	"Error in GetUnit",
	"Error: Unhandled Node in GetPoly - Bad Tree"
};


/*
invalid_char::invalid_char(void) 
{
	pExc = p_Exception[0];//"Error: Invalid Character";
}

variablesinexponent::variablesinexponent(void) 
{
	pExc = p_Exception[1];//"Error: Not a Polynomial- variables in the exponent";
}

negativeexponent::negativeexponent(void)
{
	pExc = p_Exception[2];//"Error: Not a Polynomial- negative exponent";
}

nonwholeexponent::nonwholeexponent(void) 
{
	pExc = p_Exception[3];//"Error: Not a Polynomial- exponent not a whole number";
}

unreal::unreal(void) 
{
	pExc = p_Exception[4];//"Error: Coefficient Probably not a real number";
}

exponenterror::exponenterror(void) 
{
	pExc = p_Exception[5];//"Error: Domain or Range Problem During Exponentiation";
}

zerodivide::zerodivide(void) 
{
	pExc = p_Exception[6];//"Error: Division by 0";
}

dpolydegree::dpolydegree(void) 
{
	pExc = p_Exception[7];//"Error: Denominator Polynomial must be degree 0";
}

dpolyempty::dpolyempty(void) 
{
	pExc = p_Exception[8];//"Error: Denominator Polynomial is empty";
}

incorrectGetEndParentheses::incorrectGetEndParentheses(void) 
{
	pExc = p_Exception[9];//"Error: GetEndParentheses called incorrectly";
}

parenthesesunmatched::parenthesesunmatched(void) 
{
	pExc = p_Exception[10];//"Error: Unmatched Parentheses";
}

incorrectCheckParentheses::incorrectCheckParentheses(void) 
{
	pExc = p_Exception[11];//"Error: CheckParentheses called incorrectly";
}

CheckParenthesesEmpty::CheckParenthesesEmpty(void) 
{
	pExc = p_Exception[12];//"Error: CheckParentheses called with empty string";
}

incorrectRemoveSuperfluous::incorrectRemoveSuperfluous(void) 
{
	pExc = p_Exception[13];//"Error: RemoveSuperfluousParentheses called incorrectly";
}

incorrectRemoveSpaces::incorrectRemoveSpaces(void) 
{
	pExc = p_Exception[14];//"Error: RemoveSpaces called incorrectly";
}

incorrectCheckOperators::incorrectCheckOperators(void) 
{
	pExc = p_Exception[15];//"Error: CheckOperators called incorrectly";
}

adjacentops::adjacentops(void) 
{
	pExc = p_Exception[16];//"Error: Adjacent Operators";
}

opsatend::opsatend(void) 
{
	pExc = p_Exception[17];//"Error: Operator at end";
}

opsendingq::opsendingq(void) 
{
	pExc = p_Exception[18];//"Error: Operator cannot end quantity";
}

opsbeginningq::opsbeginningq(void) 
{
	pExc = p_Exception[19];//"Error: Operators *, / and ^ cannot begin quantity";
}

improperCheckCharacters::improperCheckCharacters(void) 
{
	pExc = p_Exception[20];//"Error: CheckCharacters called improperly";
}

GetNumberEmpty::GetNumberEmpty(void) 
{
	pExc = p_Exception[21];//"Error: Empty String in GetNumber";
}

overflow::overflow(void) 
{
	pExc = p_Exception[22];//"Error: Overflow while Getting Number";
}

underflow::underflow(void) 
{
	pExc = p_Exception[23];//"Error: Underflow while Getting Number";
}

overdigits::overdigits(void) 
{
	pExc = p_Exception[24];//"Error: Too Many Digits while Getting Number";
}

n_a_n::n_a_n(void) 
{
	pExc = p_Exception[25];//"Error: Not a Number";
}
	
GetWholeEmpty::GetWholeEmpty(void) 
{
	pExc = p_Exception[26];//"Error: Empty String in GetWholeNumber";
}

improperGetFromString::improperGetFromString(void) 
{
	pExc = p_Exception[27];//"Error: GetFromString called improperly";
}

GetUnitError::GetUnitError(void) 
{
	pExc = p_Exception[28];//"Error in GetUnit";
}

GetPolyError::GetPolyError(void) 
{
	pExc = p_Exception[29];//"Error: Unhandled Node in GetPoly - Bad Tree";
}
*/