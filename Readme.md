# ![fre:ac logo](https://freac.org/images/stories/freac_icon_smaller.png) fre:ac - free audio converter
[![Build Status](https://img.shields.io/github/workflow/status/enzo1982/freac/Verify%20build/master)](https://github.com/enzo1982/freac/actions?query=workflow%3A%22Verify+build%22+branch%3Amaster) [![Last commit](https://img.shields.io/github/last-commit/enzo1982/freac.svg)](https://github.com/enzo1982/freac/commits/master) [![License](https://img.shields.io/github/license/enzo1982/freac.svg)](https://github.com/enzo1982/freac/blob/master/COPYING)

fre:ac is a free and open source audio converter. It supports audio CD ripping and tag editing and converts between various audio file formats.

![fre:ac on Ubuntu Linux](https://freac.org/images/freac-linux.png)

## Features
- Supported formats include MP3, MP4/AAC, Opus and FLAC
- Integrated CD ripper, player and tag editor
- Supports playlists, cue sheets and chapters
- Fast, multi-threaded conversion engine
- Ability to convert to multiple formats at once
- Can verify lossless input and output files
- Supports freedb/CDDB queries and submits
- Supports ripping of hidden tracks (HTOA)
- Supports fully automated CD ripping
- Support for using command line codecs
- User interface available in 40+ languages
- Optional command line interface

## Download
Pre-built packages for Windows, macOS, Linux and FreeBSD are available at [freac.org](https://freac.org/latest-release/).

## Installation
### Windows
fre:ac is distributed in two variants, an .exe file containing a setup wizard or alternatively a .zip archive that contains just the application without an installer. If you downloaded the .exe installer, simply run it and the setup wizard will guide you through the installation process creating start menu icons that will run fre:ac. If you downloaded the .zip package, please extract/move the contents to a location of your choice and run freac.exe to start fre:ac.

### macOS
fre:ac is distributed as an Apple Disk Image (.dmg) file. To install it, open the .dmg image and drag the fre:ac application to a location of your choice like the desktop or the Applications folder. Then double click the application to start fre:ac.

Alternatively, users of the package manager [MacPorts](http://www.macports.org) may install fre:ac using the following command:

    sudo port install freac

### Linux
fre:ac is available as a Snap, Flatpak or AppImage package. The Snap and Flatpak versions can be found and installed from the respective app stores. Please note that these versions run in restriced environments and might offer limited functionality. The AppImage package must be marked as executable after downloading using the following command:

    chmod a+x freac*.AppImage
The AppImage can then be executed to start fre:ac.

### FreeBSD
fre:ac is distributed as a .tar.gz archive. Please extract the contents to a location of your choice and execute the freac binary to start fre:ac.

## Compiling
fre:ac depends on the [BoCA audio component framework](https://github.com/enzo1982/boca/) and the [smooth Class Library](https://github.com/enzo1982/smooth/). Please build and install those first.

Development packages for the following libraries need to be installed in order to build smooth, BoCA and fre:ac:

- For the smooth Class Library: libbz2, libcurl, libfribidi, libgtk+3.0, libjpeg, libxml2
- For the BoCA component framework: libcdio, libcdio-paranoia, libexpat, libpulse (Linux only), liburiparser
- For fre:ac itself: libudev (Linux only)

When the dependencies are met, run `make` followed by `sudo make install` for smooth, BoCA and fre:ac respectively.

You should then be able to run fre:ac using `/usr/local/bin/freac` and the command line interface using `/usr/local/bin/freaccmd`.

----
The official fre:ac homepage: https://www.freac.org/

robert.kausch@freac.org,  
Robert Kausch
