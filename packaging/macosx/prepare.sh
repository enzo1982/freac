#!/bin/bash

PREFIX=/usr/local

SMOOTHVER=0.9.0
BOCAVER=1.0.3

# Create folders
mkdir -p freac.app/Contents/MacOS
mkdir -p freac.app/Contents/Frameworks
mkdir -p freac.app/Contents/Resources/boca
mkdir -p freac.app/Contents/Resources/codecs/cmdline
mkdir -p freac.app/Contents/Resources/lang
mkdir -p freac.app/Contents/Resources/manual
mkdir -p freac.app/Contents/Resources/translator.app/Contents/MacOS
mkdir -p freac.app/Contents/Resources/translator.app/Contents/Frameworks

# Collect files
cp ../../Copying .
cp ../../Readme .
cp ../../Readme.de .

cp $PREFIX/bin/freac freac.app/Contents/MacOS/
cp $PREFIX/bin/freaccmd freac.app/Contents/MacOS/

cp $PREFIX/lib/libboca-$BOCAVER.dylib freac.app/Contents/Frameworks/
cp $PREFIX/lib/libsmooth-$SMOOTHVER.dylib freac.app/Contents/Frameworks/
#cp $PREFIX/lib/libsmooth-js-$SMOOTHVER.dylib freac.app/Contents/Frameworks/

cp $PREFIX/lib/libsmooth-$SMOOTHVER.dylib freac.app/Contents/Resources/translator.app/Contents/Frameworks/

cp $PREFIX/lib/freac/freac.dylib freac.app/Contents/Resources/

cp $PREFIX/bin/smooth-translator freac.app/Contents/Resources/translator.app/Contents/MacOS/translator

cp -R $PREFIX/share/freac/icons freac.app/Contents/Resources/
cp -R $PREFIX/share/freac/lang freac.app/Contents/Resources/
cp -R $PREFIX/share/doc/freac/manual freac.app/Contents/Resources/

cp $PREFIX/lib/boca/boca_decoder_*.1.0.dylib freac.app/Contents/Resources/boca/
cp $PREFIX/lib/boca/boca_deviceinfo_*.1.0.dylib freac.app/Contents/Resources/boca/
cp $PREFIX/lib/boca/boca_dsp_*.1.0.dylib freac.app/Contents/Resources/boca/
cp $PREFIX/lib/boca/boca_encoder_*.1.0.dylib freac.app/Contents/Resources/boca/
cp $PREFIX/lib/boca/boca_extension_*.1.0.dylib freac.app/Contents/Resources/boca/
cp $PREFIX/lib/boca/boca_output_*.1.0.dylib freac.app/Contents/Resources/boca/
cp $PREFIX/lib/boca/boca_playlist_*.1.0.dylib freac.app/Contents/Resources/boca/
cp $PREFIX/lib/boca/boca_tagger_*.1.0.dylib freac.app/Contents/Resources/boca/
cp $PREFIX/lib/boca/boca_verifier_*.1.0.dylib freac.app/Contents/Resources/boca/

cp $PREFIX/lib/boca/boca_*.1.0.xml freac.app/Contents/Resources/boca/

cp -R $PREFIX/lib/freac/freac.extension.donate freac.app/Contents/Resources/boca/
cp -R $PREFIX/lib/freac/freac.extension.langnotify freac.app/Contents/Resources/boca/
cp -R $PREFIX/lib/freac/freac.extension.notifier freac.app/Contents/Resources/boca/
#cp -R $PREFIX/lib/freac/freac.extension.youtube freac.app/Contents/Resources/boca/

cp $PREFIX/lib/freac/freac_extension_*.1.0.dylib freac.app/Contents/Resources/boca/
cp $PREFIX/lib/freac/freac_verifier_*.1.0.dylib freac.app/Contents/Resources/boca/

cp $PREFIX/lib/libcdio.dylib freac.app/Contents/Resources/cdio.dylib
cp $PREFIX/lib/libcdio_cdda.dylib freac.app/Contents/Resources/cdio_cdda.dylib
cp $PREFIX/lib/libcdio_paranoia.dylib freac.app/Contents/Resources/cdio_paranoia.dylib

