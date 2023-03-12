using Nut.Math;
using System;
using static Nut.InternalCalls;

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

        public bool HasComponent<T>() where T : Component
        {
            return Native_Entity_HasComponent(id, typeof(T));
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if(!HasComponent<T>())
                return null; // Or throw exception maybe?
            return new T() { Entity = this };
        }

        public static Entity FindEntityByName(string name)
        {
            ulong entityID = InternalCalls.Native_Entity_FindEntityByName(name);
            if (entityID == 0)
                return null;

            return new Entity(entityID);
        }

        public T As<T>() where T : Entity, new()
        {
            object instance = InternalCalls.Native_GetScriptInstance(id);
            return instance as T;
        }
    }
}
