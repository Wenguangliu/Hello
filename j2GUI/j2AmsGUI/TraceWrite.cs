using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Threading;

namespace j2AMSGUI
{
    public class TraceWrite
    {
        public static int ErrCount;

        /// <summary>
        /// call Trace Write to write a message into our log file
        /// </summary>
        /// <param name="Msg"></param>
        public static void TraceLog(string Msg)
        {
            string FileName = "Trace.log";
            TraceLog(Msg, FileName);
            Object thisLock = new Object();
            lock (thisLock)
            {
                ErrCount++;
            }            
        }

        /// <summary>
        /// Writes to default directory WebSecureLogs\YYYYMM\YYYYMMDD
        /// </summary>
        /// <param name="sMsg"></param>
        /// <param name="sFile"></param>
        public static void TraceLog(string Msg, string File)
        {
            //string Dir = String.Format(path+@"\{0}\{1}", DateTime.Now.ToString("yyyyMM"), DateTime.Now.ToString("yyyyMMdd"));
            string path = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().GetModules()[0].FullyQualifiedName);
            
            string Dir = String.Format(path + @"\{0}\{1}", DateTime.Now.ToString("yyyyMM"), DateTime.Now.ToString("yyyyMMdd"));
            TraceLog(Msg, File, Dir);
        }

        /// <summary>
        /// Writes to specified file and directory
        /// </summary>
        /// <param name="sMsg"></param>
        /// <param name="sFile"></param>
        /// <param name="sDir"></param>
        public static void TraceLog(string Msg, string File, string Dir)
        {

            //Create a directory if it doesn't already exist
            if (!Directory.Exists(Dir))
            {
                Directory.CreateDirectory(Dir);
            }

            string Path = Dir + @"\" + File;
            string LogTime = "\r\n" + DateTime.Now.ToString("H:mm:ss:fff");     //time like 15:25:36

            StreamWriter stmw = null;

            int i = 0;
            while (i <= 3)
            {
                try
                {
                    Object thisLock = new Object();

                    lock (thisLock)
                    {
                        i++;
                        stmw = new StreamWriter(new FileStream(Path, FileMode.Append, FileAccess.Write));
                        stmw.WriteLine(LogTime + ": " + Msg);
                        i = 5;		//success - so exit while loop
                    }
                    
                
                }
                catch (IOException ex)
                {
                    Debug.WriteLine(ex.Message);
                    Thread.Sleep(20);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
                finally
                {
                    if (stmw != null)
                        stmw.Close();
                }
            }
            Debug.WriteLine(LogTime + ": " + Msg);
        }

    }
}
