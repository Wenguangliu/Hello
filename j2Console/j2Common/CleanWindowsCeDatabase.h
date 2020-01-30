#ifndef _CLEAN_WINDOWS_CE_DATABASE_H_
#define _CLEAN_WINDOWS_CE_DATABASE_H_

extern void
OutputNotifications (void);
extern void
DisableEventNotifications (void);
extern void
CleanDatabaseEvents (FactoryData_t *fData, bool bootup);
extern int
GetDataBaseFileSize (FactoryData_t *fData, bool Bootup, unsigned long maxAllowedSize);
extern void
CleanDatabaseAndReboot ();

#endif
