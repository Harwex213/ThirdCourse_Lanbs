using System;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
using ConsoleTables;

namespace task_1
{
    class Program
    {
        static void Main(string[] args)
        {
            BigInteger number = 35;
            var exponents = new int[]
            {
                1019, 3359, 5003, 7177, 8597
            };
            BigInteger modulus = 42365232352352;
            
            var table = new ConsoleTable("number (a)", "exponent (y)", "modulus (n)", "result (y)", "execution time (ticks)");

            foreach (var exponent in exponents)
            {
                var stopwatch = new Stopwatch();
                stopwatch.Start();
                var result = BigInteger.ModPow(number, exponent, modulus);
                stopwatch.Start();
                table.AddRow(number, exponent, modulus, result, stopwatch.ElapsedTicks);
            }

            table.Write();
        }
    }
}