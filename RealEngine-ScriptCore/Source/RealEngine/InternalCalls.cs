using Coral.Managed.Interop;
using System.Numerics;

#pragma warning disable 0649
namespace RealEngine {
    public static class InternalCalls {
        //Logging
        internal static unsafe delegate*<LogLevel, NativeString, void> NativeLog;
        //Logging

        //Entity
        internal static unsafe delegate*<ulong, ReflectionType, bool> Entity_HasComponent;
        internal static unsafe delegate*<ulong, ReflectionType, void*> Entity_GetComponent;
        //Entity
    }
}
#pragma warning restore 0649
