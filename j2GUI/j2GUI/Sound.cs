using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Media;
using System.Windows.Forms;
using CppCsWrapper;
using System.IO;
using System.Threading;

namespace j2GUI
{
    /// <summary>
    /// This class deals with all the functionality of sound for the analyzer
    /// </summary>
    public class Sound
    {
        private SoundPlayer Player = new SoundPlayer();

        public Sound()
        {

        }
        /// <summary>
        /// Get the click volume setting from SettingsData shared memory of j2Console and set the volume on Win CE
        /// </summary>
        internal void SetClickVolume(byte volume)
        {
            //SetVolume(volume);
            lock (GuiDefines.ClickVolumeLock)
            {
                GuiDefines.ClickVolume = volume;
            }
        }

        /// <summary>
        /// Get the alert volume setting from SettingsData shared memory of j2Console and set the volume on Win CE
        /// </summary>
        internal void SetAlertVolume(byte volume)
        {
            //SetVolume(volume);
            GuiDefines.AlertVolume = volume;
        }

        /// <summary>
        /// Get the status volume setting from SettingsData shared memory of j2Console and set the volume on Win CE
        /// </summary>
        internal void SetStatusVolume(byte volume)
        {
            //SetVolume(volume);
            GuiDefines.StatusVolume = volume;
        }

        /// <summary>
        /// The user has clicked controls on the gui and so reset the InactionTime to zero which disables the 
        /// screen saver screens
        /// </summary>
        internal void Click()
        {
            lock (GuiDefines.InactionTimeLock)
            {
                GuiDefines.InactionTime = 0;//reset
            }

            lock (GuiDefines.ClickVolumeLock)
            {
                if (GuiDefines.ClickVolume == 0)
                {
                    return;
                }
                else if (GuiDefines.ClickVolume != GuiDefines.WaveSetVolume)
                {
                    SetWaveOutVolume(GuiDefines.ClickVolume);
                }
            }
            
            PlayWave(@"\Hard Disk\Abaxis\Sound\Click.wav");
        }

        /// <summary>
        /// Set the volume control. Call the AbaxisSetup library function to set the volume.
        /// </summary>
        /// <param name="volume"></param>
        internal void SetWaveOutVolume(byte volume)
        {
            GuiDefines.WaveSetVolume = volume;//WaveSetVolume always has the volume that is set on the sound driver chip. It can be either the status volume or the alert volume or the click volume
            Native.SetSoundVolume(volume);      
        }

