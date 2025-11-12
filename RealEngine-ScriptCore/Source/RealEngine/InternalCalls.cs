using Coral.Managed.Interop;

namespace RealEngine
{
    public static class InternalCalls
    {
        //Logging
        private static unsafe delegate*<NativeString, void> NativeLog;
        //Logging

        public static void Log(string message)
        {
            unsafe { NativeLog(message); }
        }
    }
}