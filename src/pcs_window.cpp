#include "pcs_window.h"
#include <be/StorageKit.h>

PCSWindow::PCSWindow(BRect frame, const char* Title) : 
	BWindow(frame, Title, B_TITLED_WINDOW, B_FRAME_EVENTS)
{	
	LoadSettings();

	
	// Menu:
	BRect brect = Bounds();
	
	menubar = new BMenuBar(brect, "MenuBar" );
	menubar->AddItem( menFile = new BMenu(_T("File")) );
		menFile->AddItem( new BMenuItem( _T("New Game"), new BMessage(MEN_F_NEW), 'N', B_COMMAND_KEY ) );
		menFile->AddSeparatorItem();
		menFile->AddItem( new BMenuItem( _T("View Highscores"), new BMessage(MEN_F_VIEWHIGH) ) );
		menFile->AddSeparatorItem();
		menFile->AddItem( new BMenuItem( _T("Quit"), new BMessage(MEN_F_QUIT) ) );

	menubar->AddItem( menMode = new BMenu(_T("Mode")) );
		menMode->AddItem( menModeNormal = new BMenuItem( _T("Normal"), new BMessage(MEN_M_NORMAL), '1', B_COMMAND_KEY ) );
		menMode->AddItem( menModeContinious = new BMenuItem( _T("Continious"), new BMessage(MEN_M_CONT), '2', B_COMMAND_KEY ) );

	menubar->AddItem( menOptions = new BMenu(_T("Options")) );
		BMenu* visual = new BMenu(_T("Visual"));
		menOptions->AddItem( visual );
			visual->AddItem( menUseBitmaps = new BMenuItem( _T("Use Bitmaps"), new BMessage( B_O_UseBitmaps) ) );
			visual->AddItem( menAlternativeColors = new BMenuItem( _T("Alternative Colors"), new BMessage( B_O_AlternativeColors) ) );
			visual->AddItem( menShade = new BMenuItem( _T("Shade"), new BMessage( B_O_Shade) ) );
			visual->AddItem( menBubble = new BMenuItem( _T("Bubble"), new BMessage( B_O_Bubble) ) );
			visual->AddItem( menAnimation = new BMenuItem( _T("Animation"), new BMessage( B_O_Animation) ) );
		menOptions->AddItem( menSound = new BMenuItem( _T("Sound"), new BMessage( B_O_Sound) ) );
		menOptions->AddSeparatorItem();
		menOptions->AddItem( menQuickMode = new BMenuItem( _T("Quick Mode"), new BMessage( B_O_QuickMode) ) );
		menOptions->AddSeparatorItem();
		menOptions->AddItem( menShowStatus = new BMenuItem( _T("Show Status"), new BMessage( B_O_ShowStatus) ) );

	menubar->AddItem( menHelp = new BMenu(_T("Help")) );
		menHelp->AddItem( new BMenuItem( _T("Undo"), new BMessage(MEN_H_UNDO), 'Z', B_COMMAND_KEY ) );
		menHelp->AddItem( new BMenuItem( _T("Do nothing"), NULL ) );
		menHelp->AddSeparatorItem();
		menHelp->AddItem( new BMenuItem( _T("How to play"), new BMessage(MEN_H_HOWPLAY) ) );
		menHelp->AddSeparatorItem();
		menHelp->AddItem( new BMenuItem( _T("About PCShifter"), new BMessage(MEN_H_ABOUT) ) );
		

	AddChild( menubar );

	// Game board:
	brect = Bounds();
	brect.top = menubar->Frame().bottom + 1;
	gameview = new GameView(brect, &settings);
	AddChild(gameview);
	
	NewMode( settings.GameMode );
	UpdateOptions();
}



bool
PCSWindow::QuitRequested()
{
	SaveSettings();

	//Save(myf->Config());
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(TRUE);
}

