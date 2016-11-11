/*
	---------------------------------------------------------------------------
	Copyright (c) 2003, Dominik Reichl <dominik.reichl@t-online.de>, Germany.
	All rights reserved.

	Distributed under the terms of the GNU General Public License v2.

	This software is provided 'as is' with no explicit or implied warranties
	in respect of its properties, including, but not limited to, correctness 
	and/or fitness for purpose.
	---------------------------------------------------------------------------
*/

#ifndef ___CRC32_H___
#define ___CRC32_H___

void crc32Init(unsigned long *pCrc32);
void crc32Update(unsigned long *pCrc32, unsigned char *pData, unsigned long uSize);
void crc32Finish(unsigned long *pCrc32);

#endif /* ___CRC32_H___ */
