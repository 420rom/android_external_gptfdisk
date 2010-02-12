// sgdisk.cc
// Command-line-based version of gdisk. This program is named after sfdisk,
// and it can serve a similar role (easily scripted, etc.), but it's used
// strictly via command-line arguments, and it doesn't bear much resemblance
// to sfdisk in actual use.
//
// by Rod Smith, project began February 2009; sgdisk begun January 2010.

/* This program is copyright (c) 2009, 2010 by Roderick W. Smith. It is distributed
  under the terms of the GNU GPL version 2, as detailed in the COPYING file. */

#include <stdio.h>
#include <string>
#include <popt.h>
#include <errno.h>
#include <iostream>
#include "mbr.h"
#include "gpt.h"
#include "support.h"
#include "parttypes.h"

using namespace std;

#define MAX_OPTIONS 50

uint64_t GetInt(char* Info, int itemNum);
string GetString(char* Info, int itemNum);

int main(int argc, char *argv[]) {
   GPTData theGPT;
   int opt, numOptions = 0, saveData = 0, neverSaveData = 0;
   int partNum = 0, deletePartNum = 0, infoPartNum = 0, bsdPartNum = 0, saveNonGPT = 1;
   int alignment = 8, retval = 0, pretend = 0;
   unsigned int hexCode;
   uint32_t tableSize = 128;
   uint64_t startSector, endSector;
   char *device = NULL;
   char *newPartInfo = NULL, *typeCode = NULL, *partName;
   char *backupFile = NULL;
   PartType typeHelper;

   poptContext poptCon;
   struct poptOption theOptions[] =
   {
      {"set-alignment", 'a', POPT_ARG_INT, &alignment, 'a', "set sector alignment", "value"},
      {"backup", 'b', POPT_ARG_STRING, &backupFile, 'b', "backup GPT to file", "file"},
      {"change-name", 'c', POPT_ARG_STRING, &partName, 'c', "change partition's name", "partnum:name"},
      {"delete", 'd', POPT_ARG_INT, &deletePartNum, 'd', "delete a partition", "partnum"},
      {"move-second-header", 'e', POPT_ARG_NONE, NULL, 'e', "move second header to end of disk", ""},
      {"end-of-largest", 'E', POPT_ARG_NONE, NULL, 'E', "show end of largest free block", ""},
      {"first-in-largest", 'f', POPT_ARG_NONE, NULL, 'f', "show start of the largest free block", ""},
      {"mbrtogpt", 'g', POPT_ARG_NONE, NULL, 'g', "convert MBR to GPT", ""},
      {"info", 'i', POPT_ARG_INT, &infoPartNum, 'i', "show detailed information on partition", "partnum"},
      {"load-backup", 'l', POPT_ARG_STRING, &backupFile, 'l', "load GPT backup from file", "file"},
      {"list-types", 'L', POPT_ARG_NONE, NULL, 'L', "list known partition types", ""},
      {"new", 'n', POPT_ARG_STRING, &newPartInfo, 'n', "create new partition", "partnum:start:end"},
      {"clear", 'o', POPT_ARG_NONE, NULL, 'o', "clear partition table", ""},
      {"print", 'p', POPT_ARG_NONE, NULL, 'p', "print partition table", ""},
      {"pretend", 'P', POPT_ARG_NONE, NULL, 'P', "make changes in memory, but don't write them", ""},
      {"sort", 's', POPT_ARG_NONE, NULL, 's', "sort partition table entries", ""},
      {"resize-table", 'S', POPT_ARG_INT, &tableSize, 'S', "resize partition table", "numparts"},
      {"typecode", 't', POPT_ARG_STRING, &typeCode, 't', "change partition type code", "partnum:hexcode"},
      {"transform-bsd", 'T', POPT_ARG_INT, &bsdPartNum, 'T', "transform BSD disklabel partition to GPT", "partnum"},
      {"verify", 'v', POPT_ARG_NONE, NULL, 'v', "check partition table integrity", ""},
      {"version", 'V', POPT_ARG_NONE, NULL, 'V', "display version information", ""},
      {"zap", 'z', POPT_ARG_NONE, NULL, 'z', "zap (destroy) GPT data structures", ""},
      POPT_AUTOHELP { NULL, 0, 0, NULL, 0 }
   };

   // Create popt context...
   poptCon = poptGetContext(NULL, argc, (const char**) argv, theOptions, 0);

   poptSetOtherOptionHelp(poptCon, " [OPTION...] <device>");

   if (argc < 2) {
      poptPrintUsage(poptCon, stderr, 0);
      exit(1);
   }

   // Do one loop through the options to find the device filename and deal
   // with options that don't require a device filename....
   while ((opt = poptGetNextOpt(poptCon)) > 0) {
      switch (opt) {
         case 'L':
            typeHelper.ShowAllTypes();
            break;
         case 'P':
            pretend = 1;
            break;
         case 'V':
            cout << "GPT fdisk (sgdisk) version " << GPTFDISK_VERSION << "\n\n";
            break;
         default:
            break;
      } // switch
      numOptions++;
   } // while

   // Assume first non-option argument is the device filename....
   device = (char*) poptGetArg(poptCon);
   poptResetContext(poptCon);

   if (device != NULL) {
      theGPT.JustLooking(); // reset as necessary
      theGPT.BeQuiet(); // Tell called functions to be less verbose & interactive
      if (theGPT.LoadPartitions((string) device)) {
         if ((theGPT.WhichWasUsed() == use_mbr) || (theGPT.WhichWasUsed() == use_bsd))
            saveNonGPT = 0; // flag so we don't overwrite unless directed to do so
         while ((opt = poptGetNextOpt(poptCon)) > 0) {
            switch (opt) {
               case 'a':
                  theGPT.SetAlignment(alignment);
                  break;
               case 'b':
                  theGPT.SaveGPTBackup(backupFile);
                  free(backupFile);
                  break;
               case 'c':
                  theGPT.JustLooking(0);
                  partNum = (int) GetInt(partName, 1) - 1;
                  if (theGPT.SetName(partNum, GetString(partName, 2))) {
                     saveData = 1;
                  } else {
                     cerr << "Unable set set partition " << partNum + 1
                          << "'s name to '" << GetString(partName, 2) << "'!\n";
                     neverSaveData = 1;
                  } // if/else
                  free(partName);
                  break;
               case 'd':
                  theGPT.JustLooking(0);
                  if (theGPT.DeletePartition(deletePartNum - 1) == 0) {
                     cerr << "Error " << errno << " deleting partition!\n";
                     neverSaveData = 1;
                  } else saveData = 1;
                  break;
               case 'e':
                  theGPT.JustLooking(0);
                  theGPT.MoveSecondHeaderToEnd();
                  saveData = 1;
                  break;
               case 'E':
                  cout << theGPT.FindLastInFree(theGPT.FindFirstInLargest()) << "\n";
                  break;
               case 'f':
                  cout << theGPT.FindFirstInLargest() << "\n";
                  break;
               case 'g':
                  theGPT.JustLooking(0);
                  saveData = 1;
                  saveNonGPT = 1;
                  break;
               case 'i':
                  theGPT.ShowPartDetails(infoPartNum - 1);
                  break;
               case 'l':
                  if (theGPT.LoadGPTBackup((string) backupFile) == 1)
                     saveData = 1;
                  else {
                     saveData = 0;
                     neverSaveData = 1;
                     cerr << "Error loading backup file!\n";
                  } // else
                  free(backupFile);
                  break;
               case 'L':
                  break;
               case 'n':
                  theGPT.JustLooking(0);
                  partNum = (int) GetInt(newPartInfo, 1) - 1;
                  startSector = GetInt(newPartInfo, 2);
                  endSector = GetInt(newPartInfo, 3);
                  if (theGPT.CreatePartition(partNum, startSector, endSector)) {
                     saveData = 1;
                  } else {
                     cerr << "Could not create partition " << partNum << " from "
                          << startSector << " to " << endSector << "\n";
                     neverSaveData = 1;
                  } // if/else
                  free(newPartInfo);
                  break;
               case 'o':
                  theGPT.JustLooking(0);
                  theGPT.ClearGPTData();
                  saveData = 1;
                  break;
               case 'p':
                  theGPT.DisplayGPTData();
                  break;
               case 'P':
                  pretend = 1;
                  break;
               case 's':
                  theGPT.JustLooking(0);
                  theGPT.SortGPT();
                  saveData = 1;
                  break;
               case 'S':
                  theGPT.JustLooking(0);
                  if (theGPT.SetGPTSize(tableSize) == 0)
                     neverSaveData = 1;
                  else
                     saveData = 1;
                  break;
               case 't':
                  theGPT.JustLooking(0);
                  partNum = (int) GetInt(typeCode, 1) - 1;
                  sscanf(GetString(typeCode, 2).c_str(), "%x", &hexCode);
                  if (theGPT.ChangePartType(partNum, hexCode)) {
                     saveData = 1;
                  } else {
                     cerr << "Could not change partition " << partNum + 1
                          << "'s type code to " << hex << hexCode << "!\n" << dec;
                     neverSaveData = 1;
                  } // if/else
                  free(typeCode);
                  break;
               case 'T':
                  theGPT.JustLooking(0);
                  theGPT.XFormDisklabel(bsdPartNum);
                  saveData = 1;
                  break;
               case 'v':
                  theGPT.Verify();
                  break;
               case 'z':
                  if (!pretend)
                     theGPT.DestroyGPT(-1);
                  saveNonGPT = 0;
                  break;
               default:
                  cerr << "Unknown option (-" << opt << ")!\n";
                  break;
            } // switch
         } // while
         if ((saveData) && (!neverSaveData) && (saveNonGPT) && (!pretend))
            theGPT.SaveGPTData(1);
         if (saveData && (!saveNonGPT)) {
            cout << "Non-GPT disk; not saving changes. Use -g to override.\n";
            retval = 3;
         } // if
         if (neverSaveData) {
            cerr << "Error encountered; not saving changes.\n";
            retval = 4;
         } // if
      } else { // if loaded OK
         retval = 2;
      } // if/else loaded OK
   } // if (device != NULL)
   poptFreeContext(poptCon);

   return retval;
} // main

// Extract integer data from argument string, which should be colon-delimited
uint64_t GetInt(char* argument, int itemNum) {
   int startPos = -1, endPos = -1;
   unsigned long long retval = 0;
   string Info;

   Info = argument;
   while (itemNum-- > 0) {
      startPos = endPos + 1;
      endPos = Info.find(':', startPos);
   }
   if (endPos == (int) string::npos)
      endPos = Info.length();
   endPos--;

   sscanf(Info.substr(startPos, endPos - startPos + 1).c_str(), "%llu", &retval);
   return retval;
} // GetInt()

// Extract string data from argument string, which should be colon-delimited
string GetString(char* argument, int itemNum) {
   int startPos = -1, endPos = -1;
   string Info;

   Info = argument;
   while (itemNum-- > 0) {
      startPos = endPos + 1;
      endPos = Info.find(':', startPos);
   }
   if (endPos == (int) string::npos)
      endPos = Info.length();
   endPos--;

   return Info.substr(startPos, endPos - startPos + 1);
} // GetString()
