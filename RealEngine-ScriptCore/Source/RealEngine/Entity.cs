
namespace RealEngine {
    // TODO: Implement Entity class
    public class Entity {
        protected Entity() { ID = 0; }

        internal Entity(ulong id) {
            ID = id;
        }

        public readonly ulong ID;

    }
}
