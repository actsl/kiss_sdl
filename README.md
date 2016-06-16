NAME
====

kiss\_sdl - Simple universal GUI widget toolkit for SDL2


SCREENSHOTS
===========

![Example One](https://raw.githubusercontent.com/actsl/kiss_sdl/master/kiss_ss1.jpg)

![Example Two](https://raw.githubusercontent.com/actsl/kiss_sdl/master/kiss_ss2.jpg)


DESCRIPTION
===========

KISS is an acronym and a software development approach, meaning "Keep It Short and Simple". Differing from other widget toolkits where one is supposed to choose a widget of "best-fit", kiss\_sdl is designed so that if you cannot find a widget satisfying all of your needs, it's easy to modify an existing one, or even write your own. Most other toolkits say they allow you to write your own widgets, but aren't designed for it, and generally don't support intricate designs. kiss\_sdl was made as simple as possible to solve the common problem of complexity, which can make it impossible for some people to even understand other toolkits. Unfortunately, many of the simpler toolkits that have tried to solve this problem in the past, don't allow you to do all the things that you want, due to their restricted internal design. kiss\_sdl implements an original, innovative approach, meaning it's an easier widget toolkit; and since it's written in pure C, it can be used with many other languages, such as C++ and Objective-C/Swift.


KISS_SDL CRASH-COURSE
=====================

This widget toolkit is made as modifiable as possible, so that it will not stand in the way of what the user wants to do. It's divided into four files: `kiss_draw.c`, `kiss_general.c`, `kiss_posix.c`, and `kiss_widgets.c`. The file `kiss_widgets.c` contains only widgets, and none of the SDL or external library functions, so that it's very easy to add new GUI widgets. The other three files serve as abstraction layers, and rewriting or modifying the functions in these files allows you to redefine the operations that use external libraries, like SDL2. `kiss_draw.c` is for all widget drawing functions, `kiss_general.c` contains utility functions, such as an implementation of the variable size array, and `kiss_posix.c` holds necessary POSIX functions.

None of the global variables are declared as constant to make the code more easily modifiable, but they should be considered constants; only change them during initialization. In the case of an error, functions return -1 or NULL, depending on the return type, and event processing functions return 0.

Each widget has three related functions: a function to create a new widget, a function to process it's events, and a function to draw it. These are, however, only the bare-minimum functions to make a working widget. A user can easily write their own functionality, either by extending an existing function or starting from scratch, to do anything that they want. Please see the two included examples for simple implementation and basic GUI to get you started.

Widgets can be made visible and invisible. Do note that an invisible widget is also inactive and doesn't perform any functions. In the examples, all widgets are created first, and then only those that should be active and present on the screen are made visible. It is also possible to create and free widgets dynamically. Making a user interface in that way may sound strange at first, but all the user written functions for widgets have to be there anyway; a lot has to be there for all widgets. For bigger interfaces it is possible to create widgets dynamically. But making a widget by switching the visibility enables a simpler implementation of a user interface.

The functions for creating widgets also have an optional argument for a window, `wdw`, which, if provided, makes the widget match wdw's visibility. A window is, thus, like a dialog window, on which all the widgets in the dialog are drawn. The provided examples show how to do this. Some functions also have an additional argument named `decorate`, this determines whether the widget should be decorated by the default function. This will draw a blue rectangle around the edge of the widget.

In addition to visibility, the widgets may also have focus. Focus determines whether the widget processes mouse events. The focus of the widget is determined by the focus of the window, `wdw`. If the window provided has no focus, the focus of a widget is determined by the focus member of its structure.

Event processing functions are called after one another in the event processing loop. The scrollbar, progressbar and combo box event functions are also called after the event processing loop, with the event argument `NULL`. Base functions will return nonzero when they process an event that the user may want to use elsewhere. The event functions have one final argument: a pointer to an integer that is assigned a nonzero value. This is an optimization, used to determine when the widgets have to be redrawn. Default widgets take advantage of this fully, and custom widgets should too, as drawing every frame, even when it's not needed, will impact performance.

The drawing functions are also called one after another, but in every iteration, and only after the event processing loop. This ensures that there are no redundant drawing calls, as drawing is the most expensive action. A user may write their own drawing functions, and call base functions from inside of them to do an additional drawing. The order of the drawing functions is quite important, as the next function draws over the last. This is especially important when combo boxes are used, as their popup text boxes draw over the widgets below the combo box.

Ten widgets are implemented as base widgets, these are a window, multiline label, button, select button, vertical scrollbar, horizontal scrollbar, progress bar, entry box, text box, and combo box. The two examples show how to use all of these widgets together. The first example implements a working file selection dialog with scrollable text boxes, and the second example shows how to use a combo box. Likely, most user interfaces can be made by using only these widgets, but the toolkit doesn't prevent adding any additional functionality.

Is it possible to make composite widgets from several other widgets? Yes, indeed! Widgets have coordinates relative to the main window, but it is possible, and not too difficult, to add the coordinates of a local window to a group of widgets. It is also possible to create and free such a group of widgets in one function, or process its events or draw it together. The combo box is one example of a composite widget.

For the default textbox widget, editing text is similar the early versions of Unix: backspace deletes the last character and control-u deletes the entire text. Clicking inside the entry widget causes it to become active, and its text can then be edited. Pressing Enter when the widget is active, causes the entry widget to become inactive, and causes its event function to return true. This makes it easy to run code on the changed value. Additional functionality can easily be added, such as use of the clipboard or caret navigation, but this has not been provided.

Is it possible to do the event processing in a separate function? Yes, it is, by using separate event loops for different widgets, or by passing objects using a variable size array, but for simplicity this has not been done in the examples of this toolkit. This also makes a re-entrant or thread-safe code possible; different widgets could be drawn in different threads. In kiss\_sdl, there is essentially no central code, although in a threaded implementation, a table may be necessary, which would keep track of whether each thread has received an expose event, and whether it has processed it.

Can images for the widgets be compiled into the code, instead of being in separate files? Yes, they can; images compiled into C code can be used by kiss\_sdl. The open-source tool, Gimp, makes saving directly to code very easy, if you're interested. However, they will need to be retrieved from the data in the code with a special function. But again because of simplicity, this has not been done in the included examples.

Is it possible to use other colors, images, etc? Yes! To use other fonts or textures, you may have to rewrite the `init` function in `kiss_draw.c`. You can draw anything you like on the widgets in the drawing function, they will be automatically redrawn after every update.

Can I do this, or that other thing? You can do whatever you want! Different from some other widget toolkits, kiss\_sdl has not been made to restrict your freedom, nor dictate how you should do things or what you can do. Just start by learning the basics about how it works; this toolkit is simple and thus not difficult to learn, but without a fundamental understanding of its components, you may become frustrated when what you try doesn't work. If you've read this much of the README, you're well on your way!

kiss\_sdl is fully capable of using UTF-8 (8-bit Unicode); rendered text, keyboard input, and the C source code, all use UTF-8. The kiss\_sdl manual, which includes the full reference, as well as the Hello World example, and example 2, can be found in all kiss\_sdl downloads, and at the following link: [https://raw.githubusercontent.com/actsl/kiss_sdl/master/kiss_manual.pdf](https://raw.githubusercontent.com/actsl/kiss_sdl/master/kiss_manual.pdf).


INSTALL
=======

To maintain simplicity, no library has been created, but both a static and dynamic library can be made. The path to kiss\_sdl resources (images and fonts) can be defined by adding the macro definition, `-DRESDIR=\`, followed by a path, to the compiler's command line. The simplest way to use kiss\_sdl is to copy all of the files to your project's directory and compile them with your code.

The common way to do this in Windows is to copy all the dll-s from the proper bin directories (or library directories when using Visual Studio) of SDL, SDL\_image, and SDL\_ttf, to your project's directory. This is how SDL programs are usually distributed. It is also possible to copy them to the Windows system directory, but then they may have to be replaced when working with different versions of SDL2.

The kiss\_makefile compiles both examples, which show all of the default widgets included in this toolkit. Note that the kiss\_makefile was made for Linux; to use it in Windows or macOS, comment the lines `LDFLAGS`, `CFLAGS`, `EXE1` and `EXE2` under the Linux heading, and uncomment the corresponding lines under 32 bit Windows, 64 bit Windows, or macOS. Next, just change the paths to the directories under which you installed the SDL2 development libraries. By default, it is assumed that the libraries are installed under `C:\\` or `/Library/Frameworks/`, for Windows and macOS respectively. No change may be necessary, or the change may be simple, like only changing the version of the library from 2.0.4 to 2.0.6. The Xcode command line tools must be installed standalone to compile the makefile in macOS.

In Windows, this toolkit was tested with MinGW-w64, installed from the file i686-5.3.0-release-posix-dwarf-rt_v4-rev0.7z (extracted to `C:\\` with `C:\\mingw32\\bin` added to the PATH), which is a 32 bit version ofMinGW-w64 . Other versions of MinGW should also work, but they are not tested. A 32 bit compiler also works with 64 bit Windows, but a 64 bit compiler cannot be used with 32 bit Windows.

kiss\_sdl has also been tested in Visual Studio Community 2015. The project files for Visual Studio are provided, but a similar changes to the library paths should be made if necessary, as described above, in project -> properties -> configuration properties -> vc++ directories. The paths for 64 bit SDL2 libraries for x64 are entered, but has not been tested with x64.

For compiling in Visual Studio, run `kiss_example1.vcxproj` or `kiss_example2.vcxproj`. Make sure that on the toolbar, x86 is selected (instead of x64), then build. Use one of the following commands to compile either in Linux and macOS, or in Windows using MinGW.

`make -f kiss_makefile`

`mingw32-make -f kiss_makefile`

The kiss\_sdl project page is [https://github.com/actsl/kiss_sdl](https://github.com/actsl/kiss_sdl).


CREDITS
=======

Thanks to all users of kiss\_sdl! Start from simple, then hack the hell out of it. These are the people who found bugs, recommended changes, or otherwise helped to develop kiss\_sdl. Many thanks!

Michael Bethke ([https://github.com/AVividLight](https://github.com/AVividLight)), shuttersparks.


VERSION
=======

1.0.6


LICENSE
=======

kiss\_sdl is licensed under the [zlib license](http://www.zlib.net/zlib_license.html).
