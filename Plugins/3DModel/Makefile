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
CPPFLAGS = -fPIC -Wall -pedantic -I../../
MAJOR_VER = 1
MINOR_VER = 0
LDFLAGS = -shared -soname $(SONAME).$(MAJOR_VER) -o $(SONAME).$(MAJOR_VER).$(MINOR_VER)
LIBDIR = /usr/local/lib
LDCONFIG = ldconfig
LN = ln

OBJS = \
DummyModel.o \
DummyModelFactory.o

objs:	$(OBJS)
	g++ -Wl,-E -shared -o dummy.so *.o 
	cd SpindizzyGERALDBallModel; make
	cd SpindizzyGERALDGyroscopeModel; make
	cd SpindizzyGERALDPyramidModel; make
	cd SpindizzyJewelModel; make

all:
	make clean
	make objs

clean:
	$(RM) *.o dummy.so
	cd SpindizzyGERALDBallModel; make clean
	cd SpindizzyGERALDGyroscopeModel; make clean
	cd SpindizzyGERALDPyramidModel; make clean
	cd SpindizzyJewelModel; make clean
