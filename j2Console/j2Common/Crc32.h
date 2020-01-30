#ifndef __CRC32_H_INCLUDED
#define __CRC32_H_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

unsigned long	Crc32Init( void );
unsigned long	Crc32Update( unsigned long crc, const unsigned char * buffer, size_t size );
unsigned long	Crc32Finalize( unsigned long crc );

#if defined(__cplusplus)
}
#endif

#endif	// __CRC32_H_INCLUDED
