# EggHunt

Project Requirements:

The Great Spring 2019 CS473 Easter Egg Hunt aka Assignment #7
-------------------------------------------------------------
Running on the effie.indstate.edu is an egg server running on your CS473 port
that holds a 1K by 1K virtual field of spaces in which are hidden 10 virtual
Easter Eggs.  The server accept internet data-gram messages in the form:

  "look x,y"

where x and y are integer numbers in the range 0 to 1023 inclusive.  Because
you are searching frantically for eggs you may only receive a message back
from the server one quarter of the time, thus it is pointless to wait for the
server to reply before moving on to the next search location.  The messages
sent to you from the egg server are in the form:

  "x,y: Found one!"
or
  "x,y: No egg."

Where x,y are the integer coordinates of a location that you searched.  There
are only 10 eggs in total, so you may stop searching when you have discovered
the location of all 10.

You must now write a program called hunt.c that will send the look messages to
the egg server and search for the 10 eggs as quickly as possible.  Your
program should probably use epoll to check for when to read/write data-grams on
the socket for maximum search speed.  To parse the received message I would
probably recommend using sscanf().

Your program should output a list of the coordinates of the 10 eggs you
discovered when it has discovered them all (it may do this as it finds them),
then exit.
