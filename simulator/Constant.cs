using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MESI
{
    public struct blockingInfo
    {
        public int processorId; public CacheAccessResult result; public MESI_States processorState;
    }

       public enum Label   //The label from the PRG File
    {
            Fetch, Read, Write, SnoopInvalidate, SnoopedRead, SnoopedWrite, SnoopedReadToModify, ClearResetCache, TraceCache
    }

       public enum MESI_States
       {
           Modified, Exclusive, Shared, Invalid, Forward
       }

       public enum BusSignals
       {
           BusRd, BusRdX, BusInvalidate, NoSignal
       }

       public enum CacheAccessResult
       {
           ReadHit, ReadMiss, WriteHit, WriteMiss
       }

    static class Constants
    {
        public const int PARAMS = 5;
        public const int addressLenght = 32;  //32 bit memory address
        public const int memoryToCache_cycles = 10;
        public const string filePath = @"./benchmarks/";
    }
}
