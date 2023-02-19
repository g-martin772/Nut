using System;
using System.Collections.Generic;

using static Nut.NativeCalls;

namespace Nut
{
    class Main
    {
        public Main()
        {
            Console.WriteLine("Constructor from C#");
            Random rnd = new Random();
            List<int> ints = new List<int>();
            ints.Add(1);
            ints.Add(1);
            ints.Add(1);
            ints.Add(1);
            foreach (var item in ints)
            {
                int value = (int)item;
                Native_Print(value.ToString());
            }
        }

        static void PrintStaticTest()
        {
            Console.WriteLine("StaticTest from C#");
        }

        void PrintTest()
        {
            Console.WriteLine("Test from C#");
        }

        void PrintInt(int value)
        {
            Console.WriteLine($"Value: {value}");
        }

        
    }
}
