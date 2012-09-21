#include "hoa.h"

void ultrax1_event(int fd, short event, void *arg) {

	ux1_state *ux1 = arg;
	int i, j, k;

	switch (event) {
		case EV_READ:
			read(ux1->fd, ux1->new, 8);
			for (i = 2; i <= 6; i++) {
				k = 0;
				if (ux1->new[i]) {
					for (j = 2; j <=6; j++) {
						if (ux1->new[i] == ux1->old[j]) {
							k = 1;
							break;
						}
					}
					if (!k) {
						switch (ux1->new[i]) {
							case KEY_1:
								denon_send(ux1->avr, "SIVDP");
								break;
							case KEY_2:
								denon_send(ux1->avr, "SIV.AUX");
								break;
							case KEY_3:
								denon_send(ux1->avr, "SIDBS");
								break;
							case KEY_4:
								printf("4\n");
								break;
							case KEY_5:
								printf("5\n");
								break;
							case KEY_6:
								printf("6\n");
								break;
							case KEY_7:
								printf("7\n");
								break;
							case KEY_8:
								printf("8\n");
								break;
							case KEY_9:
								printf("9\n");
								break;
							case KEY_0:
								printf("0\n");
								break;
							case KEY_CLR:
								printf("CLR\n");
								break;
							case KEY_ENT:
								printf("ENTR\n");
								break;
							case KEY_UP:
								ux1->delay_up = 0;
								denon_send(ux1->avr, "SSCUP");
								//write(mplayer, "menu up\n", 8);
								break;
							case KEY_DOWN:
								ux1->delay_down = 0;
								denon_send(ux1->avr, "SSCDN");
								//write(mplayer, "menu down\n", 10);
								break;
							case KEY_LEFT:
								ux1->delay_left = 0;
								denon_send(ux1->avr, "SSCLT");
								//write(mplayer, "menu cancel\n", 12);
								break;
							case KEY_RIGHT:
								ux1->delay_right = 0;
								denon_send(ux1->avr, "SSCRT");
								//write(mplayer, "menu ok\n", 8);
								break;
							case KEY_OK:
								denon_send(ux1->avr, "SSENT");
								//write(mplayer, "menu ok\n", 8);
								break;
							default:
								printf("unknown key code!\n");
								break;
						}
					}
				}
			}
			memcpy(ux1->old, ux1->new, 8);
			break;
		case EV_TIMEOUT:
			event_del(&ux1->ev);
			event_add(&ux1->ev, &ux1->timeout);
			for (i = 2; i <= 6; i++) {
				switch (ux1->new[i]) {
					case KEY_UP:
						if (ux1->delay_up < DKEYS_DELAY) {
							ux1->delay_up++;
							break;
						}
						printf("UP\n");
						break;
					case KEY_DOWN:
						if (ux1->delay_down < DKEYS_DELAY) {
							ux1->delay_down++;
							break;
						}
						printf("DOWN\n");
						break;
					case KEY_LEFT:
						if (ux1->delay_left < DKEYS_DELAY) {
							ux1->delay_left++;
							break;
						}
						printf("LEFT\n");
						break;
					case KEY_RIGHT:
						if (ux1->delay_right < DKEYS_DELAY) {
							ux1->delay_right++;
							break;
						}
						printf("RIGHT\n");
					break;
				}
			}
			break;
	}
}

