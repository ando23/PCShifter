#include "gameview.h"
#include <Bitmap.h>
#include <TranslatorRoster.h>
#include <TranslationUtils.h>

#define base BView


GameView::GameView(BRect frame, settings_t *set) :
	base(frame, "GameView", B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS | B_FULL_UPDATE_ON_RESIZE /*| B_PULSE_NEEDED */)
	, settings(set)
	, _debug(false)
{
	srand(time(0));
	//NewGame(settings->GameMode);
	
	abs_x = (frame.right - frame.left) / (BOARD_HEIGHT+2);
	abs_y = (frame.bottom - frame.top) / BOARD_HEIGHT;
	
	radius_x = abs_x/3;
	radius_y = abs_y/3;
	
	for (int x=1; x<BOARD_WIDTH+1; x++) {
		board[0][x].color = NONE;
		board[0][x].marked = false;
		
		for (int y=1; y<BOARD_WIDTH+1; y++) {
			board[y][x].color = NONE;
			board[y][x].marked = false;
		}
	}
	
	LoadBitmaps();
}

GameView::~GameView()
{
}


void
GameView::Draw(BRect frame)
{
	rgb_color high = HighColor();

	pattern pat		= B_SOLID_HIGH;
	pattern pat_m	= B_SOLID_HIGH;
	
	
	BRect f = Bounds();
	
	SetHighColor(255,255,255);
	FillRect(f);
	
	SetHighColor(220,220,220);
	f.right = abs_x;
	FillRect(f);
	f.OffsetBy((BOARD_WIDTH+1)*abs_x,0);
	FillRect(f);
	
	if (settings->GameMode == modeContinious) {	
		for (int y=0;y<BOARD_HEIGHT;y++) {
			int col = board[y+1][0].color;
			if (col!=NONE) {
				BPoint pos(0.5*abs_x, (y+0.5)*abs_y);
				
				rgb_color h;
				GetColor( col, h );
				
				SetHighColor(h);
				FillEllipse(pos, radius_x/2, radius_y/2, pat);
				SetHighColor(0,0,0,0);
				StrokeEllipse(pos, radius_x/2, radius_y/2, pat);		
			}
		}
	}
			
	for (int y=0;y<BOARD_HEIGHT+1;y++) {
		float pos_y = (y+0.5)*abs_y;
			
		for (int x=1;x<BOARD_WIDTH+1;x++) {
			BPoint pos((x+0.5)*abs_x, pos_y);
			
			int col = board[y+1][x].color;
			
			if (col!=NONE) {
				rgb_color h;
				GetColor( col, h );
				
				BRect tile = BRect( x * abs_x, y * abs_y, (x+1) * abs_x - 1, (y+1)*abs_y - 1 );
				
				if (settings->UseBitmaps) {
					BRect bitmapRect = tile;
					bitmapRect.InsetBy(abs_x / 20, abs_y / 20);
					BBitmap* bitmap = GetBitmap(col);
					if (bitmap != NULL) {
						DrawBitmap( bitmap, bitmapRect);
					}
					if (board[y+1][x].marked) {
						SetHighColor(h.red, h.green, h.blue, 128);
						SetBlendingMode(B_CONSTANT_ALPHA, B_ALPHA_OVERLAY);
						SetDrawingMode(B_OP_ALPHA);
						FillRect(tile, pat_m );
						SetDrawingMode(B_OP_COPY);
					}
				}
				else {
				
					if (board[y+1][x].marked) {
						SetHighColor(h);
						FillRect(tile, pat_m );
						SetHighColor(0,0,0,0);
						StrokeEllipse(pos, radius_x, radius_y, pat);
					}
					else {
						
						SetHighColor(h);
						FillEllipse(pos, radius_x, radius_y, pat);
						SetHighColor(0,0,0,0);
						StrokeEllipse(pos, radius_x, radius_y, pat);
					}
				}
			}
		}
	}

	// Score	
	SetHighColor(0,0,0);
	SetLowColor(220,220,220);
	BString sScore;
	sScore << _T("Score: ") << score;

	BPoint pos( Bounds().right-abs_x/2, Bounds().Height()/2+StringWidth(sScore.String())/2);
	
	BFont font;
	font.SetRotation(90.0);
	SetFont(&font, B_FONT_ROTATION);
	DrawString(sScore.String(), pos);
	
	// Status
	if (settings->ShowStatus) {
		BPoint pos2( Bounds().right, Bounds().Height()/2+StringWidth(sStatus.String())/2);
		DrawString(sStatus.String(), pos2);
	}
		
	SetHighColor(high);
}


