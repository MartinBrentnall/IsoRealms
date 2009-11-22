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
CPPFLAGS = -fPIC -Wall -pedantic
MAJOR_VER = 1
MINOR_VER = 0
LDFLAGS = -shared -soname $(SONAME).$(MAJOR_VER) -o $(SONAME).$(MAJOR_VER).$(MINOR_VER)
LIBDIR = /usr/local/lib
LDCONFIG = ldconfig
LN = ln

OBJS = \
AbstractSpindizzyBlock.o \
ISpindizzyBlockFactory.o \
RollableSplitSurface.o \
RollableSurface.o \
SpindizzyBlock.o \
SpindizzyBlockConfigurationComponent.o \
SpindizzyBlockFactory.o \
SpindizzyBlockProperties.o \
SpindizzyBlockSet.o \
SpindizzyIceBlock.o \
SpindizzySwitchBlock.o \
SpindizzyTrampolineBlock.o \
SpindizzyWater.o \
SpindizzyWaterFactory.o \
WallSurface.o

objs:	$(OBJS)
	g++ -Wl,-E -shared -o ElementSet.so *.o 

all:
	make clean
	make objs

clean:
	$(RM) *.o ElementSet.so
