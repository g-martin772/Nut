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
        internal static extern void 
        Native_TransformComponent_GetTranslation(UInt64 entityID, out Vector3 translation);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void 
        Native_TransformComponent_SetTranslation(UInt64 entityID, ref Vector3 translation);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void 
        Native_TransformComponent_GetRotation(UInt64 entityID, out Vector3 rotation);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void 
        Native_TransformComponent_SetRotation(UInt64 entityID, ref Vector3 rotation);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void 
        Native_TransformComponent_GetScale(UInt64 entityID, out Vector3 scale);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void 
        Native_TransformComponent_SetScale(UInt64 entityID, ref Vector3 scale);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void 
        Native_SpriteRenderComponent_GetColor(UInt64 entityID, out Vector4 color);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void 
        Native_SpriteRenderComponent_SetColor(UInt64 entityID, ref Vector4 color);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_CircleRenderComponent_GetColor(UInt64 entityID, out Vector4 color);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_CircleRenderComponent_SetColor(UInt64 entityID, ref Vector4 color);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float
        Native_CircleRenderComponent_GetFade(UInt64 entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_CircleRenderComponent_SetFade(UInt64 entityID, float fade);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float
        Native_CircleRenderComponent_GetRadius(UInt64 entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_CircleRenderComponent_SetRadius(UInt64 entityID, float radius);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float
        Native_CircleRenderComponent_GetThickness(UInt64 entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_CircleRenderComponent_SetThickness(UInt64 entityID, float thickness);
        #endregion


        #region Input
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool 
        Native_IsKeyDown(Key keycode);
        #endregion


        #region Physics2D
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_RigidBody2DComponent_ApplyLinearImpulse(UInt64 entityID, ref Vector2 impulse, ref Vector2 pos, bool wake);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_RigidBody2DComponent_ApplyLinearImpulseCenter(UInt64 entityID, ref Vector2 impulse, bool wake);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_RigidBody2DComponent_ApplyForce(UInt64 entityID, ref Vector2 impulse, ref Vector2 pos, bool wake);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_RigidBody2DComponent_ApplyForceCenter(UInt64 entityID, ref Vector2 impulse, bool wake);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_RigidBody2DComponent_ApplyTorque(UInt64 entityID, float torque, bool wake);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_BoxCollider2DComponent_GetOffset(UInt64 entityID, out Vector2 offset);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_BoxCollider2DComponent_SetOffset(UInt64 entityID, ref Vector2 offset);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_BoxCollider2DComponent_GetSize(UInt64 entityID, out Vector2 size);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_BoxCollider2DComponent_SetSize(UInt64 entityID, ref Vector2 size);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float
        Native_BoxCollider2DComponent_GetDensity(UInt64 entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_BoxCollider2DComponent_SetDensity(UInt64 entityID, float density);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float
        Native_BoxCollider2DComponent_GetRestitution(UInt64 entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_BoxCollider2DComponent_SetRestitution(UInt64 entityID, float restitution);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float
        Native_BoxCollider2DComponent_GetRestitutionThreshold(UInt64 entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void
        Native_BoxCollider2DComponent_SetRestitutionThreshold(UInt64 entityID, float restitutionThreshold);
        #endregion
    }
}
