using System.Numerics;

namespace RealEngine {
    public struct TransformComponent {
        private Vector3 translation;
        public Vector3 Translation {
            get => translation;
            set {
                translation = value;
                dirty = true;
            }
        }

        private Quaternion rotation;
        public Quaternion Rotation {
            get => rotation;
            set {
                rotation = value;
                dirty = true;
            }
        }

        private Vector3 scale;
        public Vector3 Scale {
            get => scale;
            set {
                scale = value;
                dirty = true;
            }
        }

        private bool dirty;
        public Matrix4x4 cachedMatrix;
    }
}