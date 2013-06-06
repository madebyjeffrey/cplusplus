// -s seed	    Specifies seed as number between 0.0 and 1.0
//   -w width	    Specifies width in pixels, default = 800
//   -h height	    Specifies height in pixels, default = 600
//   -m magnification  Specifies magnification, default = 1.0
//   -o output_file    Specifies output file, default is standard output
//   -l longitude	    Specifies longitude of centre in degrees, default = 0.0
//   -L latitude	    Specifies latitude of centre in degrees, default = 0.0
//   -g gridsize	    Specifies vertical gridsize in degrees, default = 0.0 (no grid)
//   -G gridsize	    Specifies horisontal gridsize in degrees, default = 0.0 (no grid)
//   -i init_alt	    Specifies initial altitude (default = -0.02)
//   -c		    Colour depends on latitude (default: only altitude)
//   -C file	    Read colour definitions from file
//   -O		    Produce a black and white outline map
//   -E		    Trace the edges of land in black on colour map
//   -B		    Use ``bumpmap'' shading
//   -b		    Use ``bumpmap'' shading on land only
//   -d                Use ``daylight'' shading

//   -pprojection	    Specifies projection: m = Mercator (default)
// 					  p = Peters
// 					  q = Square
// 					  s = Stereographic
// 					  o = Orthographic
// 					  g = Gnomonic
// 					  a = Area preserving azimuthal
// 					  c = Conical (conformal)
// 					  M = Mollweide
// 					  S = Sinusoidal
// 					  h = Heightfield
//                                           i = Icosahedral
// 					  f = Find match, see below

#include <string>
#include <tclap/CmdLine.h>

void cmd_parse(int argc, char**argv) {
    /*	try {
    		TCLAP::CmdLine cmd("Planet Generator", ' ', "1.0");
    		TCLAP::SwitchArg debugArg("X", "debug", "Debug mode", cmd, false);
    		TCLAP::SwitchArg daylightShadingArg("d", "Use daylight shading", cmd, false);
    		TCLAP::SwitchArg bumpmapLandShading("b", "

    		TCLAP::ValueArg<double> dd2Arg("V", "distancevariation",
    			"Distance contribution to variation (default = 0.035)", false, 0.035, "number under 1", cmd);
    		TCLAP::ValueArg<double> dd1Arg("v", "altitudevariation",
    			"Altitude contribution to variation (default = 0.45)", false, 0.45, "number under 1", cmd);
    		TCLAP::ValueArg<double> shadeAngle2Arg("A", "sunlatitude",
    			"Latitude of sun in daylight shading", false, 20.0, "angle in degrees", cmd);
    		TCLAP::ValueArg<double> shadeAngleArg("a", "sunlongitude",
    			"Angle of 'light' in bumpmap shading or longitude of sun in daylight shading",
    			false, 150.0, "angle in degrees", cmd);

    		*/

}

/*
	Idea for a config file:

	 -s seed	    Specifies seed as number between 0.0 and 1.0
  -w width	    Specifies width in pixels, default = 800
  -h height	    Specifies height in pixels, default = 600
  -m magnification  Specifies magnification, default = 1.0
  -o output_file    Specifies output file, default is standard output
  -l longitude	    Specifies longitude of centre in degrees, default = 0.0
  -L latitude	    Specifies latitude of centre in degrees, default = 0.0
  -g gridsize	    Specifies vertical gridsize in degrees, default = 0.0 (no grid)
  -G gridsize	    Specifies horisontal gridsize in degrees, default = 0.0 (no grid)
  -i init_alt	    Specifies initial altitude (default = -0.02)
  -c		    Colour depends on latitude (default: only altitude)
  -C file	    Read colour definitions from file
  -O		    Produce a black and white outline map
  -E		    Trace the edges of land in black on colour map
  -B		    Use ``bumpmap'' shading
  -b		    Use ``bumpmap'' shading on land only
  -d                Use ``daylight'' shading
  -a angle	    Angle of ``light'' in bumpmap shading
                    or longitude of sun in daylight shading
  -A latitude	    Latitude of sun in daylight shading
  -P		    Use PPM file format (default is BMP)
  -x		    Use XPM file format (default is BMP)
  -V number         Distance contribution to variation (default = 0.035)
  -v number         Altitude contribution to variation (default = 0.45)
  -pprojection	    Specifies projection: m = Mercator (default)
					  p = Peters
					  q = Square
					  s = Stereographic
					  o = Orthographic
					  g = Gnomonic
					  a = Area preserving azimuthal
					  c = Conical (conformal)
					  M = Mollweide
					  S = Sinusoidal
					  h = Heightfield
                                          i = Icosahedral
					  f = Find match, see below

===

[World.bmp] <-- determined
Seed: [0.0 - 1.0]
Width: def 800
Height: def 600
Mag: def 1.0




*/

