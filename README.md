NAME
====

kiss_sdl - Simple generic GUI widget toolkit for SDL2


SCREENSHOTS
===========

![](https://raw.githubusercontent.com/actsl/kiss_sdl/master/kiss_ss1.jpg)

![](https://raw.githubusercontent.com/actsl/kiss_sdl/master/kiss_ss2.jpg)


DESCRIPTION
===========

KISS is an acronym and software development approach that means "Keep
It Small and Simple". Sometimes all the user interaction is GUI, so
GUI is very general. Certainly there are an immense number of ways
how a developer may want to do it. Having everything ready made is
thus not a good idea, except for some special cases. The solution is
to make GUI toolkit general, but small and simple, so that for basic
purposes it can be used as is, or it can be changed when one wants to
do it differently. Most other toolkits enable to write your own widgets,
but it is not simple to do that, and they are not intended for that.

This toolkit was made as simple as possible, to solve the problem of an
immense complexity of many existing toolkits, which makes it impossible
for many people to even use them. At that it was made general, many
simpler toolkits don't enable you to do many things that you want, due
to their restricted internal design. This toolkit implements an original
innovative approach for a widget toolkit that i call principal GUI,
going further from immediate GUI, this new approach enabled to make the
widget toolkit simpler. The toolkit is written in C, but it can also be
used in C++.

The GUI toolkit was made as modifiable as possible, so that it will not
stand on the way of doing what the user may want to do. The code of this
GUI toolkit is split into four files. The file kiss_widgets.c contains
only the functions of the widgets, and none of the SDL functions or
external library functions are directly called in that file. The three
other files serve as abstraction layers, and rewriting the functions
in some of these files, enables to redefine how the operations that
use the external library functions, are implemented. These three files
are kiss_draw.c , for general drawing functions, kiss_general.c , for
general functions such as an implementation of the variable size array,
and kiss_posix.c , for some of the most necessary POSIX functions.

What is necessary for using the toolkit in a code is simple and
straightforward. Every widget has three functions written for it,
a function to create a new widget, a function to process the events,
and a function to draw. The base functions implemented for the widgets,
do all the automagical things, and the user can write one's own function,
using a base function inside it, to do any additional things that the
user may want to do. See the examples and the header file kiss_sdl.h .

The few global variables there are, are not declared as constant, to make
the code more easily modifiable, but they should be used as constants,
by changing them only during the initialization. In case of an error,
the functions of this toolkit return -1 or NULL, depending on the return
type, and the event processing functions return 0.

Widgets can be made visible and invisible, an invisible widget is also
inactive and doesn't perform any functions. In the examples, all widgets
are created first, and then only these that should be active and present
on the screen, are made visible. It is also possible to create and free
widgets dynamically. Making a user interface in that way may sound strange
at first, but all the user written functions for widgets have to be there
anyway, a lot has to be there for all widgets. For bigger interfaces it
is possible to create widgets dynamically. But making it only by switching
the visibility, enables a simpler implementation of a user interface.

The functions for creating widgets also have an argument wdw which,
if provided, makes the widget to become visible when the window widget
pointed by that argument becomes visible, and not visible when that
window becomes not visible. That window is thus like a dialog window on
which all the widgets in the dialog are drawn, the examples show how to
do that. Some of these functions also have an additional argument named
decorate, this determines whether the widget should be decorated by the
default function. The default function draws a blue rectangle around
the edge of the widget.

In addition to visibility, the widgets also may or may not have
focus. Focus determines whether the widget processes mouse events. The
focus of the widget is determined by the focus of the window wdw. If this
window is provided and has no focus, the focus of a widget is determined
by the focus member of the structure of that widget.

The event processing functions are called after each other in the
event processing loop. The scrollbar, progressbar and combo box event
functions are also called after the event processing loop with the event
argument NULL. The base functions return nonzero when an event happened
in the widget that the user may want to additionally process. The event
functions have an additional argument, a pointer to an integer that is
assigned a nonzero value when the widgets have to be redrawn.

The drawing functions are called after each other in every iteration,
after the event processing loop, when there is a need to redraw the
widgets. The user may write ones own drawing functions and call the
base functions inside them, to do an additional drawing. The order of
the drawing functions in the loop is important, the next functions draw
over the drawing done by the previous functions. Especially when combo
boxes are used, as their popup text boxes draw over the widgets below
the combo box.

Ten widgets are implemented as base widgets, these are a window, multiline
label, button, select button, vertical scrollbar, horizontal scrollbar,
progress bar, entry box, text box, and combo box. The two examples
show how to use these widgets together. The first example implements
a working file selection dialog with scrollable text boxes, the second
example shows a use of a combo box. Likely most of the user interfaces
can be made by using only these widgets, but the toolkit doesn't prevent
adding any additional functionality.

Is it possible to make composite widgets from several other widgets? The
widgets have coordinates relative to the main window, but it is possible
and not so difficult, to add the coordinates of a local window to a
certain group of widgets. It is possible to create and free such group
of widgets in one function, or process its events or draw it in one
function. Combo box is an example of a composite widget.

For the entry widget, the editing provided is that of the early versions
of Unix, backspace deletes the last character and ctrl-u deletes the
entire text. Clicking inside the entry widget causes it to become active,
and its text can then be edited. Pressing Enter when the widget is active,
causes the entry widget to become inactive, and causes its event function
to return true. Which enables to run a code to use the changed value. An
additional functionality can be added, such as clipboard, but what it
may be depends on the particular needs.

Is it possible to do the event processing in a separate function? Yes it
is, by using separate event loops for different widgets, or by passing
objects using a variable size array, but because of simplicity this
has not been done in the examples of this toolkit. This also makes a
re-entrant or thread-safe code possible, and also different widgets
can then be drawn in different threads. In such widget toolkit there is
essentially nothing central, except in that case a table may be necessary,
that shows for every thread whether it has received an expose event and
whether it has processed it.

Can the images be compiled into the code, instead of being in separate
files? Yes they can, images can be compiled into the C code, also Gimp
enables to save them directly in that way. Then with a special function
they can be retrieved from the data in the code. But again because of
simplicity, this has not been done in the examples of this toolkit.

Can i use other colors, images, etc? Yes. Different widgets can have
different fonts and images, that can be loaded after the init function
and assigned to the widget structure before creating the widget. To
use certain other types of fonts or images, you may have to rewrite the
init function. You can also draw anything you like on the widgets in the
drawing function, this will be automatically redrawn every time after
the window is exposed.

Can i do this or that other thing? You can do whatever you want, different
from some other widget toolkits, this toolkit has not been made to
restrict your freedom, or dictating to you how you should do things
or what you can do. Just learn the basic things about how it works,
the toolkit is simple and thus not difficult to learn, but it would be
difficult to do what you want otherwise.

kiss_sdl is fully capable of UTF-8 (8-bit Unicode). The rendered text,
the keyboard input and also the C source code, all is in UTF-8.

The kiss_sdl manual, including the full reference, Hello World example
and example 2 tutorial, is at the following link.

https://raw.githubusercontent.com/actsl/kiss_sdl/master/kiss_manual.pdf


INSTALL
=======

To maintain simplicity, no library has been made, but either a static
or a dynamic library can be made, and the path to the resources
(images and fonts) can be defined by adding a macro definition like
-DRESDIR=\\"kiss/\\" to the compiler's command line. The simplest way
to use it, is to copy all files to your project's directory and compile
them with your code.

The common way in Windows is to copy all the dll-s from the proper bin
directories (or library directories when using Visual Studio) of SDL,
SDL_image and SDL_ttf, to your project's directory (copy the duplicate
files from SDL_image, not from SDL_ttf), this is how SDL programs are
usually distributed. Or to add all these bin directories to Path (in
user variables). Or copy them to the Windows system directory, but then
they may have to be replaced when using different versions of SDL2.

The kiss_makefile compiles the examples that show all the widgets in the
toolkit. The kiss_makefile was made for Linux, to use it in Windows, edit
it, comment the lines LDFLAGS, CFLAGS, BIN1 and BIN2, and uncomment the
corresponding lines for 32 bit Windows, 64 bit Windows or macOS. The Xcode
command line tools also have to be installed standalone in macOS. Then
change the paths to the directories under which you installed the SDL2
development libraries. By default, it is assumed that the libraries are
installed under C:\\. No change may be necessary, or the change may be
simple, like only changing the version of the library from 2.0.8 to 2.0.9.

In Windows, this toolkit was tested with MinGW-w64, installed with the
installer downloaded from https://sourceforge.net/projects/mingw-w64 for
i686 (32 bit) to C:\\mingw-w64 , with C:\\mingw-w64\\mingw32\\bin added
to the Path. Other versions of MinGW should also work, but they are not
tested. A 32 bit compiler also works with 64 bit Windows, but a 64 bit
compiler cannot be used with 32 bit Windows.

It was also tested in the Visual Studio Community 2015 and SDL2 version
2.0.7. The project files for the Visual Studio are provided, but a similar
changes to the library paths should be made if necessary, as described
above, in the project -> properties -> configuration properties -> vc++
directories. The paths of the 64 bit SDL2 libraries for x64 are entered,
but it was not tested with x64.

For compiling in Visual Studio, run kiss_example1.vcxproj or
kiss_example2.vcxproj, make sure that on the toolbar x86 is selected
(instead of x64), it was tested with x86, then build. Use one of the
following commands to compile either in Linux or macOS, or in Windows
using MinGW.

make -f kiss_makefile

mingw32-make -f kiss_makefile

The kiss_sdl project page is https://github.com/actsl/kiss_sdl .


CREDITS
=======

Thanks to all users of kiss_sdl, start from simple, then hack the hell
out of it. These are the people who found bugs, recommended changes,
or helped otherwise to develop kiss_sdl, many thanks.

Michael Bethke (https://github.com/AVividLight), solarHunter (https://github.com/solarHunter).


VERSION
=======

1.2.12


LICENSE
=======

zlib license

