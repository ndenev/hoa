#include "hoa.h"

int mplayer_send(mplayer *mpl, char *str) {
        snprintf(mpl->buf, sizeof(mpl->buf), "%s\n", str);
        return write(mpl->fd, mpl->buf, strlen(mpl->buf));
}


int mplayer_open(mplayer *mpl) {
	mpl->fd = open("/home/ndenev/.mplayer/remote.ctrl", O_RDWR);
	return mpl->fd;
}
