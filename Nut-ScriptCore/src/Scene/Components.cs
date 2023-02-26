using Nut.Math;

using static Nut.InternalCalls;

namespace Nut.Scene
{
    public abstract class Component
    {
        public Entity Entity { get; internal set; }
    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get
            {
                Native_TransformComponent_GetTranslation(Entity.GetEntityID(), out Vector3 translation);
                return translation;
            }
            set
            {
                Native_TransformComponent_SetTranslation(Entity.GetEntityID(), ref value);
            }
        }

        public void Move(Vector3 modifier)
        {
            Vector3 pos = Translation;
            pos += modifier;
            Translation = pos;
        }

        public Vector3 Scale
        {
            get
            {
                Native_TransformComponent_GetScale(Entity.GetEntityID(), out Vector3 scale);
                return scale;
            }
            set
            {
                Native_TransformComponent_SetScale(Entity.GetEntityID(), ref value);
            }
        }

        public Vector3 Rotation
        {
            get
            {
                Native_TransformComponent_GetRotation(Entity.GetEntityID(), out Vector3 rotation);
                return rotation;
            }
            set
            {
                Native_TransformComponent_SetRotation(Entity.GetEntityID(), ref value);
            }
        }
    }


    public class SpriteRendererComponent : Component
    {
        public Vector4 Color
        {
            get
            {
                Native_SpriteRenderComponent_GetColor(Entity.GetEntityID(), out Vector4 translation);
                return translation;
            }
            set
            {
                Native_SpriteRenderComponent_SetColor(Entity.GetEntityID(), ref value);
            }
        }
    }



    public class RigidBody2DComponent : Component
    {
        public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldposition, bool wake = true)
        {
            Native_RigidBody2DComponent_ApplyLinearImpulse(Entity.GetEntityID(), ref impulse, ref worldposition, wake);
        }

        public void ApplyLinearImpulse(Vector2 impulse, bool wake = true)
        {
            Native_RigidBody2DComponent_ApplyLinearImpulseCenter(Entity.GetEntityID(), ref impulse, wake);
        }
    }
}