void
GameView::MouseDown(BPoint point)
{
	if (!playing)
		return;
		
	sStatus = "";

	base::MouseDown(point);
	
	int pos_x = (int)(point.x / abs_x);
	int pos_y = (int)(point.y / abs_y) + 1;
	
	//printf("Pos: %d:%d\n", pos_x, pos_y);
	
	if (pos_x > 0 && pos_x < BOARD_WIDTH+1) {
		if (board[pos_y][pos_x].color != NONE) {
			if (board[pos_y][pos_x].marked) {				
				int anz = RemoveMarked();
				Collapse();
				
				score += anz*(anz-1);
				
				if (anz > scoreLongestChain)
					scoreLongestChain = anz;
				
				if (CheckChainLeft()==false) {
					sStatus = "Game Over!";
					
					Invalidate();
					GameOver();
				}
			}
			else {
				int anz =  MarkClicked(pos_x, pos_y);
				sStatus = "Balls marked: ";
				sStatus << anz;
				sStatus << " valued " << anz*(anz-1);
				sStatus << " points";				
			}
			Invalidate();
		}
	}
	else {
		DeselectAll();
		Invalidate();
	}
}


void
GameView::FrameResized(float width, float height)
{
	abs_x = (width) / (BOARD_WIDTH+2);
	abs_y = (height) / BOARD_HEIGHT;
	
	radius_x = abs_x/3;
	radius_y = abs_y/3;

	base::FrameResized(width, height);
}


void
GameView::GetColor( int color, rgb_color& col )
{
	if (_debug || settings->AlternativeColors) {
		switch (color) {
		case RED:
			col.red = 255, col.green = 0, col.blue = 0;
			break;
	
		case YELLOW:
			col.red = 255, col.green = 255, col.blue = 0;
			break;
	
		case GREEN:
			col.red = 200, col.green = 200, col.blue = 200;
			break;
	
		case BLUE:
			col.red = 0, col.green = 0, col.blue = 255;
			break;
	
		case PURPLE:
			col.red = 100, col.green = 100, col.blue = 100;
			break;
	
		default:
			col.red = 0, col.green = 0, col.blue = 0;
			break;
		};	
	}
	else {
		switch (color) {
		case RED:
			col.red = 255, col.green = 0, col.blue = 0;
			break;
	
		case YELLOW:
			col.red = 255, col.green = 255, col.blue = 0;
			break;
	
		case GREEN:
			col.red = 0, col.green = 255, col.blue = 0;
			break;
	
		case BLUE:
			col.red = 0, col.green = 0, col.blue = 255;
			break;
	
		case PURPLE:
			col.red = 200, col.green = 0, col.blue = 200;
			break;
	
		default:
			col.red = 0, col.green = 0, col.blue = 0;
			break;
		};
	}
}

BBitmap*
GameView::GetBitmap(int color)
{
	if (color < 0 || color >= MAX_COLOR)
		return NULL;
		
	BBitmap** list = _bitmapsSmall;
	return list[color];		
}

void
GameView::LoadBitmaps()
{
	_bitmapsSmall[NONE] = BTranslationUtils::GetBitmap( "res/kugel_weiss.bmp" );
	_bitmapsSmall[RED] = BTranslationUtils::GetBitmap( "res/kugel_rot.bmp" );
	_bitmapsSmall[YELLOW] = BTranslationUtils::GetBitmap( "res/kugel_gelb.bmp" );
	_bitmapsSmall[GREEN] = BTranslationUtils::GetBitmap( "res/kugel_gruen.bmp" );
	_bitmapsSmall[BLUE] = BTranslationUtils::GetBitmap( "res/kugel_blau.bmp" );
	_bitmapsSmall[PURPLE] = BTranslationUtils::GetBitmap( "res/kugel_orange.bmp" );

	_bitmapsLarge[NONE] = BTranslationUtils::GetBitmap( "res/kugel48_weiss.bmp" );
	_bitmapsLarge[RED] = BTranslationUtils::GetBitmap( "res/kugel48_rot.bmp" );
	_bitmapsLarge[YELLOW] = BTranslationUtils::GetBitmap( "res/kugel48_gelb.bmp" );
	_bitmapsLarge[GREEN] = BTranslationUtils::GetBitmap( "res/kugel48_gruen.bmp" );
	_bitmapsLarge[BLUE] = BTranslationUtils::GetBitmap( "res/kugel48_blau.bmp" );
	_bitmapsLarge[PURPLE] = BTranslationUtils::GetBitmap( "res/kugel48_orange.bmp" );
}



