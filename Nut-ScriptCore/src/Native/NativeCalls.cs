using System.Runtime.CompilerServices;

namespace Nut
{
    internal static class NativeCalls
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_Print(string text);
    }
}
