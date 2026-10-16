//-----------------------------------------------------------------------------
// @file    : Libmxml.h
// @brief   : mxml-related functions
// @details : 
// @author  : 
// @version :      
// @date    :      
// @history :                  
//-----------------------------------------------------------------------------
#ifndef LIB_MXML_H
#define LIB_MXML_H

//-----------------------------------------------------------------------------
// Function declaration
//-----------------------------------------------------------------------------

typedef enum mxml_add_e
{
    MXML_ADD_BEFORE,
    MXML_ADD_AFTER
}mxml_add_t;

typedef enum mxml_descend_e
{
    MXML_DESCEND_FIRST = -1,
    MXML_DESCEND_NONE = 0,
    MXML_DESCEND_ALL = 1
}mxml_descend_t;

typedef enum mxml_sax_event_e
{
    MXML_SAX_EVENT_CDATA,
    MXML_SAX_EVENT_COMMENT,
    MXML_SAX_EVENT_DATA,
    MXML_SAX_EVENT_DECLARATION,
    MXML_SAX_EVENT_DIRECTIVE,
    MXML_SAX_EVENT_ELEMENT_CLOSE,
    MXML_SAX_EVENT_ELEMENT_OPEN
}mxml_sax_event_t;

typedef enum mxml_type_e
{
    MXML_TYPE_IGNORE = -1,
    MXML_TYPE_CDATA,
    MXML_TYPE_COMMENT,
    MXML_TYPE_DECLARATION,
    MXML_TYPE_DIRECTIVE,
    MXML_TYPE_ELEMENT,
    MXML_TYPE_INTEGER,
    MXML_TYPE_OPAQUE,
    MXML_TYPE_REAL,
    MXML_TYPE_TEXT,
    MXML_TYPE_CUSTOM
}mxml_type_t;

typedef enum mxml_ws_e
{
    MXML_WS_BEFORE_OPEN,
    MXML_WS_AFTER_OPEN,
    MXML_WS_BEFORE_CLOSE,
    MXML_WS_AFTER_CLOSE,
}mxml_ws_t;

typedef void (*mxml_error_cb_t)(void *cbdata, const char *message);
typedef struct _mxml_node_s mxml_node_t;
typedef struct _mxml_index_s mxml_index_t;
typedef struct _mxml_options_s mxml_options_t;
typedef void (*mxml_custfree_cb_t)(void *cbdata, void *custdata);
typedef bool (*mxml_custload_cb_t)(void *cbdata, mxml_node_t *node, const char *s);
typedef char *(*mxml_custsave_cb_t)(void *cbdata, mxml_node_t *node);
typedef int (*mxml_entity_cb_t)(void *cbdata, const char *name);
typedef size_t (*mxml_io_cb_t)(void *cbdata, void *buffer, size_t bytes);
typedef bool (*mxml_sax_cb_t)(void *cbdata, mxml_node_t *node, mxml_sax_event_t event);
typedef char *(*mxml_strcopy_cb_t)(void *cbdata, const char *s);
typedef void (*mxml_strfree_cb_t)(void *cbdata, char *s);
typedef mxml_type_t (*mxml_type_cb_t)(void *cbdata, mxml_node_t *node);
typedef const char *(*mxml_ws_cb_t)(void *cbdata, mxml_node_t *node, mxml_ws_t when);

typedef struct _mxml_attr_s
{
    char *name;
    char *value;
}_mxml_attr_t;

typedef struct _mxml_element_s
{
    char *name;
    size_t num_attrs;
    _mxml_attr_t *attrs;
}_mxml_element_t;

typedef struct _mxml_text_s
{
    bool whitespace;
    char *string;
}_mxml_text_t;

typedef struct _mxml_custom_s
{
    void *data;
    mxml_custfree_cb_t free_cb;
    void *free_cbdata;
}_mxml_custom_t;

typedef union _mxml_value_u
{
    char *cdata;
    char *comment;
    char *declaration;
    char *directive;
    _mxml_element_t element;
    long integer;
    char *opaque;
    double real;
    _mxml_text_t text;
    _mxml_custom_t custom;
}_mxml_value_t;

struct _mxml_node_s
{
    mxml_type_t type;
    struct _mxml_node_s *next;
    struct _mxml_node_s *prev;
    struct _mxml_node_s *parent;
    struct _mxml_node_s *child;
    struct _mxml_node_s *last_child;
    _mxml_value_t value;
    size_t ref_count;
    void *user_data;
};

typedef struct _mxml_global_s
{
    mxml_strcopy_cb_t strcopy_cb;
    mxml_strfree_cb_t strfree_cb;
    void *str_cbdata;
}_mxml_global_t;

struct _mxml_index_s
{
    char *attr;
    size_t num_nodes;
    size_t alloc_nodes;
    size_t cur_node;
    mxml_node_t **nodes;
};

