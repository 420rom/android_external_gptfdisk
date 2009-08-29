/* bsd.cc -- Functions for loading, saving, and manipulating legacy BSD disklabel
   data. */

/* By Rod Smith, August, 2009 */

/* This program is copyright (c) 2009 by Roderick W. Smith. It is distributed
  under the terms of the GNU GPL version 2, as detailed in the COPYING file. */

#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
//#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include "crc32.h"
#include "support.h"
#include "bsd.h"

using namespace std;


BSDData::BSDData(void) {
   state = unknown;
   signature = UINT32_C(0);
   signature2 = UINT32_C(0);
   sectorSize = 512;
   numParts = 0;
   labelFirstLBA = 0;
   labelLastLBA = 0;
   labelStart = LABEL_OFFSET1; // assume raw disk format
//   deviceFilename[0] = '\0';
   partitions = NULL;
} // default constructor

BSDData::~BSDData(void) {
   free(partitions);
} // destructor

int BSDData::ReadBSDData(char* device, uint64_t startSector, uint64_t endSector) {
   int fd, allOK = 1;

   if ((fd = open(device, O_RDONLY)) != -1) {
      ReadBSDData(fd, startSector, endSector);
   } else {
      allOK = 0;
   } // if

   close(fd);

//   if (allOK)
//      strcpy(deviceFilename, device);

   return allOK;
} // BSDData::ReadBSDData() (device filename version)

// Load the BSD disklabel data from an already-opened disk
// file, starting with the specified sector number.
void BSDData::ReadBSDData(int fd, uint64_t startSector, uint64_t endSector) {
   uint8_t buffer[2048]; // I/O buffer
   uint64_t startByte;
   int i, err, foundSig = 0, bigEnd = 0;
   int relative = 0; // assume absolute partition sector numbering
   uint32_t realSig;
   uint32_t* temp32;
   uint16_t* temp16;
   BSDRecord* tempRecords;

   labelFirstLBA = startSector;
   labelLastLBA = endSector;

   // Read two sectors into memory; we'll extract data from
   // this buffer. (Done to work around FreeBSD limitation)
   lseek64(fd, startSector * 512, SEEK_SET);
   err = read(fd, buffer, 2048);

   // Do some strangeness to support big-endian architectures...
   bigEnd = (IsLittleEndian() == 0);
   realSig = BSD_SIGNATURE;
   if (bigEnd)
      ReverseBytes(&realSig, 4);

   // Look for the signature at one of two locations
   labelStart = LABEL_OFFSET1;
   temp32 = (uint32_t*) &buffer[labelStart];
   signature = *temp32;
   if (signature == realSig) {
      temp32 = (uint32_t*) &buffer[labelStart + 132];
      signature2 = *temp32;
      if (signature2 == realSig)
         foundSig = 1;
   } // if/else
   if (!foundSig) { // look in second location
      labelStart = LABEL_OFFSET2;
      temp32 = (uint32_t*) &buffer[labelStart];
      signature = *temp32;
      if (signature == realSig) {
         temp32 = (uint32_t*) &buffer[labelStart + 132];
         signature2 = *temp32;
         if (signature2 == realSig)
            foundSig = 1;
      } // if/else
   } // if

   // Load partition metadata from the buffer....
   temp32 = (uint32_t*) &buffer[labelStart + 40];
   sectorSize = *temp32;
   temp16 = (uint16_t*) &buffer[labelStart + 138];
   numParts = *temp16;

   // Make it big-endian-aware....
   if (IsLittleEndian() == 0)
      ReverseMetaBytes();

   // Check validity of the data and flag it appropriately....
   if (foundSig && (numParts <= MAX_BSD_PARTS)) {
      state = bsd;
   } else {
      state = bsd_invalid;
   } // if/else

   // If the state is good, go ahead and load the main partition data....
   if (state == bsd) {
      partitions = (struct BSDRecord*) malloc(numParts * sizeof (struct BSDRecord));
      for (i = 0; i < numParts; i++) {
         // Once again, we use the buffer, but index it using a BSDRecord
         // pointer (dangerous, but effective)....
         tempRecords = (BSDRecord*) &buffer[labelStart + 148];
         partitions[i].lengthLBA = tempRecords[i].lengthLBA;
         partitions[i].firstLBA = tempRecords[i].firstLBA;
         partitions[i].fsType = tempRecords[i].fsType;
         if (bigEnd) { // reverse data (fsType is a single byte)
            ReverseBytes(&partitions[i].lengthLBA, 4);
            ReverseBytes(&partitions[i].firstLBA, 4);
         } // if big-endian
         // Check for signs of relative sector numbering: A "0" first sector
         // number on a partition with a non-zero length -- but ONLY if the
         // length is less than the disk size, since NetBSD has a habit of
         // creating a disk-sized partition within a carrier MBR partition
         // that's too small to house it, and this throws off everything....
         if ((partitions[i].firstLBA == 0) && (partitions[i].lengthLBA > 0)
             && (partitions[i].lengthLBA < labelLastLBA))
            relative = 1;
      } // for
      // Some disklabels use sector numbers relative to the enclosing partition's
      // start, others use absolute sector numbers. If relative numbering was
      // detected above, apply a correction to all partition start sectors....
      if (relative) {
         for (i = 0; i < numParts; i++) {
            partitions[i].firstLBA += startSector;
         } // for
      } // if
   } // if signatures OK
//   DisplayBSDData();
} // BSDData::ReadBSDData(int fd, uint64_t startSector)

