// support.cc
// Non-class support functions for gdisk program.
// Primarily by Rod Smith, February 2009, but with a few functions
// copied from other sources (see attributions below).

/* This program is copyright (c) 2009 by Roderick W. Smith. It is distributed
  under the terms of the GNU GPL version 2, as detailed in the COPYING file. */

#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <sstream>
#include "support.h"

#include <sys/types.h>

// As of 1/2010, BLKPBSZGET is very new, so I'm explicitly defining it if
// it's not already defined. This should become unnecessary in the future.
// Note that this is a Linux-only ioctl....
#ifndef BLKPBSZGET
#define BLKPBSZGET _IO(0x12,123)
#endif

using namespace std;

void ReadCString(char *inStr, int numchars) {
   if (!fgets(inStr, numchars, stdin)) {
      cerr << "Critical error! Failed fgets() in ReadCString()\n";
      exit(1);
   } // if
} // ReadCString()

// Get a numeric value from the user, between low and high (inclusive).
// Keeps looping until the user enters a value within that range.
// If user provides no input, def (default value) is returned.
// (If def is outside of the low-high range, an explicit response
// is required.)
int GetNumber(int low, int high, int def, const string & prompt) {
   int response, num;
   char line[255];

   if (low != high) { // bother only if low and high differ...
      do {
         cout << prompt;
         cin.getline(line, 255);
         num = sscanf(line, "%d", &response);
         if (num == 1) { // user provided a response
            if ((response < low) || (response > high))
               cout << "Value out of range\n";
         } else { // user hit enter; return default
            response = def;
         } // if/else
      } while ((response < low) || (response > high));
   } else { // low == high, so return this value
      cout << "Using " << low << "\n";
      response = low;
   } // else
   return (response);
} // GetNumber()

// Gets a Y/N response (and converts lowercase to uppercase)
char GetYN(void) {
   char line[255];
   char response;

   do {
      cout << "(Y/N): ";
      ReadCString(line, sizeof(line));
      response = toupper(line[0]);
   } while ((response != 'Y') && (response != 'N'));
   return response;
} // GetYN(void)

// Obtains a sector number, between low and high, from the
// user, accepting values prefixed by "+" to add sectors to low,
// or the same with "K", "M", "G", "T", or "P" as suffixes to add
// kilobytes, megabytes, gigabytes, terabytes, or petabytes,
// respectively. If a "-" prefix is used, use the high value minus
// the user-specified number of sectors (or KiB, MiB, etc.). Use the
// def value as the default if the user just hits Enter. The sSize is
// the sector size of the device.
uint64_t GetSectorNum(uint64_t low, uint64_t high, uint64_t def, uint64_t sSize,
                      const string & prompt) {
   uint64_t response;
   char line[255];

   do {
      cout << prompt;
      cin.getline(line, 255);
      response = IeeeToInt(line, sSize, low, high, def);
   } while ((response < low) || (response > high));
   return response;
} // GetSectorNum()

// Convert an IEEE-1541-2002 value (K, M, G, T, P, or E) to its equivalent in
// number of sectors. If no units are appended, interprets as the number
// of sectors; otherwise, interprets as number of specified units and
// converts to sectors. For instance, with 512-byte sectors, "1K" converts
// to 2. If value includes a "+", adds low and subtracts 1; if SIValue
// inclues a "-", subtracts from high. If IeeeValue is empty, returns def.
// Returns integral sector value.
uint64_t IeeeToInt(string inValue, uint64_t sSize, uint64_t low, uint64_t high, uint64_t def) {
   uint64_t response = def, bytesPerUnit = 1, mult = 1, divide = 1;
   size_t foundAt = 0;
   char suffix, plusFlag = ' ';
   string suffixes = "KMGTPE";

   if (sSize == 0) {
      sSize = SECTOR_SIZE;
      cerr << "Bug: Sector size invalid in SIToInt()!\n";
   } // if

   // Remove leading spaces, if present
   while (inValue[0] == ' ')
      inValue.erase(0, 1);

   // If present, flag and remove leading plus or minus sign
   if ((inValue[0] == '+') || (inValue[0] == '-')) {
      plusFlag = inValue[0];
      inValue.erase(0, 1);
   } // if

   // Extract numeric response and, if present, suffix
   istringstream inString(inValue);
   if (((inString.peek() >= '0') && (inString.peek() <= '9')) || (inString.peek() == -1)) {
      inString >> response >> suffix;
      suffix = toupper(suffix);
      
      // If no response, or if response == 0, use default (def)
      if ((inValue.length() == 0) || (response == 0)) {
         response = def;
         suffix = ' ';
         plusFlag = 0;
      } // if
      
      // Find multiplication and division factors for the suffix
      foundAt = suffixes.find(suffix);
      if (foundAt != string::npos) {
         bytesPerUnit = UINT64_C(1) << (10 * (foundAt + 1));
         mult = bytesPerUnit / sSize;
         divide = sSize / bytesPerUnit;
      } // if
      
      // Adjust response based on multiplier and plus flag, if present
      if (mult > 1)
         response *= mult;
      else if (divide > 1)
         response /= divide;
      if (plusFlag == '+') {
         // Recompute response based on low part of range (if default = high
         // value, which should be the case when prompting for the end of a
         // range) or the defaut value (if default != high, which should be
         // the case for the first sector of a partition).
         if (def == high)
            response = response + low - UINT64_C(1);
         else
            response = response + def;
      } else if (plusFlag == '-') {
         response = high - response;
      } // if   
   } else { // user input is invalid
      response = high + UINT64_C(1);
   } // if/else

   return response;
} // IeeeToInt()

