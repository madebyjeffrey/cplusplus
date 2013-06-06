
#include <stdio.h>
#include <math.h>

#include "planet.h"

void mercator() {
    double y,scale1,cos2,theta1;
    int i,j,k;

    y = sin(lat);
    y = (1.0+y)/(1.0-y);
    y = 0.5*log(y);
    k = (int)(0.5*y*Width*scale/PI);
    for (j = 0; j < Height; j++) {
        if (debug && ((j % (Height/25)) == 0)) {
            fprintf (stderr, "%c", view);
            fflush(stderr);
        }
        y = PI*(2.0*(j-k)-Height)/Width/scale;
        y = exp(2.*y);
        y = (y-1.)/(y+1.);
        scale1 = scale*Width/Height/sqrt(1.0-y*y)/PI;
        cos2 = sqrt(1.0-y*y);
        Depth = 3*((int)(log2(scale1*Height)))+3;
        for (i = 0; i < Width ; i++) {
            theta1 = longi-0.5*PI+PI*(2.0*i-Width)/Width/scale;
            planet0(cos(theta1)*cos2,y,-sin(theta1)*cos2, i,j);
        }
    }
}

void peter() {
    double y,cos2,theta1,scale1;
    int k,i,j,water,land;

    y = 2.0*sin(lat);
    k = (int)(0.5*y*Width*scale/PI);
    water = land = 0;
    for (j = 0; j < Height; j++) {
        if (debug && ((j % (Height/25)) == 0)) {
            fprintf (stderr, "%c", view);
            fflush(stderr);
        }
        y = 0.5*PI*(2.0*(j-k)-Height)/Width/scale;
        if (fabs(y)>1.0)
            for (i = 0; i < Width ; i++) {
                col[i][j] = BACK;
                if (doshade>0) shades[i][j] = 255;
            }
        else {
            cos2 = sqrt(1.0-y*y);
            if (cos2>0.0) {
                scale1 = scale*Width/Height/cos2/PI;
                Depth = 3*((int)(log2(scale1*Height)))+3;
                for (i = 0; i < Width ; i++) {
                    theta1 = longi-0.5*PI+PI*(2.0*i-Width)/Width/scale;
                    planet0(cos(theta1)*cos2,y,-sin(theta1)*cos2, i,j);
                    if (col[i][j] < LAND) water++;
                    else land++;
                }
            }
        }
    }
    if (debug)
        fprintf(stderr,"\n");
    fprintf(stderr,"water percentage: %d\n",100*water/(water+land));
}

void squarep() {
    double y,scale1,theta1,cos2;
    int k,i,j;

    k = (int)(0.5*lat*Width*scale/PI);
    for (j = 0; j < Height; j++) {
        if (debug && ((j % (Height/25)) == 0)) {
            fprintf (stderr, "%c", view);
            fflush(stderr);
        }
        y = (2.0*(j-k)-Height)/Width/scale*PI;
        if (fabs(y)>=0.5*PI) for (i = 0; i < Width ; i++) {
                col[i][j] = BACK;
                if (doshade>0) shades[i][j] = 255;
            }
        else {
            cos2 = cos(y);
            if (cos2>0.0) {
                scale1 = scale*Width/Height/cos2/PI;
                Depth = 3*((int)(log2(scale1*Height)))+3;
                for (i = 0; i < Width ; i++) {
                    theta1 = longi-0.5*PI+PI*(2.0*i-Width)/Width/scale;
                    planet0(cos(theta1)*cos2,sin(y),-sin(theta1)*cos2, i,j);
                }
            }
        }
    }
}

