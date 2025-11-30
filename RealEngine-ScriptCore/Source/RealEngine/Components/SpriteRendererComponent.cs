using System.Numerics;
using System.Runtime.InteropServices;

namespace RealEngine {
    [StructLayout(LayoutKind.Sequential, Size = 8)]
    public struct SpriteRendererComponent {
        private Vector4 color;
        public Vector4 Color {
            get {
                return color * 255f;
            }
            set {
                color = value / 255f;
            }
        }
    }
}