#include <stdio.h>

#include "planet.h"

void printxpm(FILE *outfile) /* prints picture in XPM (X-windows pixel map) format */
{
  int x,y,i,nbytes;

  x = nocols - 1;
  for (nbytes = 0; x != 0; nbytes++)
    x >>= 5;
  
  fprintf(outfile,"/* XPM */\n");
  fprintf(outfile,"static char *xpmdata[] = {\n");
  fprintf(outfile,"/* width height ncolors chars_per_pixel */\n");
  fprintf(outfile,"\"%d %d %d %d\",\n", Width, Height, nocols, nbytes);
  fprintf(outfile,"/* colors */\n");
  for (i = 0; i < nocols; i++)
    fprintf(outfile,"\"%s c #%2.2X%2.2X%2.2X\",\n", 
	    nletters(nbytes, i), rtable[i], gtable[i], btable[i]);

  fprintf(outfile,"/* pixels */\n");
  for (y = 0 ; y < Height; y++) {
    fprintf(outfile,"\"");
    for (x = 0; x < Width; x++)
      fprintf(outfile, "%s", nletters(nbytes, col[x][y]));
    fprintf(outfile,"\",\n");
  }
  fprintf(outfile,"};\n");

  fclose(outfile);
}

void printxpmBW(FILE *outfile) /* prints picture in XPM (X-windows pixel map) format */
{
  int x,y,nbytes;

  x = nocols - 1;
  nbytes = 1;
  
  fprintf(outfile,"/* XPM */\n");
  fprintf(outfile,"static char *xpmdata[] = {\n");
  fprintf(outfile,"/* width height ncolors chars_per_pixel */\n");
  fprintf(outfile,"\"%d %d %d %d\",\n", Width, Height, 2, nbytes);
  fprintf(outfile,"/* colors */\n");
  
  fprintf(outfile,"\". c #FFFFFF\",\n");
  fprintf(outfile,"\"X c #000000\",\n");

  fprintf(outfile,"/* pixels */\n");
  for (y = 0 ; y < Height; y++) {
    fprintf(outfile,"\"");
    for (x = 0; x < Width; x++)
      fprintf(outfile, "%s",
	      (col[x][y] < WHITE)
	      ? "X" : ".");
    fprintf(outfile,"\",\n");
  }
  fprintf(outfile,"};\n");

  fclose(outfile);
}