void mollweide() {
    double y,y1,zz,scale1,cos2,theta1,theta2;
    int i,j,i1=1,k;

    for (j = 0; j < Height; j++) {
        if (debug && ((j % (Height/25)) == 0)) {
            fprintf (stderr, "%c", view);
            fflush(stderr);
        }
        y1 = 2*(2.0*j-Height)/Width/scale;
        if (fabs(y1)>=1.0) for (i = 0; i < Width ; i++) {
                col[i][j] = BACK;
                if (doshade>0) shades[i][j] = 255;
            }
        else {
            zz = sqrt(1.0-y1*y1);
            y = 2.0/PI*(y1*zz+asin(y1));
            cos2 = sqrt(1.0-y*y);
            if (cos2>0.0) {
                scale1 = scale*Width/Height/cos2/PI;
                Depth = 3*((int)(log2(scale1*Height)))+3;
                for (i = 0; i < Width ; i++) {
                    theta1 = PI/zz*(2.0*i-Width)/Width/scale;
                    if (fabs(theta1)>PI) {
                        col[i][j] = BACK;
                        if (doshade>0) shades[i][j] = 255;
                    } else {
                        double x2,y2,z2, x3,y3,z3;
                        theta1 += -0.5*PI;
                        x2 = cos(theta1)*cos2;
                        y2 = y;
                        z2 = -sin(theta1)*cos2;
                        x3 = clo*x2+slo*sla*y2+slo*cla*z2;
                        y3 = cla*y2-sla*z2;
                        z3 = -slo*x2+clo*sla*y2+clo*cla*z2;

                        planet0(x3,y3,z3, i,j);
                    }
                }
            }
        }
    }
}

void sinusoid() {
    double y,theta1,theta2,cos2,l1,i1,scale1;
    int k,i,j,l,c;

    k = (int)(lat*Width*scale/PI);
    for (j = 0; j < Height; j++) {
        if (debug && ((j % (Height/25)) == 0)) {
            fprintf (stderr, "%c", view);
            fflush(stderr);
        }
        y = (2.0*(j-k)-Height)/Width/scale*PI;
        if (fabs(y)>=0.5*PI) for (i = 0; i < Width ; i++) {
                col[i][j] = BACK;
                if (doshade>0) shades[i][j] = 255;
            }
        else {
            cos2 = cos(y);
            if (cos2>0.0) {
                scale1 = scale*Width/Height/cos2/PI;
                Depth = 3*((int)(log2(scale1*Height)))+3;
                for (i = 0; i<Width; i++) {
                    l = i*12/Width;
                    l1 = l*Width/12.0;
                    i1 = i-l1;
                    theta2 = longi-0.5*PI+PI*(2.0*l1-Width)/Width/scale;
                    theta1 = (PI*(2.0*i1-Width/12)/Width/scale)/cos2;
                    if (fabs(theta1)>PI/12.0) {
                        col[i][j] = BACK;
                        if (doshade>0) shades[i][j] = 255;
                    } else {
                        planet0(cos(theta1+theta2)*cos2,sin(y),-sin(theta1+theta2)*cos2,
                                i,j);
                    }
                }
            }
        }
    }
}

void stereo() {
    double x,y,ymin,ymax,z,zz,x1,y1,z1,theta1,theta2;
    int i,j;

    ymin = 2.0;
    ymax = -2.0;
    for (j = 0; j < Height; j++) {
        if (debug && ((j % (Height/25)) == 0)) {
            fprintf (stderr, "%c", view);
            fflush(stderr);
        }
        for (i = 0; i < Width ; i++) {
            x = (2.0*i-Width)/Height/scale;
            y = (2.0*j-Height)/Height/scale;
            z = x*x+y*y;
            zz = 0.25*(4.0+z);
            x = x/zz;
            y = y/zz;
            z = (1.0-0.25*z)/zz;
            x1 = clo*x+slo*sla*y+slo*cla*z;
            y1 = cla*y-sla*z;
            z1 = -slo*x+clo*sla*y+clo*cla*z;
            if (y1 < ymin) ymin = y1;
            if (y1 > ymax) ymax = y1;

            /* for level-of-detail effect:  Depth = 3*((int)(log2(scale*Height)/(1.0+x1*x1+y1*y1)))+6; */

            planet0(x1,y1,z1, i,j);
        }
    }
}

