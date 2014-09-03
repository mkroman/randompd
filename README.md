randompd
========

**randompd** is a command-line utillity for quickly adding a random track to
your mpd playlist. 

Music players such as ncmpcpp always forced you to create a new playlist
with your entire library, and then playing a random track. This can be a very
exhaustive method depending on the size of the library.

Installation
------------

To build and install a dynamic binary just run `make` and `make install`.

```
% make
% sudo make install
```

To build a static binary, make sure you have the libmpdclient static library
before building.

```
% LDFLAGS=-static make
% sudo make install
```
