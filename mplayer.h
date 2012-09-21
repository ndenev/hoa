typedef struct {
	int fd;
	char buf[512];
} mplayer;

int mplayer_send(mplayer *, char *);
int mplayer_open(mplayer *);