cp $PREFIX/lib/libfaad.dylib freac.app/Contents/Resources/codecs/faad.dylib
cp $PREFIX/lib/libFLAC.dylib freac.app/Contents/Resources/codecs/FLAC.dylib
cp $PREFIX/lib/libMAC.dylib freac.app/Contents/Resources/codecs/MAC.dylib
cp $PREFIX/lib/libmpg123.dylib freac.app/Contents/Resources/codecs/mpg123.dylib
cp $PREFIX/lib/libmp3lame.dylib freac.app/Contents/Resources/codecs/mp3lame.dylib
cp $PREFIX/lib/libmp4v2.dylib freac.app/Contents/Resources/codecs/mp4v2.dylib
cp $PREFIX/lib/libogg.dylib freac.app/Contents/Resources/codecs/ogg.dylib
cp $PREFIX/lib/libopus.dylib freac.app/Contents/Resources/codecs/opus.dylib
cp $PREFIX/lib/librnnoise.dylib freac.app/Contents/Resources/codecs/rnnoise.dylib
cp $PREFIX/lib/librubberband.dylib freac.app/Contents/Resources/codecs/rubberband.dylib
cp $PREFIX/lib/libsamplerate.dylib freac.app/Contents/Resources/codecs/samplerate.dylib
cp $PREFIX/lib/libsndfile.dylib freac.app/Contents/Resources/codecs/sndfile.dylib
cp $PREFIX/lib/libspeex.dylib freac.app/Contents/Resources/codecs/speex.dylib
cp $PREFIX/lib/libvorbis.dylib freac.app/Contents/Resources/codecs/vorbis.dylib
cp $PREFIX/lib/libvorbisenc.dylib freac.app/Contents/Resources/codecs/vorbisenc.dylib

cp $PREFIX/bin/ffmpeg freac.app/Contents/Resources/codecs/cmdline/
cp $PREFIX/bin/mpcdec freac.app/Contents/Resources/codecs/cmdline/
cp $PREFIX/bin/mpcenc freac.app/Contents/Resources/codecs/cmdline/
cp $PREFIX/bin/wavpack freac.app/Contents/Resources/codecs/cmdline/
cp $PREFIX/bin/wvunpack freac.app/Contents/Resources/codecs/cmdline/

rm freac.app/Contents/Resources/boca/boca_decoder_alac.1.0.xml
rm freac.app/Contents/Resources/boca/boca_decoder_ffmpeg_mpc.1.0.xml
rm freac.app/Contents/Resources/boca/boca_decoder_ffmpeg_wavpack.1.0.xml
rm freac.app/Contents/Resources/boca/boca_encoder_ffmpeg_alac.1.0.xml

rm freac.app/Contents/Resources/boca/freac_extension_youtube.1.0.dylib

# Fix library names
install_name_tool -change libsmooth-$SMOOTHVER.dylib @executable_path/../Frameworks/libsmooth-$SMOOTHVER.dylib freac.app/Contents/MacOS/freac
install_name_tool -change libsmooth-$SMOOTHVER.dylib @executable_path/../Frameworks/libsmooth-$SMOOTHVER.dylib freac.app/Contents/MacOS/freaccmd
install_name_tool -change libsmooth-$SMOOTHVER.dylib @executable_path/../Frameworks/libsmooth-$SMOOTHVER.dylib freac.app/Contents/Frameworks/libboca-$BOCAVER.dylib
#install_name_tool -change libsmooth-$SMOOTHVER.dylib @executable_path/../Frameworks/libsmooth-$SMOOTHVER.dylib freac.app/Contents/Frameworks/libsmooth-js-$SMOOTHVER.dylib

install_name_tool -change libboca-$BOCAVER.dylib @executable_path/../Frameworks/libboca-$BOCAVER.dylib freac.app/Contents/Resources/freac.dylib
install_name_tool -change libsmooth-$SMOOTHVER.dylib @executable_path/../Frameworks/libsmooth-$SMOOTHVER.dylib freac.app/Contents/Resources/freac.dylib

