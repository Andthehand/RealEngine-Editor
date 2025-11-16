namespace RealEngine {
    public enum LogLevel {
        Trace       = 0,
        Info        = 1,
        Warn        = 2,
        Error       = 3,
        Critical    = 4
    }

    public static class Logger {
        public static void Log(LogLevel level, string message) {
            unsafe { InternalCalls.NativeLog(level, message); }
        }

        public static void Trace(string message) => Log(LogLevel.Trace, message);
        public static void Info(string message) => Log(LogLevel.Info, message);
        public static void Warn(string message) => Log(LogLevel.Warn, message);
        public static void Error(string message) => Log(LogLevel.Error, message);
        public static void Critical(string message) => Log(LogLevel.Critical, message);

    }
}
