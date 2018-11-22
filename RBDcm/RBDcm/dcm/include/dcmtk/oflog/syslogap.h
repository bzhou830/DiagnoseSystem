// -*- C++ -*-
// Module:  Log4CPLUS
// File:    syslogap.h
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

/** @file */

#ifndef DCMTK_LOG4CPLUS_SYSLOG_APPENDER_HEADER_
#define DCMTK_LOG4CPLUS_SYSLOG_APPENDER_HEADER_

#include "dcmtk/oflog/config.h"

#if defined (DCMTK_LOG4CPLUS_HAVE_PRAGMA_ONCE)
#pragma once
#endif

#include "dcmtk/oflog/appender.h"
#include "dcmtk/oflog/helpers/socket.h"


namespace dcmtk
{
namespace log4cplus
{

    /**
     * Appends log events to a file.
     *
     * <h3>Properties</h3>
     * <dl>
     * <dt><tt>ident</tt></dt>
     * <dd>First argument to <code>openlog()</code>, a string that
     * will be prepended to every message.</dd>
     *
     * <dt><tt>facility</tt></dt>
     * <dd>Facility is used in combination with syslog level in first
     * argument to syslog(). It can be one of the supported facility
     * names (case insensitive), e.g. auth, cron, kern, mail, news
     * etc.</dd>
     *
     * <dt><tt>host</tt></dt>
     * <dd>Destination syslog host. When this property is specified,
     * messages are sent using UDP to destination host, otherwise
     * messages are logged to local syslog.</dd>
     *
     * <dt><tt>port</tt></dt>
     * <dd>Destination port of syslog service on host specified by the
     * <tt>host</tt> property. The default value is port 514.</dd>
     * </dl>
     *
     * \note Messages sent to remote syslog using UDP are conforming
     * to RFC5424.
     */
    class DCMTK_LOG4CPLUS_EXPORT SysLogAppender : public Appender {
    public:
      // Ctors
#if defined (DCMTK_LOG4CPLUS_HAVE_SYSLOG_H)
        SysLogAppender(const tstring& ident);
#endif
        SysLogAppender(const tstring& ident, const tstring & host,
            int port = 514, const tstring & facility = tstring ());
        SysLogAppender(const log4cplus::helpers::Properties & properties);

      // Dtor
        virtual ~SysLogAppender();

      // Methods
        virtual void close();

    protected:
        virtual int getSysLogLevel(const LogLevel& ll) const;
        virtual void append(const spi::InternalLoggingEvent& event);
#if defined (DCMTK_LOG4CPLUS_HAVE_SYSLOG_H)
        void appendLocal(const spi::InternalLoggingEvent& event);
#endif
        void appendRemote(const spi::InternalLoggingEvent& event);

      // Data
        tstring ident;
        int facility;

        typedef void (SysLogAppender:: * AppendFuncType) (
            const spi::InternalLoggingEvent&);
        AppendFuncType appendFunc;

        tstring host;
        int port;
        helpers::Socket syslogSocket;

        static tstring const remoteTimeFormat;

    private:
      // Disallow copying of instances of this class
        SysLogAppender(const SysLogAppender&);
        SysLogAppender& operator=(const SysLogAppender&);

        tstring identStr;
        tstring hostname;
    };

} // end namespace log4cplus
} // end namespace dcmtk


#endif // DCMTK_LOG4CPLUS_SYSLOG_APPENDER_HEADER_
