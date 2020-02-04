using System;
using System.Linq;
using System.Collections.Generic;
using System.Diagnostics;
using System.Windows.Forms;

namespace j2GUI
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [MTAThread]
        static void Main(string[] args)
        {
            if (args.Length > 0)
            {
                Application.Run(new j2Startup.MainScreen(args[0]));
            }
            else
            {
                Application.Run(new j2Startup.MainScreen(string.Empty));
            }
        }
    }
}