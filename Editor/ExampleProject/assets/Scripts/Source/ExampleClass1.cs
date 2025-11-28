using System;
using System.Numerics;
using RealEngine;

[AttributeUsage(AttributeTargets.Class)]
public sealed class CustomAttribute : Attribute
{
	public float Value;
}

[Custom(Value = -2500.0f)]
public class ExampleClass1 : Entity {
	void OnCreate() {
		Logger.Warn($"ExampleClass Entity Id is : {ID}");
        SpriteRendererComponent? spriteComponent = GetComponent<SpriteRendererComponent>();

		if (spriteComponent != null) {
			spriteComponent.Color = new Vector4(100.0f, 20.0f, 50.0f, 255.0f);
            Logger.Warn($"ExampleClass has componnet : {spriteComponent.Color}");
		}
		else {
			Logger.Warn("ExampleClass does not have TransformComponent");
        }
    }

	~ExampleClass1() {
		Logger.Info("ExampleClass1 Destructor");
    }

    public static void StaticMethod(float value)
	{
		Console.WriteLine($"StaticMethod: {value}");
	}
}
