// -*- C++ -*-
// Module:  Log4cplus
// File:    asyncap.h
// Created: 1/2009
// Author:  Vaclav Haisman  
//
//
//  Copyright (C) 2009-2010, Vaclav Haisman. All rights reserved.
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
   
 
//

/** @file */

#ifndef DCMTK_LOG4CPLUS_ASYNCAPPENDER_H
#define DCMTK_LOG4CPLUS_ASYNCAPPENDER_H

#include "dcmtk/oflog/config.h"

#if defined (DCMTK_LOG4CPLUS_HAVE_PRAGMA_ONCE)
#pragma once
#endif

#ifndef DCMTK_LOG4CPLUS_SINGLE_THREADED

#include "dcmtk/oflog/helpers/queue.h"
#include "dcmtk/oflog/appender.h"
#include "dcmtk/oflog/thread/threads.h"
#include "dcmtk/oflog/helpers/apndimpl.h"


namespace dcmtk
{
namespace log4cplus
{


class DCMTK_LOG4CPLUS_EXPORT AsyncAppender
    : public Appender
    , public helpers::AppenderAttachableImpl
{
public:
    AsyncAppender (SharedAppenderPtr const & app, unsigned max_len);
    AsyncAppender (helpers::Properties const &);
    virtual ~AsyncAppender ();

    virtual void close ();

protected:
    virtual void append (spi::InternalLoggingEvent const &);

    void init_queue_thread (unsigned);

    thread::AbstractThreadPtr queue_thread;
    thread::QueuePtr queue;

private:
    AsyncAppender (AsyncAppender const &);
    AsyncAppender & operator = (AsyncAppender const &);
};


typedef helpers::SharedObjectPtr<AsyncAppender> AsyncAppenderPtr;


} // namespace log4cplus
} // end namespace dcmtk


#endif // DCMTK_LOG4CPLUS_SINGLE_THREADED

#endif // DCMTK_LOG4CPLUS_ASYNCAPPENDER_H
