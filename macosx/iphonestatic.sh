xcodebuild -project Chipmunk.xcodeproj -sdk iphoneos4.0 -configuration Release -target ChipmunkStatic-iPhone
xcodebuild -project Chipmunk.xcodeproj -sdk iphonesimulator4.0 -configuration Debug -target ChipmunkStatic-iPhone
lipo build/Debug-iphonesimulator/libChipmunk-iPhone.a build/Release-iphoneos/libChipmunk-iPhone.a -create -output libChipmunk-iPhone.a
