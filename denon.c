#include "hoa.h"

int denon_send(avr_state *avr, char *str) {
	
	int r;
	snprintf(avr->buf, sizeof(avr->buf), "%s\r", str);
	r = write(avr->prt.fd, avr->buf, strlen(avr->buf));
	tcdrain(avr->prt.fd);
	//usleep(200000);
	return r;
}

void denon_vol_up(avr_state *avr) {
	denon_send(avr, "MVUP");
}

void denon_vol_dn(avr_state *avr) {
	denon_send(avr, "MVDOWN");
}

void denon_toggle_mute(avr_state *avr) {
	if (avr->vol.mute == 1) {
		denon_send(avr, "MUOFF");
	} else if (avr->vol.mute == 0) {
		denon_send(avr, "MUON");
	} else {
		printf("unknown mute value = %d\n", avr->vol.mute);
	}
}

int denon_is_muted(avr_state *avr) {
	return avr->vol.mute;
}

void denon_toggle_power(avr_state *avr) {
	if (avr->pwr.main == 1) {
		denon_send(avr, "PWSTANDBY");
	} else if (avr->pwr.main == 0) {
		denon_send(avr, "PWON");
	} else {
		printf("unknown power value = %d\n", avr->vol.mute);
	}
}

int denon_sync(avr_state *avr) {

	/* XXX this is called before the call to event_dispatch() */
	printf("syncing with DENON AVR-2807...\n");

	/* first run may not succeed */
	denon_send(avr, "PW?");
	usleep(200000);
	event_loop(EVLOOP_ONCE);
	event_loop(EVLOOP_ONCE);

	/* so retry */
	denon_send(avr, "PW?");
	usleep(200000);
	event_loop(EVLOOP_ONCE);
	event_loop(EVLOOP_ONCE);

	denon_send(avr, "MV?");
	usleep(200000);
	event_loop(EVLOOP_ONCE);
	event_loop(EVLOOP_ONCE);

	denon_send(avr, "MU?");
	usleep(200000);
	event_loop(EVLOOP_ONCE);
	
	denon_send(avr, "ZM?");
	usleep(200000);
	event_loop(EVLOOP_ONCE);

	denon_send(avr, "SI?");
	usleep(200000);
	event_loop(EVLOOP_ONCE);

	denon_send(avr, "SV?");
	usleep(200000);
	event_loop(EVLOOP_ONCE);

	denon_send(avr, "SD?");
	usleep(200000);
	event_loop(EVLOOP_ONCE);

	denon_send(avr, "MS?");
	usleep(200000);
	event_loop(EVLOOP_ONCE);

	printf("done.\n");

	printf("dumping AVR state : \n");

	denon_dump_state(avr);

	return 0;
}

#define IS(str)	(!strncmp(str, avr->buf, strlen(str)))
void denon_event(int fd, short event, void *arg) {

	avr_state *avr = arg;

	int r;

	switch (event) {
		case EV_READ:
			r = read(fd, avr->buf, sizeof(avr->buf));
			if (r <= 0) break;
			/* properly terminate result string */
			avr->buf[r-1] = '\0';
			/* the data has newline already */
			//printf("%s", avr->buf);
			if IS("MUON") {
				avr->vol.mute = 1;
			} else if IS("MUOFF") {
				avr->vol.mute = 0;
			} else if IS("PWON") {
				avr->pwr.main = 1;
			} else if IS("PWSTANDBY") {
				avr->pwr.main = 0;
			} else if IS("MVMAX") {
				avr->vol.max = (int)strtol(avr->buf + strlen("MVMAX"),(char **)NULL,10);
			} else if IS("MV") {
				avr->vol.master = (int)strtol(avr->buf + strlen("MV"),(char **)NULL,10);
			} else if IS("ZMON") {
				avr->pwr.zone1 = 1;
			} else if IS("ZMOFF") {
				avr->pwr.zone1 = 0;
			} else if IS("SI") {
				memcpy(avr->in.audio, avr->buf+2, r-2);
			} else if IS("SV") {
				memcpy(avr->in.video, avr->buf+2, r-2);
			} else if IS("SD") {
				memcpy(avr->in.mode, avr->buf+2, r-2);
			} else if IS("MS") {
				memcpy(avr->surr, avr->buf+2, r-2);
			}
			break;
		case EV_TIMEOUT:
			//denon_sync(avr);
			break;
		default:
			break;
	}
	
	denon_dump_state(avr);

	return;
}

void denon_dump_state(avr_state *avr) {
	printf( "volume master	: %d\n"
		"volume max	: %d\n"
		"volume mute	: %d\n"
		"power main	: %d\n"
		"power zone 1	: %d\n"
		"input source	: %s\n"
		"video source	: %s\n"
		"input mode	: %s\n"
		"surround mode	: %s\n",
		avr->vol.master,
		avr->vol.max,
		avr->vol.mute,
		avr->pwr.main,
		avr->pwr.zone1,
		avr->in.audio,
		avr->in.video,
		avr->in.mode,
		avr->surr
	);
	printf("\n\n\n");
}

#if 0
void denon_toggle_power () {
	/* query status (this will invoke the sig handler */
	//SEND(denon, "PW?\r\n");
	if (avr->pwr.main == 1) {
		//SEND(denon, "PWSTANDBY\r\n");
	} else {
		//SEND(denon, "PWON\r\n");
	}
}
#endif

int denon_open(avr_state *avr) {
	
	if ((avr->prt.fd = open(AVR_DEV, O_RDWR | O_NOCTTY | O_NONBLOCK)) == -1)
		goto fail;

	if (cfsetspeed(&avr->prt.topt, B9600) == -1)
		goto fail;

	if (tcgetattr(avr->prt.fd, &avr->prt.topt) == -1)
		goto fail;	

	avr->prt.topt.c_cflag &= ~CSIZE;
	avr->prt.topt.c_cflag |= CS8;
	avr->prt.topt.c_cflag &= ~PARENB;
	avr->prt.topt.c_cflag &= ~CSTOPB;
	avr->prt.topt.c_oflag &= ~OPOST;
	avr->prt.topt.c_cflag |= CLOCAL;
	avr->prt.topt.c_cflag |= CREAD;
	
	if (tcsetattr(avr->prt.fd, TCSANOW, &avr->prt.topt) == -1)
		goto fail;

	//if (tcflush(avr->prt.fd, TCIOFLUSH))
	//	goto fail;

	avr->timeout.tv_sec = 5;
	avr->timeout.tv_usec = 0;
	
	return 0;

fail:
	printf("error opening and configuring prt %s : %s\n",
		AVR_DEV, strerror(errno));
	return -1;
}
