/****************
 * ui_toolbox.h *
 ****************/

/****************************************************************************
 *   (c) 2018 Marcio Teixeira                                               *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#ifndef _UI_TOOLBOX_
#define _UI_TOOLBOX_

#include "ui.h"

#include "ftdi_eve_constants.h"
#include "ftdi_eve_functions.h"
#include "ftdi_eve_panels.h"
#include "ftdi_eve_dl.h"
#include "ftdi_eve_spi.h"

#include "ui_framework.h"
#include "ui_sounds.h"
#include "ui_bitmaps.h"
#include "ui_builder.h"
#include "ui_event_loop.h"

namespace Extensible_UI_API {
  void onStartup();
  void onUpdate();
}

using namespace FTDI;
using namespace Extensible_UI_API;

class InterfaceScreen : public UIScreen {
  public:
    static void onRefresh(){
      using namespace FTDI;
      CLCD::CommandFifo cmd;
      cmd.cmd(CMD_DLSTART);

      current_screen.onRedraw(BOTH);

      cmd.cmd(DL::DL_DISPLAY);
      cmd.cmd(CMD_SWAP);
      cmd.execute();
    }
};

#endif // _UI_TOOLBOX_