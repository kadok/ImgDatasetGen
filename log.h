#ifndef LOG_H
#define LOG_H

#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>

enum logType {
    ERROR,
    WARN,
    INFO
};

/**
 * @file
 * @author  Renato Moraes <rmoraes@inf.puc-rio.br>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The class LogImgDataset represents the log of the system using the library log4cpp.
 */
class LogImgDataset
{
    public:

         /**
         * @brief getInstance - Get the instance of LogImgDataset
         * @return instance -Return the instance of LogImgDataset
         */
        static LogImgDataset& getInstance()
        {
            static LogImgDataset instance;
            return instance;
        }

        /**
         * @brief Log - Function that write log file.
         * @param type - Log Type = ERROR = 0, WARN = 1, INFO = 2
         * @param content - Log message
         */
        void Log(int type, char *content);

    private:
        log4cpp::Appender *app;
        LogImgDataset();
        LogImgDataset(LogImgDataset const&);
        LogImgDataset& operator=(LogImgDataset const &);
        ~LogImgDataset();


};

#endif // LOG_H
