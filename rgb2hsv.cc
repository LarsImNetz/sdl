#include "rgb2hsv.h"

bool allegleich(struct RGB *col) {
    if (col->r == col->g && col->g == col->b)
        return true;
    else
        return false;
}

// ----------------------------------------------------------------------
// int MAX(int a,int b) {
//     if (a > b) return a;
//     return b;
// }
// ----------------------------------------------------------------------

int sextant(struct RGB *col) {
//  pre: ein RGB value
// post: liefert den Winkelabschnitt

// Es gibt 6 Winkelarten.
// Grad        Typ
//   0- 60    0
//  60-120    1
// 120-180    2
// 180-240    3
// 240-300    4
// 300-360    5
    long r = col->r;
    long g = col->g;
    long b = col->b;

    if (r >  g && r >  b && g >= b) return 0;
    if (r <= g && r >  b && g >  b) return 1;
    if (r <  g && r <= b && g >  b) return 2;
    if (r <  g && r <  b && g <= b) return 3;
    if (r >= g && r <  b && g <  b) return 4;
    return 5;
}
// ----------------------------------------------------------------------

// long antiprop(long wert) {
//  pre: value 0 < wert < MAXCOLOR
// post: liefert den antiproportionalen wert zu MAXCOLOR
//        return MAXCOLOR - wert;
// }
// ----------------------------------------------------------------------

bool changeRGB2HSV(struct RGB *rgb,struct HSV *hsv) {
//  pre: RGB must be init
// post: RGB -> HSV

    long r,g,b;
    long h,s,v;
    long master;

    r = rgb->r & 0xffff;
    g = rgb->g & 0xffff;
    b = rgb->b & 0xffff;

    if (allegleich(rgb)) {                            // alle Farben sind gleich
        hsv->h = 0;                                        // es ist schwarz/weiß
        hsv->s = 0;
        hsv->v = r;                                        // nimm 1. Farbe
        return true;
    }

    switch(sextant(rgb)) {
    case 0:                                                // RG   G waechst!
        master= r;                                        // staerkste farbe
        h = WINKEL60 * (g - b) / master;    // 2. Farbe
        s = MAXCOLOR-b;
        v = master;
        break;
    case 1:                                                // RG   R faellt!
        master= g;
        h = 2 * WINKEL60 - WINKEL60 * (r - b) / master;
        s = MAXCOLOR-b;
        v = master;
        break;
    case 2:                                                // GB   B waechst!
        master= g;
        h = 2 * WINKEL60 + WINKEL60 * (b - r) / master;
        s = MAXCOLOR-r;
        v = master;
        break;
    case 3:                                                // GB   G faellt!
        master= b;
        h = 4 * WINKEL60 - WINKEL60 * (g - r) / master;
        s = MAXCOLOR-r;
        v = master;
        break;
    case 4:                                                // RB   R waechst!
        master= b;
        h = 4 * WINKEL60 + WINKEL60 * (r - g) / master;
        s = MAXCOLOR-g;
        v = master;
        break;
    case 5:                                                // RB   B faellt!
        master= r;
        h = 6 * WINKEL60 - WINKEL60 * (b - g) / master;
        s = MAXCOLOR-g;
        v = master;
        break;
    default:
        return false;            // dieser Fehler sollte eigentlich nicht auftreten.
    }

    hsv->h = h;
    hsv->s = s;
    hsv->v = v;
    return true;
}

// ======================================================================

bool changeHSV2RGB(struct HSV *hsv,struct RGB *rgb) {
    int tmp;                                                // temponaer
    int typ;
    unsigned long volume;
    unsigned long weiss;
    unsigned long r,g,b;
    long deltaA,deltaB;

    if (hsv->h >= 6*WINKEL60) hsv->h = 6*WINKEL60-1;

    typ = hsv->h >> 16;                                // Winkeltyp

    if (hsv->h == 0 && hsv->s == 0) {                // Schwarz weiß?
        tmp=hsv->v;                                        // ja
        rgb->r = tmp;
        rgb->g = tmp;
        rgb->b = tmp;
        return true;
    }

    volume = hsv->v;
    weiss = MAXCOLOR - hsv->s;

    switch(typ) {
    case 0:
        r = volume;                                                // rot   = v
        g = ( hsv->h * volume ) >> 16;                        // gruen = winkel
        deltaA = volume;
        deltaB = volume - g;
        b = ( deltaA * weiss ) >> 16;
        g +=( deltaB * weiss ) >> 16;
        break;

    case 1:
        r = ((2 * MAXCOLp1 - hsv->h) * volume) >> 16;
        g = volume;                                                // gruen = v
        deltaA = volume;
        deltaB = volume - r;
        b = ( deltaA * weiss ) >> 16;
        r +=( deltaB * weiss ) >> 16;
        break;

    case 2:
        g = volume;                                                // gruen = v
        b = ((hsv->h - 2 * MAXCOLp1) * volume) >> 16;     // blau waechst
        deltaA = volume;
        deltaB = volume - b;
        r = ( deltaA * weiss ) >> 16;
        b +=( deltaB * weiss ) >> 16;
        break;

    case 3:
        g    = ((4 * MAXCOLp1 - hsv->h) * volume) >> 16;
        b = volume;
        deltaA = volume;
        deltaB = volume - g;
        r = (deltaA * weiss ) >> 16;
        g +=(deltaB * weiss ) >> 16;
        break;

    case 4:
        r = ((hsv->h - 4 * MAXCOLp1) * volume ) >> 16;
        b = volume;
        deltaA = volume;
        deltaB = volume - r;
        g = (deltaA * weiss) >> 16;
        r +=(deltaB * weiss) >> 16;
        break;

    case 5:
        r = volume;
        b = ((6 * MAXCOLp1 - hsv->h) * volume) >> 16;
        deltaA = volume;
        deltaB = volume - b;
        g = (deltaA * weiss) >> 16;
        b +=(deltaB * weiss) >> 16;
        break;

    default:
        return false;
    }
    rgb->r = r;
    rgb->g = g;
    rgb->b = b;
    return true;
}

// ----------------------------------------------------------------------

// void printRGB(RGB &v) {
//     cout << "R: " << v.r << " G: " << v.g << " B: " << v.b << "\t";
// }

// ----------------------------------------------------------------------
// void printHSV(HSV &v) {
//     cout << "H: " << (int)(float(v.h)/float(4.27)) << " S: " << v.s << " V: " << v.v << endl;
// }
