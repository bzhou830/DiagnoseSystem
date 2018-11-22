// -*- C++ -*-
//  Copyright (C) 2010-2012, Vaclav Zeman. All rights reserved.
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

#ifndef DCMTK_LOG4CPLUS_HELPERS_SNPRINTF_H
#define DCMTK_LOG4CPLUS_HELPERS_SNPRINTF_H

#include "dcmtk/oflog/config.h"

#if defined (DCMTK_LOG4CPLUS_HAVE_PRAGMA_ONCE)
#pragma once
#endif

#include "dcmtk/oflog/tchar.h"
#include <cstdarg>
#include "dcmtk/ofstd/ofvector.h"


namespace dcmtk {
namespace log4cplus { namespace helpers {


class DCMTK_LOG4CPLUS_EXPORT snprintf_buf
{
public:
    snprintf_buf ();

    tchar const * print (tchar const * fmt, ...)
        DCMTK_LOG4CPLUS_FORMAT_ATTRIBUTE (__printf__, 2, 3);
    tchar const * print_va_list (tchar const * fmt, va_list)
        DCMTK_LOG4CPLUS_FORMAT_ATTRIBUTE (__printf__, 2, 0);

private:
    OFVector<tchar> buf;
};
  

} } // namespace log4cplus { namespace helpers
} // end namespace dcmtk



#endif // DCMTK_LOG4CPLUS_HELPERS_SNPRINTF_H
