
using System;

namespace RealEngine {
    public class Entity {
        protected Entity() { ID = 0; }

        internal Entity(ulong id) {
            ID = id;
        }

        protected readonly ulong ID;

        public bool HasComponent<T>() {
            Type componentType = typeof(T);
            unsafe { return InternalCalls.Entity_HasComponent(ID, componentType); }
        }

        public unsafe T* GetComponent<T>() => (T*)InternalCalls.Entity_GetComponent(ID, typeof(T));
        public unsafe T* AddComponent<T>() => (T*)InternalCalls.Entity_AddComponent(ID, typeof(T));

        public Entity Instantiate(Entity entity) {
            unsafe { return new Entity(InternalCalls.Entity_Instantiate(entity.ID)); }
        }
    }
}