// Takes a size and converts this to a size in IEEE-1541-2002 units (KiB, MiB,
// GiB, TiB, PiB, or EiB), returned in C++ string form. The size is either in
// units of the sector size or, if that parameter is omitted, in bytes.
// (sectorSize defaults to 1).
string BytesToIeee(uint64_t size, uint32_t sectorSize) {
   float sizeInIeee;
   uint index = 0;
   string units, prefixes = " KMGTPE";
   ostringstream theValue;

   sizeInIeee = size * (float) sectorSize;
   while ((sizeInIeee > 1024.0) && (index < (prefixes.length() - 1))) {
      index++;
      sizeInIeee /= 1024.0;
   } // while
   theValue.setf(ios::fixed);
   if (prefixes[index] == ' ') {
      units = " bytes";
      theValue.precision(0);
   } else {
      units = "  iB";
      units[1] = prefixes[index];
      theValue.precision(1);
   } // if/else
   theValue << sizeInIeee << units;
   return theValue.str();
} // BlocksToIeee()

// Converts two consecutive characters in the input string into a
// number, interpreting the string as a hexadecimal number, starting
// at the specified position.
unsigned char StrToHex(const string & input, unsigned int position) {
   unsigned char retval = 0x00;
   unsigned int temp;

   if (input.length() >= (position + 2)) {
      sscanf(input.substr(position, 2).c_str(), "%x", &temp);
      retval = (unsigned char) temp;
   } // if
   return retval;
} // StrToHex()

// Returns 1 if input can be interpreted as a hexadecimal number --
// all characters must be spaces, digits, or letters A-F (upper- or
// lower-case), with at least one valid hexadecimal digit; otherwise
// returns 0.
int IsHex(const string & input) {
   int isHex = 1, foundHex = 0, i;

   for (i = 0; i < (int) input.length(); i++) {
      if ((input[i] < '0') || (input[i] > '9')) {
         if ((input[i] < 'A') || (input[i] > 'F')) {
            if ((input[i] < 'a') || (input[i] > 'f')) {
               if ((input[i] != ' ') && (input[i] != '\n')) {
                  isHex = 0;
               }
            } else foundHex = 1;
         } else foundHex = 1;
      } else foundHex = 1;
   } // for
   if (!foundHex)
      isHex = 0;
   return isHex;
} // IsHex()

// Return 1 if the CPU architecture is little endian, 0 if it's big endian....
int IsLittleEndian(void) {
   int littleE = 1; // assume little-endian (Intel-style)
   union {
      uint32_t num;
      unsigned char uc[sizeof(uint32_t)];
   } endian;

   endian.num = 1;
   if (endian.uc[0] != (unsigned char) 1) {
      littleE = 0;
   } // if
   return (littleE);
} // IsLittleEndian()

// Reverse the byte order of theValue; numBytes is number of bytes
void ReverseBytes(void* theValue, int numBytes) {
   char* tempValue = NULL;
   int i;

   tempValue = new char [numBytes];
   if (tempValue != NULL) {
      memcpy(tempValue, theValue, numBytes);
      for (i = 0; i < numBytes; i++)
         ((char*) theValue)[i] = tempValue[numBytes - i - 1];
      delete[] tempValue;
   } // if
} // ReverseBytes()

// Extract integer data from argument string, which should be colon-delimited
uint64_t GetInt(const string & argument, int itemNum) {
   uint64_t retval;

   istringstream inString(GetString(argument, itemNum));
   inString >> retval;
   return retval;
} // GetInt()

// Extract string data from argument string, which should be colon-delimited
// If string begins with a colon, that colon is skipped in the counting. If an
// invalid itemNum is specified, returns an empty string.
string GetString(string argument, int itemNum) {
   size_t startPos = 0, endPos = 0;
   string retVal = "";
   int foundLast = 0;
   int numFound = 0;

   if (argument[0] == ':')
      argument.erase(0, 1);
   while ((numFound < itemNum) && (!foundLast)) {
      endPos = argument.find(':', startPos);
      numFound++;
      if (endPos == string::npos) {
         foundLast = 1;
         endPos = argument.length();
      } else if (numFound < itemNum) {
         startPos = endPos + 1;
      } // if/elseif
   } // while
   if ((numFound == itemNum) && (numFound > 0))
      retVal = argument.substr(startPos, endPos - startPos);

   return retVal;
} // GetString()