/*
void
PCSWindow::Save(config_t* conf)
{
	int		ref;
	BPath	path;		
	BRect	frame = Frame();
	
	if (find_directory (B_USER_SETTINGS_DIRECTORY, &path, true) == B_OK) {
		path.Append(APP_SETTINGS_FILE);
		ref = creat(path.Path(), 0777);
		if (ref >= 0) {
			write(ref, (char *)&frame, sizeof(frame));
			
			// Config
			write(ref, (config_t*)conf, sizeof(config_t));
			
			for (int i=0;i<conf->anz;i++) {
				FStringItem *fi = (FStringItem*)conf->list->ItemAt(i);
				if (!fi)
					(new BAlert("shit","shit","shit"))->Go();
				
				BString t(fi->Text());
				int tl = t.Length()+1;
				write(ref, (int*)&tl, sizeof(int));
				write(ref, (char*)t.String(), tl);

				int ul = fi->sUrl.Length()+1;
				write(ref, (int*)&ul, sizeof(int));
				write(ref, (char*)fi->sUrl.String(), ul);
			}
							
			close(ref);
		}
	}	
}
*/	

void
PCSWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
	case MEN_F_NEW:
		NewMode( settings.GameMode );
		break;
		
	case MEN_F_VIEWHIGH:
		ViewHighscores();
		break;
	
	case MEN_F_QUIT:
		be_app->PostMessage(B_QUIT_REQUESTED);
		break;
	
	case MEN_M_NORMAL:
		if (settings.GameMode != modeNormal)
			NewMode( modeNormal );
		break;

	case MEN_M_CONT:
		if (settings.GameMode != modeContinious)
			NewMode( modeContinious );
		break;

	case MEN_H_UNDO:
		Undo();
		break;

	case MEN_H_HOWPLAY:
		(new BAlert(_T("How to play"), _T(STR_HOW_TO_PLAY), _T("Ok")))->Go();
		break;
				
	case MEN_H_ABOUT:
		AboutRequested();
		break;

	case B_O_UseBitmaps:	
		UpdateOption(settings.UseBitmaps);
		break;
		
	case B_O_AlternativeColors:
		UpdateOption(settings.AlternativeColors);
		break;

	case B_O_Sound:
		UpdateOption(settings.Sound);
		break;

	case B_O_Shade:
		UpdateOption(settings.Shade);
		break;

	case B_O_QuickMode:
		UpdateOption(settings.QuickMode);
		break;

	case B_O_Bubble:
		UpdateOption(settings.Bubble);
		break;

	case B_O_Animation:
		UpdateOption(settings.Animation);
		break;
	
	case B_O_ShowStatus:
		UpdateOption(settings.ShowStatus);
		break;
		
	case MSG_GAMEOVER:
		HandleGameOver();
		break;
		
	default:
		BWindow::MessageReceived(msg);
	}
}


void
PCSWindow::NewMode(GameMode_t mode)
{
	settings.GameMode = mode;
	
	menModeNormal->SetMarked(		mode == modeNormal );
	menModeContinious->SetMarked(	mode == modeContinious );
	
	gameview->NewGame( mode );
}


void
PCSWindow::Undo()
{
	switch (rand() % 22) {
	case 0:
		(new BAlert(_T("Undo"), "4 secretely deleted files have been restored.", _T("What?!")))->Go();
		break;
	case 1:
		(new BAlert(_T("Undo"), "defragmentation of hard disk was undone.", _T("Argh!")))->Go();
		break;
	case 2:
		(new BAlert(_T("Undo"), "2 critical system updates have been uninstalled.", _T("Hey!")))->Go();
		break;
	case 3:
		(new BAlert(_T("Undo"), "3 sent messages have been canceled.", _T("Nooo!")))->Go();
		break;
	case 4:
		(new BAlert(_T("Undo"), "Your score hase been resetted.", _T("WTF?")))->Go();
		break;								
	case 5:
		(new BAlert(_T("Undo"), "task 'World Peace' has been canceled and reverted to old state.", _T("Huh?")))->Go();
		break;								
	case 6:
		(new BAlert(_T("Undo"), "Cannot undo 'Lunch'.\n\nPlease invoke this process manually.", _T("X-(~")))->Go();
		break;								
	case 7:
		(new BAlert(_T("Undo"), "Microsoft monopoly sucessfully restored.", _T("Eh?")))->Go();
		break;								
	case 8:
		(new BAlert(_T("Undo"), "Access denied.", _T("Ok?")))->Go();
		break;								
	case 9:
		(new BAlert(_T("Undo"), "This operation caused a general protection fault in module EMM386.EXE", _T("Err...")))->Go();
		break;								
	case 10:
		(new BAlert(_T("Undo"), "Undo is considered cheating.\n\nAll your secret mails have been sent to your boss.", _T("Argh!")))->Go();
		break;								
	case 11:
		(new BAlert(_T("Undo"), "Cheatmode disabled.", _T("Umm...")))->Go();
		break;								
	default:
		(new BAlert(_T("Undo"), "No, I don't want to!", _T("Never mind...")))->Go();
	}
}

