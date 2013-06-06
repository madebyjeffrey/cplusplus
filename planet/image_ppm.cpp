#include <stdio.h>

#include "planet.h"

void printppm(FILE *outfile) /* prints picture in PPM (portable pixel map) format */
{
  int i,j,c,s;

  fprintf(outfile,"P6\n");
  fprintf(outfile,"#fractal planet image\n");
  fprintf(outfile,"%d %d 255\n",Width,Height);
 
  if (doshade) {
    for (j=0; j<Height; j++) {
      for (i=0; i<Width; i++) {
	s =shades[i][j];
	c = s*rtable[col[i][j]]/150;
	if (c>255) c=255;
	putc(c,outfile);
	c = s*gtable[col[i][j]]/150;
	if (c>255) c=255;
	putc(c,outfile);
	c = s*btable[col[i][j]]/150;
	if (c>255) c=255;
	putc(c,outfile);
      }
    }
  } else {
    for (j=0; j<Height; j++)
      for (i=0; i<Width; i++) {
	putc(rtable[col[i][j]],outfile);
	putc(gtable[col[i][j]],outfile);
	putc(btable[col[i][j]],outfile);
      }
  }
  fclose(outfile);
}

void printppmBW(FILE *outfile) /* prints picture in b/w PPM format */
{
  int i,j,c;

  fprintf(outfile,"P6\n");
  fprintf(outfile,"#fractal planet image\n");
  fprintf(outfile,"%d %d 1\n",Width,Height);
 
  for (j=0; j<Height; j++)
    for (i=0; i<Width; i++) {
      if (col[i][j] < WHITE)
	c=0;
      else c=1;
      putc(c,outfile);
      putc(c,outfile);
      putc(c,outfile);
    }
  fclose(outfile);
}