struct _mxml_options_s
{
    struct lconv *loc;
    size_t loc_declen;
    mxml_custload_cb_t custload_cb;
    mxml_custsave_cb_t custsave_cb;
    void *cust_cbdata;
    mxml_entity_cb_t entity_cb;
    void *entity_cbdata;
    mxml_error_cb_t	error_cb;
    void *error_cbdata;
    mxml_sax_cb_t sax_cb;
    void *sax_cbdata;
    mxml_type_cb_t type_cb;
    void *type_cbdata;
    mxml_type_t type_value;
    int wrap;
    mxml_ws_cb_t ws_cb;
    void *ws_cbdata;
};


void Lib_mxmlAdd(mxml_node_t *parent, mxml_add_t add, mxml_node_t *child, mxml_node_t *node);

void Lib_mxmlDelete(mxml_node_t *node);

void Lib_mxmlElementClearAttr(mxml_node_t *node, const char *name);

const char *Lib_mxmlElementGetAttr(mxml_node_t *node, const char *name);

const char *Lib_mxmlElementGetAttrByIndex(mxml_node_t *node, size_t idx, const char **name);

size_t Lib_mxmlElementGetAttrCount(mxml_node_t *node);

void Lib_mxmlElementSetAttr(mxml_node_t *node, const char *name, const char *value);

void Lib_mxmlElementSetAttrf(mxml_node_t *node, const char *name, const char *format, ...);

mxml_node_t *Lib_mxmlFindElement(mxml_node_t *node, mxml_node_t *top, const char *element, const char *attr, const char *value, mxml_descend_t descend);

mxml_node_t *Lib_mxmlFindPath(mxml_node_t *node, const char *path);

const char *Lib_mxmlGetCDATA(mxml_node_t *node);

const char *Lib_mxmlGetComment(mxml_node_t *node);

const void *Lib_mxmlGetCustom(mxml_node_t *node);

const char *Lib_mxmlGetDeclaration(mxml_node_t *node);

const char *Lib_mxmlGetDirective(mxml_node_t *node);

const char *Lib_mxmlGetElement(mxml_node_t *node);

mxml_node_t *Lib_mxmlGetFirstChild(mxml_node_t *node);

long Lib_mxmlGetInteger(mxml_node_t *node);

mxml_node_t *Lib_mxmlGetLastChild(mxml_node_t *node);

mxml_node_t *Lib_mxmlGetNextSibling(mxml_node_t *node);

const char *Lib_mxmlGetOpaque(mxml_node_t *node);

mxml_node_t *Lib_mxmlGetParent(mxml_node_t *node);

mxml_node_t *Lib_mxmlGetPrevSibling(mxml_node_t *node);

double Lib_mxmlGetReal(mxml_node_t *node);

size_t Lib_mxmlGetRefCount(mxml_node_t *node);

const char *Lib_mxmlGetText(mxml_node_t *node, bool *whitespace);

mxml_type_t Lib_mxmlGetType(mxml_node_t *node);

void *Lib_mxmlGetUserData(mxml_node_t *node);

void Lib_mxmlIndexDelete(mxml_index_t *ind);

mxml_node_t *Lib_mxmlIndexEnum(mxml_index_t *ind);

mxml_node_t *Lib_mxmlIndexFind(mxml_index_t *ind, const char *element, const char *value);

size_t Lib_mxmlIndexGetCount(mxml_index_t *ind);

mxml_index_t *Lib_mxmlIndexNew(mxml_node_t *node, const char *element, const char *attr);

mxml_node_t *Lib_mxmlIndexReset(mxml_index_t *ind);

mxml_node_t *Lib_mxmlLoadFd(mxml_node_t *top, mxml_options_t *options, int fd);

mxml_node_t *Lib_mxmlLoadFile(mxml_node_t *top, mxml_options_t *options, FILE *fp);

mxml_node_t *Lib_mxmlLoadFilename(mxml_node_t *top, mxml_options_t *options, const char *filename);

mxml_node_t *Lib_mxmlLoadIO(mxml_node_t *top, mxml_options_t *options, mxml_io_cb_t io_cb, void *io_cbdata);

mxml_node_t *Lib_mxmlLoadString(mxml_node_t *top, mxml_options_t *options, const char *s);

void Lib_mxmlOptionsDelete(mxml_options_t *options);

mxml_options_t *Lib_mxmlOptionsNew(void);

void Lib_mxmlOptionsSetCustomCallbacks(mxml_options_t *options, mxml_custload_cb_t load_cb, mxml_custsave_cb_t save_cb, void *cbdata);

void Lib_mxmlOptionsSetEntityCallback(mxml_options_t *options, mxml_entity_cb_t cb, void *cbdata);

void Lib_mxmlOptionsSetErrorCallback(mxml_options_t *options, mxml_error_cb_t cb, void *cbdata);

void Lib_mxmlOptionsSetSAXCallback(mxml_options_t *options, mxml_sax_cb_t cb, void *cbdata);

void Lib_mxmlOptionsSetTypeCallback(mxml_options_t *options, mxml_type_cb_t cb, void *cbdata);

void Lib_mxmlOptionsSetTypeValue(mxml_options_t *options, mxml_type_t type);

