using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace CppCsWrapper
{
    /// <summary>
    /// This is the class which helps in calling the native win32 api calls from C#. Add all the dllimport calls here
    /// </summary>
    public sealed class Native
    {
        private const int WM_VSCROLL = 277; // Vertical scroll
        private const int SB_LINEUP = 0; // Scrolls one line up
        private const int SB_LINEDOWN = 1; // Scrolls one line down        

        #region WIN32 STANDARD DLL IMPORT CALLS
        public struct SystemTime
        {
            public ushort wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMillisecond;
        }

        [DllImport("coredll.dll", EntryPoint = "GetSystemTime", SetLastError = true)]
        public extern static void WinCEGetSystemTime(ref SystemTime st);

        [DllImport("coredll.dll", EntryPoint = "SetSystemTime", SetLastError = true)]
        public extern static bool WinCESetSystemTime(ref SystemTime st);        

        [DllImport("coredll.dll", CharSet = CharSet.Auto)]
        private static extern int SendMessage(IntPtr hWnd, int wMsg, IntPtr wParam, IntPtr lParam);

        public static void ScrollDown(TextBox tb)
        {
            SendMessage(tb.Handle, WM_VSCROLL, (IntPtr)SB_LINEDOWN, IntPtr.Zero);
        }

        public static void ScrollUp(TextBox tb)
        {
            SendMessage(tb.Handle, WM_VSCROLL, (IntPtr)SB_LINEUP, IntPtr.Zero);
        }
        #endregion

        #region ABAXIS SETUP DLL IMPORT CALLS

        [DllImport("AbaxisSetup.dll")]
        public static extern void GetTouchClickVolume(ref byte volume);

        [DllImport("AbaxisSetup.dll")]
        public static extern void GetAlertVolume(ref byte volume);

        [DllImport("AbaxisSetup.dll")]
        public static extern void GetStatusVolume(ref byte volume);        

        [DllImport("AbaxisSetup.dll")]
        //public static extern char GetAbaxisProductCode();
        public static extern void GetAbaxisProductCode(ref byte productCode);

        [DllImport("AbaxisSetup.dll")]
        public static extern void GetScreenSaverTimeout(ref ushort timeout);

        [DllImport("AbaxisSetup.dll")]
        public static extern void GetPowerSaveTimeout(ref ushort timeout);

        [DllImport("AbaxisSetup.dll")]
        public static extern void GetBacklightLevel(ref byte level);

        [DllImport("AbaxisSetup.dll")]
        public static extern void GetKeyboard(ref CppCsWrapper.CommonDefines.Language_t language);

        [DllImport("AbaxisSetup.dll")]
        public static extern void SetDeviceBacklight(int setting);

        [DllImport("AbaxisSetup.dll")]
        public static extern byte SetDeviceLedBlink();

        [DllImport("AbaxisSetup.dll")]
        public static extern byte SetDeviceLedSteadyOn();

        [DllImport("AbaxisSetup.dll")]
        public static extern void SetSoundVolume(byte volume);

        #endregion       

        #region KEYBOARD DLL IMPORT CALLS

        [DllImport("coredll.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr ActivateKeyboardLayout(IntPtr hkl, int uFlags);

        [DllImport("coredll.dll", CharSet = CharSet.Auto)]       
        public static extern IntPtr LoadKeyboardLayout(StringBuilder s, uint Flags);

        [DllImport("coredll.dll", CharSet = CharSet.Auto)]        
        public static extern IntPtr GetKeyboardLayout(System.UInt32 thread);

        #endregion
    }
}
