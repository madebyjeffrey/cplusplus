#include <stdio.h>

#include "planet.h"


void printbmp(FILE *outfile) /* prints picture in BMP format */
{
  int i,j,c,s, W1;

  fprintf(outfile,"BM");

  W1 = (3*Width+3);
  W1 -= W1 % 4;
  s = 54+W1*Height; /* file size */
  putc(s&255,outfile);
  putc((s>>8)&255,outfile);
  putc((s>>16)&255,outfile);
  putc(s>>24,outfile);

  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(54,outfile); /* offset to data */
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(40,outfile); /* size of infoheader */
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(Width&255,outfile);
  putc((Width>>8)&255,outfile);
  putc((Width>>16)&255,outfile);
  putc(Width>>24,outfile);

  putc(Height&255,outfile);
  putc((Height>>8)&255,outfile);
  putc((Height>>16)&255,outfile);
  putc(Height>>24,outfile);

  putc(1,outfile);  /* no. of planes = 1 */
  putc(0,outfile);

  putc(24,outfile);  /* bpp */
  putc(0,outfile);  

  putc(0,outfile); /* no compression */
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(0,outfile); /* image size (unspecified) */
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(0,outfile); /* h. pixels/m */
  putc(32,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(0,outfile); /* v. pixels/m */
  putc(32,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(0,outfile); /* colours used (unspecified) */
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);


  putc(0,outfile); /* important colours (all) */
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);

  if (doshade) {
    for (j=Height-1; j>=0; j--) {
      for (i=0; i<Width; i++) {
	s =shades[i][j];
	c = s*btable[col[i][j]]/150;
	if (c>255) c=255;
	putc(c,outfile);
	c = s*gtable[col[i][j]]/150;
	if (c>255) c=255;
	putc(c,outfile);
	c = s*rtable[col[i][j]]/150;
	if (c>255) c=255;
	putc(c,outfile);
      }
      for (i=3*Width; i<W1; i++) putc(0,outfile);
    }
  } else {
    for (j=Height-1; j>=0; j--) {
      for (i=0; i<Width; i++) {
	putc(btable[col[i][j]],outfile);
	putc(gtable[col[i][j]],outfile);
	putc(rtable[col[i][j]],outfile);
      }
      for (i=3*Width; i<W1; i++) putc(0,outfile);
    }
  }
  fclose(outfile);
}

void printbmpBW(FILE *outfile) /* prints picture in b/w BMP format */
{
  int i,j,c,s, W1;

  fprintf(outfile,"BM");

  W1 = (Width+31);
  W1 -= W1 % 32;
  s = 62+(W1*Height)/8; /* file size */
  putc(s&255,outfile);
  putc((s>>8)&255,outfile);
  putc((s>>16)&255,outfile);
  putc(s>>24,outfile);

  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(62,outfile); /* offset to data */
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(40,outfile); /* size of infoheader */
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(Width&255,outfile);
  putc((Width>>8)&255,outfile);
  putc((Width>>16)&255,outfile);
  putc(Width>>24,outfile);

  putc(Height&255,outfile);
  putc((Height>>8)&255,outfile);
  putc((Height>>16)&255,outfile);
  putc(Height>>24,outfile);

  putc(1,outfile);  /* no. of planes = 1 */
  putc(0,outfile);

  putc(1,outfile);  /* bpp */
  putc(0,outfile);  

  putc(0,outfile); /* no compression */
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(0,outfile); /* image size (unspecified) */
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(0,outfile); /* h. pixels/m */
  putc(32,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(0,outfile); /* v. pixels/m */
  putc(32,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(2,outfile); /* colours used */
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);


  putc(2,outfile); /* important colours (2) */
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(0,outfile); /* colour 0 = black */
  putc(0,outfile);
  putc(0,outfile);
  putc(0,outfile);

  putc(255,outfile); /* colour 1 = white */
  putc(255,outfile);
  putc(255,outfile);
  putc(255,outfile);

  for (j=Height-1; j>=0; j--)
    for (i=0; i<W1; i+=8) {
      if (i<Width && col[i][j] >= WHITE)
	c=128;
      else c=0;
      if (i+1<Width && col[i+1][j] >= WHITE)
	c+=64;
      if (i+2<Width && col[i+2][j] >= WHITE)
	c+=32;
      if (i+3<Width && col[i+3][j] >= WHITE)
	c+=16;
      if (i+4<Width && col[i+4][j] >= WHITE)
	c+=8;
      if (i+5<Width && col[i+5][j] >= WHITE)
	c+=4;
      if (i+6<Width && col[i+6][j] >= WHITE)
	c+=2;
      if (i+7<Width && col[i+7][j] >= WHITE)
	c+=1;
      putc(c,outfile);
    }
  fclose(outfile);
}
