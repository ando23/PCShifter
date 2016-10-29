#include "pcs_options.h"

#ifndef PCSWINDOW
#define PCSWINDOW

#include "default.h"
#include "gameview.h"

#define MEN_F_NEW		'Fnew'
#define MEN_F_VIEWHIGH	'Fhis'
#define MEN_F_QUIT		'quit'

#define	MEN_M_NORMAL	'MoNo'
#define MEN_M_CONT		'MoCo'

#define B_O_UseBitmaps			'OusB'
#define B_O_AlternativeColors	'Oalt'
#define B_O_Sound				'Osnd'
#define B_O_Shade				'Oshd'
#define B_O_QuickMode			'Oqck'
#define B_O_Bubble				'Obub'
#define B_O_Animation			'Oani'
#define B_O_ShowStatus			'OsSt'


#define MEN_H_UNDO		'Undo'
#define MEN_H_HOWPLAY	'How2'
#define MEN_H_ABOUT		'Habt'


class PCSWindow : public BWindow
{
public:
			PCSWindow(BRect frame, const char* Title);

	void	AllAttached();

	bool	QuitRequested();
	
	//void Save(config_t* conf);
	
	void	MessageReceived(BMessage* msg);	

	
	void	NewMode(GameMode_t mode);
	
	void	Undo();
	void	AboutRequested();
	
	void	ViewHighscores();
	

protected:
	void	HandleGameOver();
	
	void	LoadSettings();
	void	SaveSettings();
		
	void	ResetHighscore();
	void	SortHighscores();
	
	void	UpdateOption(bool& opt);
	void	UpdateOptions();

private:
	GameView	*gameview;
	BMenuBar	*menubar;
	BMenu		*menFile, *menMode, *menOptions, *menHelp;

	// Mode
	BMenuItem	*menModeNormal, *menModeContinious;
	
	// Options
	BMenuItem	*menUseBitmaps, *menAlternativeColors, *menSound, *menShade,
				*menQuickMode, *menBubble, *menAnimation, *menShowStatus;
	
	settings_t	settings;
	
	score_t		highscores[11];
};

#endif