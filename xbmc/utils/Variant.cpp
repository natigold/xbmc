/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "Variant.h"

#include <stdlib.h>
#include <string.h>
#include <utility>

#ifndef strtoll
#ifdef TARGET_WINDOWS
#define strtoll  _strtoi64
#define strtoull _strtoui64
#define wcstoll  _wcstoi64
#define wcstoull _wcstoui64
#else // TARGET_WINDOWS
#if !defined(TARGET_DARWIN)
#define strtoll(str, endptr, base)  (int64_t)strtod(str, endptr)
#define strtoull(str, endptr, base) (uint64_t)strtod(str, endptr)
#define wcstoll(str, endptr, base)  (int64_t)wcstod(str, endptr)
#define wcstoull(str, endptr, base) (uint64_t)wcstod(str, endptr)
#endif
#endif // TARGET_WINDOWS
#endif // strtoll

std::string trimRight(const std::string &str)
{
  std::string tmp = str;
  // find_last_not_of will return string::npos (which is defined as -1)
  // or a value between 0 and size() - 1 => find_last_not_of() + 1 will
  // always result in a valid index between 0 and size()
  tmp.erase(tmp.find_last_not_of(" \n\r\t") + 1);

  return tmp;
}

std::wstring trimRight(const std::wstring &str)
{
  std::wstring tmp = str;
  // find_last_not_of will return string::npos (which is defined as -1)
  // or a value between 0 and size() - 1 => find_last_not_of() + 1 will
  // always result in a valid index between 0 and size()
  tmp.erase(tmp.find_last_not_of(L" \n\r\t") + 1);

  return tmp;
}

