/*
Copyright (C) 2010 raz0red
*/

#ifndef WII_APP_H
#define WII_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wii_main.h"

// The most recent status message
extern char wii_status_message[WII_MENU_BUFF_SIZE];

// The status message display count down
extern u8 wii_status_message_count;

/*
 * Updates the specified result string with the location of the file relative
 * to the appliation root directory.
 * 
 * file     The file name
 * result   The buffer to store the result in
 */
extern void wii_get_app_relative( const char *file, char *result );

/*
 * Returns the base application path
 *
 * return   The base application path
 */
extern char* wii_get_app_path();

/*
 * Determines and stores the base application path
 *
 * argc     The count of main arguments
 * argv     The array of argument values
 */
extern void wii_set_app_path( int argc, char *argv[] );

/*
 * Pause and wait for input. Usually used when debugging.
 */
extern void wii_pause();

/*
 * Writes the specified string to the VT display
 */
extern void wii_write_vt( char *buffer );

/*
 * Initializes the console output to the specified frame buffer. This 
 * allows us to swap between console output buffers.
 *
 * fb       The frame buffer to associated with the console
 */
extern void wii_console_init( void *fb );

/*
 * Stores a status message for display in the footer of the menu
 */ 
extern void wii_set_status_message( const char *message );

#ifdef __cplusplus
}
#endif

#endif