/*
 * Copyright 2003 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * I18NAPI.h - API support for charset conversion identifiers
 *
 */

class CharSetApi {
public:
    /*
     * The CharSet enum provides the values for the ClientApi::SetTrans
     * api call.  You may need to cast to (int)
     */

    enum CharSet {
	NOCONV = 0, UTF_8, ISO8859_1, UTF_16, SHIFTJIS, EUCJP,
	WIN_US_ANSI, WIN_US_OEM, MACOS_ROMAN, ISO8859_15
    };

    static CharSet Lookup(const char *);
};
