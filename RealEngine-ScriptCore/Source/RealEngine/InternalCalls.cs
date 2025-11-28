using Coral.Managed.Interop;
using System.Numerics;

#pragma warning disable 0649
namespace RealEngine {
    public static class InternalCalls {
        //Logging
        internal static unsafe delegate*<LogLevel, NativeString, void> NativeLog;
        //Logging

        //Entity
        internal static unsafe delegate*<ulong, ReflectionType, bool> Entity_HasComponent;
        //Entity

        //TransformComponent
        internal static unsafe delegate*<ulong, out Vector3, void> TransformComponent_GetTranslation;
        internal static unsafe delegate*<ulong, ref Vector3, void> TransformComponent_SetTranslation;

        internal static unsafe delegate*<ulong, out Quaternion, void> TransformComponent_GetRotation;
        internal static unsafe delegate*<ulong, ref Quaternion, void> TransformComponent_SetRotation;

        internal static unsafe delegate*<ulong, out Vector3, void> TransformComponent_GetScale;
        internal static unsafe delegate*<ulong, ref Vector3, void> TransformComponent_SetScale;
        //TransformComponent

        //SpriteRendererComponent
        internal static unsafe delegate*<ulong, out Vector4, void> SpriteRendererComponent_GetColor;
        internal static unsafe delegate*<ulong, ref Vector4, void> SpriteRendererComponent_SetColor;
        //SpriteRendererComponent

    }
}
#pragma warning restore 0649
