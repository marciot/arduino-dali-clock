/******************
 * ui_builder.cpp *
 ******************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018                                        *
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

#include "ui.h"

#if ENABLED(EXTENSIBLE_UI)

#include "ftdi_eve_constants.h"
#include "ftdi_eve_functions.h"
#include "ftdi_eve_panels.h"
#include "ftdi_eve_dl.h"

#include "ui_builder.h"

CommandProcessor::btn_style_func_t  *CommandProcessor::_btn_style_callback = NULL;
bool CommandProcessor::is_tracking = false;

#endif // EXTENSIBLE_UI