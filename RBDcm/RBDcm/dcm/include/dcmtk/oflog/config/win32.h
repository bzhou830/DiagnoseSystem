// -*- C++ -*-
// Module:  Log4CPLUS
// File:    config-win32.h
// Created: 4/2003
// Author:  Tad E. Smith
//
//
// Copyright 2003-2010 Tad E. Smith
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/** @file */

#ifndef DCMTK_LOG4CPLUS_CONFIG_WIN32_HEADER_
#define DCMTK_LOG4CPLUS_CONFIG_WIN32_HEADER_

#if defined (DCMTK_LOG4CPLUS_HAVE_PRAGMA_ONCE)
#pragma once
#endif

#ifdef _WIN32

/* This used to be _MSC_VER >= 1400, but MSVC 2005 is broken */
#if (defined (_MSC_VER) && _MSC_VER > 1400)
#  define DCMTK_LOG4CPLUS_HAVE_INTRIN_H
#endif

// Time related functions and headers.
#define DCMTK_LOG4CPLUS_HAVE_TIME_H
#define DCMTK_LOG4CPLUS_HAVE_SYS_TIMEB_H
#define DCMTK_LOG4CPLUS_HAVE_FTIME
#if defined (_MSC_VER) || defined (__BORLANDC__)
#define DCMTK_LOG4CPLUS_HAVE_GMTIME_S
#endif

// Use Winsock on Windows.
#define DCMTK_LOG4CPLUS_USE_WINSOCK

// Enable Win32DebugAppender
#define DCMTK_LOG4CPLUS_HAVE_OUTPUTDEBUGSTRING

// Enable Win32ConsoleAppender.
#define DCMTK_LOG4CPLUS_HAVE_WIN32_CONSOLE

#define DCMTK_LOG4CPLUS_HAVE_SYS_TYPES_H
#define DCMTK_LOG4CPLUS_HAVE_SYS_LOCKING_H
#define DCMTK_LOG4CPLUS_HAVE_FCNTL_H
#define DCMTK_LOG4CPLUS_HAVE_IO_H
#define DCMTK_LOG4CPLUS_HAVE_STDIO_H
#define DCMTK_LOG4CPLUS_HAVE_WCHAR_H
#define DCMTK_LOG4CPLUS_HAVE_STDARG_H
#define DCMTK_LOG4CPLUS_HAVE_STDLIB_H
#define DCMTK_LOG4CPLUS_HAVE_ERRNO_H
#define DCMTK_LOG4CPLUS_HAVE_SYS_STAT_H
#define DCMTK_LOG4CPLUS_HAVE_TIME_H
#define DCMTK_LOG4CPLUS_HAVE_STDLIB_H

// MSVC has both and so does MinGW.
#define DCMTK_LOG4CPLUS_HAVE_VSNPRINTF
#define DCMTK_LOG4CPLUS_HAVE__VSNPRINTF
#if _MSC_VER <= 1200 /* Additional settings for VC6 and older */
#undef DCMTK_LOG4CPLUS_HAVE_VSNPRINTF
#endif

// MS secure versions of vprintf().
#ifdef HAVE_VSPRINTF_S
#  define DCMTK_LOG4CPLUS_HAVE_VSPRINTF_S
#  define DCMTK_LOG4CPLUS_HAVE_VSWPRINTF_S
#endif

// MS secure versions of vfprintf().
#ifdef HAVE_VFPRINTF_S
#  define DCMTK_LOG4CPLUS_HAVE_VFPRINTF_S
#  define DCMTK_LOG4CPLUS_HAVE_VFWPRINTF_S
#endif

// MS secure versions of vsnprintf().
#ifdef HAVE_VSNPRINTF_S
#  define DCMTK_LOG4CPLUS_HAVE_VSNPRINTF_S
#  define DCMTK_LOG4CPLUS_HAVE__VSNPRINTF_S
#  define DCMTK_LOG4CPLUS_HAVE__VSNWPRINTF_S
#endif

