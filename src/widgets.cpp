#include "first.hpp"
#include "GUI.hpp"
#include "widgets.hpp"

using namespace std;

Glib::RefPtr<Gtk::CssProvider> GEXT::provider;
Glib::RefPtr<Gdk::Display> GEXT::display;
Glib::RefPtr<Gdk::Screen> GEXT::screen;
Glib::RefPtr<Gtk::Application> GEXT::the_app;
Glib::RefPtr<Gtk::Builder> GEXT::refBuilder;
Gtk::Dialog* GEXT::CMW = NULL;
pGEXT GEXT::pGEXTInstance = NULL;
//initialize these next statics first in main before getInst():
int GEXT::argc = 0;
char ** GEXT::argv = NULL;

extern char glade_start		asm("_binary_rsc_glade_glade_start");
extern char glade_end		asm("_binary_rsc_glade_glade_end");
extern char style_start		asm("_binary_rsc_style_css_start");
extern char style_end		asm("_binary_rsc_style_css_end");

glade_extract::glade_extract(void)
{
	the_app = Gtk::Application::create(argc, argv, "from.the.glade");
	//Load the GtkBuilder file and instantiate its widgets:
	refBuilder = Gtk::Builder::create();
	/*
	try
	{
		refBuilder->add_from_file("rsc/glade.glade");
	}
	catch(const Glib::FileError& ex)
	{
		cout << "FileError: " << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch(const Glib::MarkupError& ex)
	{
		cout << "MarkupError: " << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch(const Gtk::BuilderError& ex)
	{
		cout << "BuilderError: " << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	provider = Gtk::CssProvider::create();
	Gtk::CssProvider * pProvider = provider.get();
	display = Gdk::Display::get_default();
	Gdk::Display * pDisplay = display.get();
	screen = pDisplay->get_default_screen();
	Gtk::StyleContext::add_provider_for_screen(screen, provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	pProvider->load_from_path("rsc/style.css");
	*/

	char * g_s = &glade_start, * g_e = &glade_end;
	gsize g_z = g_e - g_s;
	refBuilder->add_from_string(g_s, g_z);
	provider = Gtk::CssProvider::create();
	display = Gdk::Display::get_default();
	screen = display->get_default_screen();
	Gtk::StyleContext::add_provider_for_screen(screen, provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	char * s_s = &style_start, * s_e = &style_end;
	string s_str(s_s, s_e);
	provider->load_from_data(s_str);

	GI[PolyDlg].classType = "GtkDialog";
	GI[PolyDlg].id = "PolyDlg";
	refBuilder->get_widget("PolyDlg", GI[PolyDlg].pW);
	mains["PolyDlg"] = &GI[PolyDlg];
	((Gtk::Dialog*)GI[PolyDlg].pW)->signal_delete_event().connect(sigc::mem_fun<GdkEventAny *, bool>(*this, &glade_extract::on_PolyDlg_delete_event));
	((Gtk::Dialog*)GI[PolyDlg].pW)->signal_size_allocate().connect(sigc::mem_fun<Gtk::Allocation&, void>(*this, &glade_extract::on_PolyDlg_size_allocate));
	CMW = ((Gtk::Dialog*)GI[PolyDlg].pW);
	GI[PolyDlgButtonBox].classType = "GtkButtonBox";
	GI[PolyDlgButtonBox].id = "PolyDlgButtonBox";
	refBuilder->get_widget("PolyDlgButtonBox", GI[PolyDlgButtonBox].pW);
	styles["PolyDlgButtonBox"] = &GI[PolyDlgButtonBox];
	GI[PolyBox].classType = "GtkBox";
	GI[PolyBox].id = "PolyBox";
	refBuilder->get_widget("PolyBox", GI[PolyBox].pW);
	styles["PolyBox"] = &GI[PolyBox];
	GI[ExitButton].classType = "GtkButton";
	GI[ExitButton].id = "ExitButton";
	refBuilder->get_widget("ExitButton", GI[ExitButton].pW);
	controls["ExitButton"] = &GI[ExitButton];
	((Gtk::Button*)GI[ExitButton].pW)->signal_clicked().connect(sigc::mem_fun(*this, &glade_extract::on_ExitButton_clicked));
	GI[OTextView].classType = "GtkTextView";
	GI[OTextView].id = "OTextView";
	refBuilder->get_widget("OTextView", GI[OTextView].pW);
	controls["OTextView"] = &GI[OTextView];
	GI[PolyInLabel].classType = "GtkLabel";
	GI[PolyInLabel].id = "PolyInLabel";
	refBuilder->get_widget("PolyInLabel", GI[PolyInLabel].pW);
	controls["PolyInLabel"] = &GI[PolyInLabel];
	GI[ITextView].classType = "GtkTextView";
	GI[ITextView].id = "ITextView";
	refBuilder->get_widget("ITextView", GI[ITextView].pW);
	controls["ITextView"] = &GI[ITextView];
	GI[StatusLabel].classType = "GtkLabel";
	GI[StatusLabel].id = "StatusLabel";
	refBuilder->get_widget("StatusLabel", GI[StatusLabel].pW);
	controls["StatusLabel"] = &GI[StatusLabel];
	//the_app->run(*CMW);
}
glade_extract::~glade_extract(void)
{	delete pCI;
	}
pGEXT glade_extract::getInst(void)
{
	if(argv == NULL)
	{
		cout << "Error: intitialize GEXT::argc & GEXT::argv before calling getInst\n";
		exit(EXIT_FAILURE);
	}

	if(pGEXTInstance == NULL) pGEXTInstance = new glade_extract;
	return pGEXTInstance;
}
Gtk::Widget * glade_extract::get(const string & wName)
{
	map<string, glade_info *>::iterator mit = controls.find(wName);
	if (mit != controls.end()) return (*mit).second->getWidget();
	mit = containers.find(wName);
	if (mit != containers.end()) return (*mit).second->getWidget();
	mit = mains.find(wName);
	if (mit != mains.end()) return (*mit).second->getWidget();
	mit = styles.find(wName);
	if (mit != styles.end()) return (*mit).second->getWidget();
	return NULL;
}
bool glade_extract::on_PolyDlg_delete_event(GdkEventAny* any_event)
{
	//cout << "PolyDlg delete connected\n";
	return false;
}
void glade_extract::on_PolyDlg_size_allocate(Gtk::Allocation & allocation)
{
	//cout << "PolyDlg size connected (x:" << 
	//allocation.get_x() << ", y:" << 
	//allocation.get_y() << ", w:" << 
	//allocation.get_width() << ", h:" << 
	//allocation.get_height() << ")\n" ;
}
void glade_extract::on_ExitButton_clicked(void)
{
	pCI->HandleExitButton();
}
