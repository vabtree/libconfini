/*  -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */

/**

    @file       confini.h
    @brief      libconfini header
    @author     Stefano Gioffr&eacute;
    @copyright  GNU Public License v3
    @date       2016-2019
    @see        https://github.com/madmurphy/libconfini/

**/


#ifndef _LIBCONFINI_HEADER_
#define _LIBCONFINI_HEADER_



#include <stdio.h>
#include <stdint.h>



/*  PRIVATE (HEADER-SCOPED) MACROS  */


#define _LIBCONFINI_INIFORMAT_DESCR_(NAME, OFFSET, SIZE, DEFVAL) \
    unsigned char NAME:SIZE;
#define _LIBCONFINI_DEFAULT_FMT_DESCR_(NAME, OFFSET, SIZE, DEFVAL) DEFVAL,
#define _LIBCONFINI_UNIXLIKE_FMT_DESCR_(NAME, OFFSET, SIZE, DEFVAL) 0,
#define _LIBCONFINI_INIFORMAT_TYPE_ \
    struct IniFormat { INIFORMAT_TABLE_AS(_LIBCONFINI_INIFORMAT_DESCR_) }
#define _LIBCONFINI_DEFAULT_FORMAT_ \
    { INIFORMAT_TABLE_AS(_LIBCONFINI_DEFAULT_FMT_DESCR_) }
#define _LIBCONFINI_UNIXLIKE_FORMAT_ \
    { INIFORMAT_TABLE_AS(_LIBCONFINI_UNIXLIKE_FMT_DESCR_) }



/*  PUBLIC MACROS  */


/**
    @brief  Calls a user-given macro for each row of the table
**/
/*
    NOTE: The following table and the order of its rows **define** (and link
    together) both the #IniFormat and #IniFormatNum data types declared in this
    header
*/
#define INIFORMAT_TABLE_AS(_____)                 /*  IniFormat table  *\

        NAME                      BIT  SIZE DEFAULT
                                                                      */\
 _____( delimiter_symbol,         0,   7,   INI_EQUALS                ) \
 _____( case_sensitive,           7,   1,   0                         )/*
                                                                      */\
 _____( semicolon_marker,         8,   2,   INI_DISABLED_OR_COMMENT   ) \
 _____( hash_marker,              10,  2,   INI_DISABLED_OR_COMMENT   ) \
 _____( section_paths,            12,  2,   INI_ABSOLUTE_AND_RELATIVE ) \
 _____( multiline_nodes,          14,  2,   INI_MULTILINE_EVERYWHERE  )/*
                                                                      */\
 _____( no_single_quotes,         16,  1,   0                         ) \
 _____( no_double_quotes,         17,  1,   0                         ) \
 _____( no_spaces_in_names,       18,  1,   0                         ) \
 _____( implicit_is_not_empty,    19,  1,   0                         ) \
 _____( do_not_collapse_values,   20,  1,   0                         ) \
 _____( preserve_empty_quotes,    21,  1,   0                         ) \
 _____( disabled_after_space,     22,  1,   0                         ) \
 _____( disabled_can_be_implicit, 23,  1,   0                         )



/**
    @brief  Checks whether a format does **not** support escape sequences
**/
#define INIFORMAT_HAS_NO_ESC(FORMAT) \
    (FORMAT.multiline_nodes == INI_NO_MULTILINE && \
    FORMAT.no_double_quotes && FORMAT.no_single_quotes)



/*  PUBLIC TYPEDEFS  */


/**
    @brief  24-bit bitfield representing the format of an INI file (INI dialect)
**/
typedef _LIBCONFINI_INIFORMAT_TYPE_ IniFormat;


/**
    @brief  Global statistics about an INI file
**/
typedef struct IniStatistics {
    const IniFormat format;
    const size_t bytes;
    const size_t members;
} IniStatistics;


/**
    @brief  Dispatch of a single INI node
**/
typedef struct IniDispatch {
    const IniFormat format;
    uint8_t type;
    char * data;
    char * value;
    const char * append_to;
    size_t d_len;
    size_t v_len;
    size_t at_len;
    size_t dispatch_id;
} IniDispatch;


/**
    @brief  The unique ID number of an INI format (24-bit maximum)
**/
typedef uint32_t IniFormatNum;


/**
    @brief  Callback function for handling an #IniStatistics structure
**/
typedef int (* IniStatsHandler) (
    IniStatistics * statistics,
    void * user_data
);


/**
    @brief  Callback function for handling an #IniDispatch structure
**/
typedef int (* IniDispHandler) (
    IniDispatch * dispatch,
    void * user_data
);


/**
    @brief  Callback function for handling an INI string
**/
typedef int (* IniStrHandler) (
    char * ini_string,
    size_t string_length,
    size_t string_num,
    IniFormat format,
    void * user_data
);


