using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.IO;
using CppCsWrapper;
using System.Drawing;

namespace j2GUI
{
    public class GuiDefines
    {
        //screen saver variables
        public static double InactionTime = 0;
        public static bool ScreenSaveEnabled = true;
        public static bool ScreenSaveActive = false;
        public static bool ErrorScreenActive = false;

        public static object InactionTimeLock = new object();
        public static object ScreenSaveEnabledLock = new object();
        public static object ScreenSaveActiveLock = new object();
        public static object ErrorScreenActiveLock = new object();
        public static object ClickVolumeLock = new object();
        public static object RepeatVolumeSettingLock = new object();

        public static byte ClickVolume;
        public static byte AlertVolume;
        public static byte StatusVolume;

        public static byte WaveSetVolume;//volume currently set on the sound chip

        public static bool RepeatVolumeSetting = true;//flag to indicate whether to enable or disable the repeat timer. For sound settings screen we need to disable the repeat timer

        public static bool IsScreenSaverThreadStarted = false;//tells the program if the screen saver thread is started or not
        public static object IsScreenSaverThreadStartedLock = new object();

        public static bool ShowAlphaNumericKeyboard = false;//tells if the alphanumeric keyboard is active or not
        public static object ShowAlphaNumericKeyboardLock = new object();

        public const int ONEBUTTON = 1;
        public const int TWOBUTTONS = 2;
        public const int THREEBUTTONS = 3;

        public const char NORMAL = (char)0;
        public const char HIGHLIGHT = (char)1;

        public const char FALSE = (char)0;
        public const char TRUE = (char)1;

        //public const int RECEIVEBUFFERSIZE = 4096;
        public const int RECEIVEBUFFERSIZE = 4096 * 2;//for chinese
        public const int REPEATTIMERINTERVAL = 200;//the interval for the repeat timer to trigger. Used by the scroll buttons
    }

    public class GuiForm
    {
        public const int FormWidth = 480;//the main window width
        public const int FormHeight = 800;//the main window height
        public const int TopFormHeight = 45;//the top section control height         
        public const int BottomFormHeight = 75; //the bottom section control height
        public const int MiddleFormHeight = 680;//the middle section control height

        //compiler is not allowing to declare  type Color as a constant. So declare it as three variables R,G,B
        //default MainScreen background color
        public const int FormRBackColor = 215;
        public const int FormGBackColor = 226;
        public const int FormBBackColor = 245;
    }

    public class TopGuiDefines
    {

        public Font Font
        {
            get
            {
                return (new Font("Arial", 18.0f, FontStyle.Regular));//reduced from 20.0f to fit in some titles in Italian
            }
        }

        public Color FontColor
        {
            get
            {
                return (Color.White);
            }
        }

        public Color BackColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }

    public class BottomGuiDefines
    {
        //color for both label and user control
        public Color LabelBackColor
        {
            get
            {
                return (Color.FromArgb(7, 4, 39));
            }
        }

        /// <summary>
        /// Bottom label font
        /// </summary>
        public Font LabelFont
        {
            get
            {
                return (new Font("Arial", 16.0f, FontStyle.Regular));
            }
        }

        /// <summary>
        /// Bottom buttons font
        /// </summary>
        public Font Font
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color NormalFontColor
        {
            get
            {
                return (Color.White);
            }
        }

        public Color PressedFontColor
        {
            get
            {
                return (Color.White);
            }
        }

        public Color NormalStartButtonColor
        {
            get
            {
                return (Color.FromArgb(42, 89, 169));
            }
        }

        public Color NormalEndButtonColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Color HighlightStartButtonColor
        {
            get
            {
                return (Color.FromArgb(131, 192, 248));
            }
        }

        public Color HighlightEndButtonColor
        {
            get
            {
                return (Color.FromArgb(42, 89, 169));
            }
        }

        public Color PressedNormalStartButtonColor
        {
            get
            {
                return (Color.FromArgb(7, 4, 39));
            }
        }

        public Color PressedNormalEndButtonColor
        {
            get
            {
                return (Color.FromArgb(42, 89, 169));
            }
        }

        public Color PressedHighlightStartButtonColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Color PressedHighlightEndButtonColor
        {
            get
            {
                return (Color.FromArgb(131, 192, 248));
            }
        }

        public Color NormalButtonBorderColor
        {
            get
            {
                return (Color.FromArgb(7, 4, 39));
            }
        }

        public Color HighlightButtonBorderColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public string FillDirection
        {
            get
            {
                return ("Vertical");
            }
        }
    }

    public class WinCriticalStopGuiDefines
    {
        public Font TitleFont
        {
            get
            {
                return (new Font("Arial", 27.0f, FontStyle.Regular));
            }
        }

        public Font MessageFont
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color TitleFontColor
        {
            get
            {
                return (Color.White);
            }
        }

        public Color TitleBackColor
        {
            get
            {
                return (Color.FromArgb(168, 1, 1));
            }
        }

        public Color BackColor
        {
            get
            {
                return (Color.FromArgb(254, 1, 1));
            }
        }

        public Color MessageFontColor
        {
            get
            {
                return (Color.FromArgb(7, 4, 39));
            }
        }

        public Font ButtonFont
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color NormalButtonFontColor
        {
            get
            {
                return ((Color.White));
            }
        }

        public Color PressedNormalButtonFontColor
        {
            get
            {
                return ((Color.White));
            }
        }

        public Color NormalStartButtonColor
        {
            get
            {
                return (Color.FromArgb(203, 50, 1));//nj 04/15/2016 not used currently
            }
        }

        public Color NormalEndButtonColor
        {
            get
            {
                return (Color.FromArgb(234, 0, 1));//nj 04/15/2016 not used currently
            }
        }

        public Color HighlightStartButtonColor
        {
            get
            {
                return (Color.FromArgb(203, 50, 1));
            }
        }

        public Color HighlightEndButtonColor
        {
            get
            {
                return (Color.FromArgb(234, 0, 1));
            }
        }

        public Color PressedNormalStartButtonColor
        {
            get
            {
                return (Color.FromArgb(234, 0, 1));//nj 04/15/2016 not used currently
            }
        }

        public Color PressedNormalEndButtonColor
        {
            get
            {
                return (Color.FromArgb(178, 50, 1));//nj 04/15/2016 not used currently
            }
        }

        public Color PressedHighlightStartButtonColor
        {
            get
            {
                return (Color.FromArgb(234, 0, 1));
            }
        }

        public Color PressedHighlightEndButtonColor
        {
            get
            {
                return (Color.FromArgb(178, 50, 1));
            }
        }

        public Color NormalButtonBorderColor
        {
            get
            {
                return (Color.FromArgb(178, 50, 1));//nj 04/15/2016 not used currently
            }
        }

        public Color PressedNormalButtonBorderColor
        {
            get
            {
                return (Color.FromArgb(178, 50, 1));//nj 04/15/2016 not used currently
            }
        }


        public Color HighlightButtonBorderColor
        {
            get
            {
                return (Color.FromArgb(178, 50, 1));
            }
        }

        public Color PressedHighlightButtonBorderColor
        {
            get
            {
                return (Color.FromArgb(178, 50, 1));
            }
        }

        public string FillDirection
        {
            get
            {
                return ("Vertical");
            }
        }
    }

    public class WinWarningGuiDefines
    {
        public Font TitleFont
        {
            get
            {
                return (new Font("Arial", 27.0f, FontStyle.Regular));
            }
        }

        public Font MessageFont
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color TitleFontColor
        {
            get
            {
                return (Color.White);
            }
        }

        public Color TitleBackColor
        {
            get
            {
                return (Color.FromArgb(255, 150, 0));
            }
        }

        public Color BackColor
        {
            get
            {
                return (Color.FromArgb(254, 255, 177));
            }
        }

        public Font ButtonFont
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color NormalButtonFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Color PressedNormalButtonFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Color NormalStartButtonColor
        {
            get
            {
                //return (Color.FromArgb(254, 255, 177));
                return (Color.FromArgb(255, 255, 133));
            }
        }

        public Color NormalEndButtonColor
        {
            get
            {
                return (Color.FromArgb(255, 206, 0));
            }
        }

