NAME
====

kiss_sdl - Simple universal GUI widget toolkit for SDL2


SCREENSHOTS
===========

![](https://raw.githubusercontent.com/actsl/kiss_sdl/master/kiss_ss1.jpg)

![](https://raw.githubusercontent.com/actsl/kiss_sdl/master/kiss_ss2.jpg)


DESCRIPTION
===========

Kiss is a known saying that means "Keep it simple". Different from other
widget toolkits where you are supposed to choose from the existing
widgets the one that fits you the best, in this toolkit, when there
is no widget that satisfies all your needs, modify an existing widget,
or write your own. Most other toolkits enable to write your own widgets,
but it is not simple to do that, and they are not intended for that. The
toolkit was made as simple as possible, to solve the problem of an immense
complexity of most of the existing toolkits, which makes it impossible
for many people to even use them. At that it was made universal, many
simpler toolkits don't enable you to do many things that you want, due
to their restricted internal design. This toolkit implements an original
innovative approach for a widget toolkit, this new approach enabled
to make the widget toolkit simpler than widget toolkits previously
written. The toolkit is written in C, but it can also be used in C++.

The widget toolkit is made as modifiable as possible, so that it will
not stand on the way of doing what the user may want to do. The code of
this widget toolkit is split into four files. The file kiss_widgets.c
contains only the functions of the widgets, and none of the SDL functions
or external library functions are directly used in that file. The three
other files serve as abstraction layers, and rewriting the functions
in some of these files, enables to redefine how the operations that
use the external library functions, are implemented. These three files
are kiss_draw.c , for general drawing functions, kiss_general.c , for
general functions such as an implementation of the variable size array,
and kiss_posix.c , for some of the most necessary POSIX functions.

The global variables are not declared as constant, to make the code more
easily modifiable, but they should be used as constants, by changing
them only during the initialization. In case of an error, the functions of
this toolkit return -1 or NULL, depending on the return type, and the
event processing funtions return 0.

Every widget has three functions written for it, a function to create a
new widget, a function to process the events, and a function to draw. The
standard functions implemented for widgets, do all the automagical
things, and the user can write one's own function, using a standard
function inside it, to do any additional things that the user may want
to do. See the examples and the header file kiss_sdl.h .

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
standard function. The standard function draws a blue rectangle around
the edge of the widget.

In addition to visibility, the widgets also may or may not have
focus. Focus determines whether the widget processes mouse events. The
focus of the widget is determined by the focus of the window wdw. If this
window is provided and has no focus, the focus of a widget is determined
by the focus member of the structure of that widget.

The event processing functions are called after each other in the event
processing loop. The scrollbar, progressbar and combo box event functions
are also called after the event processing loop with the event argument
NULL. The standard function returns nonzero when an event happened in
that widget which the user may want to additionally process. The event
functions have an additional argument, a pointer to an integer that is
assigned a nonzero value when the widgets have to be redrawn.

The drawing functions are called after each other in every cycle, after
the event processing loop, when there is a need to redraw the widgets. The
user may write ones own drawing functions and call the standard functions
inside them, to do an additional drawing. The order of the drawing
functions in the loop is important, the next functions draw over the
drawing done by the previous functions. Especially when combo boxes are
used, as their popup textboxes draw over the widgets below the combo box.

Ten widgets are implemented as standard widgets, these are a window,
multiline label, button, select button, vertical scrollbar, horizontal
scrollbar, progress bar, entry box, textbox, and combo box. The two
examples show how to use these widgets together. The first example
implements a working file selection dialog with scrollable text boxes,
the second example shows a use of a combo box. Likely most of the user
interfaces can be made by using only these widgets, but the toolkit
doesn't prevent adding any additional functionality.

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
is, but because of simplicity this has not been done in the examples of
this toolkit. The general variable size array implemented in this toolkit,
enables to add the widget structures to it, and pass them together to
an external function. The variable size array also has an id number for
every element, this enables to use unique numbers for every added widget,
and by these numbers it is possible to identify these widgets in another
function. Passing objects like that also makes a re-entrant or thread-safe
code possible, also in this widget toolkit there is nothing central.

Can the images be compiled into the code, instead of being in separate
files? Yes they can, images can be compiled into the C code, also Gimp
enables to save them directly in that way. Then with a special function
they can be retrieved from the data in the code. But again because of
simplicity, this has not been done in the examples of this toolkit.

Can i use other colors, textures, etc? Yes. To use certain other fonts or
textures, you may have to rewrite the init function. You can also draw
anything you like on the widgets in the drawing function, this will be
automatically redrawn every time after the window is exposed.

Can i do this or that other thing? You can do whatever you want, different
from some other widget toolkits, this toolkit has not been made to
restrict your freedom, or dictating to you how you should do things
or what you can do. Just learn the basic things about how it works,
the toolkit is simple and thus not difficult to learn, but it would be
difficult to do what you want otherwise.

The kiss_sdl manual, including the full reference and example 2 tutorial,
is at the following link.

https://raw.githubusercontent.com/actsl/kiss_sdl/master/kiss_manual.pdf


INSTALL
=======

To maintain simplicity, no library has been made. To use it, copy all
files to your project's directory and compile them with your code. The
 kiss_makefile compiles the examples that show all the widgets in
the toolkit. The makefile was made for Linux, to use it in Windows,
edit the makefile, comment the LDFLAGS, CFLAGS, EXE1 and EXE2, and
uncomment the LDFLAGS, CFLAGS, EXE1 and EXE2 for Windows. Then change
the paths to the directories under the folders where you installed your
SDL development libraries. By default it is assumed that the libraries
are installed under C:\\. The change may be simple, like only changing
the version of the library from 2.0.4 to 2.0.6.

The common way in Windows is to copy all the dll-s from the proper bin
directories (or library directories when using Visual Studio) of SDL,
SDL_image and SDL_ttf, to your project's directory, this is how SDL
programs are usually distributed. Or copy them to the Windows system
directory, but then they may have to be replaced when using different
versions of SDL.

In Windows, this toolkit was tested with MinGW64, MinGW32 should also
work, and in the Visual Studio Community 2015. The project files for the
Visual Studio are provided, but a similar changes to the library paths
should be made, as described above, in the project -> properties ->
configuration properties -> vc++ directories. For compiling in Visual
Studio, run kiss_example1.vcxproj or kiss_example2.vcxproj, make sure
that on toolbar x64 or x86 is selected, it was tested with x86, then
build. Use one of the following commands to compile either in Linux,
or in Windows using MinGW.

make -f kiss_makefile

mingw32-make -f kiss_makefile


VERSION
=======

0.8.12


LICENSE
=======

zlib license