void orthographic() {
    double x,y,z,x1,y1,z1,ymin,ymax,theta1,theta2,zz;
    int i,j;

    ymin = 2.0;
    ymax = -2.0;
    for (j = 0; j < Height; j++) {
        if (debug && ((j % (Height/25)) == 0)) {
            fprintf (stderr, "%c", view);
            fflush(stderr);
        }
        for (i = 0; i < Width ; i++) {
            x = (2.0*i-Width)/Height/scale;
            y = (2.0*j-Height)/Height/scale;
            if (x*x+y*y>1.0) {
                col[i][j] = BACK;
                if (doshade>0) shades[i][j] = 255;
            } else {
                z = sqrt(1.0-x*x-y*y);
                x1 = clo*x+slo*sla*y+slo*cla*z;
                y1 = cla*y-sla*z;
                z1 = -slo*x+clo*sla*y+clo*cla*z;
                if (y1 < ymin) ymin = y1;
                if (y1 > ymax) ymax = y1;
                planet0(x1,y1,z1, i,j);
            }
        }
    }
}

void icosahedral() { /* modified version of gnomonic */
    double x,y,z,x1,y1,z1,zz,theta1,theta2,ymin,ymax;
    int i,j;
    double lat1, longi1, sla, cla, slo, clo, x0, y0, sq3_4, sq3;
    double L1, L2, S;

    ymin = 2.0;
    ymax = -2.0;
    sq3 = sqrt(3.0);
    L1 = 10.812317;
    L2 = -52.622632;
    S = 55.6;
    for (j = 0; j < Height; j++) {
        if (debug && ((j % (Height/25)) == 0)) {
            fprintf (stderr, "%c", view);
            fflush(stderr);
        }
        for (i = 0; i < Width ; i++) {

            x0 = 198.0*(2.0*i-Width)/Width/scale-36;
            y0 = 198.0*(2.0*j-Height)/Width/scale - lat/DEG2RAD;

            longi1 = 0.0;
            lat1 = 500.0;
            if (y0/sq3 <= 18.0 && y0/sq3 >= -18.0) { /* middle row of triangles */
                /* upward triangles */
                if (x0-y0/sq3 < 144.0 && x0+y0/sq3 >= 108.0) {
                    lat1 = -L1;
                    longi1 = 126.0;
                } else if (x0-y0/sq3 < 72.0 && x0+y0/sq3 >= 36.0) {
                    lat1 = -L1;
                    longi1 = 54.0;
                } else if (x0-y0/sq3 < 0.0 && x0+y0/sq3 >= -36.0) {
                    lat1 = -L1;
                    longi1 = -18.0;
                } else if (x0-y0/sq3 < -72.0 && x0+y0/sq3 >= -108.0) {
                    lat1 = -L1;
                    longi1 = -90.0;
                } else if (x0-y0/sq3 < -144.0 && x0+y0/sq3 >= -180.0) {
                    lat1 = -L1;
                    longi1 = -162.0;
                }

                /* downward triangles */
                else if (x0+y0/sq3 < 108.0 && x0-y0/sq3 >= 72.0) {
                    lat1 = L1;
                    longi1 = 90.0;
                } else if (x0+y0/sq3 < 36.0 && x0-y0/sq3 >= 0.0) {
                    lat1 = L1;
                    longi1 = 18.0;
                } else if (x0+y0/sq3 < -36.0 && x0-y0/sq3 >= -72.0) {
                    lat1 = L1;
                    longi1 = -54.0;
                } else if (x0+y0/sq3 < -108.0 && x0-y0/sq3 >= -144.0) {
                    lat1 = L1;
                    longi1 = -126.0;
                } else if (x0+y0/sq3 < -180.0 && x0-y0/sq3 >= -216.0) {
                    lat1 = L1;
                    longi1 = -198.0;
                }
            }

            if (y0/sq3 > 18.0) { /* bottom row of triangles */
                if (x0+y0/sq3 < 180.0 && x0-y0/sq3 >= 72.0) {
                    lat1 = L2;
                    longi1 = 126.0;
                } else if (x0+y0/sq3 < 108.0 && x0-y0/sq3 >= 0.0) {
                    lat1 = L2;
                    longi1 = 54.0;
                } else if (x0+y0/sq3 < 36.0 && x0-y0/sq3 >= -72.0) {
                    lat1 = L2;
                    longi1 = -18.0;
                } else if (x0+y0/sq3 < -36.0 && x0-y0/sq3 >= -144.0) {
                    lat1 = L2;
                    longi1 = -90.0;
                } else if (x0+y0/sq3 < -108.0 && x0-y0/sq3 >= -216.0) {
                    lat1 = L2;
                    longi1 = -162.0;
                }
            }
            if (y0/sq3 < -18.0) { /* top row of triangles */
                if (x0-y0/sq3 < 144.0 && x0+y0/sq3 >= 36.0) {
                    lat1 = -L2;
                    longi1 = 90.0;
                } else if (x0-y0/sq3 < 72.0 && x0+y0/sq3 >= -36.0) {
                    lat1 = -L2;
                    longi1 = 18.0;
                } else if (x0-y0/sq3 < 0.0 && x0+y0/sq3 >= -108.0) {
                    lat1 = -L2;
                    longi1 = -54.0;
                } else if (x0-y0/sq3 < -72.0 && x0+y0/sq3 >= -180.0) {
                    lat1 = -L2;
                    longi1 = -126.0;
                } else if (x0-y0/sq3 < -144.0 && x0+y0/sq3 >= -252.0) {
                    lat1 = -L2;
                    longi1 = -198.0;
                }
            }

            if (lat1 > 400.0) {
                col[i][j] = BACK;
                if (doshade>0) shades[i][j] = 255;
            } else {
                x = (x0 - longi1)/S;
                y = (y0 + lat1)/S;

                longi1 = longi1*DEG2RAD - longi;
                lat1 = lat1*DEG2RAD;

                sla = sin(lat1);
                cla = cos(lat1);
                slo = sin(longi1);
                clo = cos(longi1);


                zz = sqrt(1.0/(1.0+x*x+y*y));
                x = x*zz;
                y = y*zz;
                z = sqrt(1.0-x*x-y*y);
                x1 = clo*x+slo*sla*y+slo*cla*z;
                y1 = cla*y-sla*z;
                z1 = -slo*x+clo*sla*y+clo*cla*z;

                if (y1 < ymin) ymin = y1;
                if (y1 > ymax) ymax = y1;
                planet0(x1,y1,z1, i,j);
            }
        }
    }
}

