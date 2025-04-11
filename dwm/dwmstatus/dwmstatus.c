/*
 * Copy me if you can.
 * by 20h
 */

#define _BSD_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <X11/Xlib.h>


enum batstat {
	BAT_CHARGING,
	BAT_DISCHARGING,
	BAT_NEITHER,
};


static Display *dpy;


static double
round(double f)
{
	long long i = f;
	if (f - (double)i >= 0.5) return i + 1;
	return i;
}


char *
smprintf(char *fmt, ...)
{
	va_list fmtargs;
	char *ret;
	int len;

	va_start(fmtargs, fmt);
	len = vsnprintf(NULL, 0, fmt, fmtargs);
	va_end(fmtargs);

	ret = malloc(++len);
	if (ret == NULL) {
		perror("malloc");
		exit(1);
	}

	va_start(fmtargs, fmt);
	vsnprintf(ret, len, fmt, fmtargs);
	va_end(fmtargs);

	return ret;
}


char *
mktimes(char *fmt)
{
	char buf[129];
	time_t tim;
	struct tm *timtm;

	tim = time(NULL);
	timtm = localtime(&tim);
	if (timtm == NULL)
		return smprintf("");

	if (!strftime(buf, sizeof(buf)-1, fmt, timtm)) {
		fprintf(stderr, "strftime == 0\n");
		return smprintf("");
	}

	return smprintf("%s", buf);
}


float
getbat(void)
{
	static char snow[128];
	static char sfull[128];

	FILE * fnow;
	FILE * ffull;

	// /sys/class/power_supply/BAT0/charge_{now,full}
	long now;
	long full;

	fnow = fopen("/sys/class/power_supply/BAT0/charge_now", "r");
	ffull = fopen("/sys/class/power_supply/BAT0/charge_full", "r");

	fgets(snow, sizeof(snow), fnow);
	fgets(sfull, sizeof(sfull), ffull);

	fclose(fnow);
	fclose(ffull);

	now = strtol(snow, NULL, 0);
	full = strtol(sfull, NULL, 0);

	return (float)now / (float)full * 100.;
}


int
getbatstat()
{
	FILE * fp;
	char c;

	fp = fopen("/sys/class/power_supply/BAT0/status", "r");
	c = fgetc(fp);
	fclose(fp);

	switch (c) {
		default:
		case 'D': return BAT_DISCHARGING;
		case 'C': return BAT_CHARGING;
		case 'N': return BAT_NEITHER;
	}
}


int
main(void)
{
	char *status, *time, *statstr;;
	int batstat;
	float bat;

	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "dwmstatus: cannot open display.\n");
		return 1;
	}

	int cnt;
	for (cnt = 0;;sleep(1), cnt++) {
		time = mktimes("%a %b %d %I:%M %p");
		batstat = getbatstat();
		bat = getbat();

		#define STATSTR "%.0f%% | %s"
		switch (batstat) {
			case BAT_CHARGING:
				statstr = " ^"STATSTR;
				break;
			case BAT_NEITHER:
				statstr = " *"STATSTR;
				break;
			case BAT_DISCHARGING:
				statstr = " "STATSTR;
				break;
		}

		status = smprintf(statstr, round(bat), time);

		// draw status to screen
		XStoreName(dpy, DefaultRootWindow(dpy), status);
		XSync(dpy, False);

		free(time);
		free(status);
	}

	XCloseDisplay(dpy);

	return 0;
}
