// -*- C++ -*-
// Module:  Log4CPLUS
// File:    env.h
// Created: 7/2010
// Author:  Vaclav Haisman
//
//
//  Copyright (C) 2010, Vaclav Haisman. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without modifica-
//  tion, are permitted provided that the following conditions are met:
//
//  1. Redistributions of  source code must  retain the above copyright  notice,
//     this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED WARRANTIES,
//  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//  FITNESS  FOR A PARTICULAR  PURPOSE ARE  DISCLAIMED.  IN NO  EVENT SHALL  THE
//  APACHE SOFTWARE  FOUNDATION  OR ITS CONTRIBUTORS  BE LIABLE FOR  ANY DIRECT,
//  INDIRECT, INCIDENTAL, SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL  DAMAGES (INCLU-
//  DING, BUT NOT LIMITED TO, PROCUREMENT  OF SUBSTITUTE GOODS OR SERVICES; LOSS
//  OF USE, DATA, OR  PROFITS; OR BUSINESS  INTERRUPTION)  HOWEVER CAUSED AND ON
//  ANY  THEORY OF LIABILITY,  WHETHER  IN CONTRACT,  STRICT LIABILITY,  OR TORT
//  (INCLUDING  NEGLIGENCE OR  OTHERWISE) ARISING IN  ANY WAY OUT OF THE  USE OF
//  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef DCMTK_LOG4CPLUS_INTERNAL_ENV_H
#define DCMTK_LOG4CPLUS_INTERNAL_ENV_H

#include "dcmtk/oflog/config.h"

#if defined (DCMTK_LOG4CPLUS_HAVE_PRAGMA_ONCE)
#pragma once
#endif

#include "dcmtk/oflog/tstring.h"

#if defined (_WIN32)
#include "dcmtk/oflog/config/windowsh.h"
#endif
#ifdef DCMTK_LOG4CPLUS_HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef DCMTK_LOG4CPLUS_HAVE_UNISTD_H
#include <unistd.h>
#endif


namespace dcmtk {
namespace log4cplus { namespace internal {


bool get_env_var (tstring & value, tstring const & name);
bool parse_bool (bool & val, tstring const & str);

inline
#if defined (_WIN32)
DWORD
get_process_id ()
{
    return GetCurrentProcessId ();
}

#elif defined (DCMTK_LOG4CPLUS_HAVE_GETPID)
pid_t
get_process_id ()
{
    return getpid ();
}

#else
int
get_process_id ()
{
    return 0; 
}

#endif


} } // namespace log4cplus { namespace internal {
} // end namespace dcmtk


#endif // DCMTK_LOG4CPLUS_INTERNAL_ENV_H
