#!/bin/bash

#RELEASE=1.1-rc1
RELEASE=1.1-rc-`date +"%Y%m%d"`

VERSION="v1.1 RC 1"

UNAME=macosx

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
  codesign --sign "$CERTNAME" --timestamp -o runtime ./freac.app/Contents/Resources/codecs/cmdline/avconv
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
mv Readme dmg
mv Readme.de dmg
cp .VolumeIcon.icns dmg

chmod 644 dmg/Copying
chmod 644 dmg/Readme
chmod 644 dmg/Readme.de
chmod 644 dmg/.VolumeIcon.icns

cp -R freac.app dmg
rm -f freac-$RELEASE-$UNAME.dmg

hdiutil create -fs HFS+ -format UDSP -volname "freac $VERSION" -srcfolder dmg freac-$RELEASE-$UNAME.dmg

# Mount .dmg
mkdir -p vol
hdiutil attach freac-$RELEASE-$UNAME.dmg.sparseimage -mountpoint vol
rm -r vol/.fseventsd

# Set custom icon
SetFile -a C vol

# Unmount .dmg
hdiutil detach vol

# Convert and finish .dmg
hdiutil convert -format UDBZ -o freac-$RELEASE-$UNAME.dmg freac-$RELEASE-$UNAME.dmg.sparseimage
hdiutil internet-enable -yes freac-$RELEASE-$UNAME.dmg

rm -f freac-$RELEASE-$UNAME.dmg.sparseimage
rm -fr dmg vol
