#ifndef RGB2HSV_H
#define RGB2HSV_H

// ----------------------------------------------------------------------
#define MAXCOLOR 0x0ffff
#define MAXCOLp1 0x10000
#define GRADZAHL 1092					/* Anzahl für 1 Grad im 360 Grad Kreis */
#define WINKEL60 0x10000



// ----------------------------------------------------------------------
struct HSV {
	long h; // winkel
	unsigned short s; // intensitaet
	unsigned short v; // Helligkeit
};

struct RGB {
	unsigned short r,g,b;
};

// ----------------------------------------------------------------------
bool changeRGB2HSV(struct RGB *rgb,struct HSV *hsv);
bool changeHSV2RGB(struct HSV *hsv,struct RGB *rgb);

#endif
