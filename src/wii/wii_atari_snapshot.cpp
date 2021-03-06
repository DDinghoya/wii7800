/*
Wii7800 : Port of the ProSystem Emulator for the Wii

Copyright (C) 2010 raz0red
*/

#include <gctypes.h>

#include "ProSystem.h"

#include "wii_main.h"
#include "wii_snapshot.h"
#include "wii_util.h"

#include "wii_app_common.h"
#include "wii_atari_emulation.h"

/*
 * Saves with the specified save name
 *
 * filename   The name of the save file
 * return     Whether the save was successful
 */
BOOL wii_snapshot_handle_save( char* filename )
{
  return prosystem_Save( filename, false );   
}

/*
 * Determines the save name for the specified rom file
 *
 * romfile  The name of the rom file
 * buffer   The buffer to write the save name to
 */
void wii_snapshot_handle_get_name( const char *romfile, char *buffer )
{
  char filename[WII_MAX_PATH];            
  Util_splitpath( romfile, NULL, filename );
  snprintf( buffer, WII_MAX_PATH, "%s%s.%s",  
    WII_SAVES_DIR, filename, WII_SAVE_GAME_EXT );
}

/*
 * Starts the emulator for the specified snapshot file.
 *
 * savefile The name of the save file to load. 
 */
BOOL wii_start_snapshot( char *savefile )
{
  BOOL succeeded = FALSE;
  BOOL seterror = FALSE;

  // Determine the extension
  char ext[WII_MAX_PATH];
  Util_getextension( savefile, ext );

  if( !strcmp( ext, WII_SAVE_GAME_EXT ) )
  {
    char savename[WII_MAX_PATH];

    // Get the save name (without extension)
    Util_splitpath( savefile, NULL, savename );

    int namelen = strlen( savename );
    int extlen = strlen( WII_SAVE_GAME_EXT );

    if( namelen > extlen )
    {
      // build the associated rom name
      savename[namelen - extlen - 1] = '\0';

      char romfile[WII_MAX_PATH];
      snprintf( romfile, WII_MAX_PATH, "%s%s", WII_ROMS_DIR, savename );

      int exists = Util_fileexists( romfile );

      // Ensure the rom exists
      if( !exists )            
      {
        wii_set_status_message(
          "Unable to find associated ROM file." );                
        seterror = TRUE;
      }
      else
      {
        // launch the emulator for the save
        wii_start_emulation( romfile, savefile );
        succeeded = TRUE;
      }
    }
  }

  if( !succeeded && !seterror )
  {
    wii_set_status_message( 
      "The file selected is not a valid saved state file." );    
  }

  return succeeded;
}


