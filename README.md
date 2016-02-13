NAME
====

kiss_sdl - Simple universal GUI widget toolkit for SDL


DESCRIPTION
===========

Kiss is a known saying that means "Keep it simple". Different from
other widget toolkits where you are supposed to choose from the existing
widgets the one that fits you the best, in this toolkit, when there is
no widget that satisfies all your needs, modify an existing widget, or
write your own, it is simple to do that. The toolkit was made as simple
as ever possible, to finally solve the problem of an immense complexity
of most of the existing toolkits, which makes it impossible for many
people to even use them. At that it was made universal, many simpler
toolkits don't enable you to do many things that you want, due to their
restricted internal design.

This toolkit uses an original innovative approach for a widget toolkit,
this new approach enabled to make the widget toolkit very simple, and
simpler than widget toolkits previously written. The toolkit is written
in C, but can also be used in C++.

Every widget has three functions written for it, a function to create a
new widget, a function to process the events, and a function to draw. The
standard functions implemented for widgets, do all the automagical
things, and the user can write one's own function, using a standard
function inside it, to do any additional things that the user may want
to do. See the example to understand how it can be done.

Widgets can be made visible and invisible, an invisible widget is
also inactive and doesn't perform any functions. In the example, all
widgets are created first, and then only these that should be active
and present on the screen, are made visible. As it is said, it is also
possible to create and free widgets dynamically. Making a user interface
in that way may sound strange at first, but to think, the functions
for all widgets have to be in the code anyway, so creating all widgets
at once does not significantly increase the use of memory, especially
for smaller user interfaces. For bigger ones it is possible to create
widgets dynamically. But making it only by switching the visibility,
enables a simpler implementation of a user interface.

The event processing functions are called after each other in the event
processing loop. The scrollbar and progressbar event functions are also
called after the event processing loop with the event argument NULL. The
standard function returns true when an event happened in that widget
which the user may want to additionally process. The event functions
have an additional argument, a pointer to an integer that is assigned a
nonzero value when the widgets have to be redrawn. The drawing functions
are called after each other in every cycle, after the event processing
loop, when there is a need to redraw the widgets. The user may write
ones own drawing functions and call the standard functions inside them,
to do an additional drawing. The order of the drawing functions in the
loop is important, the next functions draw over the drawing done by the
previous functions.

Seven widgets are implemented as standard widgets, these are a window,
a multiline label, a button, a scrollbar, a progress bar, an entry box,
and a textbox. The example shows how to use these widgets together,
by implementing a working file selection dialog with scrollable
textboxes. Likely most of the user interfaces can be made by using only
these widgets, but the toolkit doesn't prevent adding any additional
functionality.

Is it possible to make composite widgets from several other widgets? The
widgets have coordinates relative to the main window, but it is possible
and not so difficult, to add the coordinates of a local window to a
certain group of widgets. It is possible to create and free such group
of widgets in one function, or process its events or draw it in one
function. The draw function of every widget also has a third argument which,
if provided, makes the widget to become visible when the window widget
pointed by that argument becomes visible, and not visible when that window
becomes not visible. That window is thus like a dialog window on which
all the widgets in the dialog are drawn, the example shows how to do that.

For the entry widget, the editing provided is that of the early
versions of Unix, backspace deletes the last character and ctrl-u
deletes the entire text. Clicking inside the entry widget causes it
to become active, and its text can then be edited. Clicking outside
the entry widget or pressing Enter when the widget is active, causes
the entry widget to become inactive, and causes its event function to
return true. Which enables to run a code to use the changed value. An
additional functionality can be added, such as clipboard, but what it
may be depends on the particular needs.

Is it possible to dynamically create and free the widgets? Yes you can
do that, but because of simplicity this has not been done in the example
of this toolkit.

Is it possible to do the event processing in a separate function? Yes
it is, but again because of simplicity this has not been done in the
example of this toolkit. The general variable size array implemented
in this toolkit, enables to add the widget structures to it, and pass
them together to an external function. The variable size array also
has an id number for every element, this enables to use unique numbers
for every added widget, and by these numbers it is possible to identify
these widgets in another function. Passing objects like that also makes
a re-entrant or thread-safe code possible.

Can the images be compiled into the code, instead of being in separate
files? Yes they can, images can be compiled into the C code, also Gimp
enables to save them directly in that way. Then with a special function
they can be retrieved from the data in the code. But because of simplicity
this has not been done in the example of this toolkit.

Can i use other colors, textures, etc? Yes. To use certain other fonts
or textures, you may have to rewrite the init function, or write your
own widgets. You can also draw anything you like on the widgets in the
drawing function, this will be automatically redrawn every time after
the window is exposed.

Can i do this or that other thing? You can do whatever you want,
different from some other widget toolkits, this toolkit has not been made
to restrict your freedom, or dictating to you how you should do things
or what you can do. Just learn the basic things about how it works, the
toolkit is very simple and thus not difficult to learn, but it would be
difficult to do what you want otherwise.


INSTALL
=======

To maintain simplicity, no library has been made. To use it, copy
all files to your project's directory and compile them with your code. The
kiss_makefile compiles the example that implements a file selection
diolog, and shows all the widgets in the toolkit. The makefile was
made for Linux, to use it in Windows, edit the makefile, comment the
LDFLAGS, CFLAGS and EXE, and uncomment the LDFLAGS, CFLAGS and EXE for
Windows. Then change the paths to the directories under the folders where
you installed your SDL development libraries. The change may be simple,
like only changing the version of the library from 2.0.4 to 2.0.6.

The common way in Windows is to copy all dll-s from the proper bin directories of
SDL, SDL_image and SDL_ttf, to your project's directory, this is how SDL
programs are usually distributed. Or copy them to the Windows system
directory, but then they may have to be replaced when using different
versions of SDL.

In Windows, this toolkit was tested with MinGW64, MinGW32 should also
work. The toolkit should also work with Visual C, though it was not tested
with it. The example doesn't work with Visual C, because Visual C is not
capable of POSIX. In Visual C one should use _getcwd and _chdir from direct.h,
_stat32 or _stat64 instead of stat, and the dirent library  
https://github.com/tronkko/dirent . Use one of the following commands to
compile either in Linux or in Windows using MinGW.

make -f kiss_makefile

mingw32-make -f kiss_makefile


SCREENSHOT
==========

https://raw.githubusercontent.com/actsl/kiss_sdl/master/kiss_ss1.jpg


VERSION
=======

0.6.0


LICENSE
=======

zlib license

