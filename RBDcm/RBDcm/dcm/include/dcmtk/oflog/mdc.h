// -*- C++ -*-
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

#ifndef DCMTK_LOG4CPLUS_MDC_H_HEADER
#define DCMTK_LOG4CPLUS_MDC_H_HEADER

#include "dcmtk/oflog/config.h"

#if defined (DCMTK_LOG4CPLUS_HAVE_PRAGMA_ONCE)
#pragma once
#endif

#include "dcmtk/oflog/tstring.h"

#include "dcmtk/ofstd/ofmap.h"


namespace dcmtk
{
namespace log4cplus
{


typedef OFMap<tstring, tstring> MappedDiagnosticContextMap;


class DCMTK_LOG4CPLUS_EXPORT MDC
{
public:
    /**
     * Clear any nested diagnostic information if any. This method is
     * useful in cases where the same thread can be potentially used
     * over and over in different unrelated contexts.
     */
    void clear();

    void put (tstring const & key, tstring const & value);
    bool get (tstring * value, tstring const & key) const;
    void remove (tstring const & key);

    MappedDiagnosticContextMap const & getContext () const;

    // Public ctor and dtor but only to be used by internal::DefaultContext.
    MDC ();
    virtual ~MDC ();

private:
    DCMTK_LOG4CPLUS_PRIVATE static MappedDiagnosticContextMap * getPtr ();
};


DCMTK_LOG4CPLUS_EXPORT MDC & getMDC ();


} // namespace log4cplus
} // end namespace dcmtk


#endif // DCMTK_LOG4CPLUS_MDC_H_HEADER
