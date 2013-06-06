/* planet.c */
/* planet generating program */
/* Copyright 1988--2009 Torben AE. Mogensen */

/* version of January 2009 */

/* The program generates planet maps based on recursive spatial subdivision */
/* of a tetrahedron containing the globe. The output is a colour PPM bitmap. */

/* The colours may optionally be modified according to latitude to move the */
/* icecaps lower closer to the poles, with a corresponding change in land colours. */

/* The Mercator map at magnification 1 is scaled to fit the Width */
/* it uses the full height (it could extend infinitely) */
/* The orthographic projections are scaled so the full view would use the */
/* full Height. Areas outside the globe are coloured black. */
/* Stereographic and gnomonic projections use the same scale as orthographic */
/* in the center of the picture, but distorts scale away from the center. */

/* It is assumed that pixels are square */
/* I have included procedures to print the maps as bmp (Windows) or */
/* ppm(portable pixel map) bitmaps  on standard output or specified files. */

/* I have tried to avoid using machine specific features, so it should */
/* be easy to port the program to any machine. Beware, though that due */
/* to different precision on different machines, the same seed numbers */
/* can yield very different planets. */

/* The primitive user interface is primarily a result of portability concerns */

#ifdef THINK_C
#define macintosh 1
#endif

#ifdef macintosh
#include <console.h>
#include <unix.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


#include "planet.h"


int BLACK = 0;
int WHITE = 1;
int BACK = 2;
int GRID = 3;
int OUTLINE1 = 4;
int OUTLINE2 = 5;
int LOWEST = 6;
int SEA = 7;
int LAND = 8;
int HIGHEST = 9;

int debug = 0;

char view;

int nocols = 65536;

int rtable[65536], gtable[65536], btable[65536];

/* Supported output file types:
    BMP - Windows Bit MaPs
    PPM - Portable Pix Maps
    XPM - X-windows Pix Maps
 */

typedef enum ftype {
    bmp,
    ppm,
    xpm
}
ftype;

ftype file_type = bmp;

char const * file_ext(ftype file_type) {
    switch (file_type) {
    case bmp:
        return ".bmp";
    case ppm:
        return ".ppm";
    case xpm:
        return ".xpm";
    default:
        return "";
    }
}

/* Character table for XPM output */

char letters[64] = {
    '@','$','.',',',':',';','-','+','=','#','*','&','A','B','C','D',
    'E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T',
    'U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j',
    'k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
};


/* these three values can be changed to change world characteristica */

double M  = -.02;   /* initial altitude (slightly below sea level) */
double dd1 = 0.45;  /* weight for altitude difference */
double dd2 = 0.035; /* weight for distance */
double POW = 0.47;  /* power for distance function */

int Depth; /* depth of subdivisions */
double r1,r2,r3,r4; /* seeds */
double longi,lat,scale;
double vgrid, hgrid;

int latic = 0; /* flag for latitude based colour */

int Width = 800, Height = 600; /* default map size */

unsigned short **col;  /* colour array */
int **heights;         /* heightfield array */
double **xxx, **yyy, **zzz; /* x,y,z arrays  (used fo gridlines */
int cl0[60][30]; /* search map */

int do_outline = 0;  /* if 1, draw coastal outline */
int do_bw = 0;       /* if 1, reduce map to black outline on white */
int contourstep = 0; /* if >0, # of colour steps between contour lines */
int *outx, *outy;

int doshade = 0;
int shade;
unsigned short **shades; /* shade array */
double shade_angle = 150.0; /* angle of "light" on bumpmap */
double shade_angle2 = 20.0; /* with daylight shading, these two are
			       longitude/latitude */

double cla, sla, clo, slo;

double rseed, increment = 0.0000001;

int best = 500000;
int weight[30];

int min(int x, int y) {
    return(x<y ? x : y);
}

int max(int x, int y) {
    return(x<y ? y : x);
}

