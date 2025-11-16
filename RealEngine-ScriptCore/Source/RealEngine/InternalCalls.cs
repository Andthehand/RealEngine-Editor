using Coral.Managed.Interop;

#pragma warning disable 0649
namespace RealEngine {
    public static class InternalCalls {
        //Logging
        internal static unsafe delegate*<LogLevel, NativeString, void> NativeLog;
        //Logging
    }
}
#pragma warning restore 0649
