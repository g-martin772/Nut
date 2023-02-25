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
                Native_Entity_GetTranslation(Entity.GetEntityID(), out Vector3 translation);
                return translation;
            }
            set
            {
                Native_Entity_SetTranslation(Entity.GetEntityID(), ref value);
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
                Native_Entity_GetTranslation(Entity.GetEntityID(), out Vector3 translation);
                return translation;
            }
            set
            {
                Native_Entity_SetTranslation(Entity.GetEntityID(), ref value);
            }
        }

        public Vector3 Rotation
        {
            get
            {
                Native_Entity_GetTranslation(Entity.GetEntityID(), out Vector3 translation);
                return translation;
            }
            set
            {
                Native_Entity_SetTranslation(Entity.GetEntityID(), ref value);
            }
        }
    }

}
