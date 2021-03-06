/*
 * Logger.h
 * 
 * This file is part of the "HLSL Translator" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __HT_LOGGER_H__
#define __HT_LOGGER_H__


#include "Export.h"

#include <string>


namespace HTLib
{


//! Logger interface.
class _HT_EXPORT_ Logger
{
    
    public:
        
        virtual ~Logger()
        {
        }

        //! Prints an information.
        virtual void Info(const std::string& message)
        {
            // dummy
        }

        //! Prints a warning.
        virtual void Warning(const std::string& message)
        {
            // dummy
        }

        //! Prints an error.
        virtual void Error(const std::string& message)
        {
            // dummy
        }

        //! Increments the indentation.
        virtual void IncIndent()
        {
            // dummy
        }

        //! Decrements the indentation.
        virtual void DecIndent()
        {
            // dummy
        }

};


} // /namespace HTLib


#endif



// ================================================================================