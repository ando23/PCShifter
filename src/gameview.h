#ifndef GAMEVIEW
#define GAMEVIEW

#include "pcs_options.h"
#include "default.h"
#include "gamestructs.h"

#include <time.h>
//#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <Bitmap.h>


class GameView : public BView
{
public:
					GameView(BRect frame, settings_t *set);
	virtual			~GameView();
	
	virtual void	Draw(BRect rect);
	virtual void	MouseDown(BPoint point);
	virtual void	FrameResized(float width, float height);
	
	// initialise/start new game
	void			NewGame(GameMode_t newMode);
	
	//
	int				Score();
	int				LongestChain();

private:
	void			mark(int x, int y, const int col, int& anz);
	int				MarkClicked( int x, int y );
	void			GetColor( int color, rgb_color& col );
	BBitmap*		GetBitmap(int color);
	int				RemoveMarked();
	void			Collapse();
	
	// sets all tiles to marked=false, does not invalidate
	inline void		DeselectAll();

	void			NewRow(bool full = false);	
	void			UpdateScore(int32 tiles);	
	
	// returns true, if there is atleast one chain left
	bool			CheckChainLeft();
	
	// disables the view and notifies the window
	void			GameOver();
	
	void			LoadBitmaps();

	
private:
	field			board[BOARD_HEIGHT+1][BOARD_WIDTH+1]; // 1..10, 1..10

	time_t			timeGame;
	time_t			timeStart;
	bool			playing;
	
	int				score;
	int				scoreLongestChain;
	
	BString			sStatus;
	
	// for picking and drawing:
	float			abs_x, abs_y, radius_x, radius_y;
	
	settings_t		*settings;
	
	BBitmap*	_bitmapsSmall[6];
	BBitmap*	_bitmapsLarge[6];

	bool			_debug;
};


#endif

