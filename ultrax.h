/* Logitech UltraX media remote key codes */
/* key group 1 and 2 are read from the second uhid */
/* key group 1 (last byte is 3) */
#define KEYGROUP1	0x00000003
#define KEY_FMRADIO	0x00000400
#define KEY_INFO	0x02000000
#define KEY_REC		0x00008000
#define KEY_STOP	0x00100000
#define KEY_PREV	0x00000200
#define KEY_NEXT	0x00000100
#define KEY_PLAY	0x00000800
#define KEY_REW		0x04000000
#define KEY_FWD		0x08000000
#define KEY_BACK	0x00040000
#define KEY_CHUP	0x00010000
#define KEY_CHDN	0x00020000
#define KEY_VOLUP	0x00002000
#define KEY_VOLDN	0x00004000
#define KEY_MUTE	0x00001000
/* key group 2 (last byte is 4) */
#define KEYGROUP2	0x00000004
#define KEY_DVDMENU	0x00040000
#define KEY_ANGLE	0x00010000
#define KEY_SAP		0x00100000
#define KEY_LASTCH	0x00400000
#define KEY_LANG	0x00020000
#define KEY_REPEAT	0x00800000
#define KEY_SUBTITLE	0x00080000
#define KEY_SHUFFLE	0x00000400
#define KEY_TELETEXT	0x00200000
#define KEY_START	0x01000000
#define KEY_HOME	0x00000100
#define KEY_CLOSE	0x02000000
#define KEY_TV		0x00000200
#define KEY_DVD		0x00008000
#define KEY_PICS	0x00001000
#define KEY_VIDS	0x00002000
#define KEY_MUSIC	0x00000800

/* key codes from the first uhid device */
/* these are the directional controls and keypad */
#define KEY_1		0x1e
#define KEY_2		0x1f
#define KEY_3		0x20
#define KEY_4		0x21
#define KEY_5		0x22
#define KEY_6		0x23
#define KEY_7		0x24
#define KEY_8		0x25
#define KEY_9		0x26
#define KEY_0		0x27
#define KEY_CLR		0x4c
#define KEY_ENT		0x58
#define KEY_UP		0x52
#define KEY_DOWN	0x51
#define KEY_LEFT	0x50
#define KEY_RIGHT	0x4f
#define KEY_OK		0x28

/* directional keys typematic delay */
#define	DKEYS_DELAY 10

typedef struct {
	int fd;
	struct event ev;
	avr_state *avr;
	mplayer   *mpl;
	struct timeval timeout;
	unsigned char old[8];
	unsigned char new[8];
	int delay_up;
	int delay_down;
	int delay_left;
	int delay_right;
} ux1_state;

typedef struct {
	int fd;
	struct event ev;
	avr_state *avr;
	mplayer *mpl;
	struct timeval timeout;
	unsigned char old[4];
	unsigned char new[4];
} ux2_state;

/* function prototypes */
void ultrax1_event(int, short, void *);
void ultrax2_event(int, short, void *);
int ultrax1_open(ux1_state *);
int ultrax2_open(ux2_state *);

