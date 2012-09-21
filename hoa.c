#include "hoa.h"

int main () {

	avr_state avr;
	pwrmate_state pwm;
	ux1_state ux1;
	ux2_state ux2;
	mplayer	  mpl;

	memset(&avr, 0, sizeof(avr_state));
	memset(&pwm, 0, sizeof(pwrmate_state));
	memset(&ux1, 0, sizeof(ux1_state));
	memset(&ux2, 0, sizeof(ux2_state));

	denon_open(&avr);
	mplayer_open(&mpl);
	pwrmate_open(&pwm);
	ultrax1_open(&ux1);
	ultrax2_open(&ux2);

	ux1.avr = ux2.avr = pwm.avr = &avr;
	ux1.mpl = ux2.mpl = pwm.mpl = &mpl;

	/* seems like freebsd's uhid does not support kqueue? */
	setenv("EVENT_NOKQUEUE", "1", 1);

	event_init();

	event_set(&avr.ev, avr.prt.fd, EV_READ | EV_PERSIST, denon_event, &avr);
	event_set(&pwm.ev, pwm.fd, EV_READ | EV_PERSIST, pwrmate_event, &pwm);
	event_set(&ux1.ev, ux1.fd, EV_READ | EV_PERSIST, ultrax1_event, &ux1);
	event_set(&ux2.ev, ux2.fd, EV_READ | EV_PERSIST, ultrax2_event, &ux2);

	event_add(&avr.ev, NULL);
	event_add(&pwm.ev, NULL);
	event_add(&ux1.ev, &ux1.timeout);
	event_add(&ux2.ev, &ux2.timeout);

	denon_sync(&avr);

	event_dispatch();

	printf("error! event_dispatch() returned! code : %s\n", strerror(errno));

	return 0;

}
