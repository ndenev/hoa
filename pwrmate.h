typedef struct {
	int fd;
	unsigned char old[6];
	unsigned char new[6];
	struct event ev;
	struct timeval timeout;
	avr_state *avr;
	mplayer   *mpl;
} pwrmate_state;

extern pwrmate_state pwm;
int  pwrmate_open();
void pwrmate_event(int, short, void *);

