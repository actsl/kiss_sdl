This is the version of the kiss_sdl GUI widget toolkit for Android. It is a part of the project https://github.com/actsl/kiss_sdl . All source code is taken from the kiss_sdl testing branch.

Two packages need to be installed for Android, called Android SDK and Android NDK, Android SDK may now be a part of Android Studio, they are also available for the open source Replicant OS and LineageOS. Download them and put them in some directory so you know where they are. Android SDK also provides a full graphical Android emulator, which can be configured and started by running android in the tools directory. Android NDK and the tools directory of the Android SDK have to be in path, like add the following to the ~/.profile, but they will be added to path only after restarting the computer.

export PATH=$PATH:/usr/local/programs/replicant-sdk_linux-x86/tools
export PATH=$PATH:/usr/local/programs/android-ndk-r14b
export NDK_PROJECT_PATH=.

Also download the source packages of SDL2, SDL_ttf and SDL_image, add the links to their directories to the example's directory jni, with names SDL, SDL_ttf and SDL_image. It is important not to compile them, as their ./configure scripts have bugs which damage them in regard of using them with Android.

For making the apk file for Android, go to the example's directory and run on the command line

ant debug install

Add the install there only when the Android emulator is running, then it also installs the apk in the emulator.

