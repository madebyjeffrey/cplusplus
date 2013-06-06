#include <stdio.h>
#include <math.h>

#include "planet.h"

double ssa,ssb,ssc,ssd, ssas,ssbs,sscs,ssds,
       ssax,ssay,ssaz, ssbx,ssby,ssbz, sscx,sscy,sscz, ssdx,ssdy,ssdz;



double planet(double a,double b,double c,double d, double as,double bs,double cs,double ds,
              double ax,double ay,double az, double bx,double by,double bz, double cx,double cy,double cz, double dx,double dy,double dz,
              double x,double y,double z, int level)
// double a,b,c,d;		    /* altitudes of the 4 verticess */
// double as,bs,cs,ds;	    /* seeds of the 4 verticess */
// double ax,ay,az, bx,by,bz,  /* vertex coordinates */
//   cx,cy,cz, dx,dy,dz;
// double x,y,z;		    /* goal point */
// int level;		    /* levels to go */
{
    double abx,aby,abz, acx,acy,acz, adx,ady,adz; // ADX, ADY, and ADZ represent the point A translated by a certain delta.
    double bcx,bcy,bcz, bdx,bdy,bdz, cdx,cdy,cdz;
    double lab, lac, lad, lbc, lbd, lcd;
    double ex, ey, ez, e, es, es1, es2, es3;
    double eax,eay,eaz, epx,epy,epz;
    double ecx,ecy,ecz, edx,edy,edz;
    double x1,y1,z1,x2,y2,z2,l1,tmp;
    
    /* notes from Paul:
    Look for variables starting with the letter L. This seems to be his pattern for the norms of factors.
    Huge swathes of if else statements are nothing more than determining which of two directed line segments is longer then reorganizing the input parameters in accordance.
    */

    if (level>0) {
        if (level==11) {
            ssa=a;
            ssb=b;
            ssc=c;
            ssd=d;
            ssas=as;
            ssbs=bs;
            sscs=cs;
            ssds=ds;
            ssax=ax;
            ssay=ay;
            ssaz=az;
            ssbx=bx;
            ssby=by;
            ssbz=bz;
            sscx=cx;
            sscy=cy;
            sscz=cz;
            ssdx=dx;
            ssdy=dy;
            ssdz=dz;
        }
        
        // pythagorean theorem?
        abx = ax-bx;
        aby = ay-by;
        abz = az-bz;
        acx = ax-cx;
        acy = ay-cy;
        acz = az-cz;
        lab = abx*abx+aby*aby+abz*abz;
        lac = acx*acx+acy*acy+acz*acz;

        /* reorder vertices so ab is longest edge */
        if (lab<lac)
            return(planet(a,c,b,d, as,cs,bs,ds,
                          ax,ay,az, cx,cy,cz, bx,by,bz, dx,dy,dz,
                          x,y,z, level));
        else {
            adx = ax-dx;
            ady = ay-dy;
            adz = az-dz;
            lad = adx*adx+ady*ady+adz*adz;
            if (lab<lad)
                return(planet(a,d,b,c, as,ds,bs,cs,
                              ax,ay,az, dx,dy,dz, bx,by,bz, cx,cy,cz,
                              x,y,z, level));
            else {
                bcx = bx-cx;
                bcy = by-cy;
                bcz = bz-cz;
                lbc = bcx*bcx+bcy*bcy+bcz*bcz;
                if (lab<lbc)
                    return(planet(b,c,a,d, bs,cs,as,ds,
                                  bx,by,bz, cx,cy,cz, ax,ay,az, dx,dy,dz,
                                  x,y,z, level));
                else {
                    bdx = bx-dx;
                    bdy = by-dy;
                    bdz = bz-dz;
                    lbd = bdx*bdx+bdy*bdy+bdz*bdz;
                    if (lab<lbd)
                        return(planet(b,d,a,c, bs,ds,as,cs,
                                      bx,by,bz, dx,dy,dz, ax,ay,az, cx,cy,cz,
                                      x,y,z, level));
                    else {
                        cdx = cx-dx;
                        cdy = cy-dy;
                        cdz = cz-dz;
                        lcd = cdx*cdx+cdy*cdy+cdz*cdz;
                        if (lab<lcd)
                            return(planet(c,d,a,b, cs,ds,as,bs,
                                          cx,cy,cz, dx,dy,dz, ax,ay,az, bx,by,bz,
                                          x,y,z, level));
                        else { /* ab is longest, so cut ab */
                            es = rand2(as,bs);
                            es1 = rand2(es,es);
                            es2 = 0.5+0.1*rand2(es1,es1);
                            es3 = 1.0-es2;
                            if (ax<bx) {
                                ex = es2*ax+es3*bx;
                                ey = es2*ay+es3*by;
                                ez = es2*az+es3*bz;
                            } else if (ax>bx) {
                                ex = es3*ax+es2*bx;
                                ey = es3*ay+es2*by;
                                ez = es3*az+es2*bz;
                            } else { /* ax==bx, very unlikely to ever happen */
                                ex = 0.5*ax+0.5*bx;
                                ey = 0.5*ay+0.5*by;
                                ez = 0.5*az+0.5*bz;
                            }
                            if (lab>1.0) lab = pow(lab,0.5);
                            /* decrease contribution for very long distances */

                            /* new altitude is: */
                            e = 0.5*(a+b) /* average of end points */
                                + es*dd1*fabs(a-b) /* plus contribution for altitude diff */
                                + es1*dd2*pow(lab,POW); /* plus contribution for distance */
                            eax = ax-ex;
                            eay = ay-ey;
                            eaz = az-ez;
                            epx =  x-ex;
                            epy =  y-ey;
                            epz =  z-ez;
                            ecx = cx-ex;
                            ecy = cy-ey;
                            ecz = cz-ez;
                            edx = dx-ex;
                            edy = dy-ey;
                            edz = dz-ez;
                            if ((eax*ecy*edz+eay*ecz*edx+eaz*ecx*edy
                                    -eaz*ecy*edx-eay*ecx*edz-eax*ecz*edy)*
                                    (epx*ecy*edz+epy*ecz*edx+epz*ecx*edy
                                     -epz*ecy*edx-epy*ecx*edz-epx*ecz*edy)>0.0)
                                return(planet(c,d,a,e, cs,ds,as,es,
                                              cx,cy,cz, dx,dy,dz, ax,ay,az, ex,ey,ez,
                                              x,y,z, level-1));
                            else
                                return(planet(c,d,b,e, cs,ds,bs,es,
                                              cx,cy,cz, dx,dy,dz, bx,by,bz, ex,ey,ez,
                                              x,y,z, level-1));
                        }
                    }
                }
            }
        }
    } else { /* level == 0 */
        if (doshade==1 || doshade==2) {
            x1 = 0.25*(ax+bx+cx+dx);
            x1 = a*(x1-ax)+b*(x1-bx)+c*(x1-cx)+d*(x1-dx);
            y1 = 0.25*(ay+by+cy+dy);
            y1 = a*(y1-ay)+b*(y1-by)+c*(y1-cy)+d*(y1-dy);
            z1 = 0.25*(az+bz+cz+dz);
            z1 = a*(z1-az)+b*(z1-bz)+c*(z1-cz)+d*(z1-dz);
            l1 = sqrt(x1*x1+y1*y1+z1*z1);
            if (l1==0.0) l1 = 1.0;
            tmp = sqrt(1.0-y*y);
            if (tmp<0.0001) tmp = 0.0001;
            x2 = x*x1+y*y1+z*z1;
            y2 = -x*y/tmp*x1+tmp*y1-z*y/tmp*z1;
            z2 = -z/tmp*x1+x/tmp*z1;
            shade =
                (int)((-sin(PI*shade_angle/180.0)*y2-cos(PI*shade_angle/180.0)*z2)
                      /l1*48.0+128.0);
            if (shade<10) shade = 10;
            if (shade>255) shade = 255;
            if (doshade==2 && (a+b+c+d)<0.0) shade = 150;
        } else if (doshade==3) {
            if ((a+b+c+d)<0.0) {
                x1 = x;
                y1 = y;
                z1 = z;
            } else {
                l1 = 50.0/
                     sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by)+(az-bz)*(az-bz)+
                          (ax-cx)*(ax-cx)+(ay-cy)*(ay-cy)+(az-cz)*(az-cz)+
                          (ax-dx)*(ax-dx)+(ay-dy)*(ay-dy)+(az-dz)*(az-dz)+
                          (bx-cx)*(bx-cx)+(by-cy)*(by-cy)+(bz-cz)*(bz-cz)+
                          (bx-dx)*(bx-dx)+(by-dy)*(by-dy)+(bz-dz)*(bz-dz)+
                          (cx-dx)*(cx-dx)+(cy-dy)*(cy-dy)+(cz-dz)*(cz-dz));
                x1 = 0.25*(ax+bx+cx+dx);
                x1 = l1*(a*(x1-ax)+b*(x1-bx)+c*(x1-cx)+d*(x1-dx)) + x;
                y1 = 0.25*(ay+by+cy+dy);
                y1 = l1*(a*(y1-ay)+b*(y1-by)+c*(y1-cy)+d*(y1-dy)) + y;
                z1 = 0.25*(az+bz+cz+dz);
                z1 = l1*(a*(z1-az)+b*(z1-bz)+c*(z1-cz)+d*(z1-dz)) + z;
            }
            l1 = sqrt(x1*x1+y1*y1+z1*z1);
            if (l1==0.0) l1 = 1.0;
            x2 = cos(PI*shade_angle/180.0-0.5*PI)*cos(PI*shade_angle2/180.0);
            y2 = -sin(PI*shade_angle2/180.0);
            z2 = -sin(PI*shade_angle/180.0-0.5*PI)*cos(PI*shade_angle2/180.0);
            shade = (int)((x1*x2+y1*y2+z1*z2)/l1*170.0+10);
            if (shade<10) shade = 10;
            if (shade>255) shade = 255;
        }
        return((a+b+c+d)/4);
    }
}

