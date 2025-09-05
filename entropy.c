/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <csjp@sqrt.ca> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Christian SJ Peron 
 * ----------------------------------------------------------------------------
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vis.h>
#include <ctype.h>
#include <assert.h>

static int sort, nsymbols, symbwidth, probwidth, ientwidth, tentwidth;
static char *tflag;

#define	ENTROPY_VERSION	"2.0"
#define HDRSIZE(x)	(sizeof(x) - 1)
#define	SYMHEADER	"SYMBOL"
#define	PROBHEADER	"PROB."
#define	IENTHEADER	"I-ENTROPY"
#define	TENTHEADER	"T-ENTROPY"
#define	NSYMBOLS	0xFFU
#define SYMBOL_FOREACH(x)	for (x = 0; x < NSYMBOLS; x++)

struct cinfo {
        int		freq;
        char		*symbol;
        char		*sfreq;
        char		*ientr;
        char		*tentr;
};
static struct cinfo table[NSYMBOLS];

static inline unsigned 
bitsperchar(void)
{
	unsigned ret;
	char x;

	ret = 0;
	x = 0x1;
#ifdef _LITTLE_ENDIAN
	for (; x; x <<=1)
#else
	for (; x; x >>=1)
#endif	/* XXX PDP ENDIAN? */
		ret++;
	return (ret);
}

static int
cinfo_cmp(struct cinfo *a, struct cinfo *b)
{
	if (a->freq > b->freq)
		return (-1);
	else
		return (1);
}

static void
usage(void)
{
	fprintf(stderr,
	    "usage: entropy [-s] [-t string]\n");
	exit(1);
}

int
main(int argc, char *argv [])
{
	char *sfmt, buf[64];
	double Oentropy;
	FILE *fp;
	int c, set, ch, i;
	float ientr, tentr;

	while ((ch = getopt(argc, argv, "st:V")) != -1)
		switch (ch) {
		case 's':
			sort++;
			break;
		case 't':
			tflag = optarg;
			break;
		case 'V':
			printf("entropy version %s\n",
			     ENTROPY_VERSION);
			exit(0);
			break;
		default:
			usage();
		}
	fp = stdin;
	if (tflag) {
		set = strlen(tflag);
		while (*tflag)
			table[(unsigned)*tflag++].freq++;
	} else {
		set = 0;
		while ((c = getc(fp)) != EOF) {
			set++;
			table[(unsigned)c].freq++;
		}
        }
	symbwidth = HDRSIZE(SYMHEADER);
	probwidth = HDRSIZE(PROBHEADER);
	ientwidth = HDRSIZE(IENTHEADER);
	tentwidth = HDRSIZE(TENTHEADER);
	Oentropy = 0;
	SYMBOL_FOREACH(i) {
		if (table[i].freq == 0)
			continue;
		nsymbols++;
		ientr = log((double)table[i].freq / (double)set) / log(2);
		tentr = ientr * (double)table[i].freq;
		Oentropy += tentr;
		sfmt = isgraph(i) ? "'%c'" : "0x%02x";
		snprintf(buf, sizeof(buf) - 1, sfmt, i);
		table[i].symbol = strdup(buf);
		ch = snprintf(buf, sizeof(buf) - 1, "%u/%u",
		    table[i].freq, set);
		table[i].sfreq = strdup(buf);
		if (ch > probwidth)
			probwidth = ch;
		ch = snprintf(buf, sizeof(buf) - 1, "%f", ientr * -1);
		table[i].ientr = strdup(buf);
		if (ch > ientwidth)
			ientwidth = ch;
		ch = snprintf(buf, sizeof(buf) - 1, "%f", tentr * -1);
		table[i].tentr = strdup(buf);
		if (ch > tentwidth)
			tentwidth = ch; 
	}
	if (sort)
		qsort(&table[0], NSYMBOLS, sizeof(struct cinfo),
		    (void *)cinfo_cmp);
	printf("%-*.*s %-*.*s %-*.*s %-*.*s\n",
	    symbwidth, symbwidth, SYMHEADER,
	    probwidth, probwidth, PROBHEADER,
	    ientwidth, ientwidth, IENTHEADER,
	    tentwidth, tentwidth, TENTHEADER);
	SYMBOL_FOREACH(i) {
		if (table[i].freq == 0)
			continue;
		printf("%-*.*s %-*.*s %-*.*s %-*.*s\n",
		    symbwidth, symbwidth, table[i].symbol,
		    probwidth, probwidth, table[i].sfreq,
		    ientwidth, ientwidth, table[i].ientr,
		    tentwidth, tentwidth, table[i].tentr);
		free(table[i].symbol);
		free(table[i].sfreq);
		free(table[i].ientr);
		free(table[i].tentr);
	} 
	printf("MAX theorectical compression: %%%03.3f\n",
	    (100 * (1 - ((Oentropy / (double)(set * bitsperchar())) * -1))));
	printf("DISTINCT SYMBOLS: %d (%u contiguous bits per symbol)\n", 
	    nsymbols, bitsperchar());
	return (0);
}