int64_t str2int64(const std::string &str, int64_t fallback /* = 0 */)
{
  char *end = NULL;
  std::string tmp = trimRight(str);
  int64_t result = strtoll(tmp.c_str(), &end, 0);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

int64_t str2int64(const std::wstring &str, int64_t fallback /* = 0 */)
{
  wchar_t *end = NULL;
  std::wstring tmp = trimRight(str);
  int64_t result = wcstoll(tmp.c_str(), &end, 0);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

uint64_t str2uint64(const std::string &str, uint64_t fallback /* = 0 */)
{
  char *end = NULL;
  std::string tmp = trimRight(str);
  uint64_t result = strtoull(tmp.c_str(), &end, 0);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

uint64_t str2uint64(const std::wstring &str, uint64_t fallback /* = 0 */)
{
  wchar_t *end = NULL;
  std::wstring tmp = trimRight(str);
  uint64_t result = wcstoull(tmp.c_str(), &end, 0);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

double str2double(const std::string &str, double fallback /* = 0.0 */)
{
  char *end = NULL;
  std::string tmp = trimRight(str);
  double result = strtod(tmp.c_str(), &end);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

double str2double(const std::wstring &str, double fallback /* = 0.0 */)
{
  wchar_t *end = NULL;
  std::wstring tmp = trimRight(str);
  double result = wcstod(tmp.c_str(), &end);
  if (end == NULL || *end == '\0')
    return result;

  return fallback;
}

CVariant::CVariant()
  : CVariant(VariantTypeNull)
{
}

CVariant CVariant::ConstNullVariant = CVariant::VariantTypeConstNull;
CVariant::VariantArray CVariant::EMPTY_ARRAY;
CVariant::VariantMap CVariant::EMPTY_MAP;

CVariant::CVariant(VariantType type)
{
  m_type = type;

  switch (type)
  {
    case VariantTypeInteger:
      m_data.integer = 0;
      break;
    case VariantTypeUnsignedInteger:
      m_data.unsignedinteger = 0;
      break;
    case VariantTypeBoolean:
      m_data.boolean = false;
      break;
    case VariantTypeDouble:
      m_data.dvalue = 0.0;
      break;
    case VariantTypeString:
      m_data.string = new std::string();
      break;
    case VariantTypeWideString:
      m_data.wstring = new std::wstring();
      break;
    case VariantTypeArray:
      m_data.array = new VariantArray();
      break;
    case VariantTypeObject:
      m_data.map = new VariantMap();
      break;
    default:
#ifndef TARGET_WINDOWS_STORE // this corrupts the heap in Win10 UWP version
      memset(&m_data, 0, sizeof(m_data));
#endif
      break;
  }
}

CVariant::CVariant(int integer)
{
  m_type = VariantTypeInteger;
  m_data.integer = integer;
}

CVariant::CVariant(int64_t integer)
{
  m_type = VariantTypeInteger;
  m_data.integer = integer;
}

CVariant::CVariant(unsigned int unsignedinteger)
{
  m_type = VariantTypeUnsignedInteger;
  m_data.unsignedinteger = unsignedinteger;
}

CVariant::CVariant(uint64_t unsignedinteger)
{
  m_type = VariantTypeUnsignedInteger;
  m_data.unsignedinteger = unsignedinteger;
}

CVariant::CVariant(double value)
{
  m_type = VariantTypeDouble;
  m_data.dvalue = value;
}

CVariant::CVariant(float value)
{
  m_type = VariantTypeDouble;
  m_data.dvalue = (double)value;
}

CVariant::CVariant(bool boolean)
{
  m_type = VariantTypeBoolean;
  m_data.boolean = boolean;
}

CVariant::CVariant(const char *str)
{
  m_type = VariantTypeString;
  m_data.string = new std::string(str);
}

CVariant::CVariant(const char *str, unsigned int length)
{
  m_type = VariantTypeString;
  m_data.string = new std::string(str, length);
}

CVariant::CVariant(const std::string &str)
{
  m_type = VariantTypeString;
  m_data.string = new std::string(str);
}

CVariant::CVariant(std::string &&str)
{
  m_type = VariantTypeString;
  m_data.string = new std::string(std::move(str));
}

CVariant::CVariant(const wchar_t *str)
{
  m_type = VariantTypeWideString;
  m_data.wstring = new std::wstring(str);
}

CVariant::CVariant(const wchar_t *str, unsigned int length)
{
  m_type = VariantTypeWideString;
  m_data.wstring = new std::wstring(str, length);
}

CVariant::CVariant(const std::wstring &str)
{
  m_type = VariantTypeWideString;
  m_data.wstring = new std::wstring(str);
}

CVariant::CVariant(std::wstring &&str)
{
  m_type = VariantTypeWideString;
  m_data.wstring = new std::wstring(std::move(str));
}

CVariant::CVariant(const std::vector<std::string> &strArray)
{
  m_type = VariantTypeArray;
  m_data.array = new VariantArray;
  m_data.array->reserve(strArray.size());
  for (const auto& item : strArray)
    m_data.array->push_back(CVariant(item));
}

CVariant::CVariant(const std::map<std::string, std::string> &strMap)
{
  m_type = VariantTypeObject;
  m_data.map = new VariantMap;
  for (std::map<std::string, std::string>::const_iterator it = strMap.begin(); it != strMap.end(); ++it)
    m_data.map->insert(make_pair(it->first, CVariant(it->second)));
}

CVariant::CVariant(const std::map<std::string, CVariant> &variantMap)
{
  m_type = VariantTypeObject;
  m_data.map = new VariantMap(variantMap.begin(), variantMap.end());
}

CVariant::CVariant(const CVariant &variant)
{
  m_type = VariantTypeNull;
  *this = variant;
}

CVariant::CVariant(CVariant&& rhs) noexcept
{
  //Set this so that operator= don't try and run cleanup
  //when we're not initialized.
  m_type = VariantTypeNull;

  *this = std::move(rhs);
}

CVariant::~CVariant()
{
  cleanup();
}

void CVariant::cleanup()
{
  switch (m_type)
  {
  case VariantTypeString:
    delete m_data.string;
    m_data.string = nullptr;
    break;

  case VariantTypeWideString:
    delete m_data.wstring;
    m_data.wstring = nullptr;
    break;

  case VariantTypeArray:
    delete m_data.array;
    m_data.array = nullptr;
    break;

  case VariantTypeObject:
    delete m_data.map;
    m_data.map = nullptr;
    break;
  default:
    break;
  }
  m_type = VariantTypeNull;
}

bool CVariant::isInteger() const
{
  return isSignedInteger() || isUnsignedInteger();
}

bool CVariant::isSignedInteger() const
{
  return m_type == VariantTypeInteger;
}

bool CVariant::isUnsignedInteger() const
{
  return m_type == VariantTypeUnsignedInteger;
}

bool CVariant::isBoolean() const
{
  return m_type == VariantTypeBoolean;
}

bool CVariant::isDouble() const
{
  return m_type == VariantTypeDouble;
}

bool CVariant::isString() const
{
  return m_type == VariantTypeString;
}

bool CVariant::isWideString() const
{
  return m_type == VariantTypeWideString;
}

bool CVariant::isArray() const
{
  return m_type == VariantTypeArray;
}

bool CVariant::isObject() const
{
  return m_type == VariantTypeObject;
}

bool CVariant::isNull() const
{
  return m_type == VariantTypeNull || m_type == VariantTypeConstNull;
}

CVariant::VariantType CVariant::type() const
{
  return m_type;
}

int64_t CVariant::asInteger(int64_t fallback) const
{
  switch (m_type)
  {
    case VariantTypeInteger:
      return m_data.integer;
    case VariantTypeUnsignedInteger:
      return (int64_t)m_data.unsignedinteger;
    case VariantTypeDouble:
      return (int64_t)m_data.dvalue;
    case VariantTypeString:
      return str2int64(*m_data.string, fallback);
    case VariantTypeWideString:
      return str2int64(*m_data.wstring, fallback);
    default:
      return fallback;
  }

  return fallback;
}

int32_t CVariant::asInteger32(int32_t fallback) const
{
  return static_cast<int32_t>(asInteger(fallback));
}

uint64_t CVariant::asUnsignedInteger(uint64_t fallback) const
{
  switch (m_type)
  {
    case VariantTypeUnsignedInteger:
      return m_data.unsignedinteger;
    case VariantTypeInteger:
      return (uint64_t)m_data.integer;
    case VariantTypeDouble:
      return (uint64_t)m_data.dvalue;
    case VariantTypeString:
      return str2uint64(*m_data.string, fallback);
    case VariantTypeWideString:
      return str2uint64(*m_data.wstring, fallback);
    default:
      return fallback;
  }

  return fallback;
}

uint32_t CVariant::asUnsignedInteger32(uint32_t fallback) const
{
  return static_cast<uint32_t>(asUnsignedInteger(fallback));
}

double CVariant::asDouble(double fallback) const
{
  switch (m_type)
  {
    case VariantTypeDouble:
      return m_data.dvalue;
    case VariantTypeInteger:
      return (double)m_data.integer;
    case VariantTypeUnsignedInteger:
      return (double)m_data.unsignedinteger;
    case VariantTypeString:
      return str2double(*m_data.string, fallback);
    case VariantTypeWideString:
      return str2double(*m_data.wstring, fallback);
    default:
      return fallback;
  }

  return fallback;
}

float CVariant::asFloat(float fallback) const
{
  switch (m_type)
  {
    case VariantTypeDouble:
      return (float)m_data.dvalue;
    case VariantTypeInteger:
      return (float)m_data.integer;
    case VariantTypeUnsignedInteger:
      return (float)m_data.unsignedinteger;
    case VariantTypeString:
      return (float)str2double(*m_data.string, static_cast<double>(fallback));
    case VariantTypeWideString:
      return (float)str2double(*m_data.wstring, static_cast<double>(fallback));
    default:
      return fallback;
  }

  return fallback;
}

bool CVariant::asBoolean(bool fallback) const
{
  switch (m_type)
  {
    case VariantTypeBoolean:
      return m_data.boolean;
    case VariantTypeInteger:
      return (m_data.integer != 0);
    case VariantTypeUnsignedInteger:
      return (m_data.unsignedinteger != 0);
    case VariantTypeDouble:
      return (m_data.dvalue != 0);
    case VariantTypeString:
      if (m_data.string->empty() || m_data.string->compare("0") == 0 || m_data.string->compare("false") == 0)
        return false;
      return true;
    case VariantTypeWideString:
      if (m_data.wstring->empty() || m_data.wstring->compare(L"0") == 0 || m_data.wstring->compare(L"false") == 0)
        return false;
      return true;
    default:
      return fallback;
  }

  return fallback;
}

std::string CVariant::asString(const std::string& fallback /* = "" */) const&
{
  switch (m_type)
  {
    case VariantTypeString:
      return *m_data.string;
    case VariantTypeBoolean:
      return m_data.boolean ? "true" : "false";
    case VariantTypeInteger:
      return std::to_string(m_data.integer);
    case VariantTypeUnsignedInteger:
      return std::to_string(m_data.unsignedinteger);
    case VariantTypeDouble:
      return std::to_string(m_data.dvalue);
    default:
      return fallback;
  }

  return fallback;
}

std::string CVariant::asString(const std::string& fallback /*= ""*/) &&
{
  if (m_type == VariantTypeString)
    return std::move(*m_data.string);
  else
    return asString(fallback);
}

std::wstring CVariant::asWideString(const std::wstring& fallback /* = L"" */) const&
{
  switch (m_type)
  {
    case VariantTypeWideString:
      return *m_data.wstring;
    case VariantTypeBoolean:
      return m_data.boolean ? L"true" : L"false";
    case VariantTypeInteger:
      return std::to_wstring(m_data.integer);
    case VariantTypeUnsignedInteger:
      return std::to_wstring(m_data.unsignedinteger);
    case VariantTypeDouble:
      return std::to_wstring(m_data.dvalue);
    default:
      return fallback;
  }

  return fallback;
}

std::wstring CVariant::asWideString(const std::wstring& fallback /*= L""*/) &&
{
  if (m_type == VariantTypeWideString)
    return std::move(*m_data.wstring);
  else
    return asWideString(fallback);
}

CVariant& CVariant::operator[](const std::string& key) &
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeObject;
    m_data.map = new VariantMap;
  }

  if (m_type == VariantTypeObject)
    return (*m_data.map)[key];
  else
    return ConstNullVariant;
}

const CVariant& CVariant::operator[](const std::string& key) const&
{
  VariantMap::const_iterator it;
  if (m_type == VariantTypeObject && (it = m_data.map->find(key)) != m_data.map->end())
    return it->second;
  else
    return ConstNullVariant;
}

CVariant CVariant::operator[](const std::string& key) &&
{
  if (m_type == VariantTypeObject)
    return std::move((*m_data.map)[key]);
  else
    return ConstNullVariant;
}

CVariant& CVariant::operator[](unsigned int position) &
{
  if (m_type == VariantTypeArray && size() > position)
    return m_data.array->at(position);
  else
    return ConstNullVariant;
}

const CVariant& CVariant::operator[](unsigned int position) const&
{
  if (m_type == VariantTypeArray && size() > position)
    return m_data.array->at(position);
  else
    return ConstNullVariant;
}

CVariant CVariant::operator[](unsigned int position) &&
{
  if (m_type == VariantTypeArray && size() > position)
    return std::move(m_data.array->at(position));
  else
    return ConstNullVariant;
}

CVariant &CVariant::operator=(const CVariant &rhs)
{
  if (m_type == VariantTypeConstNull || this == &rhs)
    return *this;

  cleanup();

  m_type = rhs.m_type;

  switch (m_type)
  {
  case VariantTypeInteger:
    m_data.integer = rhs.m_data.integer;
    break;
  case VariantTypeUnsignedInteger:
    m_data.unsignedinteger = rhs.m_data.unsignedinteger;
    break;
  case VariantTypeBoolean:
    m_data.boolean = rhs.m_data.boolean;
    break;
  case VariantTypeDouble:
    m_data.dvalue = rhs.m_data.dvalue;
    break;
  case VariantTypeString:
    m_data.string = new std::string(*rhs.m_data.string);
    break;
  case VariantTypeWideString:
    m_data.wstring = new std::wstring(*rhs.m_data.wstring);
    break;
  case VariantTypeArray:
    m_data.array = new VariantArray(rhs.m_data.array->begin(), rhs.m_data.array->end());
    break;
  case VariantTypeObject:
    m_data.map = new VariantMap(rhs.m_data.map->begin(), rhs.m_data.map->end());
    break;
  default:
    break;
  }

  return *this;
}

CVariant& CVariant::operator=(CVariant&& rhs) noexcept
{
  if (m_type == VariantTypeConstNull || this == &rhs)
    return *this;

  //Make sure that if we're moved into we don't leak any pointers
  if (m_type != VariantTypeNull)
    cleanup();

  m_type = rhs.m_type;
  m_data = rhs.m_data;

  //Should be enough to just set m_type here
  //but better safe than sorry, could probably lead to coverity warnings
  if (rhs.m_type == VariantTypeString)
    rhs.m_data.string = nullptr;
  else if (rhs.m_type == VariantTypeWideString)
    rhs.m_data.wstring = nullptr;
  else if (rhs.m_type == VariantTypeArray)
    rhs.m_data.array = nullptr;
  else if (rhs.m_type == VariantTypeObject)
    rhs.m_data.map = nullptr;

  rhs.m_type = VariantTypeNull;

  return *this;
}

bool CVariant::operator==(const CVariant &rhs) const
{
  if (m_type == rhs.m_type)
  {
    switch (m_type)
    {
    case VariantTypeInteger:
      return m_data.integer == rhs.m_data.integer;
    case VariantTypeUnsignedInteger:
      return m_data.unsignedinteger == rhs.m_data.unsignedinteger;
    case VariantTypeBoolean:
      return m_data.boolean == rhs.m_data.boolean;
    case VariantTypeDouble:
      return m_data.dvalue == rhs.m_data.dvalue;
    case VariantTypeString:
      return *m_data.string == *rhs.m_data.string;
    case VariantTypeWideString:
      return *m_data.wstring == *rhs.m_data.wstring;
    case VariantTypeArray:
      return *m_data.array == *rhs.m_data.array;
    case VariantTypeObject:
      return *m_data.map == *rhs.m_data.map;
    default:
      break;
    }
  }

  return false;
}

void CVariant::reserve(size_t length)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeArray;
    m_data.array = new VariantArray;
  }
  if (m_type == VariantTypeArray)
    m_data.array->reserve(length);
}

void CVariant::push_back(const CVariant &variant)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeArray;
    m_data.array = new VariantArray;
  }

  if (m_type == VariantTypeArray)
    m_data.array->push_back(variant);
}

