
#ifndef PLANET_H
#define PLANET_H


extern int rtable[65536], gtable[65536], btable[65536];
extern unsigned short **col;
extern int Width, Height;
extern int doshade;
extern unsigned short **shades;
extern int nocols;
extern double dd1;
extern double dd2;
extern double POW;
extern int shade;
extern double shade_angle;
extern double shade_angle2;
extern double M;
extern double r1,r2,r3,r4;
extern int Depth;

extern int latic;
extern int BLACK, WHITE, BACK, GRID, OUTLINE1, OUTLINE2, LOWEST, SEA, LAND, HIGHEST;
extern double vgrid, hgrid;
extern double **xxx, **yyy, **zzz;
extern double longi,lat,scale;
extern int debug;
extern char view;
extern double cla, sla, clo, slo;
#define PI 3.14159265358979
#define DEG2RAD 0.0174532918661 /* pi/180 */


char *nletters(int n, int c);
double rand2(double p, double q);

int planet0(double x, double y, double z, int i, int j);
double planet1(double x,double y, double z);

void printppm(FILE *outfile); /* prints picture in PPM (portable pixel map) format */
void printppmBW(FILE *outfile); /* prints picture in b/w PPM format */
void printbmp(FILE *outfile); /* prints picture in BMP format */
void printbmpBW(FILE *outfile); /* prints picture in b/w BMP format */
void printxpm(FILE *outfile); /* prints picture in XPM (X-windows pixel map) format */
void printxpmBW(FILE *outfile); /* prints picture in XPM (X-windows pixel map) format */

// projections
void mercator();
void peter();
void squarep(); 
void mollweide();
void sinusoid();
void stereo();
void orthographic();
void gnomonic();
void icosahedral();
void azimuth(); 
void conical();


#endif