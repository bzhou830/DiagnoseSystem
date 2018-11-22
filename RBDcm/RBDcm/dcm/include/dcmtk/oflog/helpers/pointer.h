// -*- C++ -*-
// Module:  Log4CPLUS
// File:    pointer.h
// Created: 6/2001
// Author:  Tad E. Smith
//
//
// Copyright 2001-2010 Tad E. Smith
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

//
// Note: Some of this code uses ideas from "More Effective C++" by Scott
// Myers, Addison Wesley Longmain, Inc., (c) 1996, Chapter 29, pp. 183-213
//

/** @file */

#ifndef DCMTK_LOG4CPLUS_HELPERS_POINTERS_HEADER_
#define DCMTK_LOG4CPLUS_HELPERS_POINTERS_HEADER_

#include "dcmtk/oflog/config.h"

#if defined (DCMTK_LOG4CPLUS_HAVE_PRAGMA_ONCE)
#pragma once
#endif

#include "dcmtk/oflog/thread/syncprim.h"
#include <algorithm>
#include <cassert>
#if ! defined (DCMTK_LOG4CPLUS_SINGLE_THREADED) \
    && defined (DCMTK_LOG4CPLUS_HAVE_CXX11_ATOMICS)
#include <atomic>
#endif


namespace dcmtk {
namespace log4cplus {
    namespace helpers {

        /******************************************************************************
         *                       Class SharedObject (from pp. 204-205)                *
         ******************************************************************************/

        class DCMTK_LOG4CPLUS_EXPORT SharedObject
        {
        public:
            void addReference() const;
            void removeReference() const;

        protected:
          // Ctor
            SharedObject()
                : access_mutex()
                , count(0)
            { }

            SharedObject(const SharedObject&)
                : access_mutex()
                , count(0)
            { }

          // Dtor
            virtual ~SharedObject();

          // Operators
            SharedObject& operator=(const SharedObject&) { return *this; }

        public:
            thread::Mutex access_mutex;

        private:
#if defined (DCMTK_LOG4CPLUS_SINGLE_THREADED)
            typedef unsigned count_type;
#elif defined (DCMTK_LOG4CPLUS_HAVE_CXX11_ATOMICS)
            typedef STD_NAMESPACE atomic<unsigned> count_type;
#elif defined (_WIN32) || defined (__CYGWIN__)
            typedef long count_type;
#else
            typedef unsigned count_type;
#endif
            mutable count_type count;
        };


        /******************************************************************************
         *           Template Class SharedObjectPtr (from pp. 203, 206)               *
         ******************************************************************************/
        template<class T>
        class SharedObjectPtr
        {
        public:
            // Ctor
            explicit
            SharedObjectPtr(T* realPtr = 0)
                : pointee(realPtr)
            {
                addref ();
            }

            SharedObjectPtr(const SharedObjectPtr& rhs)
                : pointee(rhs.pointee)
            {
                addref ();
            }

#if defined (DCMTK_LOG4CPLUS_HAVE_RVALUE_REFS)
            SharedObjectPtr(SharedObjectPtr && rhs)
                : pointee (STD_NAMESPACE move (rhs.pointee))
            {
                rhs.pointee = 0;
            }

            SharedObjectPtr & operator = (SharedObjectPtr && rhs)
            {
                rhs.swap (*this);
                return *this;
            }
#endif

            // Dtor
            ~SharedObjectPtr()
            {
                if (pointee)
                    pointee->removeReference();
            }

            // Operators
            bool operator==(const SharedObjectPtr& rhs) const { return (pointee == rhs.pointee); }
            bool operator!=(const SharedObjectPtr& rhs) const { return (pointee != rhs.pointee); }
            bool operator==(const T* rhs) const { return (pointee == rhs); }
            bool operator!=(const T* rhs) const { return (pointee != rhs); }
            T* operator->() const {assert (pointee); return pointee; }
            T& operator*() const {assert (pointee); return *pointee; }

            SharedObjectPtr& operator=(const SharedObjectPtr& rhs)
            {
                return this->operator = (rhs.pointee);
            }

            SharedObjectPtr& operator=(T* rhs)
            {
                SharedObjectPtr<T> (rhs).swap (*this);
                return *this;
            }

          // Methods
            T* get() const { return pointee; }

            void swap (SharedObjectPtr & other) throw ()
            {
                T* tmp = pointee;
                pointee = other.pointee;
                other.pointee = tmp;
            }

            typedef T * (SharedObjectPtr:: * unspec_bool_type) () const;
            operator unspec_bool_type () const
            {
                return pointee ? &SharedObjectPtr::get : 0;
            }

            bool operator ! () const
            {
                return ! pointee;
            }

        private:
          // Methods
            void addref() const
            {
                if (pointee)
                    pointee->addReference();
            }

          // Data
            T* pointee;
        };

    } // end namespace helpers
} // end namespace log4cplus
} // end namespace dcmtk


#endif // DCMTK_LOG4CPLUS_HELPERS_POINTERS_HEADER_
