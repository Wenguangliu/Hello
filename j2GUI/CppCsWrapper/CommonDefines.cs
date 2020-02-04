using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;

namespace CppCsWrapper
{
    /// <summary>
    /// This is used to maintain the same definition between j2Console and j2Gui
    /// </summary>
    public class CommonDefines
    {
        public static char VETSCAN_PRODUCT_CODE = '1';
        public static char PICCOLO_PRODUCT_CODE = '2';

        /// <summary>
        /// Specifies how numbers should be displayed
        /// </summary>
        public enum NumericFormat_t
        {
            Xxxx,		// Three decimal places
            Xxx,		// Two decimal places
            Xx,			// One decimal place
            X,			// No decimal places
            X0,			// Always 0 in 10's place
            X00			// Always 0 in both 10's and 100's places
        }

        /// <summary>
        /// Specifies the keyboard languages
        /// </summary>
        public enum Language_t
        {
            LANGUAGE_ENGLISH,
            LANGUAGE_FRENCH,
            LANGUAGE_GERMAN,
            LANGUAGE_SPANISH,
            LANGUAGE_ITALIAN,
            LANGUAGE_BRAZIL,
            LANGUAGE_PORTUGUESE,
            LANGUAGE_CHINESE,
            NUM_LANGUAGES
        }
    }
}
