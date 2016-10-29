#include <FindDirectory.h>
#include <Path.h>
#include <unistd.h>

#include "pcs_options.h"
#include "default.h"
#include "gamestructs.h"
#include "pcs_window.h"

const char *app_signature = APP_SIGNATURE;


class MyApplication : public BApplication
{
private:
	PCSWindow	*theWindow;

public:
	MyApplication(const char *signature) :
		BApplication(signature) {
		
		// Creates the window and sets the title with the application name. 
		BRect	windowRect;
		bool 		ok = false;
		
		/*
		BPath		path;
		int			ref;
		settings	config;
		
		
		if (find_directory (B_USER_SETTINGS_DIRECTORY, &path) == B_OK) {
			path.Append(APP_SETTINGS_FILE);
			ref = open(path.Path(), O_RDONLY);
			if (ref >= 0) {
				read(ref, (char *)&windowRect, sizeof(windowRect));
				
				// Config
				read(ref, (config_t*)&config, sizeof(config_t));
				
				config.list = new BList(config.anz);	
				
				char buf[1000];
				for (int i=0;i<config.anz;i++) {
					int tl,ul;
					BString d1,d2;
					
					read(ref, (int*)&tl, sizeof(int));
					read(ref, buf, tl);
					d1 = buf;
					
					read(ref, (int*)&ul, sizeof(int));
					read(ref, buf, ul);
					d2 = buf;
					
					config.list->AddItem( new FStringItem(d1.String(),d2.String()), i );
				}
				
		
				close(ref);
				ok = true;
			}
		}
		*/
		
		if (!ok) { // Defaults:
			windowRect.Set(150,150,600,600);

			/*
			// Default configuration:
			config.ColBackMode	= ColBackDesktop;
			config.col.red		= 51;
			config.col.green	= 102;
			config.col.blue		= 152;
			config.col.alpha	= 0;
			
			config.ColForeMode	= ColForeAdapt;
			config.high.red		= 0;
			config.high.green	= 0;
			config.high.blue	= 0;
			config.high.alpha	= 0;
			
			config.RefreshRate = REFRESH_MIN_NORM * 120;
			config.RefreshAdvances = 0;
			
			config.ScrollbarMode = ScrollbarModeAuto;
			config.reserved[0] = 0;
			config.reserved[1] = 0;
		
			config.anz = 4;
			config.index = 0;			
			config.list = new BList();	
			config.list->AddItem( new FStringItem("Fortune","fortune") );		
			config.list->AddItem( new FStringItem("bebits","http://www.bebits.com/backend/recent.rdf") );
			config.list->AddItem( new FStringItem("isComputerOn","http://www.iscomputeron.com/index/backend.php") );
			config.list->AddItem( new FStringItem("BeosJournal","http://www.beosjournal.org/recent.rdf") );
			config.list->AddItem( new FStringItem("OSNews", "http://www.osnews.com/files/recent.rdf") );
			//config.list->AddItem( new FStringItem("Heise Newsticker","http://www.heise.de/newsticker/heise.rdf") );
			*/
		}
		
		
		theWindow = new PCSWindow(/*config,*/windowRect,VERSION_);
		theWindow->Show();
	}
	
	virtual void MessageReceived(BMessage *msg)
	{
		BApplication::MessageReceived(msg);
	}
};

int main()
{
	new MyApplication(app_signature);
	be_app->Run();

	delete be_app;
	return 0;
}
