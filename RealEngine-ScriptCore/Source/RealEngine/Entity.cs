
using System;

namespace RealEngine {
    // TODO: Implement Entity class
    public class Entity {
        protected Entity() { ID = 0; }

        internal Entity(ulong id) {
            ID = id;
        }

        public readonly ulong ID;

        public bool HasComponent<T>() where T : Component, new() {
            Type componentType = typeof(T);
            unsafe { return InternalCalls.Entity_HasComponent(ID, componentType); }
        }

        public T? GetComponent<T>() where T : Component, new() {
            if (!HasComponent<T>())
                return null;

            T component = new T() { Entity = this };
            return component;
        }
    }
}