double fmin(double x, double y) {
    return(x<y ? x : y);
}

double fmax(double x, double y) {
    return(x<y ? y : x);
}

void printheights(FILE *outfile); /* prints heightfield */
void print_error(char const *filename, char const *ext);

void heightfield();
void search();



double rand2(double p, double q); /* random number generator taking two seeds */

void readcolors(FILE *colfile, char* colorsname);
void readmap();
void makeoutline(int do_bw);
void smoothshades();

int main(int ac, char **av) {
    FILE *outfile, *colfile = NULL;
    char filename[256] = "planet-map";
    char colorsname[256] = "Olsson.col";
    int do_file = 0;
    int i;


#ifdef macintosh
    _ftype = 'TEXT';
    _fcreator ='ttxt';

    ac = ccommand (&av);
    debug = 1;
    do_file = 1;
#endif

    longi = 0.0;
    lat = 0.0;
    scale = 1.0;
    rseed = 0.123;
    view = 'm';
    vgrid = hgrid = 0.0;
    outfile = stdout;

    for (i = 1; i<ac; i++) {
        if (av[i][0] == '-') {
            switch (av[i][1]) {
            case 'X' :
                debug = 1;
                break;
            case 'V' :
                sscanf(av[++i],"%lf",&dd2);
                break;
            case 'v' :
                sscanf(av[++i],"%lf",&dd1);
                break;
            case 's' :
                sscanf(av[++i],"%lf",&rseed);
                break;
            case 'w' :
                sscanf(av[++i],"%d",&Width);
                break;
            case 'h' :
                sscanf(av[++i],"%d",&Height);
                break;
            case 'm' :
                sscanf(av[++i],"%lf",&scale);
                break;
            case 'o' :
                sscanf(av[++i],"%s",filename);
                do_file = 1;
                break;
            case 'x' :
                file_type =xpm;
                break;
            case 'C' :
                sscanf(av[++i],"%s",colorsname);
                break;
            case 'l' :
                sscanf(av[++i],"%lf",&longi);
                break;
            case 'L' :
                sscanf(av[++i],"%lf",&lat);
                break;
            case 'g' :
                sscanf(av[++i],"%lf",&vgrid);
                break;
            case 'G' :
                sscanf(av[++i],"%lf",&hgrid);
                break;
            case 'c' :
                latic = 1;
                break;
            case 'O' :
                do_outline = 1;
                do_bw = 1;
                if (strlen(av[i])>2)
                    sscanf(av[i],"-O%d",&contourstep);
                break;
            case 'E' :
                do_outline = 1;
                if (strlen(av[i])>2)
                    sscanf(av[i],"-E%d",&contourstep);
                break;
            case 'B' :
                doshade = 1;
                break;
            case 'b' :
                doshade = 2;
                break;
            case 'd' :
                doshade = 3;
                break;
            case 'P' :
                file_type = ppm;
                break;
            case 'a' :
                sscanf(av[++i],"%lf",&shade_angle);
                break;
            case 'A' :
                sscanf(av[++i],"%lf",&shade_angle2);
                break;
            case 'i' :
                sscanf(av[++i],"%lf",&M);
                break;
            case 'p' :
                if (strlen(av[i])>2) view = av[i][2];
                else view = av[++i][0];
                switch (view) {
                case 'm' :
                case 'p' :
                case 'q' :
                case 's' :
                case 'o' :
                case 'g' :
                case 'a' :
                case 'c' :
                case 'M' :
                case 'S' :
                case 'h' :
                case 'i' :
                case 'f' :
                    break;
                default:
                    fprintf(stderr,"Unknown projection: %s\n",av[i]);
                    print_error(do_file ? filename : "standard output",
                                !do_file ? "" : file_ext(file_type));
                }
                break;
            default:
                fprintf(stderr,"Unknown option: %s\n",av[i]);
                print_error(do_file ? filename : "standard output",
                            !do_file ? "" : file_ext(file_type));
            }
        } else {
            fprintf(stderr,"Unknown option: %s\n\n",av[i]);
            print_error(do_file ? filename : "standard output",
                        !do_file ? "" : file_ext(file_type));
        }
    }

    readcolors(colfile, colorsname);

    if (do_file &&'\0' != filename[0]) {
        if (strchr (filename, '.') == 0)
            strcpy(&(filename[strlen(filename)]), file_ext(file_type));

#ifdef macintosh
        switch (file_type) {
        case bmp:
            _ftype = 'BMPf';
            break;
        case ppm:
            _ftype = 'PPGM';
            break;
        case xpm:
            _ftype = 'TEXT';
            break;
        }

        _fcreator ='GKON';
#endif

        outfile = fopen(filename,"wb");

#ifdef macintosh
        _ftype = 'TEXT';
        _fcreator ='ttxt';
#endif

        if (outfile == NULL) {
            fprintf(stderr,
                    "Could not open output file %s, error code = %d\n",
                    filename, errno);
            exit(0);
        }
    } else
        outfile = stdout;

    if (longi>180) longi -= 360;
    longi = longi*DEG2RAD;
    lat = lat*DEG2RAD;

    sla = sin(lat);
    cla = cos(lat);
    slo = sin(longi);
    clo = cos(longi);

    if (view == 'f') readmap();

    if (view == 'h') {
        heights = (int**)calloc(Width,sizeof(int*));
        if (heights == 0) {
            fprintf(stderr, "Memory allocation failed.");
            exit(1);
        }
        for (i=0; i<Width; i++) {
            heights[i] = (int*)calloc(Height,sizeof(int));
            if (heights[i] == 0) {
                fprintf(stderr,
                        "Memory allocation failed at %d out of %d heights\n",
                        i+1,Width);
                exit(1);
            }
        }
    }

    col = (unsigned short**)calloc(Width,sizeof(unsigned short*));
    if (col == 0) {
        fprintf(stderr, "Memory allocation failed.");
        exit(1);
    }
    for (i=0; i<Width; i++) {
        col[i] = (unsigned short*)calloc(Height,sizeof(unsigned short));
        if (col[i] == 0) {
            fprintf(stderr,
                    "Memory allocation failed at %d out of %d cols\n",
                    i+1,Width);
            exit(1);
        }
    }

    if (doshade>0) {
        shades = (unsigned short**)calloc(Width,sizeof(unsigned short*));
        if (shades == 0) {
            fprintf(stderr, "Memory allocation failed.");
            exit(1);
        }
        for (i=0; i<Width; i++) {
            shades[i] = (unsigned short*)calloc(Height,sizeof(unsigned short));
            if (shades[i] == 0) {
                fprintf(stderr,
                        "Memory allocation failed at %d out of %d shades\n",
                        i,Width);
                exit(1);
            }
        }
    }

    if (vgrid != 0.0) {
        xxx = (double**)calloc(Width,sizeof(double*));
        if (xxx == 0) {
            fprintf(stderr, "Memory allocation failed.");
            exit(1);
        }
        for (i=0; i<Width; i++) {
            xxx[i] = (double*)calloc(Height,sizeof(double));
            if (xxx[i] == 0) {
                fprintf(stderr,
                        "Memory allocation failed at %d out of %d xs\n",
                        i+1,Width);
                exit(1);
            }
        }

        yyy = (double**)calloc(Width,sizeof(double*));
        if (yyy == 0) {
            fprintf(stderr, "Memory allocation failed.");
            exit(1);
        }
        for (i=0; i<Width; i++) {
            yyy[i] = (double*)calloc(Height,sizeof(double));
            if (yyy[i] == 0) {
                fprintf(stderr,
                        "Memory allocation failed at %d out of %d ys\n",
                        i+1,Width);
                exit(1);
            }
        }
    }

    if (hgrid != 0.0) {
        zzz = (double**)calloc(Width,sizeof(double*));
        if (zzz == 0) {
            fprintf(stderr, "Memory allocation failed.");
            exit(1);
        }
        for (i=0; i<Width; i++) {
            zzz[i] = (double*)calloc(Height,sizeof(double));
            if (zzz[i] == 0) {
                fprintf(stderr,
                        "Memory allocation failed at %d out of %d zs\n",
                        i+1,Width);
                exit(1);
            }
        }
    }

    if (view == 'c') {
        if (lat == 0) view = 'm';
        /* Conical approaches mercator when lat -> 0 */
        if (abs(lat) >= PI - 0.000001) view = 's';
        /* Conical approaches stereo when lat -> +/- 90 */
    }

    Depth = 3*((int)(log2(scale*Height)))+6;

    r1 = rseed;

    r1 = rand2(r1,r1);
    r2 = rand2(r1,r1);
    r3 = rand2(r1,r2);
    r4 = rand2(r2,r3);

    if (debug && (view != 'f'))
        fprintf(stderr, "+----+----+----+----+----+\n");

    switch (view) {

    case 'm': /* Mercator projection */
        mercator();
        break;

    case 'p': /* Peters projection (area preserving cylindrical) */
        peter();
        break;

    case 'q': /* Square projection (equidistant latitudes) */
        squarep();
        break;

    case 'M': /* Mollweide projection (area preserving) */
        mollweide();
        break;

    case 'S': /* Sinusoid projection (area preserving) */
        sinusoid();
        break;

    case 's': /* Stereographic projection */
        stereo();
        break;

    case 'o': /* Orthographic projection */
        orthographic();
        break;

    case 'g': /* Gnomonic projection */
        gnomonic();
        break;

    case 'i': /* Icosahedral projection */
        icosahedral();
        break;

    case 'a': /* Area preserving azimuthal projection */
        azimuth();
        break;

    case 'c': /* Conical projection (conformal) */
        conical();
        break;

    case 'h': /* heightfield */
        heightfield();
        break;

    case 'f': /* Search */
        while (1) {
            search();
            rseed += increment;
            r1 = rseed;
            r1 = rand2(r1,r1);
            r2 = rand2(r1,r1);
            r3 = rand2(r1,r2);
            r4 = rand2(r2,r3);
        }
    }

    if (do_outline) makeoutline(do_bw);

    if (vgrid != 0.0) { /* draw longitudes */
        int i,j;
        for (i=0; i<Width-1; i++)
            for (j=0; j<Height-1; j++) {
                double t;
                int g = 0;
                if (fabs(yyy[i][j])==1) g=1;
                else {
                    t = floor((atan2(xxx[i][j],zzz[i][j])*180/PI+360)/vgrid);
                    if (t != floor((atan2(xxx[i+1][j],zzz[i+1][j])*180/PI+360)/vgrid))
                        g=1;
                    if (t != floor((atan2(xxx[i][j+1],zzz[i][j+1])*180/PI+360)/vgrid))
                        g=1;
                }
                if (g) {
                    col[i][j] = GRID;
                    if (doshade>0) shades[i][j] = 255;
                }
            }
    }

    if (hgrid != 0.0) { /* draw latitudes */
        int i,j;
        for (i=0; i<Width-1; i++)
            for (j=0; j<Height-1; j++) {
                double t;
                int g = 0;
                t = floor((asin(yyy[i][j])*180/PI+360)/hgrid);
                if (t != floor((asin(yyy[i+1][j])*180/PI+360)/hgrid))
                    g=1;
                if (t != floor((asin(yyy[i][j+1])*180/PI+360)/hgrid))
                    g=1;
                if (g) {
                    col[i][j] = GRID;
                    if (doshade>0) shades[i][j] = 255;
                }
            }
    }

    if (doshade>0) smoothshades();

    if (debug)
        fprintf(stderr, "\n");

    /* plot picture */
    switch (file_type) {
    case ppm:
        if (do_bw) printppmBW(outfile);
        else if (view != 'h') printppm(outfile);
        else printheights(outfile);
        break;
    case xpm:
        if (do_bw) printxpmBW(outfile);
        else if (view != 'h') printxpm(outfile);
        else printheights(outfile);
        break;
    case bmp:
        if (do_bw) printbmpBW(outfile);
        else if (view != 'h') printbmp(outfile);
        else printheights(outfile);
        break;
    }

    return(0);
}