        public Color HighlightStartButtonColor
        {
            get
            {
                return (Color.FromArgb(255, 206, 0));
            }
        }

        public Color HighlightEndButtonColor
        {
            get
            {
                return (Color.FromArgb(255, 175, 60));
            }
        }

        public Color PressedNormalStartButtonColor
        {
            get
            {
                return (Color.FromArgb(255, 175, 60));
            }
        }

        public Color PressedNormalEndButtonColor
        {
            get
            {
                //return (Color.FromArgb(254, 255, 177));
                return (Color.FromArgb(255, 206, 0));
            }
        }

        public Color PressedHighlightStartButtonColor
        {
            get
            {
                return (Color.FromArgb(255, 150, 0));
            }
        }

        public Color PressedHighlightEndButtonColor
        {
            get
            {
                return (Color.FromArgb(255, 175, 60));
            }
        }

        public Color NormalButtonBorderColor
        {
            get
            {
                //return (Color.FromArgb(200, 135, 50));
                return (Color.FromArgb(255, 175, 60));
            }
        }

        public Color PressedNormalButtonBorderColor
        {
            get
            {
                //return (Color.FromArgb(255, 175, 60));
                return (Color.FromArgb(255, 175, 60));
            }
        }


        public Color HighlightButtonBorderColor
        {
            get
            {
                //
                //return (Color.FromArgb(255, 175, 60));
                return (Color.FromArgb(200, 135, 50));
            }
        }

        public Color PressedHighlightButtonBorderColor
        {
            get
            {
                //return (Color.FromArgb(255, 150, 0));
                return (Color.FromArgb(200, 135, 50));
            }
        }

        public string FillDirection
        {
            get
            {
                return ("Vertical");
            }
        }

        public Color MessageFontColor
        {
            get
            {
                return (Color.FromArgb(7, 4, 39));
            }
        }
    }

    public class WinErrorGuiDefines
    {
        public Font TitleFont
        {
            get
            {
                return (new Font("Arial", 27.0f, FontStyle.Regular));
            }
        }

        public Font MessageFont
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color TitleFontColor
        {
            get
            {
                return (Color.White);
            }
        }

        public Color TitleBackColor
        {
            get
            {
                return (Color.FromArgb(255, 150, 0));
            }
        }

        public Color BackColor
        {
            get
            {
                return (Color.FromArgb(255, 175, 60));
            }
        }

        public Font ButtonFont
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color NormalButtonFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Color PressedNormalButtonFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Color NormalStartButtonColor
        {
            get
            {
                return (Color.FromArgb(254, 255, 177));
            }
        }

        public Color NormalEndButtonColor
        {
            get
            {
                return (Color.FromArgb(255, 206, 0));
            }
        }

        public Color HighlightStartButtonColor
        {
            get
            {
                return (Color.FromArgb(255, 206, 0));
            }
        }

        public Color HighlightEndButtonColor
        {
            get
            {
                return (Color.FromArgb(255, 175, 60));
            }
        }

        public Color PressedNormalStartButtonColor
        {
            get
            {
                return (Color.FromArgb(255, 175, 60));
            }
        }

        public Color PressedNormalEndButtonColor
        {
            get
            {
                return (Color.FromArgb(254, 255, 177));
            }
        }

        public Color PressedHighlightStartButtonColor
        {
            get
            {
                return (Color.FromArgb(255, 150, 0));
            }
        }

        public Color PressedHighlightEndButtonColor
        {
            get
            {
                return (Color.FromArgb(255, 175, 60));
            }
        }

        public Color NormalButtonBorderColor
        {
            get
            {
                //return (Color.FromArgb(200, 135, 50));
                return (Color.FromArgb(255, 175, 60));
            }
        }

        public Color PressedNormalButtonBorderColor
        {
            get
            {
                //return (Color.FromArgb(255, 175, 60));
                return (Color.FromArgb(255, 175, 60));
            }
        }


        public Color HighlightButtonBorderColor
        {
            get
            {
                //
                //return (Color.FromArgb(255, 175, 60));
                return (Color.FromArgb(200, 135, 50));
            }
        }

        public Color PressedHighlightButtonBorderColor
        {
            get
            {
                //return (Color.FromArgb(255, 150, 0));
                return (Color.FromArgb(200, 135, 50));
            }
        }