/**
    @brief  Callback function for handling a selected fragment of an INI string
**/
typedef int (* IniSubstrHandler) (
    const char * ini_string,
    size_t fragm_offset,
    size_t fragm_length,
    size_t fragm_num,
    IniFormat format,
    void * user_data
);



/*  PUBLIC FUNCTIONS  */


extern int strip_ini_cache (
	register char * const ini_buffer,
	const size_t ini_length,
	const IniFormat format,
	const IniStatsHandler f_init,
	const IniDispHandler f_foreach,
	void * const user_data
);


extern int load_ini_file (
    FILE * const ini_file,
    const IniFormat format,
    const IniStatsHandler f_init,
    const IniDispHandler f_foreach,
    void * const user_data
);


extern int load_ini_path (
    const char * const path,
    const IniFormat format,
    const IniStatsHandler f_init,
    const IniDispHandler f_foreach,
    void * const user_data
);


extern _Bool ini_string_match_ss (
    const char * const simple_string_a,
    const char * const simple_string_b,
    const IniFormat format
);


extern _Bool ini_string_match_si (
    const char * const simple_string,
    const char * const ini_string,
    const IniFormat format
);


extern _Bool ini_string_match_ii (
    const char * const ini_string_a,
    const char * const ini_string_b,
    const IniFormat format
);


extern _Bool ini_array_match (
    const char * const ini_string_a,
    const char * const ini_string_b,
    const char delimiter,
    const IniFormat format
);


extern size_t ini_unquote (
    char * const ini_string,
    const IniFormat format
);


extern size_t ini_string_parse (
    char * const ini_string,
    const IniFormat format
);


extern size_t ini_array_get_length (
    const char * const ini_string,
    const char delimiter,
    const IniFormat format
);


extern int ini_array_foreach (
    const char * const ini_string,
    const char delimiter,
    const IniFormat format,
    const IniSubstrHandler f_foreach,
    void * const user_data
);


extern size_t ini_array_shift (
    const char ** const ini_strptr,
    const char delimiter,
    const IniFormat format
);


extern size_t ini_array_collapse (
    char * const ini_string,
    const char delimiter,
    const IniFormat format
);


extern char * ini_array_break (
    char * const ini_string,
    const char delimiter,
    const IniFormat format
);


extern char * ini_array_release (
    char ** ini_strptr,
    const char delimiter,
    const IniFormat format
);


extern int ini_array_split (
    char * const ini_string,
    const char delimiter,
    const IniFormat format,
    const IniStrHandler f_foreach,
    void * const user_data
);


extern void ini_global_set_lowercase_mode (
    _Bool lowercase
);


extern void ini_global_set_implicit_value (
    char * const implicit_value,
    const size_t implicit_v_len
);


extern IniFormatNum ini_fton (
    const IniFormat format
);


extern IniFormat ini_ntof (
    IniFormatNum format_id
);


extern int ini_get_bool (
    const char * const ini_string,
    const int return_value
);



/*  PUBLIC LINKS  */


extern int (* const ini_get_int) (
    const char * ini_string
);


extern long int (* const ini_get_lint) (
    const char * ini_string
);


extern long long int (* const ini_get_llint) (
    const char * ini_string
);


extern double (* const ini_get_float) (
    const char * ini_string
);



/*  PUBLIC CONSTANTS AND VARIABLES  */


/**
    @brief  Error mask (flags not present in user-generated interruptions)
**/
#define CONFINI_ERROR 252


/**
    @brief  Error codes
**/
enum ConfiniInterruptNo {
    CONFINI_SUCCESS = 0,    /**< There have been no interruptions, everything
                                 went well [value=0] **/
    CONFINI_IINTR = 1,      /**< Interrupted by the user during `f_init()`
                                 [value=1] **/
    CONFINI_FEINTR = 2,     /**< Interrupted by the user during `f_foreach()`
                                 [value=2] **/
    CONFINI_ENOENT = 4,     /**< File inaccessible [value=4] **/
    CONFINI_ENOMEM = 5,     /**< Error allocating virtual memory [value=5] **/
    CONFINI_EIO = 6,        /**< Error reading the file [value=6] **/
    CONFINI_EOOR = 7        /**< Out-of-range error: callbacks are more than
                                 expected [value=7] **/
};


/**
    @brief  INI node types
**/
enum IniNodeType {
    INI_UNKNOWN = 0,            /**< Node impossible to categorize [value=0] **/
    INI_VALUE = 1,              /**< Not used here (values are dispatched
                                     together with keys) -- but available for
                                     user's implementations [value=1] **/
    INI_KEY = 2,                /**< Key [value=2] **/
    INI_SECTION = 3,            /**< Section path [value=3] **/
    INI_COMMENT = 4,            /**< Comment [value=4] **/
    INI_INLINE_COMMENT = 5,     /**< Inline comment [value=5] **/
    INI_DISABLED_KEY = 6,       /**< Disabled key [value=6] **/
    INI_DISABLED_SECTION = 7    /**< Disabled section path [value=7] **/
};


