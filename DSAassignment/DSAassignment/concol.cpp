/*
concol.cpp
Utility Class File for Console Classes (Functions are in the header)
Team Name: Wat.jpg
Team Member 1: Soh Yan Quan, Kenneth (S10147249G)
Team Member 2: Soh Hak Wee (S10146869K)
*/

#include "concol.h"

using namespace consolecolors;

HANDLE consolecolors::std_con_out;
//Standard Output Handle
bool consolecolors::colorprotect = false;
//If colorprotect is true, background and text colors will never be the same
consolecolors::concol consolecolors::textcol, consolecolors::backcol, consolecolors::deftextcol, consolecolors::defbackcol;
/*textcol - current text color
backcol - current back color
deftextcol - original text color
defbackcol - original back color*/