#pragma once

class glade_extract;
typedef class glade_extract GEXT;
typedef GEXT * pGEXT;

class GUI
{
public:
	GUI(void);
	~GUI(void);

	void ResetGUI(pGEXT);
	void HandleExitButton(void);
    void on_InputBuffer_insert(const Gtk::TextBuffer::iterator &, const Glib::ustring &, int);
    void on_InputBuffer_changed(void);
    bool on_timeout(int);
    void setupTimer(void);
	bool loadinStr(const Glib::ustring &);

	sigc::connection conn;
	sigc::slot<bool> my_slot;
	int m_timer_number;
	std::vector<Glib::ustring> inStr;

	pGEXT pG;
	Gtk::Dialog * PolyDlg;
	Gtk::Button * ExitButton;
	Gtk::TextView * OTextView;
	Gtk::Label * PolyInLabel;
	Gtk::TextView * ITextView;
	Gtk::Label * StatusLabel;
    Glib::RefPtr<Gtk::TextBuffer> otb, itb;

};