double planet1(double x,double y, double z) {
    double abx,aby,abz, acx,acy,acz, adx,ady,adz, apx,apy,apz;
    double bax,bay,baz, bcx,bcy,bcz, bdx,bdy,bdz, bpx,bpy,bpz;

    abx = ssbx-ssax;
    aby = ssby-ssay;
    abz = ssbz-ssaz;
    acx = sscx-ssax;
    acy = sscy-ssay;
    acz = sscz-ssaz;
    adx = ssdx-ssax;
    ady = ssdy-ssay;
    adz = ssdz-ssaz;
    apx = x-ssax;
    apy = y-ssay;
    apz = z-ssaz;
    if ((adx*aby*acz+ady*abz*acx+adz*abx*acy
            -adz*aby*acx-ady*abx*acz-adx*abz*acy)*
            (apx*aby*acz+apy*abz*acx+apz*abx*acy
             -apz*aby*acx-apy*abx*acz-apx*abz*acy)>0.0) {
        /* p is on same side of abc as d */
        if ((acx*aby*adz+acy*abz*adx+acz*abx*ady
                -acz*aby*adx-acy*abx*adz-acx*abz*ady)*
                (apx*aby*adz+apy*abz*adx+apz*abx*ady
                 -apz*aby*adx-apy*abx*adz-apx*abz*ady)>0.0) {
            /* p is on same side of abd as c */
            if ((abx*ady*acz+aby*adz*acx+abz*adx*acy
                    -abz*ady*acx-aby*adx*acz-abx*adz*acy)*
                    (apx*ady*acz+apy*adz*acx+apz*adx*acy
                     -apz*ady*acx-apy*adx*acz-apx*adz*acy)>0.0) {
                /* p is on same side of acd as b */
                bax = -abx;
                bay = -aby;
                baz = -abz;
                bcx = sscx-ssbx;
                bcy = sscy-ssby;
                bcz = sscz-ssbz;
                bdx = ssdx-ssbx;
                bdy = ssdy-ssby;
                bdz = ssdz-ssbz;
                bpx = x-ssbx;
                bpy = y-ssby;
                bpz = z-ssbz;
                if ((bax*bcy*bdz+bay*bcz*bdx+baz*bcx*bdy
                        -baz*bcy*bdx-bay*bcx*bdz-bax*bcz*bdy)*
                        (bpx*bcy*bdz+bpy*bcz*bdx+bpz*bcx*bdy
                         -bpz*bcy*bdx-bpy*bcx*bdz-bpx*bcz*bdy)>0.0) {
                    /* p is on same side of bcd as a */
                    /* Hence, p is inside tetrahedron */
                    return(planet(ssa,ssb,ssc,ssd, ssas,ssbs,sscs,ssds,
                                  ssax,ssay,ssaz, ssbx,ssby,ssbz,
                                  sscx,sscy,sscz, ssdx,ssdy,ssdz,
                                  x,y,z, 11));
                }
            }
        }
    } /* otherwise */
    return(planet(M,M,M,M,
                  /* initial altitude is M on all corners of tetrahedron */
                  r1,r2,r3,r4,
                  /* same seed set is used in every call */
                  -sqrt(3.0)-0.20, -sqrt(3.0)-0.22, -sqrt(3.0)-0.23,
                  -sqrt(3.0)-0.19,  sqrt(3.0)+0.18,  sqrt(3.0)+0.17,
                  sqrt(3.0)+0.21, -sqrt(3.0)-0.24,  sqrt(3.0)+0.15,
                  sqrt(3.0)+0.24,  sqrt(3.0)+0.22, -sqrt(3.0)-0.25,
                  /* coordinates of vertices of tetrahedron*/
                  x,y,z,
                  /* coordinates of point we want colour of */
                  Depth));
    /* subdivision depth */

}

int planet0(double x, double y, double z, int i, int j) {
    double alt, y2;
    int colour;

    alt = planet1(x,y,z);
    y2 = y*y;
    y2 = y2*y2;
    y2 = y2*y2;

    /* calculate colour */
    if (alt <=0.) { /* if below sea level then */
        if (latic && y2+alt >= 0.98)
            colour = HIGHEST;	 /* icecap if close to poles */
        else {
            colour = SEA+(int)((SEA-LOWEST+1)*(10*alt));
            if (colour<LOWEST) colour = LOWEST;
        }
    } else {
        if (latic) alt += 0.1*y2;  /* altitude adjusted with latitude */
        if (alt >= 0.1) /* if high then */
            colour = HIGHEST;
        else {
            colour = LAND+(int)((HIGHEST-LAND+1)*(10*alt));
            if (colour>HIGHEST) colour = HIGHEST;
        }
    }

    col[i][j] = colour;
    if (vgrid != 0.0) {
        xxx[i][j] = x;
        yyy[i][j] = y;
    }
    if (hgrid != 0.0) zzz[i][j] = z;
    if (doshade>0) shades[i][j] = shade;
    return(colour);
}

