#include "first.hpp"
#include "PolynomialString.hpp"
#include "GUI.hpp"
#include "widgets.hpp"

using namespace std;

int main (int argc, char **argv)
{
	GEXT::argc = argc;
	GEXT::argv = argv;

	pGEXT pG = GEXT::getInst();
	GUI * p_CI = new GUI;
	pG->pCI = p_CI;
	p_CI->ResetGUI(pG);
	GEXT::the_app->run(*GEXT::CMW);

	exit(EXIT_SUCCESS);
}
