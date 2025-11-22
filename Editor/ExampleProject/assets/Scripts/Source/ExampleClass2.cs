using System;
using RealEngine;

public class ExampleClass2 : Entity
{
    void OnCreate()
    {
        Logger.Warn($"ExampleClass2 Entity Id is : {ID}");
    }

    ~ExampleClass2()
    {
        Logger.Info("ExampleClass2 Destructor");
    }

    public static void StaticMethod(float value)
    {
        Console.WriteLine($"StaticMethod: {value}");
    }
}
