#ifndef _FTDI_H_
#define _FTDI_H_

#ifdef __cplusplus
extern "C" {
#endif

extern int
InitialEngineBoardInterface (void);

extern int
ReBootEngineBoard (void);

extern int
EngineBoardReset (void);


#ifdef __cplusplus
}
#endif

#endif
