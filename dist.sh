#!/bin/bash

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
DATE="$( date '+%Y%m%d%H%S' )"
DIST_DIR=$SCRIPTPATH/dist
LAYOUT_DIR=$SCRIPTPATH/src/wii/res/layout/
BOOT_DOL_SRC=$SCRIPTPATH/boot.dol
BOOT_DOL_DEST=$DIST_DIR/apps/WII7800
BOOT_ELF_SRC=$SCRIPTPATH/boot.elf
BOOT_ELF_DEST=$DIST_DIR
META_FILE=$DIST_DIR/apps/WII7800/meta.xml
SDL_DIR=$SCRIPTPATH/thirdparty/sdl

#
# Function that is invoked when the script fails.
#
# $1 - The message to display prior to exiting.
#
function fail() {
    echo $1
    echo "Exiting."
    exit 1
}

# Build SDL
echo "Building SDL..."
cd $SDL_DIR || { fail 'Error changing to SDL directory.'; }
make || { fail 'Error building SDL.'; }

# Change to script directory
echo "Changing to script directory..."
cd $SCRIPTPATH || { fail 'Error changing to script directory.'; }

# Build Wii7800
echo "Building Wii7800..."
make || { fail 'Error building Wii7800.'; }

# Clear dist directory
if [ -d $DIST_DIR ]; then
    echo "Clearing dist directory..."
    rm -rf $DIST_DIR || { fail 'Error clearing dist directory.'; }
fi

# Copy layout
echo "Copy layout..."
cp -R $LAYOUT_DIR $DIST_DIR || { fail 'Error copying layout.'; }

# Remove .gitignore
echo "Cleaning layout..."
find $DIST_DIR -name .gitignore -type f -delete \
    || { fail 'Error cleaning layout.'; }

# Copy boot files
echo "Copying boot files..."
cp $BOOT_DOL_SRC $BOOT_DOL_DEST || { fail 'Error copying boot.dol.'; }

# Update date in meta file
echo "Setting date in meta file..."
sed -i "s,000000000000,$DATE,g" $META_FILE \
    || { fail 'Error setting date in meta file.'; }

# Update version in meta-file (if SNAPSHOT)
echo "Updating version in meta file..."
sed -i "s,-SNAPSHOT,-SNAPSHOT-$DATE,g" $META_FILE \
    || { fail 'Error setting version in meta file.'; }
    
# Create the distribution (zip)    
echo "Creating distribution..."
VERSION=$( sed -ne "s/.*version>\(.*\)<\/version.*/\1/p" $META_FILE )
VERSION_NO_DOTS="${VERSION//./_}"
DIST_FILE=wii7800-$VERSION_NO_DOTS.zip
cd $DIST_DIR || { fail 'Error changing to distribution directory.'; }
zip -r $DIST_FILE . || { fail 'Error creating zip file.'; }
rm -rf $DIST_DIR/WII7800 \
    || { fail 'Error deleting WII7800 directory in dist.'; }
rm -rf $DIST_DIR/apps \
    || { fail 'Error deleting apps directory in dist.'; }
cp $BOOT_ELF_SRC $BOOT_ELF_DEST || { fail 'Error copying boot.elf.'; }