// Reverse metadata's byte order; called only on big-endian systems
void BSDData::ReverseMetaBytes(void) {
   ReverseBytes(&signature, 4);
   ReverseBytes(&sectorSize, 4);
   ReverseBytes(&signature2, 4);
   ReverseBytes(&numParts, 2);
} // BSDData::ReverseMetaByteOrder()

// Display basic BSD partition data. Used for debugging.
void BSDData::DisplayBSDData(void) {
   int i;

   if (state == bsd) {
      printf("BSD partitions:\n");
      printf("Number\t Start (sector)\t Length (sectors)\tType\n");
      for (i = 0; i < numParts; i++) {
         printf("%4d\t%13lu\t%15lu \t0x%02X\n", i + 1,
                (unsigned long) partitions[i].firstLBA,
                (unsigned long) partitions[i].lengthLBA, partitions[i].fsType);
      } // for
   } // if
} // BSDData::DisplayBSDData()

// Displays the BSD disklabel state. Called during program launch to inform
// the user about the partition table(s) status
int BSDData::ShowState(void) {
   int retval = 0;

   switch (state) {
      case bsd_invalid:
         printf("  BSD: not present\n");
         break;
      case bsd:
         printf("  BSD: present\n");
         retval = 1;
         break;
      default:
         printf("\a  BSD: unknown -- bug!\n");
         break;
   } // switch
   return retval;
} // BSDData::ShowState()

// Returns the BSD table's partition type code
uint8_t BSDData::GetType(int i) {
   uint8_t retval = 0; // 0 = "unused"

   if ((i < numParts) && (i >= 0) && (state == bsd) && (partitions != 0))
      retval = partitions[i].fsType;

   return(retval);
} // BSDData::GetType()

// Returns the number of the first sector of the specified partition
uint64_t BSDData::GetFirstSector(int i) {
   uint64_t retval = UINT64_C(0);

   if ((i < numParts) && (i >= 0) && (state == bsd) && (partitions != 0))
      retval = (uint64_t) partitions[i].firstLBA;

   return retval;
} // BSDData::GetFirstSector

// Returns the length (in sectors) of the specified partition
uint64_t BSDData::GetLength(int i) {
   uint64_t retval = UINT64_C(0);

   if ((i < numParts) && (i >= 0) && (state == bsd) && (partitions != 0))
      retval = (uint64_t) partitions[i].lengthLBA;

   return retval;
} // BSDData::GetLength()

// Returns the number of partitions defined in the current table
int BSDData::GetNumParts(void) {
   return numParts;
} // BSDData::GetNumParts()

// Returns the specified partition as a GPT partition. Used in BSD-to-GPT
// conversion process
GPTPart BSDData::AsGPT(int i) {
   GPTPart guid;                  // dump data in here, then return it
   uint64_t sectorOne, sectorEnd; // first & last sectors of partition
   char tempStr[NAME_SIZE];       // temporary string for holding GPT name
   int passItOn = 1;              // Set to 0 if partition is empty or invalid

   guid.BlankPartition();
   sectorOne = (uint64_t) partitions[i].firstLBA;
   sectorEnd = sectorOne + (uint64_t) partitions[i].lengthLBA;
   if (sectorEnd > 0) sectorEnd--;
   // Note on above: BSD partitions sometimes have a length of 0 and a start
   // sector of 0. With unsigned ints, the usual (start + length - 1) to
   // find the end will result in a huge number, which will be confusing

   // Do a few sanity checks on the partition before we pass it on....
   // First, check that it falls within the bounds of its container
   // and that it starts before it ends....
   if ((sectorOne < labelFirstLBA) || (sectorEnd > labelLastLBA) || (sectorOne > sectorEnd))
      passItOn = 0;
   // Some disklabels include a pseudo-partition that's the size of the entire
   // disk or containing partition. Don't return it.
   if ((sectorOne <= labelFirstLBA) && (sectorEnd >= labelLastLBA) &&
       (GetType(i) == 0))
      passItOn = 0;
   // If the end point is 0, it's not a valid partition.
   if (sectorEnd == 0)
      passItOn = 0;

   if (passItOn) {
      guid.SetFirstLBA(sectorOne);
      guid.SetLastLBA(sectorEnd);
      // Now set a random unique GUID for the partition....
      guid.SetUniqueGUID(1);
      // ... zero out the attributes and name fields....
      guid.SetAttributes(UINT64_C(0));
      // Most BSD disklabel type codes seem to be archaic or rare.
      // They're also ambiguous; a FreeBSD filesystem is impossible
      // to distinguish from a NetBSD one. Thus, these code assignment
      // are going to be rough to begin with. For a list of meanings,
      // see http://fxr.watson.org/fxr/source/sys/dtype.h?v=DFBSD,
      // or Google it.
      switch (GetType(i)) {
         case 1: // BSD swap
            guid.SetType(0xa502); break;
         case 7: // BSD FFS
            guid.SetType(0xa503); break;
         case 8: case 11: // MS-DOS or HPFS
            guid.SetType(0x0700); break;
         case 9: // log-structured fs
            guid.SetType(0xa903); break;
         case 13: // bootstrap
            guid.SetType(0xa501); break;
         case 14: // vinum
            guid.SetType(0xa505); break;
         case 15: // RAID
            guid.SetType(0xa903); break;
         case 27: // FreeBSD ZFS
            guid.SetType(0xa504); break;
         default:
            guid.SetType(0x0700); break;
      } // switch
      // Set the partition name to the name of the type code....
      guid.SetName((unsigned char*) guid.GetNameType(tempStr));
   } // if
   return guid;
} // BSDData::AsGPT()