void readcolors(FILE *colfile, char* colorsname) {
    int crow, cNum = 0, oldcNum, i;

    if (NULL == (colfile = fopen(colorsname, "r"))) {
        fprintf(stderr,
                "Cannot open %s\n",
                colorsname);
        exit(1);
    }


    /* Format of colour file is a sequence of lines       */
    /* each consisting of four integers:                  */
    /*   colour_number red green blue                     */
    /* where 0 <= colour_number <= 65535                  */
    /* and 0<= red, green, blue <= 255                    */
    /* The colour numbers must be increasing              */
    /* The first colours have special uses:               */
    /* 0 is usually black (0,0,0)                         */
    /* 1 is usually white (255,255,255)                   */
    /* 2 is the background colour                         */
    /* 3 is used for latitude/longitude grid lines        */
    /* 4 and 5 are used for outlines and contour lines    */
    /* 6 upwards are used for altitudes                   */
    /* Halfway between 6 and the max colour is sea level  */
    /* Shallowest sea is (max+6)/2 and land is above this */
    /* With 65536 colours, (max+6)/2 = 32770              */
    /* Colours between specified are interpolated         */

    for (crow = 0; !feof(colfile); crow++) {
        int	rValue,
            gValue,
            bValue,
            result = 0;

        oldcNum = cNum;  /* remember last colour number */
        result = fscanf(colfile, " %d %d %d %d",
                        &cNum, &rValue, &gValue, &bValue);

        if (result > 0) {
            if (cNum < oldcNum) cNum = oldcNum;
            if (cNum > 65535) cNum = 65535;
            rtable[cNum] = rValue;
            gtable[cNum] = gValue;
            btable[cNum] = bValue;
            /* interpolate colours between oldcNum and cNum */
            for (i = oldcNum+1; i<cNum; i++) {
                rtable[i] = (rtable[oldcNum]*(cNum-i)+rtable[cNum]*(i-oldcNum))
                            / (cNum-oldcNum+1);
                gtable[i] = (gtable[oldcNum]*(cNum-i)+gtable[cNum]*(i-oldcNum))
                            / (cNum-oldcNum+1);
                btable[i] = (btable[oldcNum]*(cNum-i)+btable[cNum]*(i-oldcNum))
                            / (cNum-oldcNum+1);
            }
        }
    }

    nocols = cNum+1;
    if (nocols < 10) nocols = 10;

    HIGHEST = nocols - 1;
    SEA = (HIGHEST+LOWEST)/2;
    LAND = SEA+1;

    for (i = cNum+1; i<nocols; i++) {
        /* fill up rest of colour table with last read colour */
        rtable[i] = rtable[cNum];
        gtable[i] = gtable[cNum];
        btable[i] = btable[cNum];
    }
}

