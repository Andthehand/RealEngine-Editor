using Coral.Managed.Interop;

using System;

using RealEngine;

namespace Example.Managed {

	[AttributeUsage(AttributeTargets.Class)]
	public sealed class CustomAttribute : Attribute
	{
		public float Value;
	}

	[Custom(Value = -2500.0f)]
	public class ExampleClass : Entity {
		void OnCreate() {
			Logger.Warn($"Entity Id is : {ID}");
        }

		~ExampleClass() {
			Logger.Info("ExampleClass Destructor");
        }

        public static void StaticMethod(float value)
		{
			Console.WriteLine($"StaticMethod: {value}");
		}
	}

}
