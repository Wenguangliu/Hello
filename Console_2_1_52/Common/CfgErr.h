#ifndef __CFGERR_H_INCLUDED
#define __CFGERR_H_INCLUDED

// Return type for the parse and write routines.
typedef enum
{
	CFG_ERR_NONE,
	CFG_ERR_FILE_DOES_NOT_EXIST,
	CFG_ERR_FILE_OPEN,
	CFG_ERR_FILE_READ,
	CFG_ERR_PARSER_CREATE,
	CFG_ERR_SYNTAX,
	CFG_ERR_CRC,
	CFG_ERR_UNSPECIFIED,
} CfgError_t;

#endif	// __CFGERR_H_INCLUDED
