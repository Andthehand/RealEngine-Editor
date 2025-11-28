

using System.Numerics;

namespace RealEngine {
    public class TransformComponent : Component {
        public Vector3 Translation {
            get {
                unsafe {
                    InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
                    return translation;
                }
            }
            set {
                unsafe { InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value); }
            }
        }

        public Quaternion Rotation {
            get {
                unsafe {
                    InternalCalls.TransformComponent_GetRotation(Entity.ID, out Quaternion rotation);
                    return rotation;
                }
            }
            set {
                unsafe { InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value); }
            }
        }

        public Vector3 Scale
        {
            get
            {
                unsafe
                {
                    InternalCalls.TransformComponent_GetScale(Entity.ID, out Vector3 scale);
                    return scale;
                }
            }
            set
            {
                unsafe { InternalCalls.TransformComponent_SetScale(Entity.ID, ref value); }
            }
        }
    }
}