#define DCMTK_LOG4CPLUS_DLLMAIN_HINSTANCE HINSTANCE
#define DCMTK_LOG4CPLUS_HAVE_NT_EVENT_LOG

// log4cplus_EXPORTS is used by the CMake build system.  DLL_EXPORT is
// used by the autotools build system.
#if (defined (log4cplus_EXPORTS) || defined (log4cplusU_EXPORTS) \
     || defined (DLL_EXPORT))                                    \
    && ! defined (DCMTK_LOG4CPLUS_STATIC)
#  undef DCMTK_LOG4CPLUS_BUILD_DLL
#  define DCMTK_LOG4CPLUS_BUILD_DLL
#endif

#if ! defined (DCMTK_LOG4CPLUS_BUILD_DLL)
#  undef DCMTK_LOG4CPLUS_STATIC
#  define DCMTK_LOG4CPLUS_STATIC
#endif

#if defined (DCMTK_LOG4CPLUS_STATIC) && defined (DCMTK_LOG4CPLUS_BUILD_DLL)
#  error DCMTK_LOG4CPLUS_STATIC and DCMTK_LOG4CPLUS_BUILD_DLL cannot be defined both.
#endif

#if defined (DCMTK_LOG4CPLUS_BUILD_DLL)
#  if defined (DCMTK_INSIDE_LOG4CPLUS)
#    define DCMTK_LOG4CPLUS_EXPORT __declspec(dllexport)
#  else
#    define DCMTK_LOG4CPLUS_EXPORT __declspec(dllimport)
#  endif
#else
#  define DCMTK_LOG4CPLUS_EXPORT
#endif

#ifndef DCMTK_LOG4CPLUS_SINGLE_THREADED
#  define DCMTK_LOG4CPLUS_USE_WIN32_THREADS
#endif

#if defined (_WIN32_WINNT) && _WIN32_WINNT < 0x0600
#  define DCMTK_LOG4CPLUS_POOR_MANS_SHAREDMUTEX
#endif

#if defined(_MSC_VER)
  // Warning about: identifier was truncated to '255' characters in the debug information
#  pragma warning( disable : 4786 )
  // Warning about: <type1> needs to have dll-interface to be used by clients of class <type2>
#  pragma warning( disable : 4251 )

#  define DCMTK_LOG4CPLUS_INLINES_ARE_EXPORTED

#  if _MSC_VER >= 1400
#    define DCMTK_LOG4CPLUS_WORKING_LOCALE
#    define DCMTK_LOG4CPLUS_HAVE_FUNCTION_MACRO
#    define DCMTK_LOG4CPLUS_HAVE_FUNCSIG_MACRO
#    define DCMTK_LOG4CPLUS_HAVE_C99_VARIADIC_MACROS
#  endif
#endif

#if defined (__GNUC__)
#  undef DCMTK_LOG4CPLUS_INLINES_ARE_EXPORTED
#  if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#    define DCMTK_LOG4CPLUS_INLINES_ARE_EXPORTED
#    define DCMTK_LOG4CPLUS_HAVE___SYNC_SUB_AND_FETCH
#    define DCMTK_LOG4CPLUS_HAVE___SYNC_ADD_AND_FETCH
#  endif
#  define DCMTK_LOG4CPLUS_HAVE_FUNCTION_MACRO
#  define DCMTK_LOG4CPLUS_HAVE_GNU_VARIADIC_MACROS
#  define DCMTK_LOG4CPLUS_HAVE_C99_VARIADIC_MACROS
#  if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#    define DCMTK_LOG4CPLUS_HAVE_PRETTY_FUNCTION_MACRO
#  endif
#  if defined (__MINGW32__)
#    define DCMTK_LOG4CPLUS_WORKING_C_LOCALE
#  endif
#endif


#endif // _WIN32
#endif // DCMTK_LOG4CPLUS_CONFIG_WIN32_HEADER_
