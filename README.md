# AuS
![Screenshot 2023-10-31 at 12 12 48 AM](https://github.com/AurimasBalciunas/AuS/assets/56936689/dd6c2cfc-fdb4-4dda-abc1-2e594af02089)
AuS allows you to display current album art and song info on your Raspberry Pi while playing music elsewhere. It is a C++ Qt6 application that relies on Spotify API to gather the data.

AuS can also be run on desktop. Required modules are QtBase, QtDeclarative, QtNetworkAuth, QtShaderTools, and Qt Quick.


# Why?
I have a smart speaker with no display, and I wanted album art and song info to be displayed on my TV regardless of what system I was playing the music through. 

# Setting up Spotify API App

I don't plan on hosting an authentication server for this small project so you will have to roll your own authorization app through Spotify for Developers. Thankfully, it is super easy:

Go here: https://developer.spotify.com

Click "Create App"

Give it a redirect_uri of https://localhost:3000

Save the client secret as environmental variable SPOTIFY_CLIENT_SECRET. If you are running this project through Qt Creator, you will have to go to Projects -> Project Settings -> Environment and add it there, since Qt Creator runs in an isolated environment. 

# Cross-Compiling
This Qt guide is mostly correct:
https://wiki.qt.io/Cross-Compile_Qt_6_for_Raspberry_Pi

However, a few important things that are not mentioned:
1) When picking a host distribution, make sure to pick one with the same GLIBC version as the raspi. You can check this by doing
"ldd --version" on the Raspberry Pi.
I ended up going with a Docker container running Ubuntu 20.04, which has GLIBC:
ldd (Ubuntu GLIBC 2.31-0ubuntu9.12) 2.31

2) When compiling the host distribution, you can minimize build time by specifying only required modules. Do this by specifying a module-subset in the perl-init repository command:
perl init-repository -f -module-subset=qtbase,qtdeclarative,qtnetworkauth,qtshadertools
qtshadertools is not used directly, but its component QSB is required for Qt Quick.

3) In your toolchain.cmake, you may need to set an ASM compiler:
set(CMAKE_ASM_COMPILER /usr/bin/aarch64-linux-gnu-gcc-9)

4) This is mentioned in the Known Issues section of the linked wiki page, but you may need to turn off dbus in your configuration command:
../qt5/configure -release -opengl es2 -nomake examples -nomake tests -qt-host-path $HOME/qt-host -extprefix $HOME/qt-raspi -prefix /usr/local/qt6 -device linux-rasp-pi4-aarch64 -device-option CROSS_COMPILE=aarch64-linux-gnu- -- -DCMAKE_TOOLCHAIN_FILE=$HOME/toolchain.cmake -DQT_FEATURE_xcb=ON -DFEATURE_xcb_xlib=ON -DQT_FEATURE_xlib=ON