void makeoutline(int do_bw) {
    int i,j,k,t;

    outx = (int*)calloc(Width*Height,sizeof(int));
    outy = (int*)calloc(Width*Height,sizeof(int));
    k=0;
    for (i=1; i<Width-1; i++)
        for (j=1; j<Height-1; j++)
            if ((col[i][j] >= LOWEST && col[i][j] <= SEA) &&
                    (col[i-1][j] >= LAND || col[i+1][j] >= LAND ||
                     col[i][j-1] >= LAND || col[i][j+1] >= LAND ||
                     col[i-1][j-1] >= LAND || col[i-1][j+1] >= LAND ||
                     col[i+1][j-1] >= LAND || col[i+1][j+1] >= LAND)) {
                /* if point is sea and any neighbour is not, add to outline */
                outx[k] = i;
                outy[k++] = j;
            }

    if (contourstep>0) {

        for (i=1; i<Width-1; i++)
            for (j=1; j<Height-1; j++) {
                t = (col[i][j] - LAND) / contourstep;
                if (t>=0 &&
                        ((col[i-1][j]-LAND) / contourstep > t ||
                         (col[i+1][j]-LAND) / contourstep > t ||
                         (col[i][j-1]-LAND) / contourstep > t ||
                         (col[i][j+1]-LAND) / contourstep > t)) {
                    /* if point is at countour line and any neighbour is higher */
                    outx[k] = i;
                    outy[k++] = j;
                }
            }
    }
    if (do_bw) /* if outline only, clear colours */
        for (i=0; i<Width; i++)
            for (j=0; j<Height; j++) {
                if (col[i][j] >= LOWEST)
                    col[i][j] = WHITE;
                else col[i][j] = BLACK;
            }
    /* draw outline (in black if outline only) */
    while (k-->0) {
        if (do_bw) t = BLACK;
        else if (contourstep == 0 || col[outx[k]][outy[k]]<LAND ||
                 ((col[outx[k]][outy[k]]-LAND)/contourstep)%2 == 1)
            t = OUTLINE1;
        else t = OUTLINE2;
        col[outx[k]][outy[k]] = t;
    }
}