void gnomonic() {
    double x,y,z,x1,y1,z1,zz,theta1,theta2,ymin,ymax;
    int i,j;

    ymin = 2.0;
    ymax = -2.0;
    for (j = 0; j < Height; j++) {
        if (debug && ((j % (Height/25)) == 0)) {
            fprintf (stderr, "%c", view);
            fflush(stderr);
        }
        for (i = 0; i < Width ; i++) {
            x = (2.0*i-Width)/Height/scale;
            y = (2.0*j-Height)/Height/scale;
            zz = sqrt(1.0/(1.0+x*x+y*y));
            x = x*zz;
            y = y*zz;
            z = sqrt(1.0-x*x-y*y);
            x1 = clo*x+slo*sla*y+slo*cla*z;
            y1 = cla*y-sla*z;
            z1 = -slo*x+clo*sla*y+clo*cla*z;
            if (y1 < ymin) ymin = y1;
            if (y1 > ymax) ymax = y1;
            planet0(x1,y1,z1, i,j);
        }
    }
}

void azimuth() {
    double x,y,z,x1,y1,z1,zz,theta1,theta2,ymin,ymax;
    int i,j;

    ymin = 2.0;
    ymax = -2.0;
    for (j = 0; j < Height; j++) {
        if (debug && ((j % (Height/25)) == 0)) {
            fprintf (stderr, "%c", view);
            fflush(stderr);
        }
        for (i = 0; i < Width ; i++) {
            x = (2.0*i-Width)/Height/scale;
            y = (2.0*j-Height)/Height/scale;
            zz = x*x+y*y;
            z = 1.0-0.5*zz;
            if (z<-1.0) {
                col[i][j] = BACK;
                if (doshade>0) shades[i][j] = 255;
            } else {
                zz = sqrt(1.0-0.25*zz);
                x = x*zz;
                y = y*zz;
                x1 = clo*x+slo*sla*y+slo*cla*z;
                y1 = cla*y-sla*z;
                z1 = -slo*x+clo*sla*y+clo*cla*z;
                if (y1 < ymin) ymin = y1;
                if (y1 > ymax) ymax = y1;
                planet0(x1,y1,z1, i,j);
            }
        }
    }
}

