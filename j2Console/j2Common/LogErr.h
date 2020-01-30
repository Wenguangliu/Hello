#ifndef __LOGERR_H_INCLUDED
#define __LOGERR_H_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#define         LogSem         L"/LogSemahpore"     // log file semaphore        
void LogError( const char * fmt, ... );
void LogWarning( const char * fmt, ... );
void LogInfo( const char * fmt, ... );
void LogDebug( const char * fmt, ... );
void LogRenameFiles (void);

#if defined(__cplusplus)
}
#endif

#endif	// __LOGERR_H_INCLUDED
