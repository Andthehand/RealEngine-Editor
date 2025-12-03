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
		unsafe {
			Entity clone = Instantiate(this);
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