void
PCSWindow::AboutRequested()
{
	(new BAlert(_T("About PCShifter"), ABOUT_BOX, _T("Ok")))->Go();
}






void
PCSWindow::LoadSettings()
{
	BRect		windowRect;
	BPath		path;
	int			ref;
	bool		ok = false;

	if (find_directory (B_USER_SETTINGS_DIRECTORY, &path, true) == B_OK) {
		path.Append(APP_SETTINGS_FILE);
		ref = open(path.Path(), O_RDONLY);
		if (ref >= 0) {
			read(ref, (char *)&windowRect, sizeof(windowRect));
			read(ref, (char *)&settings, sizeof(settings));
			
			// highscores:
			for (int i=0;i<10;i++) {
				int size;
				char buf[1024];
				buf[1023] = 0;
				
				read(ref, &size, sizeof(int));
				size %= 1023;
				read(ref, buf, size);
				highscores[i].Name.SetTo(buf, size);

				read(ref, &size, sizeof(int));
				size %= 1023;
				read(ref, buf, size);
				highscores[i].Date.SetTo(buf, size);
			
				read(ref, &highscores[i].Score, sizeof(int));
				read(ref, &highscores[i].LongestChain, sizeof(int));	
			}
			
			ok = true;
			close(ref);
		}
	}

	if (!ok) {
		// defaults:
		windowRect.Set(150,150,600,600);
		
		// settings:
		settings.UseBitmaps = false;
		settings.AlternativeColors = false;
		settings.Sound = false;
		settings.Shade = false;
		settings.QuickMode = false;
		settings.Bubble = false;
		settings.Animation = false;
		settings.GameMode = modeNormal;
		
		// highscores:
		ResetHighscore();
	}
	
	MoveTo(windowRect.left, windowRect.top);
	ResizeTo(windowRect.Width(), windowRect.Height());
}

void
PCSWindow::SaveSettings()
{
	BRect		windowRect = Frame();
	BPath		path;
	int			ref;

	if (find_directory (B_USER_SETTINGS_DIRECTORY, &path, true) == B_OK) {
		path.Append(APP_SETTINGS_FILE);
		ref = creat(path.Path(), 0777);
		if (ref >= 0) {
			write(ref, (char *)&windowRect, sizeof(windowRect));
			write(ref, (char *)&settings, sizeof(settings));
		
		
			// highscores:
			for (int i=0;i<10;i++) {
				int size;
				char buf[1024];
				buf[1023] = 0;
				
				size = highscores[i].Name.Length()+1;
				write(ref, &size, sizeof(int));
				write(ref, highscores[i].Name.String(), size);

				size = highscores[i].Date.Length()+1;
				write(ref, &size, sizeof(int));
				write(ref, highscores[i].Date.String(), size);

				write(ref, &highscores[i].Score, sizeof(int));
				write(ref, &highscores[i].LongestChain, sizeof(int));	
			}
					
			close(ref);
		}
	}	
}