void CVariant::push_back(CVariant &&variant)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeArray;
    m_data.array = new VariantArray;
  }

  if (m_type == VariantTypeArray)
    m_data.array->push_back(std::move(variant));
}

void CVariant::append(const CVariant &variant)
{
  push_back(variant);
}

void CVariant::append(CVariant&& variant)
{
  push_back(std::move(variant));
}

const char *CVariant::c_str() const
{
  if (m_type == VariantTypeString)
    return m_data.string->c_str();
  else
    return NULL;
}

void CVariant::swap(CVariant &rhs)
{
  VariantType  temp_type = m_type;
  VariantUnion temp_data = m_data;

  m_type = rhs.m_type;
  m_data = rhs.m_data;

  rhs.m_type = temp_type;
  rhs.m_data = temp_data;
}

CVariant::iterator_array CVariant::begin_array()
{
  if (m_type == VariantTypeArray)
    return m_data.array->begin();
  else
    return EMPTY_ARRAY.begin();
}

CVariant::const_iterator_array CVariant::begin_array() const
{
  if (m_type == VariantTypeArray)
    return m_data.array->begin();
  else
    return EMPTY_ARRAY.begin();
}

CVariant::iterator_array CVariant::end_array()
{
  if (m_type == VariantTypeArray)
    return m_data.array->end();
  else
    return EMPTY_ARRAY.end();
}

