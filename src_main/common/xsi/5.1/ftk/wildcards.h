#ifndef WILDCARD_H_INCLUDED
#define WILDCARD_H_INCLUDED

// Copyright (C) 1996 - 2002 Florian Schintke
//
// This is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2, or (at your option) any later
// version.
//
// Thanks to the E.S.O. - ACS project that has done this C++ interface
// to the wildcards pttern matching algorithm

#ifndef __cplusplus
#error This is a C++ include file and cannot be used from plain C
#endif

// Implementation of the UN*X wildcards
// Supported wild-characters: '*', '?'; sets: [a-z], '!' negation
// Examples:
//       '[a-g]l*i?n' matches 'florian'
//       '[!abc]*e' matches 'smile'
//       '[-z] matches 'a'

class Wildcard
{
public:
  // This function implements the UN*X wildcards and returns:
  // 0 - if *wildcard does not match *test
  // 1 - if *wildcard matches *test
  static int wildcardfit (const char *wildcard, const char *test);
       
private:
  // Scans a set of characters and returns 0 if the set mismatches at this
  // position in the teststring and 1 if it is matching
  // wildcard is set to the closing ] and test is unmodified if mismatched
  // and otherwise the char pointer is pointing to the next character
  static int set (const char **wildcard, const char **test);

  // Scans an asterisk
  static int asterisk (const char **wildcard, const char **test);
};

#endif