void
PCSWindow::HandleGameOver()
{
	int score = gameview->Score();
	int longc = gameview->LongestChain();
	int highscore = highscores[9].Score;
	
	BString text;
	text << _T("Game Over!") << "\n\n" << _T("Your score is") << " \t" << score << "\n";
	
	if (score>highscore) {
		text << _T("You have achieved a new Highscore!");
		
		char sss[1024];
		const char* username = cuserid(sss);
		
		if (username!=NULL)		
			highscores[10].Name.SetTo(username);
		else
			highscores[10].Name.SetTo("Be-Nutzer");
			
		highscores[10].Date = "now";
		highscores[10].Score = score;
		highscores[10].LongestChain = longc;
		
		time_t now = time(NULL);
		tm* localtm = localtime( &now );
		
		BString d;

		d << localtm->tm_mday << ".";
		d << (localtm->tm_mon+1) << ".";
		d << (localtm->tm_year+1900) << " ";

		
		if (localtm->tm_hour<10) d << "0";	d << localtm->tm_hour << ":";
		if (localtm->tm_min<10) d << "0";	d << localtm->tm_min;
		
		
		highscores[10].Date.SetTo(d);
		
		SortHighscores();
	}
	else {
		text << "You need at least " << highscore << " points for a highscore.\n";
	}	

	BAlert *ali = new BAlert(_T("GameOver"), text.String(), _T("Cancel"), _T("View Highscores"), _T("New Game"), B_WIDTH_AS_USUAL, B_OFFSET_SPACING);
	ali->SetShortcut(0, B_ESCAPE);
	ali->TextView()->SetFont(be_fixed_font);
	
	int ret = ali->Go();

	switch (ret) {
		case 0:
			//be_app->PostMessage(B_QUIT_REQUESTED);
			break;

		case 1:
			ViewHighscores();
			break;
		
		case 2:
			gameview->NewGame( settings.GameMode );
			break;
	}
}

void
PCSWindow::ViewHighscores()
{
	BString text;
	char buf[1024];
	
	text << "Highscores\n\n";
	
	sprintf(buf, "Nr.\t%-10s\t%-40s\t%s\n", _T("Score"), _T("Name"), _T("Date"));
	text << buf;
	for (int i=0;i<10;i++) {
		sprintf(buf, "%2d.\t%06d\t%-40s\t%s\n", i+1, highscores[i].Score, highscores[i].Name.String(), highscores[i].Date.String());
		text << buf;
	}
	
	BAlert *ali = new BAlert(_T("Highscores"), text.String(), _T("Reset"), _T("Ok"));
	ali->ResizeTo(400,250);
	ali->SetShortcut(1, B_ESCAPE);
	int ret = ali->Go();
	if (ret == 0) {
		ResetHighscore();
		PostMessage(MEN_F_VIEWHIGH);
	}
}

void
PCSWindow::ResetHighscore()
{
	for (int i=0;i<10;i++) {
		highscores[i].Name = "<noone>";
		highscores[i].Date = "<never>";
		highscores[i].Score = 10*(10-i);
		highscores[i].LongestChain = 10-i;
	}
}

void
PCSWindow::SortHighscores()
{	
	for (int i=0;i<10;i++) {
		int max	 = highscores[i].Score;
		int maxn = i;
		
		for (int o=i;o<11;o++) {
			int moritz = highscores[o].Score;
			if (moritz > max) {
				max = moritz;
				maxn = o;
			}
		}
		
		if (i!=maxn) {
			score_t dummy;
			dummy = highscores[i];
			highscores[i] = highscores[maxn];
			highscores[maxn] = dummy;
		}
	}
}


void
PCSWindow::UpdateOption(bool& opt)
{
	opt = (opt==false);

	UpdateOptions();
	gameview->Invalidate();
}

void
PCSWindow::UpdateOptions()
{
	// Unused:
	menUseBitmaps->SetEnabled(false);
	menAlternativeColors->SetMarked( settings.AlternativeColors );
	menShade->SetEnabled(false);
	menBubble->SetEnabled(false);
	menAnimation->SetEnabled(false);

	menSound->SetEnabled(false);
	menQuickMode->SetEnabled(false);
	
	// update:
	menShowStatus->SetMarked( settings.ShowStatus );
}