void readmap() {
    int i,j;
    double y;
    char c;

    Width = 47;
    Height = 21;
    for (j = 0; j < Height; j++) {
        y = 0.5*7.5*(2.0*j-Height+1);
        y = cos(DEG2RAD*y);
        weight[j] = (int)(100.0*y+0.5);
    }
    for (j = 0; j < Height; j+=2) {
        for(i = 0; i < Width ; i+=2) {
            c = getchar();
            switch (c) {
            case '.':
                cl0[i][j] = -8;
                break;
            case ',':
                cl0[i][j] = -4;
                break;
            case ':':
                cl0[i][j] = -2;
                break;
            case ';':
                cl0[i][j] = -1;
                break;
            case '-':
                cl0[i][j] = 0;
                break;
            case '*':
                cl0[i][j] = 1;
                break;
            case 'o':
                cl0[i][j] = 2;
                break;
            case 'O':
                cl0[i][j] = 4;
                break;
            case '@':
                cl0[i][j] = 16;
                break;
            default:
                printf("Wrong map symbol: %c\n",c);
            }
            if (i>0) cl0[i-1][j] = (cl0[i][j]+cl0[i-2][j])/2;
        }
        c = getchar();
        if (c!='\n') printf("Wrong map format: %c\n",c);
    }
    for (j = 1; j < Height; j+=2)
        for(i = 0; i < Width ; i++)
            cl0[i][j] = (cl0[i][j-1]+cl0[i][j+1])/2;
}


