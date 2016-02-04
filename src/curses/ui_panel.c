/**************************************************************************
 **
 ** sngrep - SIP Messages flow viewer
 **
 ** Copyright (C) 2013-2016 Ivan Alonso (Kaian)
 ** Copyright (C) 2013-2016 Irontec SL. All rights reserved.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/
/**
 * @file ui_panel.c
 * @author Ivan Alonso [aka Kaian] <kaian@irontec.com>
 *
 * @brief Source of functions defined in ui_panel.h
 */

#include "config.h"
#include "ui_panel.h"

ui_t *
ui_create(ui_t *ui)
{
    // If ui has no panel
    if (!ui->panel) {
        // Create the new panel for this ui
        if (ui->create) {
            ui->create(ui);
        }
    }

    // And return it
    return ui;
}

void
ui_destroy(ui_t *ui)
{
    // If there is no ui panel, we're done
    if (!ui || !ui->panel)
        return;

    // Hide this panel before destroying
    hide_panel(ui->panel);

    // If panel has a destructor function use it
    if (ui->destroy) {
        ui->destroy(ui);
    }

    // Initialize panel pointer
    ui->panel = NULL;
}

PANEL *
ui_get_panel(ui_t *ui)
{
    // Return panel pointer of ui struct
    return (ui) ? ui->panel : NULL;
}

int
ui_draw_panel(ui_t *ui)
{
    //! Sanity check, this should not happen
    if (!ui || !ui->panel)
        return -1;

    // Request the panel to draw on the scren
    if (ui->draw) {
        return ui->draw(ui);
    } else {
        touchwin(ui->win);
    }

    return 0;
}

int
ui_resize_panel(ui_t *ui)
{
    //! Sanity check, this should not happen
    if (!ui)
        return -1;

    // Notify the panel screen size has changed
    if (ui->resize) {
        return ui->resize(ui);
    }

    return 0;
}

void
ui_help(ui_t *ui)
{
    // Disable input timeout
    nocbreak();
    cbreak();

    // If current ui has help function
    if (ui->help) {
        ui->help(ui);
    }
}

int
ui_handle_key(ui_t *ui, int key)
{
    if (ui->handle_key) {
        return ui->handle_key(ui, key);
    }

    return 0;
}


void
ui_panel_create(ui_t *ui, int height, int width)
{
    ui->width = width;
    ui->height = height;
    ui->x = ui->y = 0;

    // If panel doesn't fill the screen center it
    if (ui->height != LINES) ui->x = (LINES - height) / 2;
    if (ui->width != COLS) ui->y = (COLS - width) / 2;

    ui->win = newwin(height, width, ui->x, ui->y);
    ui->panel = new_panel(ui->win);
}

void
ui_panel_destroy(ui_t *ui)
{
    // Deallocate panel window
    delwin(ui->win);
    // Deallocate panel pointer
    del_panel(ui->panel);
}
