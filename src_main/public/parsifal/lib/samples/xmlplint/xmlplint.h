#define VER_XMLPLINT "1.0.0"

#define PFOUT fout
#define ASSERT_MEM_ABORT(p) \
  if (!(p)) { printf("Out of memory! Line: %d\n", __LINE__); return XML_ABORT; }

enum outputformat {
	XMLOUTPUTFORMAT_ROUNDRIP,
	XMLOUTPUTFORMAT_EVENTS,
	XMLOUTPUTFORMAT_CANONICAL,
	XMLOUTPUTFORMAT_SILENT
};

struct XMLNOTATION {
	char *name;
	char *publicID;
	char *systemID;
};