void ultrax2_event(int fd, short event, void *arg) {

	ux2_state *ux2 = arg;

#define NEW *(u_int32_t*)&ux2->new
#define OLD *(u_int32_t*)&ux2->old
#define IS_ON(key) (NEW & key)
#define WAS_ON(key) (OLD & key)
#define TOG(key) ((IS_ON(key) && (!WAS_ON(key))))

	switch (event) {
		case EV_READ:
			read(ux2->fd, ux2->new, 4);
			if IS_ON(KEYGROUP1) {
				if TOG(KEY_FMRADIO)	printf("FMRADIO\n");
				if TOG(KEY_INFO)	printf("INFO\n");
				if TOG(KEY_REC)		printf("RECORD\n");
				if TOG(KEY_STOP)	printf("STOP\n");
				if TOG(KEY_PREV) {
					printf("PREV\n");
					mplayer_send(ux2->mpl, "seek_chapter -1 0");
				}
				if TOG(KEY_NEXT) {
					printf("NEXT\n");
					mplayer_send(ux2->mpl, "seek_chapter +1 0");
				}
				if TOG(KEY_PLAY) {
					printf("PLAY\n");
					mplayer_send(ux2->mpl, "pause");
				}
				if IS_ON(KEY_REW) {
					printf("REW\n");
					mplayer_send(ux2->mpl, "seek -1 0");
				}
				if IS_ON(KEY_FWD) {
					printf("FWD\n");
					mplayer_send(ux2->mpl, "seek +1 0");
				}
				if TOG(KEY_BACK) {
					printf("BACK\n");
					//write(mplayer, "menu hide\n", 7);
				}
				if TOG(KEY_CHUP)	printf("CHAN_UP\n");
				if TOG(KEY_CHDN)	printf("CHAN_DN\n");

				if IS_ON(KEY_VOLUP)
					denon_vol_up(ux2->avr);

				if IS_ON(KEY_VOLDN)
					denon_vol_dn(ux2->avr);

				if TOG(KEY_MUTE)
					denon_toggle_mute(ux2->avr);

			} else if IS_ON(KEYGROUP2) {

				if TOG(KEY_DVDMENU)	{
					printf("DVDMENU\n");
					//write(mplayer, "menu 1\n", 7);
				}
				if TOG(KEY_ANGLE)	printf("ANGLE\n");
				if TOG(KEY_SAP)		printf("SAP\n");
				if TOG(KEY_LASTCH)	printf("LAST_CH\n");
				if TOG(KEY_LANG)	printf("LANG\n");
				if TOG(KEY_REPEAT)	printf("REPEAT\n");
				if TOG(KEY_SUBTITLE) {
					printf("SUBTITLE\n");
					//write(mplayer, "sub_select\n", 11);
				}
				if TOG(KEY_SHUFFLE)	printf("SHUFFLE\n");
				if TOG(KEY_TELETEXT)	printf("TELETEXT\n");
				if TOG(KEY_START) {
					denon_toggle_power(ux2->avr);
				}
				if TOG(KEY_HOME)	printf("HOME\n");
				if TOG(KEY_CLOSE)	printf("CLOSE\n");
				if TOG(KEY_TV)		printf("TV\n");
				if TOG(KEY_DVD)		printf("DVD\n");
				if TOG(KEY_PICS)	printf("PICS\n");
				if TOG(KEY_VIDS)	printf("VIDS\n");
				if TOG(KEY_MUSIC)	printf("MUSIC\n");
			}
			memcpy(ux2->old, ux2->new, 4);
			break;

		case EV_TIMEOUT:
			/* reschedule event with reset timer */
			event_del(&ux2->ev);
			event_add(&ux2->ev, &ux2->timeout);
		
			/* these are for press-and-hold keys */
			if IS_ON(KEYGROUP1) {
				if IS_ON(KEY_VOLUP)
					denon_vol_up(ux2->avr);
				if IS_ON(KEY_VOLDN)
					denon_vol_dn(ux2->avr);
				if IS_ON(KEY_REW) {
					printf("REW\n");
				}
				if IS_ON(KEY_FWD) {
					printf("FWD\n");
				}
			}
			break;
	}
}

int ultrax1_open(ux1_state *ux1) {
	ux1->fd = open(ULTRAX1_DEV, O_RDONLY);
	ux1->timeout.tv_sec = 0;
	ux1->timeout.tv_usec = 50000;
	return ux1->fd;
}

int ultrax2_open(ux2_state *ux2) {
	ux2->fd = open(ULTRAX2_DEV, O_RDONLY);
	ux2->timeout.tv_sec = 0;
	ux2->timeout.tv_usec = 50000;
	return ux2->fd;
}
