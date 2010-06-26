# Copyright 2009 Martin Brentnall
#
# This file is part of Iso-Realms.
#
# Iso-Realms is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Iso-Realms is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
CPPFLAGS = -fPIC -Wall -pedantic -I.
MAJOR_VER = 1
MINOR_VER = 0
LDFLAGS = -shared -soname $(SONAME).$(MAJOR_VER) -o $(SONAME).$(MAJOR_VER).$(MINOR_VER)
LIBDIR = /usr/local/lib
LDCONFIG = ldconfig
LN = ln

OBJS = \
Main.o

objs:	$(OBJS)
	cd Elements; make
	cd Engine; make
	cd FontEngine; make
	cd IsoRealms; make
	cd Plugins; make

install:
	cd IsoRealms; make install
	g++ *.o -o IsoRealms.bin `sdl-config --cflags --libs` -L./IsoRealms -lIsoRealms -lGL -lGLU -lxerces-c -lfreetype
	ln -s "$(PWD)" /usr/share/IsoRealms.bin

all:
	make clean
	make objs

clean:
	$(RM) *.o IsoRealms.bin
	cd Elements; make clean
	cd Engine; make clean
	cd FontEngine; make clean
	cd IsoRealms; make clean
	cd Plugins; make clean

