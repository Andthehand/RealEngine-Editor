using System;
using System.Drawing;
using System.Numerics;
using System.Runtime.InteropServices;

namespace RealEngine {
    [StructLayout(LayoutKind.Sequential)]
    public struct TextRendererComponent {
        // 40 Byte padding because std::string is not blittable
        private long _A;
        private long _B;
        private long _C; 
        private long _D;  
        private long _E;  

        private Vector4 color;
        public Vector4 Color
        {
            get
            {
                return color * 255f;
            }
            set
            {
                color = value / 255f;
            }
        }
    }
}