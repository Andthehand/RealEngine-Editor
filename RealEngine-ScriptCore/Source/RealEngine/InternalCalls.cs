using Coral.Managed.Interop;

namespace RealEngine
{
    public static class InternalCalls
    {
        //Logging
        internal static unsafe delegate*<NativeString, void> NativeLog;
        //Logging
    }
}