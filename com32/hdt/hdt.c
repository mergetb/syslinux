/* ----------------------------------------------------------------------- *
 *
 *   Copyright 2009 Erwan Velu - All Rights Reserved
 *
 *   Permission is hereby granted, free of charge, to any person
 *   obtaining a copy of this software and associated documentation
 *   files (the "Software"), to deal in the Software without
 *   restriction, including without limitation the rights to use,
 *   copy, modify, merge, publish, distribute, sublicense, and/or
 *   sell copies of the Software, and to permit persons to whom
 *   the Software is furnished to do so, subject to the following
 *   conditions:
 *
 *   The above copyright notice and this permission notice shall
 *   be included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 *
 * -----------------------------------------------------------------------
 */

/*
 * hdt.c
 *
 * An Hardware Detection Tool
 */

#include <stdio.h>
#include <console.h>
#include <consoles.h>
#include <syslinux/vesacon.h>
#include "hdt.h"
#include "hdt-cli.h"
#include "hdt-menu.h"
#include "hdt-common.h"

int display_line_nb = 0;
bool disable_more_printf = false;
/* Defines the number of lines in the console
 * Default is 20 for a std console */
int max_console_lines=20;

int main(const int argc, const char *argv[])
{
  char version_string[256];
  const char *arg;
  struct s_hardware hardware;

  snprintf(version_string, sizeof version_string, "%s %s (%s)",
           PRODUCT_NAME,VERSION, CODENAME);

  /* Detecting parameters */
  detect_parameters(argc, argv, &hardware);

  /* Opening the Syslinux console */
  if (vesamode) openconsole(&dev_rawcon_r, &dev_vesaserial_w);
  else console_ansi_raw();

  /* Cleaning structures */
  init_hardware(&hardware);

  /* Detecting Syslinux version */
  detect_syslinux(&hardware);
  
  /* Clear the screen and reset position of the cursor */
  clear_screen();
  printf("\033[1;1H");
  
  printf("%s\n", version_string);

  if ((arg = find_argument(argv + 1, "nomenu")) || (find_argument(argv+1,"auto")))
  	start_cli_mode(&hardware);
  else {
  	int return_code = start_menu_mode(&hardware, version_string);
    	if (return_code == HDT_RETURN_TO_CLI)
      		start_cli_mode(&hardware);
    	else
  		return return_code;
  }
  return 0;
}
