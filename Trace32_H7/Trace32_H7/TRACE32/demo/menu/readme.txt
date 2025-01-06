; --------------------------------------------------------------------------------
; @Title: Readme for TRACE32 Menu Files
; @Description: File description of the menu directory
; @Author: -
; @Keywords: menu, icon, button, bitmap, editor, practice
; @Copyright: (C) 1989-2014 Lauterbach GmbH, licensed for use with TRACE32(R) only
; --------------------------------------------------------------------------------
; $Id: readme.txt 17181 2021-02-11 14:51:48Z mschaeffner $

t32.men             is the default english  menu for ICD and Simulator.
t32jp.men           is the default japanese menu for ICD and Simulator.
t32jp-utf-8.men     is the default japanese menu for ICD and Simulator in UTF-8 format.

demo.men            is a demo how to add menus and buttons to the standard menu.

internal_icons.cmm  displays all predefined TRACE32 icons which can be used to
                    define buttons and menu items for own purposes inside a menu,
                    dialog programming and tool bar.

t32icon.h           includes all standard bitmaps. They can be used as templates to
                    generate your own bitmaps with TRACE32 command BITMAPEDIT
                    or used directly with their name.

addmybutton.cmm     template file for creating a bitmap via command BITMAPEDIT

cbitmap.exe         converts Microsoft 8bit palette based bitmap files (256 colors) into TRACE32 icons.
                    call cbitmap.exe without parameters for command line help.

editor.men          menu example for using external editor

practice.men        menu and toolbar example for PRACTICE commands
