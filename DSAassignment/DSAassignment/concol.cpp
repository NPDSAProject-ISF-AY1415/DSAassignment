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