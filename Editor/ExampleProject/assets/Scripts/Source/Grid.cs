using System;
using System.Numerics;
using RealEngine;

public class Grid : Entity {

    void OnCreate()
    {
        Logger.Warn($"ExampleClass2 Entity Id is : {ID}");
        unsafe
        {
            TextRendererComponent* textRenderer = GetComponent<TextRendererComponent>();
            textRenderer->Color = new Vector4(255.0f, 0.0f, 0.0f, 255.0f);
        }
    }

}
