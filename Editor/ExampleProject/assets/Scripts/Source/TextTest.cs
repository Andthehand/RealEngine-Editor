using System;
using System.Numerics;
using RealEngine;

public class TextTest : Entity {

    void OnCreate() {
        Logger.Warn($"ExampleClass2 Entity Id is : {ID}");
        unsafe {
            TextRendererComponent* textRenderer = GetComponent<TextRendererComponent>();
            textRenderer->Color = new Vector4(255.0f, 0.0f, 0.0f, 255.0f);
        }
    }

    public static void StaticMethod(float value) {
        Console.WriteLine($"StaticMethod: {value}");
    }
}