void smoothshades() {
    int i,j;

    for (i=0; i<Width-2; i++)
        for (j=0; j<Height-2; j++)
            shades[i][j] = (4*shades[i][j]+2*shades[i][j+1]
                            +2*shades[i+1][j]+shades[i+1][j+1]+4)/9;
}



void heightfield() {
    double x,y,z,x1,y1,z1;
    int i,j;

    for (j = 0; j < Height; j++) {
        if (debug && ((j % (Height/25)) == 0)) {
            fprintf (stderr, "%c", view);
            fflush(stderr);
        }
        for (i = 0; i < Width ; i++) {
            x = (2.0*i-Width)/Height/scale;
            y = (2.0*j-Height)/Height/scale;
            if (x*x+y*y>1.0) heights[i][j] = 0;
            else {
                z = sqrt(1.0-x*x-y*y);
                x1 = clo*x+slo*sla*y+slo*cla*z;
                y1 = cla*y-sla*z;
                z1 = -slo*x+clo*sla*y+clo*cla*z;
                heights[i][j] = 10000000*planet1(x1,y1,z1);
            }
        }
    }
}


void search() {
    double y,cos2,theta1,scale1;
    double y2,cos22,theta12;
    int i,j,k,l,c,c1,c2,c3, errcount, errcount1;

    for (j = 0; j < Height; j++) {
        y = 0.5*7.5*(2.0*j-Height+1);
        y = sin(DEG2RAD*y);
        scale1 = Width/Height/sqrt(1.0-y*y)/PI;
        cos2 = sqrt(1.0-y*y);
        y2 = 0.5*7.5*(2.0*j-Height+1.5);
        y2 = sin(DEG2RAD*y2);
        cos22 = sqrt(1.0-y2*y2);
        Depth = 3*((int)(log2(scale1*Height)))+6;
        for (i = 0; i < Width ; i++) {
            theta1 = -0.5*PI+PI*(2.0*i-Width)/Width;
            theta12 = -0.5*PI+PI*(2.0*i+0.5-Width)/Width;
            c = 128+1000*planet1(cos(theta1)*cos2,y,-sin(theta1)*cos2);
            c1 = 128+1000*planet1(cos(theta12)*cos2,y,-sin(theta12)*cos2);
            c2 = 128+1000*planet1(cos(theta1)*cos22,y2,-sin(theta1)*cos22);
            c3 = 128+1000*planet1(cos(theta12)*cos22,y2,-sin(theta12)*cos22);
            c = (c+c1+c2+c3)/4.0;
            if (c<0) c = 0;
            if (c>255) c = 255;
            col[i][j] = c;
        }
    }
    for (k=0; k<Width; k++) {
        for (l=-20; l<=20; l+=2) {
            errcount = 0;
            for (j = 0; j < Height; j++) {
                errcount1 = 0;
                for(i = 0; i < Width ; i++) {
                    if (cl0[i][j]<0 && col[(i+k)%Width][j] > 128-l)
                        errcount1-=cl0[i][j];
                    if (cl0[i][j]>0 && col[(i+k)%Width][j] <= 128-l)
                        errcount1+=cl0[i][j];
                }
                errcount += weight[j]*errcount1;
            }

            if (errcount < best) {
                printf("Errors: %d, parameters: -s %.12f -l %.1f -i %.3f\n",
                       errcount,rseed,(360.0*k)/(Width+1),M+l/1000.0);
                best = errcount;
                for (j = 0; j < Height; j++) {
                    for(i = 0; i < Width ; i++)
                        if (col[(i+k)%Width][j] <= 128-l) putchar('.');
                        else putchar('O');
                    putchar('\n');
                }
                fflush(stdout);
            }
        }
    }
}