void Lib_mxmlOptionsSetWhitespaceCallback(mxml_options_t *options, mxml_ws_cb_t cb, void *cbdata);

void Lib_mxmlOptionsSetWrapMargin(mxml_options_t *options, int column);

mxml_node_t *Lib_mxmlNewCDATA(mxml_node_t *parent, const char *string);

mxml_node_t *Lib_mxmlNewCDATAf(mxml_node_t *parent, const char *format, ...);

mxml_node_t *Lib_mxmlNewComment(mxml_node_t *parent, const char *comment);

mxml_node_t *Lib_mxmlNewCommentf(mxml_node_t *parent, const char *format, ...);

mxml_node_t *Lib_mxmlNewCustom(mxml_node_t *parent, void *data, mxml_custfree_cb_t free_cb, void *free_cbdata);

mxml_node_t *Lib_mxmlNewDeclaration(mxml_node_t *parent, const char *declaration);

mxml_node_t *Lib_mxmlNewDeclarationf(mxml_node_t *parent, const char *format, ...);

mxml_node_t *Lib_mxmlNewDirective(mxml_node_t *parent, const char *directive);

mxml_node_t *Lib_mxmlNewDirectivef(mxml_node_t *parent, const char *format, ...);

mxml_node_t *Lib_mxmlNewElement(mxml_node_t *parent, const char *name);

mxml_node_t *Lib_mxmlNewInteger(mxml_node_t *parent, long integer);

mxml_node_t *Lib_mxmlNewOpaque(mxml_node_t *parent, const char *opaque);

mxml_node_t *Lib_mxmlNewOpaquef(mxml_node_t *parent, const char *format, ...);

mxml_node_t *Lib_mxmlNewReal(mxml_node_t *parent, double real);

mxml_node_t *Lib_mxmlNewText(mxml_node_t *parent, bool whitespace, const char *string);

mxml_node_t *Lib_mxmlNewTextf(mxml_node_t *parent, bool whitespace, const char *format, ...);

mxml_node_t *Lib_mxmlNewXML(const char *version);

int Lib_mxmlRelease(mxml_node_t *node);

void Lib_mxmlRemove(mxml_node_t *node);

int Lib_mxmlRetain(mxml_node_t *node);

char *Lib_mxmlSaveAllocString(mxml_node_t *node, mxml_options_t *options);

bool Lib_mxmlSaveFd(mxml_node_t *node, mxml_options_t *options, int fd);

bool Lib_mxmlSaveFile(mxml_node_t *node, mxml_options_t *options, FILE *fp);

bool Lib_mxmlSaveFilename(mxml_node_t *node, mxml_options_t *options, const char *filename);

bool Lib_mxmlSaveIO(mxml_node_t *node, mxml_options_t *options, mxml_io_cb_t io_cb, void *io_cbdata);

size_t Lib_mxmlSaveString(mxml_node_t *node, mxml_options_t *options, char *buffer, size_t bufsize);

bool Lib_mxmlSetCDATA(mxml_node_t *node, const char *data);

bool Lib_mxmlSetCDATAf(mxml_node_t *node, const char *format, ...);

bool Lib_mxmlSetComment(mxml_node_t *node, const char *comment);

bool Lib_mxmlSetCommentf(mxml_node_t *node, const char *format, ...);

bool Lib_mxmlSetDeclaration(mxml_node_t *node, const char *declaration);

bool Lib_mxmlSetDeclarationf(mxml_node_t *node, const char *format, ...);

bool Lib_mxmlSetDirective(mxml_node_t *node, const char *directive);

bool Lib_mxmlSetDirectivef(mxml_node_t *node, const char *format, ...);

bool Lib_mxmlSetCustom(mxml_node_t *node, void *data, mxml_custfree_cb_t free_cb, void *free_cbdata);

bool Lib_mxmlSetElement(mxml_node_t *node, const char *name);

bool Lib_mxmlSetElement(mxml_node_t *node, const char *name);

bool Lib_mxmlSetInteger(mxml_node_t *node, long integer);

bool Lib_mxmlSetOpaque(mxml_node_t *node, const char *opaque);

bool Lib_mxmlSetOpaquef(mxml_node_t *node, const char *format, ...);

bool Lib_mxmlSetReal(mxml_node_t *node, double real);

void Lib_mxmlSetStringCallbacks(mxml_strcopy_cb_t strcopy_cb, mxml_strfree_cb_t strfree_cb, void *str_cbdata);

bool Lib_mxmlSetText(mxml_node_t *node, bool whitespace, const char *string);

bool Lib_mxmlSetTextf(mxml_node_t *node, bool whitespace, const char *format, ...);

bool Lib_mxmlSetUserData(mxml_node_t *node, void *data);

mxml_node_t *Lib_mxmlWalkNext(mxml_node_t *node, mxml_node_t *top, mxml_descend_t descend);

mxml_node_t *Lib_mxmlWalkPrev(mxml_node_t *node, mxml_node_t *top, mxml_descend_t descend);

#endif