find freac.app/Contents/Resources/boca -name boca_*.dylib | xargs -I $ install_name_tool -change libboca-$BOCAVER.dylib @executable_path/../Frameworks/libboca-$BOCAVER.dylib $
find freac.app/Contents/Resources/boca -name boca_*.dylib | xargs -I $ install_name_tool -change libsmooth-$SMOOTHVER.dylib @executable_path/../Frameworks/libsmooth-$SMOOTHVER.dylib $

install_name_tool -change $PREFIX/lib/libcdio.19.dylib @executable_path/../Resources/cdio.dylib freac.app/Contents/Resources/boca/boca_decoder_cdio.1.0.dylib
install_name_tool -change $PREFIX/lib/libcdio_cdda.2.dylib @executable_path/../Resources/cdio_cdda.dylib freac.app/Contents/Resources/boca/boca_decoder_cdio.1.0.dylib
install_name_tool -change $PREFIX/lib/libcdio_paranoia.2.dylib @executable_path/../Resources/cdio_paranoia.dylib freac.app/Contents/Resources/boca/boca_decoder_cdio.1.0.dylib

install_name_tool -change $PREFIX/lib/libcdio.19.dylib @executable_path/../Resources/cdio.dylib freac.app/Contents/Resources/boca/boca_deviceinfo_cdio.1.0.dylib

find freac.app/Contents/Resources/boca -name freac_*.dylib | xargs -I $ install_name_tool -change libboca-$BOCAVER.dylib @executable_path/../Frameworks/libboca-$BOCAVER.dylib $
find freac.app/Contents/Resources/boca -name freac_*.dylib | xargs -I $ install_name_tool -change libsmooth-$SMOOTHVER.dylib @executable_path/../Frameworks/libsmooth-$SMOOTHVER.dylib $

#install_name_tool -change libsmooth-js-$SMOOTHVER.dylib @executable_path/../Frameworks/libsmooth-js-$SMOOTHVER.dylib freac.app/Contents/Resources/boca/freac_extension_youtube.1.0.dylib

install_name_tool -change $PREFIX/lib/libcdio.19.dylib @executable_path/../Resources/cdio.dylib freac.app/Contents/Resources/cdio_cdda.dylib

install_name_tool -change $PREFIX/lib/libcdio.19.dylib @executable_path/../Resources/cdio.dylib freac.app/Contents/Resources/cdio_paranoia.dylib
install_name_tool -change $PREFIX/lib/libcdio_cdda.2.dylib @executable_path/../Resources/cdio_cdda.dylib freac.app/Contents/Resources/cdio_paranoia.dylib

install_name_tool -change $PREFIX/lib/libogg.0.dylib @executable_path/../Resources/codecs/ogg.dylib freac.app/Contents/Resources/codecs/FLAC.dylib

install_name_tool -change $PREFIX/lib/libogg.0.dylib @executable_path/../Resources/codecs/ogg.dylib freac.app/Contents/Resources/codecs/vorbis.dylib

install_name_tool -change $PREFIX/lib/libogg.0.dylib @executable_path/../Resources/codecs/ogg.dylib freac.app/Contents/Resources/codecs/vorbisenc.dylib
install_name_tool -change $PREFIX/lib/libvorbis.0.dylib @executable_path/../Resources/codecs/vorbis.dylib freac.app/Contents/Resources/codecs/vorbisenc.dylib

install_name_tool -change libsmooth-$SMOOTHVER.dylib @executable_path/../Frameworks/libsmooth-$SMOOTHVER.dylib freac.app/Contents/Resources/translator.app/Contents/MacOS/translator

# Create .tar.gz archive
rm -f freac.tar.gz
tar cfz freac.tar.gz freac.app Copying Readme Readme.de .VolumeIcon.icns entitlements.plist

rm -f Copying Readme Readme.de
