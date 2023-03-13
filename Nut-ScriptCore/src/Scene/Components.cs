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
                Native_SpriteRenderComponent_GetColor(Entity.GetEntityID(), out Vector4 color);
                return color;
            }
            set
            {
                Native_SpriteRenderComponent_SetColor(Entity.GetEntityID(), ref value);
            }
        }
    }

    public class CircleRendererComponent : Component
    {
        public Vector4 Color
        {
            get
            {
                Native_CircleRenderComponent_GetColor(Entity.GetEntityID(), out Vector4 color);
                return color;
            }
            set
            {
                Native_CircleRenderComponent_SetColor(Entity.GetEntityID(), ref value);
            }
        }

        public float Fade
        {
            get => Native_CircleRenderComponent_GetFade(Entity.GetEntityID());
            set => Native_CircleRenderComponent_SetFade(Entity.GetEntityID(), value);
        }

        public float Radius
        {
            get => Native_CircleRenderComponent_GetRadius(Entity.GetEntityID());
            set => Native_CircleRenderComponent_SetRadius(Entity.GetEntityID(), value);
        }

        public float Thickness
        {
            get => Native_CircleRenderComponent_GetThickness(Entity.GetEntityID());
            set => Native_CircleRenderComponent_SetThickness(Entity.GetEntityID(), value);
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

        public void ApplyForce(Vector2 impulse, Vector2 worldposition, bool wake = true)
        {
            Native_RigidBody2DComponent_ApplyForce(Entity.GetEntityID(), ref impulse, ref worldposition, wake);
        }

        public void ApplyForce(Vector2 impulse, bool wake = true)
        {
            Native_RigidBody2DComponent_ApplyForceCenter(Entity.GetEntityID(), ref impulse, wake);
        }

        public void ApplyTorque(float torque, bool wake = true)
        {
            Native_RigidBody2DComponent_ApplyTorque(Entity.GetEntityID(), torque, wake);
        }
    }

    public class BoxCollider2DComponent : Component
    {
        public Vector2 Size
        {
            get
            {
                Native_BoxCollider2DComponent_GetSize(Entity.GetEntityID(), out Vector2 size);
                return size;
            }
            set
            {
                Native_BoxCollider2DComponent_SetSize(Entity.GetEntityID(), ref value);
            }
        }

        public Vector2 Offset
        {
            get
            {
                Native_BoxCollider2DComponent_GetOffset(Entity.GetEntityID(), out Vector2 offset);
                return offset;
            }
            set
            {
                Native_BoxCollider2DComponent_SetOffset(Entity.GetEntityID(), ref value);
            }
        }

        public float Density
        {
            get => Native_BoxCollider2DComponent_GetDensity(Entity.GetEntityID());
            set => Native_BoxCollider2DComponent_SetDensity(Entity.GetEntityID(), value);
        }

        public float Restitution
        {
            get => Native_BoxCollider2DComponent_GetRestitution(Entity.GetEntityID());
            set => Native_BoxCollider2DComponent_SetRestitution(Entity.GetEntityID(), value);
        }

        public float RestitutionThreshold
        {
            get => Native_BoxCollider2DComponent_GetRestitutionThreshold(Entity.GetEntityID());
            set => Native_BoxCollider2DComponent_SetRestitutionThreshold(Entity.GetEntityID(), value);
        }
    }
}