        /// <summary>
        /// Play the wav file now
        /// </summary>
        /// <param name="audioFileName">name of the wavefile and its path to play</param>
        internal void PlayWave(string audioFileName)
        {
            Player.SoundLocation = audioFileName;

            try
            {
                Player.Load();
            }
            catch (FileNotFoundException e)
            {
                MessageBox.Show("Sound File/s does not exist on the Compact Flash card. Do not proceed...", "Analyzer Software", MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
            }
            catch (DirectoryNotFoundException e)
            {
                MessageBox.Show("Sound directory does not exist on the Compact Flash card. Do not proceed...", "Analyzer Software", MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
            }

            try
            {
                Player.PlaySync();
            }
            catch (System.InvalidOperationException e)
            {
                MessageBox.Show("Sound files existing on the Compact Flash card are not of correct format. Do not proceed...", "Analyzer Software", MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
            }

            lock (GuiDefines.InactionTimeLock)
            {
                GuiDefines.InactionTime = 0;//reset
            }
        }

        /// <summary>
        /// Set all the sounds to what is there in Settings shared memory
        /// </summary>
        internal void InitializeSound()
        {
            //initialize sound
            byte volume = 0;

            //get the screen clicks volume, alert volume and status volume from the SettingsData shared memory
            //and then set the volume static variables        

            Native.GetTouchClickVolume(ref volume);
            SetClickVolume(volume);

            Native.GetAlertVolume(ref volume);
            SetAlertVolume(volume);

            Native.GetStatusVolume(ref volume);
            SetStatusVolume(volume);

            lock (GuiDefines.ClickVolumeLock)
            {
                SetWaveOutVolume(GuiDefines.ClickVolume);
            }
        }

        /// <summary>
        /// Set the correct volume and the wav file to play
        /// </summary>
        /// <param name="snd"></param>
        internal void Play(CppCsWrapper.Sound.Sound_t snd)
        {
            switch (snd)
            {
                case (CppCsWrapper.Sound.Sound_t.SOUND_CRITICAL_STOP):
                    {
                        if (GuiDefines.AlertVolume == 0)
                        {
                            return;
                        }
                        SetWaveOutVolume(GuiDefines.AlertVolume);
                        PlayWave(@"\Hard Disk\Abaxis\Sound\Critical_Stop.wav");
                        break;
                    }
                case (CppCsWrapper.Sound.Sound_t.SOUND_ERROR):
                    {
                        if (GuiDefines.AlertVolume == 0)
                        {
                            return;
                        }
                        SetWaveOutVolume(GuiDefines.AlertVolume);
                        PlayWave(@"\Hard Disk\Abaxis\Sound\Error.wav");
                        break;
                    }
                case (CppCsWrapper.Sound.Sound_t.SOUND_WARNING):
                    {
                        if (GuiDefines.AlertVolume == 0)
                        {
                            return;
                        }
                        SetWaveOutVolume(GuiDefines.AlertVolume);
                        PlayWave(@"\Hard Disk\Abaxis\Sound\Warning.wav");
                        break;
                    }
                case (CppCsWrapper.Sound.Sound_t.SOUND_PRINTING_DONE):
                    {
                        if (GuiDefines.StatusVolume == 0)
                        {
                            return;
                        }
                        SetWaveOutVolume(GuiDefines.StatusVolume);
                        PlayWave(@"\Hard Disk\Abaxis\Sound\Printing_Done.wav");
                        break;
                    }
                case (CppCsWrapper.Sound.Sound_t.SOUND_CLICK):
                    {
                        lock (GuiDefines.ClickVolumeLock)
                        {
                            if (GuiDefines.ClickVolume == 0)
                            {
                                return;
                            }

                            SetWaveOutVolume(GuiDefines.ClickVolume);
                        }

                        PlayWave(@"\Hard Disk\Abaxis\Sound\Click.wav");
                        break;
                    }

            }

            //in NGA code, before playing the Click.wav, the click volume is set in Click(). But if the 
            //same is done in Jen II, then it slows down the button clicks since it is calling Native.waveOutSetVolume
            //every time and setting the sound driver volume. So in order to give better performance to the user,
            //we are setting the volume always to click volume but if other volumes like alert or status needs to be used,
            //then, it is set before that wav is played

            //by default always the volume control will have the level that is set to click volume. When other volumes need to play,
            //we set that volume first and then play and then we set the wave volume back to click
            //set the volume always back to click
            //SetWaveOutVolume(GuiDefines.ClickVolume);//we are commenting this out on 12/15/2011 since it causes delay in displaying a warning or critical error screen
            //we will set the volume back to click volume in another thread so that the display can be shown immediately

            //start the receive gui messages thread to start displaying the screens
            //ThreadStart setClickVolumeThreadStater = new ThreadStart(SetClickVolumeThread);
            //Thread setVolumeThread = new Thread(setClickVolumeThreadStater);
            //setVolumeThread.Start();
            
        }

        /// <summary>
        /// A separate thread to set the click volume BACK to click sound. No need to spun thread in all the cases of setting the volume since
        /// in some scenarios, it is better to be sequential like setting the volume in settings screen. The user has to hear the sound before
        /// he/she takes the next action.
        /// </summary>
        //private void SetClickVolumeThread()
        //{
        //    lock (GuiDefines.ClickVolumeLock)
        //    {
        //        SetWaveOutVolume(GuiDefines.ClickVolume);
        //    }
        //}
    }
}