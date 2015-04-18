/*
 * Copyright 2009 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef HACKS_H
#define HACKS_H

/**
 * Casts a void* to the FuncPtr type.  Compilation will fail if compiled for a
 * platform where data pointers and object pointers are not the same size.
 * This is used to prevent a gcc compiler warning due to a conflict between
 * ISO C and POSIX standards.
 * 
 * @param void*  The void pointer to cast to a function pointer.
 * @returns The function pointer.
 */
template<typename FuncPtr>
FuncPtr cast_voidptr_to_funcptr(void* p) {

  // Check that object pointers and function pointers are the same size
  // - Compare the two ptrs => result = 0 or 1 then *2 -1 => 1 or -1
  typedef char __attribute__((__unused__)) mSizeCheck[(sizeof(void*) == sizeof(void (*)())) * 2 - 1];
  union {
    void* mVoidPointer;
    void (*mFunctionPointer)();
  } mPointerUnion;
  mPointerUnion.mVoidPointer = p;
  return reinterpret_cast<FuncPtr>(mPointerUnion.mFunctionPointer);
}

#endif