CVariant::const_iterator_array CVariant::end_array() const
{
  if (m_type == VariantTypeArray)
    return m_data.array->end();
  else
    return EMPTY_ARRAY.end();
}

CVariant::iterator_map CVariant::begin_map()
{
  if (m_type == VariantTypeObject)
    return m_data.map->begin();
  else
    return EMPTY_MAP.begin();
}

CVariant::const_iterator_map CVariant::begin_map() const
{
  if (m_type == VariantTypeObject)
    return m_data.map->begin();
  else
    return EMPTY_MAP.begin();
}

CVariant::iterator_map CVariant::end_map()
{
  if (m_type == VariantTypeObject)
    return m_data.map->end();
  else
    return EMPTY_MAP.end();
}

CVariant::const_iterator_map CVariant::end_map() const
{
  if (m_type == VariantTypeObject)
    return m_data.map->end();
  else
    return EMPTY_MAP.end();
}

unsigned int CVariant::size() const
{
  if (m_type == VariantTypeObject)
    return m_data.map->size();
  else if (m_type == VariantTypeArray)
    return m_data.array->size();
  else if (m_type == VariantTypeString)
    return m_data.string->size();
  else if (m_type == VariantTypeWideString)
    return m_data.wstring->size();
  else
    return 0;
}

bool CVariant::empty() const
{
  if (m_type == VariantTypeObject)
    return m_data.map->empty();
  else if (m_type == VariantTypeArray)
    return m_data.array->empty();
  else if (m_type == VariantTypeString)
    return m_data.string->empty();
  else if (m_type == VariantTypeWideString)
    return m_data.wstring->empty();
  else if (m_type == VariantTypeNull)
    return true;

  return false;
}

void CVariant::clear()
{
  if (m_type == VariantTypeObject)
    m_data.map->clear();
  else if (m_type == VariantTypeArray)
    m_data.array->clear();
  else if (m_type == VariantTypeString)
    m_data.string->clear();
  else if (m_type == VariantTypeWideString)
    m_data.wstring->clear();
}

void CVariant::erase(const std::string &key)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeObject;
    m_data.map = new VariantMap;
  }
  else if (m_type == VariantTypeObject)
    m_data.map->erase(key);
}

void CVariant::erase(unsigned int position)
{
  if (m_type == VariantTypeNull)
  {
    m_type = VariantTypeArray;
    m_data.array = new VariantArray;
  }

  if (m_type == VariantTypeArray && position < size())
    m_data.array->erase(m_data.array->begin() + position);
}

bool CVariant::isMember(const std::string &key) const
{
  if (m_type == VariantTypeObject)
    return m_data.map->find(key) != m_data.map->end();

  return false;
}
