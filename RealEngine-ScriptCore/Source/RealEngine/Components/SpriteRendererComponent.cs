

using System.Numerics;

namespace RealEngine {
    public class SpriteRendererComponent : Component {
        public Vector4 Color {
            get {
                unsafe {
                    InternalCalls.SpriteRendererComponent_GetColor(Entity.ID, out Vector4 color);
                    return color * 255;
                }
            }
            set {
                Vector4 outColor = value / 255;
                unsafe { InternalCalls.SpriteRendererComponent_SetColor(Entity.ID, ref outColor); }
            }
        }
    }
}