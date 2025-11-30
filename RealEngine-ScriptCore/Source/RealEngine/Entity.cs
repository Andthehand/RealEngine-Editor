
using System;

namespace RealEngine {
    // TODO: Implement Entity class
    public class Entity {
        protected Entity() { ID = 0; }

        internal Entity(ulong id) {
            ID = id;
        }

        public readonly ulong ID;

        public bool HasComponent<T>() {
            Type componentType = typeof(T);
            unsafe { return InternalCalls.Entity_HasComponent(ID, componentType); }
        }

        public unsafe T* GetComponent<T>() => (T*)InternalCalls.Entity_GetComponent(ID, typeof(T));
    }
}
