/* denon global status vars */

/* as defined in AVR-2807SerialProtocol.pdf */
#define MAX_COMM_DATALEN 135

/* volume levels */
typedef struct {
	int master;
	int max;
	int mute;
	int fl;
	int fr;
	int c;
	int sw;
	int sl;
	int sr;
	int sbl;
	int sbr;
	int sb;
} volume;

/* power */
typedef struct {
	int main;
	int zone1;
	int zone2;
} power;

/* serial io filedesc */
typedef struct {
	int fd;
	struct termios topt;
} port;

/* buffer sizes may be excess, but it's better to be on the safe side */
typedef struct {
	unsigned char video[MAX_COMM_DATALEN];
	unsigned char audio[MAX_COMM_DATALEN];
	unsigned char mode[MAX_COMM_DATALEN];
} input;

typedef struct {
	volume vol;
	power pwr;
	port  prt;
	input in;
	/* surround mode */
	unsigned char surr[MAX_COMM_DATALEN];
	/* i/o buffer */
	unsigned char buf[MAX_COMM_DATALEN];
	struct event ev;
	struct timeval timeout;
} avr_state;

int  denon_open(avr_state *);
void denon_dump_state(avr_state *);
void denon_toggle_mute(avr_state *);
void denon_toggle_power(avr_state *);
void denon_event(int, short, void *);
int  denon_send(avr_state *, char *);
void denon_vol_up(avr_state *);
void denon_vol_dn(avr_state *);
int  denon_sync(avr_state *);
int  denon_is_muted(avr_state *);
