#include "hoa.h"

int pwrmate_open(pwrmate_state *pwm) {
	pwm->fd = open(PWRMATE_DEV, O_RDONLY);
	memset(&pwm->new, 0, 6);
	memset(&pwm->old, 0, 6);
	pwm->timeout.tv_sec = 0;
	pwm->timeout.tv_usec = 100000;
	return pwm->fd;
}

void pwrmate_event(int fd, short event, void *arg) {

	pwrmate_state *pwm = arg;
	int r;

	switch (event) {
		case EV_READ:
			r = read(fd, pwm->new, 6);
			if ((pwm->new[0] == 0x01) &&
				(pwm->old[0] == 0x00)) {
				denon_toggle_mute(pwm->avr);
			} else {
				if (!denon_is_muted(pwm->avr)) {
					if (pwm->new[1] < 0x7f) {
						denon_vol_up(pwm->avr);
					}
					if (pwm->new[1] > 0x7f) {
						denon_vol_dn(pwm->avr);
					}
				}
			}
			memcpy(&pwm->old, &pwm->new, 6);
			break;
		case EV_TIMEOUT:
			printf("%s timeout fired!\n", __FUNCTION__);
			break;
	}
	return;
}
