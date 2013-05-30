# Bedazzled

This application reveals all possible moves in the StarCraft II arcade game, StarJeweled. In order to use it, simply launch the game and start this application. When the game window is active an overlay window pops up to display all possible moves. This overlay can be disabled through the system menu accessible via the top-left icon or by pressing Alt+Space. In order to achieve good performance, play at high resolutions.

**Warning:** This application should only be used for educational purposes.

### Colors
* Green - Three jewel combo
* Yellow - Four jewel combo
* Red - Five or more jewel combo

### Notes
* This application works with all resolutions however, the higher the better
* Windows Aero is disabled in order to increase image capturing performance
* This application requires the [Microsoft C++ 2012 (x86)](http://www.microsoft.com/en-us/download/details.aspx?id=30679) Redistributable package
* As of this moment, only Windows 7 has been tested. Windows 8 not supported.

### Compiling
This application can only be compiled on Windows using Visual Studio 2012. The Qt Add-in must be installed along with a custom build of Qt5 (Version 5.0.2). The following configuration should be used:

configure -opensource -confirm-license -debug-and-release -static -c++11 -make libs -make tools -nomake demos -nomake examples -nomake tests -opengl desktop -qt-zlib -qt-pcre -no-gif -qt-libpng -qt-libjpeg -qt-freetype -no-angle -no-rtti -no-openssl -no-dbus -no-style-windowsxp -no-style-windowsvista -no-style-fusion

### Author
* Email: <dave@krutsko.net>
* Home: [dave.krutsko.net](http://dave.krutsko.net)
* GitHub: [github.com/dkrutsko](https://github.com/dkrutsko)