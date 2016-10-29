#ifndef GAMESTRUCTS
#define GAMESTRUCTS

struct field
{
	int		color;
	bool	marked;	
};

enum	GameMode_t
{	
	modeNormal=1,
	modeContinious, modeShifter, modeMegaShifter, modeBricks
};

struct settings_t
{
	bool	UseBitmaps;
	bool	AlternativeColors;
	bool	Sound;
	bool	Shade;
	GameMode_t GameMode;
	bool	QuickMode;
	bool	Bubble;
	bool	Animation;
	bool	ShowStatus;
	
	// PDAMode
	// Animation
};


struct score_t
{
	BString	Name;
	int		Score;
	int		LongestChain;
	BString	Date;
};

#endif