void conical() {
    double k1,c,y2,x,y,zz,x1,y1,z1,theta1,theta2,ymin,ymax,cos2;
    int i,j;

    ymin = 2.0;
    ymax = -2.0;
    if (lat>0) {
        k1 = 1.0/sin(lat);
        c = k1*k1;
        y2 = sqrt(c*(1.0-sin(lat/k1))/(1.0+sin(lat/k1)));
        for (j = 0; j < Height; j++) {
            if (debug && ((j % (Height/25)) == 0)) {
                fprintf (stderr, "%c", view);
                fflush(stderr);
            }
            for (i = 0; i < Width ; i++) {
                x = (2.0*i-Width)/Height/scale;
                y = (2.0*j-Height)/Height/scale+y2;
                zz = x*x+y*y;
                if (zz==0.0) theta1 = 0.0;
                else theta1 = k1*atan2(x,y);
                if (theta1<-PI || theta1>PI) {
                    col[i][j] = BACK;
                    if (doshade>0) shades[i][j] = 255;
                } else {
                    theta1 += longi-0.5*PI; /* theta1 is longitude */
                    theta2 = k1*asin((zz-c)/(zz+c));
                    /* theta2 is latitude */
                    if (theta2 > 0.5*PI || theta2 < -0.5*PI) {
                        col[i][j] = BACK;
                        if (doshade>0) shades[i][j] = 255;
                    } else {
                        cos2 = cos(theta2);
                        y = sin(theta2);
                        if (y < ymin) ymin = y;
                        if (y > ymax) ymax = y;
                        planet0(cos(theta1)*cos2,y,-sin(theta1)*cos2, i, j);
                    }
                }
            }
        }

    } else {
        k1 = 1.0/sin(lat);
        c = k1*k1;
        y2 = sqrt(c*(1.0-sin(lat/k1))/(1.0+sin(lat/k1)));
        for (j = 0; j < Height; j++) {
            if (debug && ((j % (Height/25)) == 0)) {
                fprintf (stderr, "%c", view);
                fflush(stderr);
            }
            for (i = 0; i < Width ; i++) {
                x = (2.0*i-Width)/Height/scale;
                y = (2.0*j-Height)/Height/scale-y2;
                zz = x*x+y*y;
                if (zz==0.0) theta1 = 0.0;
                else theta1 = -k1*atan2(x,-y);
                if (theta1<-PI || theta1>PI) {
                    col[i][j] = BACK;
                    if (doshade>0) shades[i][j] = 255;
                } else {
                    theta1 += longi-0.5*PI; /* theta1 is longitude */
                    theta2 = k1*asin((zz-c)/(zz+c));
                    /* theta2 is latitude */
                    if (theta2 > 0.5*PI || theta2 < -0.5*PI) {
                        col[i][j] = BACK;
                        if (doshade>0) shades[i][j] = 255;
                    } else {
                        cos2 = cos(theta2);
                        y = sin(theta2);
                        if (y < ymin) ymin = y;
                        if (y > ymax) ymax = y;
                        planet0(cos(theta1)*cos2,y,-sin(theta1)*cos2, i, j);
                    }
                }
            }
        }
    }
}