void
GameView::NewGame(GameMode_t newMode)
{
	sStatus = "";
	
	score = 0;
	scoreLongestChain = 0;
	
	settings->GameMode = newMode;

	for (int x=1; x<BOARD_WIDTH+1; x++) {
		// dirty: set line 0 (invalid) as NONE-colored, so that Collapse is easier ;-)
		board[0][x].color = NONE;
		board[0][x].marked = false;
		
		for (int y=1; y<BOARD_WIDTH+1; y++) {
			board[y][x].color = rand() % 5 + 1;
			board[y][x].marked = false;
		}
	}
	
	NewRow(true);
	Invalidate();
	
	// "enable" the view:
	playing = true;
}

void
GameView::NewRow(bool full)
{
	board[0][0].color = NONE;
	board[0][0].marked = false;
	
	for (int y=1; y<=BOARD_HEIGHT; y++) {
		board[y][0].color = NONE;
		board[y][0].marked = false;
	}

	if (settings->GameMode != modeNormal) {
		int h = rand() % 9;
		if (full)
			h = 0;
		for (int y=BOARD_HEIGHT; y>h; y--)
			board[y][0].color = rand() % 5 + 1;
	}
}


int
GameView::RemoveMarked()
{
	int anz = 0;
	
	for (int x=BOARD_WIDTH; x>=1; x--) {
		for (int y=BOARD_HEIGHT; y>=1; y--) {
			if (board[y][x].marked) {
				board[y][x].marked	= false;
				board[y][x].color	= NONE;
				
				anz++;
			}
		}
	}
	
	return anz;
}

void
GameView::Collapse()
{
	// drop
	for (int x=BOARD_WIDTH; x>=1; x--) {
		// drop tiles in this row:
		for (int y=BOARD_HEIGHT; y>1; y--) {
			int up = y-1;			
			while (board[y][x].color == NONE && up) {
				for (int yy=y; yy>0; yy--) {
					board[yy][x].color	= board[yy-1][x].color;
				}
				up--;
			}
		}
	}
	
	// shift right
	int x=BOARD_WIDTH, mx=BOARD_WIDTH;
	while (x>=1 && mx) {
		int anz = 0;
		for (int y=BOARD_HEIGHT; y>0; y--)
			if (board[y][x].color == NONE)
				anz++;
		
		if (anz >= BOARD_HEIGHT) {	
			for (int xx=x; xx>=1; xx--) {
				for (int y=1;y<BOARD_HEIGHT+1;y++) {
					board[y][xx] = board[y][xx-1];
				}
			}

			// Add a new row
			NewRow();
		}
		else {
			x--;
		}

		mx--;			
	}	
}


int
GameView::MarkClicked( int x, int y )
{
	const int col = board[y][x].color;
	
	// deselect:
	DeselectAll();
	
	if (col == NONE)
		return 0;
	
	
	int marked = 0;
	mark( x, y, col, marked );		

	if (marked<2) {
		DeselectAll();
	}

	return marked;
}

void
GameView::mark(int x, int y, const int col, int& anz)
{
	// stay within board, mark only same color, and don't mark twice:
	if (x<1 || x>BOARD_WIDTH || y<1 || y>BOARD_HEIGHT
	|| board[y][x].color!=col || board[y][x].marked)
		return;
	
	board[y][x].marked = true;
	anz++;
	
	// recurse and mark surrounding tiles:
	mark(x-1, y, col, anz);
	mark(x+1, y, col, anz);
	mark(x, y-1, col, anz);
	mark(x, y+1, col, anz);
}


void
GameView::DeselectAll()
{
	for (int iy = 1; iy <= BOARD_HEIGHT; iy++)
		for (int ix = 1; ix <= BOARD_WIDTH; ix++)
			board[iy][ix].marked = false;
}


bool
GameView::CheckChainLeft()
{
	bool ok = false;
	for (int iy = 1; iy <= BOARD_HEIGHT; iy++) {
		for (int ix = 1; ix <= BOARD_WIDTH; ix++) {
			int col = board[iy][ix].color;
			if (col != NONE) {
				int anz = 0;
				mark(ix,iy,col,anz);
				if (anz>1)
					ok = true;
					
				DeselectAll();
			}
		}
	}
	
	return ok;
}


void
GameView::GameOver()
{
	playing = false;
	Window()->PostMessage(MSG_GAMEOVER);
}


int
GameView::Score()
{
	return score;
}

int
GameView::LongestChain()
{
	return scoreLongestChain;
}
