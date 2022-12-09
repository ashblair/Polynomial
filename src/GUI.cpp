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
	//cout << "GUI::HandleExitButton called.\n";
	PolyDlg->close();
}

bool GUI::loadinStr(const Glib::ustring & s)
{
	size_t cr = s.find('\n'), l = 0;
	if (cr != Glib::ustring::npos)
	{
		inStr.clear();
		do
		{
			Glib::ustring sub = s.substr(l, cr - l);
			inStr.push_back(sub);
			l = cr + 1;
			while ((l < s.size()) && (s[l] == '\n')) ++l;
			cr = l >= s.size()? Glib::ustring::npos : s.find('\n', l);
		} while (cr != Glib::ustring::npos);
		Glib::ustring r = "";
		if (l < s.size()) r = s.substr(l);
		inStr.push_back(r); //insert(inStr.begin(), r);
		std::reverse(inStr.begin(), inStr.end());
		return true;
	}
	return false;
}


void GUI::on_InputBuffer_insert(const Gtk::TextBuffer::iterator & pos, const Glib::ustring & text, int bytes)
{ // textbuffer not yet updated (should happen in default handler next)
    // cout << "InputBuffer insert signal handler called w/ " << text << " & state=" << state << "\n";
    //timer_control = 0;
	if (loadinStr(itb->get_text())) setupTimer();
	/*
	size_t tSz = text.size();
	
	if (text.bytes() > 1) return;
    
	Glib::ustring::value_type lastChar = text[0]; //[text.size() - 1];
	
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
	*/
}

void GUI::on_InputBuffer_changed(void)
{ // called before textbuffer update
    // cout << "InputBuffer changed signal handler called\n";

}

bool GUI::on_timeout(int timerID)
{ // using a timer here is currently necessary because the text buffer iterator 
  // needs to be processed by its default handler
  // to avoid a warning like: 
  // Gtk-WARNING **: 17:23:41.446: Invalid text buffer iterator: either the iterator is uninitialized, or the characters/pixbufs/widgets in the buffer have been modified since the iterator was created.
  //You must use marks, character numbers, or line numbers to preserve a position across buffer modifications.
  //You can apply tags and insert marks without invalidating your iterators,
  //but any mutation that affects 'indexable' buffer contents (contents that can be referred to by character offset)
  //will invalidate all outstanding iterators
	size_t sSz = inStr.size();
	Glib::ustring iTxt = inStr[sSz - 1];
	if (sSz == 1)
	{
		//Glib::ustring lStr = inStr[0];
		//itb->erase(itb->begin(), itb->end());
		itb->set_text(iTxt);
		conn.disconnect();
	}
	else
	{
		//Glib::ustring iTxt = inStr[sSz - 1];
		//iTxt.erase(iTxt.size() - 1, 1);
		PolynomialString pstr = PolynomialString(iTxt);
		Glib::ustring w = pstr.GetInputPolyString() + " = " + pstr.GetOutputPolyString() + "\n\n";
	    //otb->remove_all_tags(otb->begin(), otb->end());
	    //if (0 != OTextView->get_pixels_inside_wrap()) OTextView->set_pixels_inside_wrap(0);
		otb->insert(otb->begin(), w);

	}

	inStr.pop_back();

    // hopefully this will allow the input text buffer's default handler to finish:
    //while (gtk_events_pending()) gtk_main_iteration(); 
	//itb->erase(itb->begin(), itb->end());
    return true;
}
    
void GUI::setupTimer(void)
{
    m_timer_number = rand();
    my_slot = sigc::bind(sigc::mem_fun(*this,
                &GUI::on_timeout), m_timer_number);

    // This is where we connect the slot to the Glib::signal_timeout()
    conn = Glib::signal_timeout().connect(my_slot, 10); // 10 ms = 1/100 seconds

}

