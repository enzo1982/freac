#!/bin/bash

RELEASE=1.1.6
#RELEASE=1.1.6-`date +"%Y%m%d"`

VERSION="v1.1.6"
#VERSION="v1.1.6 (`date +"%Y%m%d"`)"

MACOSX_TARGET=$MACOSX_DEPLOYMENT_TARGET
if [[ -z $MACOSX_TARGET ]]; then
  MACOSX_TARGET=`sw_vers | awk '$1 == "ProductVersion:" { print $2 }'`
fi

PLATFORM=macos${MACOSX_TARGET%%.*}

if [[ "$1" == "translation" ]]; then
  TRANSLATION=1
  VERSION="$VERSION Translation Kit"

  shift
fi

if [[ -n $1 ]]; then
  CERTNAME="Developer ID Application: $1"

  if [[ -n $2 ]]; then
    PASSWORD="$2"
  else
    echo Enter notarization password:
    read PASSWORD
  fi
fi

# Remove old files
rm -fr freac.app
rm -f .VolumeIcon.icns Copying Readme Readme.de

# Extract .tar.gz archive
tar xfz freac.tar.gz

# Sign and notarize
if [[ -n $CERTNAME ]]; then
  # Sign application
  echo Signing application...
  find freac.app -name *.dylib | xargs -I $ codesign --sign "$CERTNAME" --timestamp $
  codesign --sign "$CERTNAME" --timestamp -o runtime ./freac.app/Contents/Resources/codecs/cmdline/ffmpeg
  codesign --sign "$CERTNAME" --timestamp -o runtime ./freac.app/Contents/Resources/codecs/cmdline/mpcdec
  codesign --sign "$CERTNAME" --timestamp -o runtime ./freac.app/Contents/Resources/codecs/cmdline/mpcenc
  codesign --sign "$CERTNAME" --timestamp -o runtime ./freac.app/Contents/Resources/codecs/cmdline/wavpack
  codesign --sign "$CERTNAME" --timestamp -o runtime ./freac.app/Contents/Resources/codecs/cmdline/wvunpack
  codesign --sign "$CERTNAME" --timestamp -o runtime --entitlements entitlements.plist ./freac.app/Contents/Resources/translator.app
  codesign --sign "$CERTNAME" --timestamp -o runtime --entitlements entitlements.plist ./freac.app/Contents/MacOS/freaccmd
  codesign --sign "$CERTNAME" --timestamp -o runtime --entitlements entitlements.plist ./freac.app

  rm -f entitlements.plist

  # Notarize app
  if [[ -n $CERTNAME ]]; then
    zip -r freac.zip freac.app

    echo Uploading application for notarization...
    if xcrun altool --notarize-app -f freac.zip --primary-bundle-id org.freac.freac -u robert.kausch@freac.org --password "$PASSWORD" > notarize.log 2>&1; then
      cat notarize.log
      RequestUUID=$(awk -F ' = ' '/RequestUUID/ {print $2}' notarize.log)

      # Check status periodically
      while sleep 30 && date; do
        # Check notarization status
        if xcrun altool --notarization-info "$RequestUUID" -u robert.kausch@freac.org --password "$PASSWORD" > notarize.info 2>&1; then
          cat "notarize.info"

          # Once notarization is complete, run stapler
          if ! grep -q "Status: in progress" notarize.info; then
            echo Stapling ticket to application...
            xcrun stapler staple freac.app
            break
          fi
        else
          cat notarize.info 1>&2
          exit 1
        fi
      done
    else
      cat notarize.log 1>&2
      exit 1
    fi

    rm -f freac.zip notarize.log notarize.info
  fi
fi

# Create .dmg image
mkdir -p dmg

mv Copying dmg
cp .VolumeIcon.icns dmg

chmod 644 dmg/Copying
chmod 644 dmg/.VolumeIcon.icns

if [[ -n $TRANSLATION ]]; then
  rm Readme
  rm Readme.de

  cp -R freac.app/Contents/Resources/lang dmg/lang
  ln -s freac.app/Contents/Resources/translator.app dmg/Translator.app
else
  mv Readme dmg
  mv Readme.de dmg

  chmod 644 dmg/Readme
  chmod 644 dmg/Readme.de
fi

cp -R freac.app dmg
rm -f freac-$RELEASE-$PLATFORM.dmg

hdiutil create -fs HFS+ -format UDSP -volname "freac $VERSION" -srcfolder dmg freac-$RELEASE-$PLATFORM.dmg

# Mount .dmg
mkdir -p vol
hdiutil attach freac-$RELEASE-$PLATFORM.dmg.sparseimage -mountpoint vol
rm -r vol/.fseventsd

# Set custom icon
SetFile -a C vol

# Unmount .dmg
hdiutil detach vol

# Convert and finish .dmg
hdiutil convert -format UDBZ -o freac-$RELEASE-$PLATFORM.dmg freac-$RELEASE-$PLATFORM.dmg.sparseimage

if [[ -n $TRANSLATION ]]; then
  mv freac-$RELEASE-$PLATFORM.dmg freac-${RELEASE%%-*}-translation-kit-$PLATFORM.dmg
fi

rm -f freac-$RELEASE-$PLATFORM.dmg.sparseimage
rm -fr dmg vol