/**
    @brief  Most used key-value and array delimiters (but a delimiter may also
            be any other ASCII character)
**/
enum IniDelimiters {
    INI_ANY_SPACE = 0,  /**< In multi-line INIs:
                             `/(?:\\(?:\n\r?|\r\n?)|[\t \v\f])+/`, in
                             non-multi-line INIs: `/[\t \v\f])+/` **/
    INI_EQUALS = '=',   /**< Equals character (`=`) **/
    INI_COLON = ':',    /**< Colon character (`:`) **/
    INI_DOT = '.',      /**< Dot character (`.`) **/
    INI_COMMA = ','     /**< Comma character (`,`) **/
};


/**
    @brief  Possible values of #IniFormat::semicolon_marker and
            #IniFormat::hash_marker (i.e., meaning of `/\s+[#;]/` in respect to
            a format)
**/
enum IniCommentMarker {
    INI_DISABLED_OR_COMMENT = 0,    /**< This marker opens a comment or a
                                         disabled entry **/
    INI_ONLY_COMMENT = 1,           /**< This marker opens a comment **/
    INI_IGNORE = 2,                 /**< This marker opens a comment that must
                                         not be dispatched or counted **/
    INI_IS_NOT_A_MARKER = 3         /**< This is not a marker at all, but a
                                         normal character instead **/
};


/**
    @brief  Possible values of #IniFormat::section_paths
**/
enum IniSectionPaths {
    INI_ABSOLUTE_AND_RELATIVE = 0,  /**< Section paths starting with a dot
                                         express nesting to the current parent,
                                         to root otherwise **/
    INI_ABSOLUTE_ONLY = 1,          /**< Section paths starting with a dot will
                                         be cleaned of their leading dot and
                                         appended to root **/
    INI_ONE_LEVEL_ONLY = 2,         /**< Format supports sections, but the dot
                                         does not express nesting and is not a
                                         meta-character **/
    INI_NO_SECTIONS = 3             /**< Format does *not* support sections --
                                         `/\[[^\]]*\]/g`, if any, will be
                                         treated as keys! **/
};


/**
    @brief  Possible values of #IniFormat::multiline_nodes
**/
enum IniMultiline {
    INI_MULTILINE_EVERYWHERE = 0,       /**< Comments, section paths and keys --
                                             disabled or not -- are allowed to
                                             be multi-line **/
    INI_BUT_COMMENTS = 1,               /**< Only section paths and keys --
                                             disabled or not -- are allowed to
                                             be multi-line **/
    INI_BUT_DISABLED_AND_COMMENTS = 2,  /**< Only *active* section paths and
                                             *active* keys are allowed to be
                                             multi-line **/
    INI_NO_MULTILINE = 3                /**< Multi-line escape sequences are
                                             disabled **/
};


/**
    @brief  A model format for standard INI files
**/
static const IniFormat INI_DEFAULT_FORMAT = _LIBCONFINI_DEFAULT_FORMAT_;


/**
    @brief  A model format for Unix-like .conf files (space characters are
            delimiters between keys and values)
**/
/*  All properties are set to `0` here.  */
static const IniFormat INI_UNIXLIKE_FORMAT = _LIBCONFINI_UNIXLIKE_FORMAT_;


/**
    @brief  If set to `true`, key and section names in case-insensitive INI
            formats will be dispatched lowercase, verbatim otherwise (default
            value: `false`)
**/
extern _Bool INI_GLOBAL_LOWERCASE_MODE;


/**
    @brief  Value to be assigned to implicit keys (default value: `NULL`)
**/
extern char * INI_GLOBAL_IMPLICIT_VALUE;


/**
    @brief  Length of the value assigned to implicit keys -- this may be any
            unsigned number, independently of the real length of
            #INI_GLOBAL_IMPLICIT_VALUE (default value: `0`)
**/
extern size_t INI_GLOBAL_IMPLICIT_V_LEN;



/*  CLEAN THE PRIVATE ENVIRONMENT  */


#undef _LIBCONFINI_UNIXLIKE_FORMAT_ 
#undef _LIBCONFINI_DEFAULT_FORMAT_
#undef _LIBCONFINI_INIFORMAT_TYPE_
#undef _LIBCONFINI_UNIXLIKE_FMT_DESCR_
#undef _LIBCONFINI_DEFAULT_FMT_DESCR_
#undef _LIBCONFINI_INIFORMAT_DESCR_



/*  END OF `_LIBCONFINI_HEADER_`  */


#endif



/*  EOF  */

