using Nut.Math;
using System;
using static Nut.NativeCalls;

namespace Nut.Scene
{
    public class Entity
    {
        private readonly ulong id;

        protected Entity() { }
        protected Entity(UInt64 id) { this.id = id; }

        public virtual void OnCreate() { }

        public virtual void OnDestroy() { }

        public virtual void OnUpdate(float ts) { }

        public UInt64 GetEntityID() { return id; } 

        public Vector3 Translation
        {
            get { Native_Entity_GetTranslation(id, out Vector3 translation); return translation; }
            set => Native_Entity_SetTranslation(id, ref value);
        }
    }
}
