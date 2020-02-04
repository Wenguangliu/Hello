using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.IO;
using CppCsWrapper;
using System.Drawing;

namespace j2AMSGUI
{
    public class GuiDefines
    {
        public const int ONEBUTTON = 1;
        public const int TWOBUTTONS = 2;
        public const int THREEBUTTONS = 3;

        public const char NORMAL = (char)0;
        public const char HIGHLIGHT = (char)1;//int to char conversion. HIGHLIGHT is int 1 and not char '1'

        public const char CHECKED = (char)1;//if the check box is checked
        public const char PASSWORD = (char)1;//if the text box input is a password (*)
        public const char SELECTED = (char)1;//if the list item is selected or not
        public const char SELECTABLE = (char)1;//if the list box is selectable or not
        public const char HASUPDOWN = (char)1;//if the screen has up and down buttons
        public const char TWENTYFOURHOURFORMAT = (char)1;//if the time should be displayed in 12 hour or 24 hour format

        public const int RECEIVEBUFFERSIZE = 4096;
    }

    public class GuiForm
    {
        public const int FormWidth = 480;//the main window width
        public const int FormHeight = 800;//the main window height
        public const int TopFormHeight = 45;//the top section control height         
        public const int BottomFormHeight = 75; //the bottom section control height
        public const int MiddleFormHeight = 6840;//the middle section control height

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
                return (new Font("Arial", 20.0f, FontStyle.Regular));
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
                return (Color.FromArgb(255, 175, 60));
            }
        }

        public Color StartBarColor
        {
            get
            {
                return (Color.FromArgb(254, 255, 177));
            }
        }

        public Color EndBarColor
        {
            get
            {
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
                return(Color.FromArgb(255, 206, 0));
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
                return (Color.FromArgb(131,192,248));
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
                return (new Font("Arial", 18.0f, FontStyle.Regular));
            }
        }

        public Color FontColor
        {
            get
            {
                return (Color.FromArgb(21, 66, 132));
            }
        }
    }

    public class MiddleSectionTime
    {
        public Font Font
        {
            get
            {
                return (new Font("Arial", 24.0f, FontStyle.Regular));
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
                return (new Font("Arial", 26.0f, FontStyle.Regular));
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
                return (new Font("Bell MT", 38.0f, FontStyle.Regular));
            }
        }

        public Color NormalFontColor
        {
            get
            {
                return (Color.FromArgb(5,55,109));
            }
        }

        public Color PressedFontColor
        {
            get
            {
                return (Color.FromArgb(5,55,109));
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
                return (new Font("Arial", 25.0f, FontStyle.Regular));
            }
        }

        public Color LabelFontColor
        {
            get
            {
                return (Color.DarkGray);
            }
        }

        public Font TextBoxFont
        {
            get
            {
                return (new Font("Arial", 25.0f, FontStyle.Regular));
            }
        }

        public Color TextBoxFontColor
        {
            get
            {
                return (Color.Black);
            }
        }
    }

    public class MiddleSectionEnterRange
    {
        public Font LabelFont
        {
            get
            {
                return (new Font("Arial", 25.0f, FontStyle.Regular));
            }
        }

        public Color LabelFontColor
        {
            get
            {
                return (Color.DarkGray);
            }
        }

        public Font TextBoxFont
        {
            get
            {
                return (new Font("Arial", 25.0f, FontStyle.Regular));
            }
        }

        public Color TextBoxFontColor
        {
            get
            {
                return (Color.Black);
            }
        }
    }

    public class SplashScreenGuiDefines
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
                return (new Font("Arial", 24.0f, FontStyle.Regular));
            }
        }

        public Color TitleFontColor
        {
            get
            {
                return (Color.Navy);
            }
        }

        public Color TitleBackColor
        {
            get
            {
                return (Color.FromArgb(215, 226, 245));
            }
        }
    }
}
    