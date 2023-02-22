using Nut.Math;
using Nut.Input;
using System;
using System.Runtime.CompilerServices;

namespace Nut
{
    internal static class NativeCalls
    {
        #region Console
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_Print(string text);
        #endregion


        #region BasicComponents
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_Entity_GetTranslation(UInt64 entityID, out Vector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_Entity_SetTranslation(UInt64 entityID, ref Vector3 translation);
        #endregion


        #region Input
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Native_IsKeyDown(KeyCode keycode);
        #endregion
    }
}
