using static Nut.InternalCalls;

namespace Nut.Core
{
    internal enum LogLevel
    {
        None = 0,
        Trace = 1,
        Info = 2,
        Warn = 3,
        Error = 4,
        Fatal = 5
    }

    public static class Logger
    {
        public static void Log(string text) => Trace(text);
        public static void Trace(string text) => Native_Print(text, LogLevel.Trace);
        public static void Info(string text) => Native_Print(text, LogLevel.Info);
        public static void Warn(string text) => Native_Print(text, LogLevel.Warn);
        public static void Error(string text) => Native_Print(text, LogLevel.Error);
        public static void Fatal(string text) => Native_Print(text, LogLevel.Fatal);
    }
}
