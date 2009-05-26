/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* The Zenipex Library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU General Public License as 
* published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
*
* The Zenipex Library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with the Zenipex Library; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
* 02110-1301 USA.
*
* As a special exception, you may use this file as part of a free software
* library without restriction.  Specifically, if other files instantiate
* templates or use macros or inline functions from this file, or you compile
* this file and link it with other files to produce an executable, this
* file does not by itself cause the resulting executable to be covered by
* the GNU General Public License.  This exception does not however
* invalidate any other reasons why the executable file might be covered by
* the GNU General Public License.
*/

/**
 * \class Zeni::XML_Element
 *
 * \ingroup Zenilib
 *
 * \brief A node in an XML tree, possibly containing useful data.
 *
 * \note There is no functionality (currently) to cleanly exit an iteration through XML_Elements.  The only way out is to catch the Bad_XML_Access when thrown.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::XML_Reader
 *
 * \ingroup Zenilib
 *
 * \brief A class to open an XML file and manage the root node.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifdef ZENI_INLINES
#include <Zeni/XML.hxx>
#endif

#ifndef ZENI_XML_H
#define ZENI_XML_H

#include <tinyxml.h>
#include <string>

namespace Zeni {

  class XML_Element {
  public:
    inline XML_Element(const TiXmlHandle &handle);

    /// Get a named subelement
    inline XML_Element operator[](const std::string &field) const;

    /// Get the first subelement
    inline XML_Element first() const;
    /// Get the next subelement
    inline XML_Element next() const;

    inline std::string value() const; ///< Get the value of the element (For nodes, this is the name)

    inline bool to_bool() const; ///< Get the contained string as a boolean
    inline int to_int() const; ///< Get the contained string as an integer
    inline float to_float() const; ///< Get the contained string as a floating point number
    inline double to_double() const; ///< Get the contained string as a double precision floating point number
    inline std::string to_string() const; ///< Get the contained string

    inline bool good() const; ///< Test to see if the Element is valid

  private:
    inline TiXmlNode * child(const std::string &field) const;

    TiXmlHandle m_handle;
  };

  class XML_Reader {
    XML_Reader(const XML_Reader &);
    XML_Reader & operator=(const XML_Reader &);

  public:
    inline XML_Reader(const std::string &filename);
    inline ~XML_Reader();

    inline XML_Element operator[](const std::string &field) const;

    inline XML_Element first() const;\

  private:
    TiXmlDocument m_xml_file;
    XML_Element * m_root;
  };

  struct Bad_XML : public Error {
    Bad_XML() : Error("XML could not be loaded!") {}
  };

  struct Bad_XML_Access : public Error {
    Bad_XML_Access() : Error("XML query could not be satisfied!") {}
  };

}

#endif
