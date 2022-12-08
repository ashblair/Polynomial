#include "first.hpp"
#include "PolynomialString.hpp"
#include "widgets.hpp"
#include "GUI.hpp"

using namespace std;

GUI::GUI(void)
{
	pG = NULL;
	PolyDlg = NULL;
	ExitButton = NULL;
	OTextView = NULL;
	PolyInLabel = NULL;
	ITextView = NULL;
	StatusLabel = NULL;
}

GUI::~GUI(void)
{
}

void GUI::ResetGUI(pGEXT p_g)
{
	pG = p_g;
	pG->get("PolyDlg", PolyDlg);
	//PolyDlg->fullscreen();
	pG->get("ExitButton", ExitButton);

	pG->get("OTextView", OTextView);
    OTextView->set_editable(false);
    Pango::FontDescription fdesc;
    fdesc.set_family("Arial");
    fdesc.set_size(20 * PANGO_SCALE);
    OTextView->override_font(fdesc);    
    otb = Gtk::TextBuffer::create();
    OTextView->set_buffer(otb);

	pG->get("PolyInLabel", PolyInLabel);
	
	pG->get("ITextView", ITextView);
	ITextView->override_font(fdesc);
    itb = Gtk::TextBuffer::create();
    ITextView->set_buffer(itb);
	itb->signal_insert().connect(sigc::mem_fun<const Gtk::TextBuffer::iterator &, const Glib::ustring &, int, void>(*this, &GUI::on_InputBuffer_insert));
    itb->signal_changed().connect(sigc::mem_fun(*this, &GUI::on_InputBuffer_changed));
    //itb->set_text("Hi There!\n");
    //mfloat::disp.TB = itb.get();
    //Glib::RefPtr<Gtk::TextTag> dTag = itb->create_tag("DBL_ME"),
    //    cTag = itb->create_tag("CTR_ME");
    //dTag->set_property("size", 20 * PANGO_SCALE); //("scale", 2.0);
    //cTag->set_property("justification", Gtk::JUSTIFY_CENTER);

	pG->get("StatusLabel", StatusLabel);
}

void GUI::HandleExitButton(void)
{
	cout << "GUI::HandleExitButton called.\n";
	PolyDlg->close();
}

void GUI::on_InputBuffer_insert(const Gtk::TextBuffer::iterator & pos, const Glib::ustring & text, int bytes)
{ // textbuffer not yet updated (should happen in default handler next)
    // cout << "InputBuffer insert signal handler called w/ " << text << " & state=" << state << "\n";
    //timer_control = 0;
    Glib::ustring::value_type lastChar = text[text.size() - 1];
    char lst = static_cast<char>(lastChar);
	if (lst == '\n')
	{
		Glib::ustring iTxt = itb->get_text();
		iTxt.erase(iTxt.size() - 1, 1);
		PolynomialString pstr = PolynomialString(iTxt);
		Glib::ustring w = pstr.GetInputPolyString() + " = " + pstr.GetOutputPolyString() + "\n\n";
	    //otb->remove_all_tags(otb->begin(), otb->end());
	    //if (0 != OTextView->get_pixels_inside_wrap()) OTextView->set_pixels_inside_wrap(0);
		otb->insert(otb->begin(), w);
		setupTimer();
		//itb->set_text(" ");
		//itb->erase(itb->begin(), itb->end());
	}
}

void GUI::on_InputBuffer_changed(void)
{ // called before textbuffer update
    // cout << "InputBuffer changed signal handler called\n";

}

bool GUI::on_timeout(int timerID)
{
    conn.disconnect(); // one-shot timeout
    // hopefully this will allow the input text buffer's default handler to finish:
    while (gtk_events_pending()) gtk_main_iteration(); 
	itb->erase(itb->begin(), itb->end());
    return false;
}
    
void GUI::setupTimer(void)
{
    m_timer_number = rand();
    my_slot = sigc::bind(sigc::mem_fun(*this,
                &GUI::on_timeout), m_timer_number);

    // This is where we connect the slot to the Glib::signal_timeout()
    conn = Glib::signal_timeout().connect(my_slot, 10); // 10 ms = 1/100 seconds

}