double rand2(double p, double q) /* random number generator taking two seeds */
/* rand2(p,q) = rand2(q,p) is important     */
{
    double r;
    r = (p+3.14159265)*(q+3.14159265);
    return(2.*(r-(int)r)-1.);
}



char *nletters(int n, int c) {
    int i;
    static char buffer[8];

    buffer[n] = '\0';

    for (i = n-1; i >= 0; i--) {
        buffer[i] = letters[c & 0x001F];
        c >>= 5;
    }

    return buffer;
}


void printheights(FILE *outfile) { /* prints heightfield */
    int i,j;

    for (j=0; j<Height; j++) {
        for (i=0; i<Width; i++)
            fprintf(outfile,"%d ",heights[i][j]);
        putc('\n',outfile);
    }
    fclose(outfile);
}


void print_error(char const *filename, char const *ext) {
    fprintf(stderr,"Usage: planet [options]\n\n");
    fprintf(stderr,"options:\n");
    fprintf(stderr,"  -?                (or any illegal option) Output this text\n");
    fprintf(stderr,"  -s seed           Specifies seed as number between 0.0 and 1.0\n");
    fprintf(stderr,"  -w width          Specifies width in pixels, default = 800\n");
    fprintf(stderr,"  -h height         Specifies height in pixels, default = 600\n");
    fprintf(stderr,"  -m magnification  Specifies magnification, default = 1.0\n");
    fprintf(stderr,"  -o output_file    Specifies output file, default is %s%s\n",
            filename, ext);
    fprintf(stderr,"  -l longitude      Specifies longitude of centre in degrees, default = 0.0\n");
    fprintf(stderr,"  -L latitude       Specifies latitude of centre in degrees, default = 0.0\n");
    fprintf(stderr,"  -g gridsize       Specifies vertical gridsize in degrees, default = 0.0 (no grid)\n");
    fprintf(stderr,"  -G gridsize       Specifies horisontal gridsize in degrees, default = 0.0 (no grid)\n");
    fprintf(stderr,"  -i init_alt       Specifies initial altitude (default = -0.02)\n");
    fprintf(stderr,"  -c                Colour depends on latitude (default: only altitude)\n");
    fprintf(stderr,"  -C file           Read colour definitions from file\n");
    fprintf(stderr,"  -O                Produce a black and white outline map\n");
    fprintf(stderr,"  -E                Trace the edges of land in black on colour map\n");
    fprintf(stderr,"  -B                Use ``bumpmap'' shading\n");
    fprintf(stderr,"  -b                Use ``bumpmap'' shading on land only\n");
    fprintf(stderr,"  -d                Use ``daylight'' shading\n");
    fprintf(stderr,"  -a angle	      Angle of ``light'' in bumpmap shading\n");
    fprintf(stderr,"                    or longitude of sun in daylight shading\n");
    fprintf(stderr,"  -A latitude	      Latitude of sun in daylight shading\n");
    fprintf(stderr,"  -P                Use PPM file format (default is BMP)\n");
    fprintf(stderr,"  -x                Use XPM file format (default is BMP)\n");
    fprintf(stderr,"  -V number         Distance contribution to variation (default = 0.03)\n");
    fprintf(stderr,"  -v number         Altitude contribution to variation (default = 0.4)\n");
    fprintf(stderr,"  -pprojection      Specifies projection: m = Mercator (default)\n");
    fprintf(stderr,"                                          p = Peters\n");
    fprintf(stderr,"                                          q = Square\n");
    fprintf(stderr,"                                          s = Stereographic\n");
    fprintf(stderr,"                                          o = Orthographic\n");
    fprintf(stderr,"                                          g = Gnomonic\n");
    fprintf(stderr,"                                          a = Area preserving azimuthal\n");
    fprintf(stderr,"                                          c = Conical (conformal)\n");
    fprintf(stderr,"                                          M = Mollweide\n");
    fprintf(stderr,"                                          S = Sinusoidal\n");
    fprintf(stderr,"                                          i = Icosaheral\n");
    fprintf(stderr,"                                          h = Heightfield\n");
    fprintf(stderr,"                                          f = Find match, see manual\n");
    exit(0);
}

/* With the -pf option a map must be given on standard input.  */
/* This map is 11 lines of 24 characters. The characters are:  */
/*    . : very strong preference for water (value=8)	       */
/*    , : strong preference for water (value=4)		       */
/*    : : preference for water (value=2)		       */
/*    ; : weak preference for water (value=1)		       */
/*    - : don't care (value=0)				       */
/*    * : weak preference for land (value=1)		       */
/*    o : preference for land (value=2)			       */
/*    O : strong preference for land (value=4)		       */
/*    @ : very strong preference for land (value=8)	       */
/*							       */
/* Each point on the map corresponds to a point on a 15° grid. */
/*							       */
/* The program tries seeds starting from the specified and     */
/* successively outputs the seed (and rotation) of the best    */
/* current match, together with a small map of this.	       */
/* This is all ascii, no bitmap is produced.		       a*/

