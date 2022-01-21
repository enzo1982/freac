#!/bin/bash
PREFIX=/usr/local

RELEASE=1.1.6
#RELEASE=1.1.6-`date +"%Y%m%d"`

SMOOTHVER=0.9
SMOOTHREV=0

BOCAVER=1.0
BOCAREV=3

machine=`gcc -dumpmachine`

if [[ $machine != *amd64* ]] && [[ $machine != *x86_64* ]]; then
  UNAME=`uname | tr [:upper:] [:lower:]`
else
  UNAME=`uname | tr [:upper:] [:lower:]`-x64
fi

mkdir -p freac

cp $PREFIX/bin/freac freac/
cp $PREFIX/bin/freaccmd freac/

mkdir -p freac/boca

cp $PREFIX/lib/boca/boca.$BOCAVER.so freac/boca/
cp $PREFIX/lib/libsmooth-$SMOOTHVER.so.$SMOOTHREV freac/
#cp $PREFIX/lib/libsmooth-js-$SMOOTHVER.so.$SMOOTHREV freac/

cp $PREFIX/lib/freac/freac.so freac/
cp $PREFIX/bin/smooth-translator freac/translator

cp ../../COPYING freac/
cp ../../Readme freac/
cp ../../Readme.de freac/

cp -R $PREFIX/share/freac/icons freac/
cp -R $PREFIX/share/freac/lang freac/
cp -R $PREFIX/share/doc/freac/manual freac/

cp $PREFIX/lib/boca/boca_decoder_*.1.0.so freac/boca/
cp $PREFIX/lib/boca/boca_deviceinfo_*.1.0.so freac/boca/
cp $PREFIX/lib/boca/boca_dsp_*.1.0.so freac/boca/
cp $PREFIX/lib/boca/boca_encoder_*.1.0.so freac/boca/
cp $PREFIX/lib/boca/boca_extension_*.1.0.so freac/boca/
cp $PREFIX/lib/boca/boca_output_*.1.0.so freac/boca/
cp $PREFIX/lib/boca/boca_playlist_*.1.0.so freac/boca/
cp $PREFIX/lib/boca/boca_tagger_*.1.0.so freac/boca/
cp $PREFIX/lib/boca/boca_verifier_*.1.0.so freac/boca/

cp $PREFIX/lib/boca/boca_*.1.0 freac/boca/

cp $PREFIX/lib/boca/boca_*.1.0.xml freac/boca/

cp -R $PREFIX/lib/freac/freac.extension.donate freac/boca/
cp -R $PREFIX/lib/freac/freac.extension.langnotify freac/boca/
cp -R $PREFIX/lib/freac/freac.extension.notifier freac/boca/
#cp -R $PREFIX/lib/freac/freac.extension.youtube freac/boca/

cp $PREFIX/lib/freac/freac_extension_*.1.0.so freac/boca/
cp $PREFIX/lib/freac/freac_verifier_*.1.0.so freac/boca/

cp $PREFIX/lib/libcdio.so.19 freac/libcdio.so.19
cp $PREFIX/lib/libcdio_cdda.so.2 freac/libcdio_cdda.so.2
cp $PREFIX/lib/libcdio_paranoia.so.2 freac/libcdio_paranoia.so.2

mkdir -p freac/codecs

cp $PREFIX/lib/libfaad.so freac/codecs/faad.so
cp $PREFIX/lib/libfdk-aac.so freac/codecs/fdk-aac.so
cp $PREFIX/lib/libFLAC.so freac/codecs/FLAC.so
cp $PREFIX/lib/libMAC.so freac/codecs/MAC.so
cp $PREFIX/lib/libmpg123.so freac/codecs/mpg123.so
cp $PREFIX/lib/libmp3lame.so freac/codecs/mp3lame.so
cp $PREFIX/lib/libmp4v2.so freac/codecs/mp4v2.so
cp $PREFIX/lib/libogg.so freac/codecs/ogg.so
cp $PREFIX/lib/libopus.so freac/codecs/opus.so
cp $PREFIX/lib/librnnoise.so freac/codecs/rnnoise.so
cp $PREFIX/lib/librubberband.so freac/codecs/rubberband.so
cp $PREFIX/lib/libsamplerate.so freac/codecs/samplerate.so
cp $PREFIX/lib/libsndfile.so freac/codecs/sndfile.so
cp $PREFIX/lib/libspeex.so freac/codecs/speex.so
cp $PREFIX/lib/libvorbis.so freac/codecs/vorbis.so
cp $PREFIX/lib/libvorbisenc.so freac/codecs/vorbisenc.so

mkdir -p freac/codecs/cmdline

cp $PREFIX/bin/ffmpeg freac/codecs/cmdline/
cp $PREFIX/bin/mpcdec freac/codecs/cmdline/
cp $PREFIX/bin/mpcenc freac/codecs/cmdline/
cp $PREFIX/bin/wavpack freac/codecs/cmdline/
cp $PREFIX/bin/wvunpack freac/codecs/cmdline/

strip --strip-all freac/libcdio*.so.*
strip --strip-all freac/codecs/cmdline/*
strip --strip-all freac/codecs/*.so

ln -sf boca/boca.$BOCAVER.so freac/libboca-$BOCAVER.so.$BOCAREV

rm freac/boca/boca_decoder_alac.1.0.xml
rm freac/boca/boca_decoder_ffmpeg_alac.1.0.xml
rm freac/boca/boca_decoder_ffmpeg_mac.1.0.xml
rm freac/boca/boca_decoder_ffmpeg_mpc.1.0.xml
rm freac/boca/boca_decoder_ffmpeg_wavpack.1.0.xml

rm freac/boca/freac_extension_youtube.1.0.so

# Create .tar.gz package
mkdir -p freac-$RELEASE-$UNAME

cp -R freac/* freac-$RELEASE-$UNAME

tar cfz freac-$RELEASE-$UNAME.tar.gz freac-$RELEASE-$UNAME

rm -fr freac-$RELEASE-$UNAME
