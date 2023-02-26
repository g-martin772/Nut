using Nut.Math;
using Nut.Input;
using Nut.Core;
using System;
using System.Runtime.CompilerServices;

namespace Nut
{
    internal static class InternalCalls
    {
        #region Console
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_Print(string text, LogLevel level);
        #endregion


        #region Entity
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Native_Entity_HasComponent(UInt64 entityID, Type componentType);
        #endregion


        #region BasicComponents
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_TransformComponent_GetTranslation(UInt64 entityID, out Vector3 translation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_TransformComponent_SetTranslation(UInt64 entityID, ref Vector3 translation);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_TransformComponent_GetRotation(UInt64 entityID, out Vector3 rotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_TransformComponent_SetRotation(UInt64 entityID, ref Vector3 rotation);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_TransformComponent_GetScale(UInt64 entityID, out Vector3 scale);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_TransformComponent_SetScale(UInt64 entityID, ref Vector3 scale);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_SpriteRenderComponent_GetColor(UInt64 entityID, out Vector4 scale);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_SpriteRenderComponent_SetColor(UInt64 entityID, ref Vector4 scale);
        #endregion


        #region Input
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Native_IsKeyDown(KeyCode keycode);
        #endregion


        #region Physics2D
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_RigidBody2DComponent_ApplyLinearImpulse(UInt64 entityID, ref Vector2 impulse, ref Vector2 pos, bool wake);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Native_RigidBody2DComponent_ApplyLinearImpulseCenter(UInt64 entityID, ref Vector2 impulse, bool wake);
        #endregion
    }
}
