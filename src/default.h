#include <be/AppKit.h>
#include <be/InterfaceKit.h>
#include <be/SupportKit.h>

#define APP_SIGNATURE		"application/x-vnd.PCShifter"
#define	APP_SETTINGS_FILE	"pcshifter_settings"


#define	VERSION_ACTUAL	"0.3"
#define	VERSION_		"PCShifter Ver."VERSION_ACTUAL
#define VERSION			"PCShifter Ver."VERSION_ACTUAL
#define ABOUT_BOX		VERSION"\n\n(C)2003-2016 Andreas Herzig (N3S)\n\nContact:\tbeos@herzig-net.de\n"

#define STR_HOW_TO_PLAY \
	"1. Click on a colored ball to mark it. If there are more balls of the same color adjacent to it, they will be marked too.\n\n" \
	"2. Click on the marked balls to remove them.\n\n" \
	"3. You get n*(n-1) points for the removed balls.\n\n" \
	"4. GOTO 1"


#define _T(x)			x


#define	NONE			0
#define	RED				1
#define YELLOW			2
#define GREEN			3
#define BLUE			4
#define PURPLE			5
#define MAX_COLOR		6

// SHARED MESSAGES:
#define	MSG_GAMEOVER	'OVER'

#define BOARD_WIDTH		11
#define BOARD_HEIGHT	11


