#pragma once

/*CONCEPTUAL INCLUDES FOR THIS PROJECT:
#include "stdafx.h" which should contain
#include <gtkmm.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <map>

*/
/*
	usage: (in main start with:) GEXT::argc = argc; GEXT::argv = argv;
	from the glade file, an extract:
	MAINS:
		PolyDlg, a GtkDialog


	STYLES:
		PolyDlgButtonBox, a GtkButtonBox
		PolyBox, a GtkBox


	CONTAINERS:


	CONTROLS:
		ExitButton, a GtkButton
		OTextView, a GtkTextView
		PolyInLabel, a GtkLabel
		ITextView, a GtkTextView
		StatusLabel, a GtkLabel
*/
typedef class GUI * pGUI;

class glade_info
{
public:
	std::string classType, id;
	Gtk::Widget * pW;

	glade_info(void):classType{}, id{}, pW{NULL}{}
	glade_info(const glade_info &a):classType{a.classType}, id{a.id}, pW{a.pW}{}
	~glade_info(void){}

	Gtk::Widget * getWidget(void) {return pW;}
	std::string getName() {return id;}
	std::string getClassType() {return classType;}
};
typedef class glade_extract GEXT;
typedef GEXT * pGEXT;

class glade_extract
{
enum wdg_enum{PolyDlg = 0, PolyDlgButtonBox = 1, PolyBox = 2, ExitButton = 3, OTextView = 4, PolyInLabel = 5, ITextView = 6, StatusLabel = 7};
	Gtk::Widget * get(const std::string &);
public:
	void get(const std::string & s, Gtk::Box*&gtk){gtk=dynamic_cast<Gtk::Box*>(get(s));}
	void get(const std::string & s, Gtk::Button*&gtk){gtk=dynamic_cast<Gtk::Button*>(get(s));}
	void get(const std::string & s, Gtk::ButtonBox*&gtk){gtk=dynamic_cast<Gtk::ButtonBox*>(get(s));}
	void get(const std::string & s, Gtk::Dialog*&gtk){gtk=dynamic_cast<Gtk::Dialog*>(get(s));}
	void get(const std::string & s, Gtk::Label*&gtk){gtk=dynamic_cast<Gtk::Label*>(get(s));}
	void get(const std::string & s, Gtk::TextView*&gtk){gtk=dynamic_cast<Gtk::TextView*>(get(s));}
	bool on_PolyDlg_delete_event(GdkEventAny*);
	void on_PolyDlg_size_allocate(Gtk::Allocation&);
	void on_ExitButton_clicked(void);
	static Gtk::Dialog* CMW;
	static int argc;
	static char ** argv;
	static Glib::RefPtr<Gtk::Application> the_app;
	static Glib::RefPtr<Gtk::Builder> refBuilder;
	static Glib::RefPtr<Gtk::CssProvider> provider;
	static Glib::RefPtr<Gdk::Display> display;
	static Glib::RefPtr<Gdk::Screen> screen;
	static pGEXT getInst(void);
	std::map<std::string, glade_info *> mains, styles, containers, controls;
	pGUI pCI;
	~glade_extract(void);
private:
	glade_info GI[8];
	glade_extract(void);
	static pGEXT pGEXTInstance;
};