        public string FillDirection
        {
            get
            {
                return ("Vertical");
            }
        }

        public Color MessageFontColor
        {
            get
            {
                return (Color.FromArgb(7, 4, 39));
            }
        }
    }

    public class MiddleSectionMessage
    {
        public Font Font
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color FontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }

    public class MiddleSectionButton
    {
        public Font Font
        {
            get
            {
                return (new Font("Arial", 27.0f, FontStyle.Regular));
            }
        }

        public Color NormalFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Color PressedFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }

    public class MiddleSectionCheckBox
    {
        public Font Font
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color NormalFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Color PressedFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }


    public class MiddleSectionRadioButton
    {
        public Font Font
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color NormalFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Color PressedFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }

    public class MiddleMultiText
    {
        public Font Font
        {
            get
            {
                //return (new Font("Arial", 18.0f, FontStyle.Regular));
                return (new Font("Courier New", 18.0f, FontStyle.Regular));
            }
        }

        public Color FontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }

    public class MiddleMultiTextRD
    {
        public Font Font
        {
            get
            {
                //return (new Font("Arial", 18.0f, FontStyle.Regular));
                return (new Font("Courier New", 18.0f, FontStyle.Regular));
            }
        }

        public Font FontSmall   //added nwr 5/14/2013
        {
            get
            {
                return (new Font("Courier New", 10.0f, FontStyle.Regular));
            }
        }

        public Font FontMiddle   //added nwr 7/8/2013
        {
            get
            {
                return (new Font("Courier New", 14.0f, FontStyle.Regular));
            }
        }


        public Color FontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }


    public class MiddleSectionProgressBar
    {
        public Font Font
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color BarBackColor
        {
            get
            {
                //solid bar background color which is static. It is not the progress color    
                return (Color.FromArgb(255, 175, 60));
            }
        }

        public Color StartBarColor
        {
            get
            {
                //progress start color. Changes dynamically according to value set
                return (Color.FromArgb(254, 255, 177));
            }
        }

        public Color EndBarColor
        {
            get
            {
                //progress end color. Changes dynamically according to value set
                return (Color.FromArgb(255, 206, 0));
            }
        }

        public Color FontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

    }

    public class MiddleSectionDateTimePicker
    {
        public Font Font
        {
            get
            {
                return (new Font("Arial", 34.0f, FontStyle.Regular));
            }
        }

        public Color FontColor
        {
            get
            {
                return (Color.FromArgb(21, 66, 132));
            }
        }

        public Color BackColor
        {
            get
            {
                return (Color.FromArgb(255, 206, 0));
            }
        }

        public Color SelectedBackColor
        {
            get
            {
                return (Color.FromArgb(255, 150, 0));
            }
        }

        public Color DropDownColor
        {
            get
            {
                return (Color.FromArgb(255, 150, 0));
            }
        }

        public Font ButtonFont
        {
            get
            {
                return (new Font("Arial", 27.0f, FontStyle.Regular));
            }
        }

        public Color NormalButtonFontColor
        {
            get
            {
                return (Color.FromArgb(21, 66, 132));
            }
        }

        public Color PressedButtonFontColor
        {
            get
            {
                return (Color.FromArgb(21, 66, 132));
            }
        }
    }

    public class MiddleSectionList
    {
        //these not used. Changed in the designer properties window directly
        //public Font Font
        //{
        //    get
        //    {
        //        return (new Font("Arial", 26.0f, FontStyle.Regular));
        //    }
        //}

        //public Color FontColor
        //{
        //    get
        //    {
        //        return (Color.FromArgb(5, 55, 109));
        //    }
        //}

        public Color SelectedBackColor
        {
            get
            {
                return (Color.FromArgb(131, 192, 248));
            }
        }

        public int RowHeight
        {
            get
            {
                return (90);//height of each row in a list box control
            }
        }
    }

    public class MiddleSectionDate
    {
        public Font Font
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Font TextBoxFont
        {
            get
            {
                return (new Font("Bell MT", 26.0f, FontStyle.Regular));
            }
        }

        public Color FontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }

    public class MiddleSectionTime
    {
        public Font Font
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color FontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Font ButtonFont
        {
            get
            {
                return (new Font("Arial", 27.0f, FontStyle.Regular));
            }
        }

        public Color ButtonFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Font TextBoxFont
        {
            get
            {
                return (new Font("Bell MT", 38.0f, FontStyle.Regular));
            }
        }

        public Color TextBoxFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }
    //added nwr 4/26/2013
    public class MiddleSectionTemperature
    {

        public Font Font
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color FontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Font ButtonFont
        {
            get
            {
                return (new Font("Arial", 27.0f, FontStyle.Regular));
            }
        }

        public Color ButtonFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Font TextBoxFontLarge
        {
            get
            {
                return (new Font("Bell MT", 38.0f, FontStyle.Regular));
            }
        }

        public Font TextBoxFontSmall
        {
            get
            {
                return (new Font("Bell MT", 20.0f, FontStyle.Regular));
            }
        }

        public Color TextBoxFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Font LabelFont
        {
            get
            {
                return (new Font("Arial", 14.0f, FontStyle.Regular));
            }
        }

        public Color LabelFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }

    public class MiddleSectionKeyboard
    {
        public Font AlphaKeyFont
        {
            get
            {
                return (new Font("Arial", 30.0f, FontStyle.Regular));
            }
        }

        public Font NumericKeyFont
        {
            get
            {
                return (new Font("Arial", 34.0f, FontStyle.Regular));
            }
        }

        public Font ArrowKeyFont
        {
            get
            {
                return (new Font("Webdings", 34.0f, FontStyle.Regular));
            }
        }

        public Font InputTextFont
        {
            get
            {
                //return (new Font("Bell MT", 30.0f, FontStyle.Regular));
                return (new Font("Courier New", 30.0f, FontStyle.Regular));
            }
        }

        public Color NormalFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Color PressedFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }

    public class MiddleSectionTypeCheckBox
    {
        public Font Font
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color NormalFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Color PressedFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }

    public class MiddleSectionInformationMessage
    {
        public Font Font
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color FontColor
        {
            get
            {
                return (Color.FromArgb(42, 89, 169));
            }
        }
    }

    public class MiddleSectionEnterValue
    {
        public Font LabelFont
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color LabelFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Font TextBoxFont
        {
            get
            {
                return (new Font("Bell MT", 38.0f, FontStyle.Regular));
            }
        }

        public Font TextBoxFontSmall
        {
            get
            {
                return (new Font("Bell MT", 20.0f, FontStyle.Regular));
            }
        }

        public Color TextBoxFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }

    public class MiddleSectionCuvette
    {
        public Font LabelFont
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Font LabelFontSmall
        {
            get
            {
                return (new Font("Arial", 20.0f, FontStyle.Regular));
            }
        }

        public Color LabelFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Font TextBoxFont
        {
            get
            {
                return (new Font("Bell MT", 38.0f, FontStyle.Regular));
            }
        }

        public Font TextBoxFontSmall
        {
            get
            {
                return (new Font("Bell MT", 20.0f, FontStyle.Regular));
            }
        }

        public Color TextBoxFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }

    public class MiddleSectionEnterValueBarcode
    {
        public Font LabelFont
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Font LabelFontSmall
        {
            get
            {
                return (new Font("Arial", 14.0f, FontStyle.Regular));
            }
        }

        public Color LabelFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Font TextBoxFont
        {
            get
            {
                return (new Font("Bell MT", 38.0f, FontStyle.Regular));
            }
        }

        public Font TextBoxFontSmall
        {
            get
            {
                return (new Font("Bell MT", 20.0f, FontStyle.Regular));
            }
        }

        public Color TextBoxFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }

    public class MiddleSectionEnterRange
    {
        public Font LabelFont
        {
            get
            {
                return (new Font("Arial", 26.0f, FontStyle.Regular));
            }
        }

        public Color LabelFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }

        public Font TextBoxFont
        {
            get
            {
                return (new Font("Bell MT", 38.0f, FontStyle.Regular));
            }
        }

        public Color TextBoxFontColor
        {
            get
            {
                return (Color.FromArgb(5, 55, 109));
            }
        }
    }
}
