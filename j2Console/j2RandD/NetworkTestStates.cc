#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "NetworkTestStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"
#include "ParseSettings.h"
#include "ParseSystem.h"

EthernetPortInfo_t  StateNetworkTestGlobal::ethernetPortInfo;
unsigned char       StateNetworkTestGlobal::configChanged;
StringId_t          StateNetworkTestGlobal::errString = STR_BLANK;
StringId_t          StateNetworkTestGlobal::errInfoString = STR_BLANK;

bool
StateNetworkTestGlobal::
ValidateIp (char * ip)
{
    char *  p;
    char *  ipCopy;
    int     i;
    int     value;
   
    // First check for two or more '.'s in a row, since strtok
    // will treat them the same as a single '.'.
    if (strstr( ip, "..") != 0) {
        return false;
    }

    // Make a copy of the ip string, since strtok will modify it.
    ipCopy = _strdup (ip);
    if (ipCopy == 0) {
        return false;
    }

    // Now check the value of each of the '.'-delimited fields.
    p = strtok (ipCopy, ".");
    i = 0;
    while (p != 0) {
        value = atoi (p);
        if ((value < 0) || (value > 255)) {
            return false;
        }
        p = strtok (0, ".");
        i++;
    }

    // Finally, verify that we got exactly four fields - no more
    // and no fewer.
    if (i != 4) {
        return false;
    }

    // All checks passed, so return true.
    return true;
}


bool
StateNetworkTestGlobal::
ValidateSocket (char * socket)
{
    int value;

    // Check string length so atoi doesn't overflow and also to make sure
    // it's not blank.
    if ((strlen( socket) == 0) || (strlen( socket) > 5)) {
        return false;
    }

    // Now make sure we're in range.
    value = atoi (socket);
    if ((value < 0) || (value > 0xFFFF)) {
        return false;
    }

    // Legal value.
    return true;
}


void
StateNetworkConfigError::
Enter()
{
    GuiReceiveMsg_t     guiMsg;

    guiMsg.msgId = MSG_GUI_WIN_ERROR;

    wcscpy (guiMsg.data.winError.topLabel, strings[TSTR_ERROR]);
    wcscpy (guiMsg.data.winError.message, strings [errString]);
    wcscpy( guiMsg.data.winError.buttonText, strings [BSTR_CONTINUE]);
    SendGuiMessage(&guiMsg, sizeof(GuiWinWarningData_t));
}

int
StateNetworkConfigError::
ProcessMessage (int msgId, char * data)
{
    int     retVal = STATE_NETWORK_CONFIG_ERROR;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:     // Continue
            retVal = stateStack.Pop();
            break;

        case MSG_GUI_OUT_INFO:
            infoMsg = strings [errInfoString];
            retVal = STATE_INFO;
            stateStack.Push (STATE_NETWORK_CONFIG_ERROR);
            break;

        default:
            break;
    }
   
    return retVal;
}




// Network Test state  
void
StateNetworkTest::
Enter (void)
{
    GuiReceiveMsg_t             msg;
    wchar_t                     *topLable = L"Network Set";
    GuiMiddleOptionInfoData_t   *data = &msg.data.middleOptionInfo;
    configChanged = 0;

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_OPTION_INFO;

    memset (data, 0, sizeof (GuiMiddleOptionInfoData_t));
    wcscpy (data->buttonText[0], L"DHCP");
    wcscpy (data->buttonText[1], L"Static Ip");
    if (settings->comm.dhcpEnabled) {
        data->selectedButton = 0;
    } else {
        data->selectedButton = 1;
    }
    SendGuiMessage (&msg, sizeof( GuiMiddleOptionInfoData_t));

    SetBottomButtons (strings[BSTR_BACK], true);

    GetEthernetPortInfo (&ethernetPortInfo);
     
}

// Network test
state_t
StateNetworkTest::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_NETWORK_TEST;
    int                         idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_TOGGLE:
            switch (idata) {
                case 0xF0:      //enable dhcp
                case 0:
                    if (ethernetPortInfo.dhcpEnabled == false) {
                        stateStack.Push (STATE_NETWORK_TEST);
                        retVal = STATE_NETWORK_CONFIGURING;
                        configChanged = DHCP_ENABLE;
                    } else {
                        stateStack.Push (STATE_NETWORK_TEST);
                        retVal = STATE_NETWORK_CONFIG;
                    } 
                break;

                case 0xF1:      //static ip address
                case 1:
                    if (ethernetPortInfo.dhcpEnabled == true) {
                       stateStack.Push (STATE_NETWORK_TEST);
                       retVal = STATE_NETWORK_CONFIGURING;
                       configChanged = DHCP_DISABLE;
                    
                    } else {
                        stateStack.Push (STATE_NETWORK_TEST);
                        retVal = STATE_NETWORK_CONFIG;
                    }
                break;

                default:
                break;
            }
        break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            retVal = stateStack.Pop();
        break;

        default:
        break;
    }
    return retVal;
}



// network test configuration 
void
StateNetworkConfig::
Enter (void)
{
    GuiReceiveMsg_t             msg;
    wchar_t                     *topLable = L"Network Configuration";
    GuiMiddleListData_t *       data = &msg.data.middleList;
    int                         i;
   
    SetTopLabel (topLable);

    topItem = 0;

    strcpy (label[0], "DHCP Enable");
    if (ethernetPortInfo.dhcpEnabled) {
        strcpy (value[0], "Yes");
    } else {
        strcpy (value[0], "No");
    }
    strcpy (label[1], WstrToCstr (strings [MSTR_ANALYZER_IP]));
    strcpy (value[1], ethernetPortInfo.analyzerIp);
    strcpy (label[2], WstrToCstr (strings [MSTR_IP_MASK]));
    strcpy (value[2], ethernetPortInfo.ipMask);
    strcpy (label[3], WstrToCstr (strings [MSTR_GATEWAY_IP]));
    strcpy (value[3], ethernetPortInfo.gateway);
    strcpy (label[4], WstrToCstr (strings [MSTR_SERVER_IP]));
    if (ethernetPortInfo.dhcpEnabled) {
        strcpy (value[4], ethernetPortInfo.serverIp);
    } else {
        strcpy (value[4], settings->comm.serverIp);
    }
    strcpy (label[5], WstrToCstr (strings[MSTR_SERVER_SOCKET]));
    sprintf (value[5], "%d", settings->comm.serverSocket);
    strcpy (label[6], "Host Name");
    strcpy (value[6], ethernetPortInfo.hostName);
    strcpy (label[7],"Mac Address");
    strcpy (value[7], ethernetPortInfo.macAddress);
    strcpy (label[8],"Telnet Enable");
    if (systemData->rdEnableFtpTelnet) {
        strcpy (value[8], "Yes");
    } else {
        strcpy (value[8], "No");
    }
    numItems = 9;

    msg.msgId = MSG_GUI_MIDDLE_LIST_SETTINGS;

    memset (data, 0, sizeof( GuiMiddleListData_t));
    for (i = 0; i < 5 && i < numItems; i++) {
        wcscpy( data->items[i][0], CstrToWstr (label[i]));
        wcscpy( data->items[i][1], CstrToWstr (value[i]));
    }
    data->isSelectable = 1;
    data->hasUpDown = 1;

    SendGuiMessage (&msg, sizeof( GuiMiddleListData_t));

    SetBottomButtons(strings [BSTR_BACK], true);
}

// Network test
state_t
StateNetworkConfig::
ProcessMessage (int msgId, char *data)
{

    state_t retVal = STATE_NETWORK_CONFIG;
    int     idata = *(int *)data;
    bool    doUpdate = false;

    switch (msgId) {

        case MSG_GUI_OUT_UP:
            if (topItem > 0) {
                topItem -= 5;
                if (topItem < 0) {
                    topItem = 0;
                }
                doUpdate = true;
            }
            break;

        case MSG_GUI_OUT_DOWN:
            if (topItem + 5 <numItems) {
                topItem += 5;
                doUpdate = true;
            }
            break;


        case MSG_GUI_OUT_LIST:
            switch (topItem + idata) {
                case 1: // Analyzer ID
                    if (ethernetPortInfo.dhcpEnabled == 0) {
                        retVal = STATE_ENTER_ANALYZER_IP;
                        stateStack.Push (STATE_NETWORK_CONFIG);
                    } 
                break;
                case 2: // IP Mask
                    if (ethernetPortInfo.dhcpEnabled == 0) {
                        retVal = STATE_ENTER_ANALYZER_MASK;
                        stateStack.Push (STATE_NETWORK_CONFIG);
                    }
                break;
                case 3: // Gateway IP
                    if (ethernetPortInfo.dhcpEnabled == 0) {
                        retVal = STATE_ENTER_GATEWAY;
                        stateStack.Push (STATE_NETWORK_CONFIG);
                    }
                break;
                case 4: // Server IP
                    if (ethernetPortInfo.dhcpEnabled == 0) {
                        retVal = STATE_ENTER_SERVER_IP;
                        stateStack.Push (STATE_NETWORK_CONFIG);
                    }
                break;

                case 5: // Server Socket
                    retVal = STATE_ENTER_SERVER_SOCKET;
                    stateStack.Push (STATE_NETWORK_CONFIG);
                break;

                case  6: // Host name
                    retVal = STATE_ENTER_HOST_NAME; 
                    stateStack.Push (STATE_NETWORK_CONFIG);
                break;

                case 8: // telnet 
                    retVal = STATE_TELNET_CONFIG;
                    stateStack.Push (STATE_NETWORK_CONFIG);
                break;

                default:

                break;
            }
            break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:     // Back
					retVal = stateStack.Pop();
                break;

                default:
                break;
            }
            break;
        default:
        break;
    }

    if (doUpdate) {
        GuiReceiveMsg_t         msg;
        GuiMiddleListData_t *   data = &msg.data.middleList;
        int                     i, j;

        msg.msgId = MSG_GUI_UPDATE_LIST_SETTINGS;
        memset( data, 0, sizeof( GuiMiddleListData_t ) );

        for (i = 0, j = topItem; i < 5 && j < numItems; i++, j++) {
            wcscpy (data->items[i][0], CstrToWstr (label[j]));
            wcscpy (data->items[i][1], CstrToWstr (value[j]));
        }

        data->isSelectable = 1;
        data->hasUpDown = 1;

        SendGuiMessage (&msg, sizeof(GuiMiddleListData_t));
    }
    return retVal;
}

void
StateEnterAnalyzerIp::
Enter()
{
    GuiReceiveMsg_t msg;

    SetTopLabel (strings[TSTR_ENTER_ANALYZER_IP_ADDRESS]);

    msg.msgId = MSG_GUI_MIDDLE_ENTER_IP;
    msg.data.middleEnterIp.text[0] = '\0';
    SendGuiMessage (&msg, sizeof (GuiMiddleEnterIpData_t));

    SetBottomButtons (strings [BSTR_BACK], false, strings [BSTR_SAVE], true);

    memset (value, 0, sizeof(value));
}

state_t
StateEnterAnalyzerIp::
ProcessMessage (int msgId, char * data)
{
    state_t         retVal =STATE_ENTER_ANALYZER_IP;
    int             idata = ((GuiSendMsgData_t *)data)->idata;
    GuiReceiveMsg_t msg;

    switch (msgId) {
        case MSG_GUI_OUT_BUTTON:
            if (idata == -1) {
                // 'C' clear button 
                memset (value, 0, sizeof (value));
            } else if  (strlen (value) < (sizeof(value) - 1)) {
                if ((idata >= 0) && (idata <= 9)) {
                    value [strlen(value)] = idata + '0';
                } else if (idata == -2) {   
                    // '.'
                    value[strlen( value )] = '.';
                }
            }
            // display enter value
            msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
            wcscpy (msg.data.middleEnterIp.text, CstrToWstr (value));
            SendGuiMessage (&msg, sizeof (GuiMiddleEnterIpData_t));
            break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:     // Back
                    retVal = stateStack.Pop();
                    break;
                case 1:     // Save
                    if (ValidateIp (value) == true) {
                        LogDebug ("new ip =%s", value);
                        strcpy (settings->comm.analyzerIp, value);
                       // strcpy (ethernetPortInfo.analyzerIp, value);
                        configChanged |= IP_CHANGED;
                        //retVal = stateStack.Pop();
                        retVal = STATE_NETWORK_CONFIGURING;
                    } else {
                        errString = ESTR_ILLEGAL_IP_ADDRESS;
                        errInfoString = EISTR_ILLEGAL_IP_ADDRESS;
                        retVal = STATE_NETWORK_CONFIG_ERROR;
                        stateStack.Push (STATE_ENTER_ANALYZER_IP);
                    }
                    break;
                default: break;
            }
            break;

        default:
            break;
    }
    return retVal;
}

void
StateEnterAnalyzerMask::
Enter()
{
    GuiReceiveMsg_t msg;

    SetTopLabel (strings [TSTR_ENTER_IP_MASK]);

    msg.msgId = MSG_GUI_MIDDLE_ENTER_IP;
    msg.data.middleEnterIp.text[0] = '\0';
    SendGuiMessage (&msg, sizeof (GuiMiddleEnterIpData_t));

    SetBottomButtons (strings [BSTR_BACK], false, strings [BSTR_SAVE], true);

    memset (value, 0, sizeof(value));
}

state_t
StateEnterAnalyzerMask::
ProcessMessage (int msgId, char * data)
{
    state_t         retVal =STATE_ENTER_ANALYZER_MASK;
    int             idata = ((GuiSendMsgData_t *)data)->idata;
    GuiReceiveMsg_t msg;

    switch (msgId) {
        case MSG_GUI_OUT_BUTTON:
            if (idata == -1) {
                // 'C' clear button 
                memset (value, 0, sizeof (value));
            } else if  (strlen (value) < (sizeof(value) - 1)) {
                if ((idata >= 0) && (idata <= 9)) {
                    value [strlen(value)] = idata + '0';
                } else if (idata == -2) {   
                    // '.'
                    value[strlen( value )] = '.';
                }
            }
            // display enter value
            msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
            wcscpy (msg.data.middleEnterIp.text, CstrToWstr (value));
            SendGuiMessage (&msg, sizeof (GuiMiddleEnterIpData_t));
            break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:     // Back
                    retVal = stateStack.Pop();
                    break;
                case 1:     // Save
                    if (ValidateIp (value) == true) {
                        strcpy (settings->comm.ipMask, value);
                        //strcpy (ethernetPortInfo.ipMask, value);
                        configChanged |= IP_CHANGED;
                        retVal = STATE_NETWORK_CONFIGURING;
                        //retVal = stateStack.Pop();
                    } else {
                        errString = ESTR_ILLEGAL_IP_MASK;
                        errInfoString = EISTR_ILLEGAL_IP_MASK;
                        retVal = STATE_NETWORK_CONFIG_ERROR;
                        stateStack.Push (STATE_ENTER_ANALYZER_MASK);
                    }
                    break;
                default: break;
            }
            break;

        default:
            break;
    }
    return retVal;
}


void
StateEnterGateway::
Enter()
{
    GuiReceiveMsg_t msg;

    SetTopLabel (strings [TSTR_ENTER_GATEWAY_IP_ADDRESS]);

    msg.msgId = MSG_GUI_MIDDLE_ENTER_IP;
    msg.data.middleEnterIp.text[0] = '\0';
    SendGuiMessage (&msg, sizeof (GuiMiddleEnterIpData_t));

    SetBottomButtons (strings [BSTR_BACK], false, strings [BSTR_SAVE], true);

    memset (value, 0, sizeof(value));
}

state_t
StateEnterGateway::
ProcessMessage (int msgId, char * data)
{
    state_t         retVal = STATE_ENTER_GATEWAY;
    int             idata = ((GuiSendMsgData_t *)data)->idata;
    GuiReceiveMsg_t msg;

    switch (msgId) {
        case MSG_GUI_OUT_BUTTON:
            if (idata == -1) {
                // 'C' clear button 
                memset (value, 0, sizeof (value));
            } else if  (strlen (value) < (sizeof(value) - 1)) {
                if ((idata >= 0) && (idata <= 9)) {
                    value [strlen(value)] = idata + '0';
                } else if (idata == -2) {   
                    // '.'
                    value[strlen( value )] = '.';
                }
            }
            // display enter value
            msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
            wcscpy (msg.data.middleEnterIp.text, CstrToWstr (value));
            SendGuiMessage (&msg, sizeof (GuiMiddleEnterIpData_t));
            break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:     // Back
                    retVal = stateStack.Pop();
                    break;
                case 1:     // Save
                    if (ValidateIp (value) == true) {
                        strcpy (settings->comm.gateway, value);
                       // strcpy (ethernetPortInfo.gateway, value);
                        configChanged |= IP_CHANGED;
                        //retVal = stateStack.Pop();
                        retVal = STATE_NETWORK_CONFIGURING;
                    } else {
                        errString = ESTR_ILLEGAL_IP_ADDRESS; 
                        errInfoString = ESTR_ILLEGAL_IP_ADDRESS;
                        retVal = STATE_NETWORK_CONFIG_ERROR;
                        stateStack.Push (STATE_ENTER_GATEWAY);
                    }
                    break;
                default: break;
            }
            break;

        default:
            break;
    }
    return retVal;
}


void
StateEnterServerIp::
Enter()
{
    GuiReceiveMsg_t msg;

    SetTopLabel (strings [TSTR_ENTER_SERVER_IP_ADDRESS]);

    msg.msgId = MSG_GUI_MIDDLE_ENTER_IP;
    msg.data.middleEnterIp.text[0] = '\0';
    SendGuiMessage (&msg, sizeof (GuiMiddleEnterIpData_t));

    SetBottomButtons (strings [BSTR_BACK], false, strings [BSTR_SAVE], true);

    memset (value, 0, sizeof(value));
}

state_t
StateEnterServerIp::
ProcessMessage (int msgId, char * data)
{
    state_t         retVal = STATE_ENTER_SERVER_IP;
    int             idata = ((GuiSendMsgData_t *)data)->idata;
    GuiReceiveMsg_t msg;

    switch (msgId) {
        case MSG_GUI_OUT_BUTTON:
            if (idata == -1) {
                // 'C' clear button 
                memset (value, 0, sizeof (value));
            } else if  (strlen (value) < (sizeof(value) - 1)) {
                if ((idata >= 0) && (idata <= 9)) {
                    value [strlen(value)] = idata + '0';
                } else if (idata == -2) {   
                    // '.'
                    value[strlen( value )] = '.';
                }
            }
            // display enter value
            msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
            wcscpy (msg.data.middleEnterIp.text, CstrToWstr (value));
            SendGuiMessage (&msg, sizeof (GuiMiddleEnterIpData_t));
            break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:     // Back
                    retVal = stateStack.Pop();
                    break;
                case 1:     // Save
                    if (ValidateIp (value) == true) {
                        // for serer, only saved the value
                        strcpy (settings->comm.serverIp, value);
                        WriteSettings (settings);
                        retVal = stateStack.Pop();
                    } else {
                        errString = ESTR_ILLEGAL_IP_ADDRESS; 
                        errInfoString = ESTR_ILLEGAL_IP_ADDRESS;
                        retVal = STATE_NETWORK_CONFIG_ERROR;
                        stateStack.Push (STATE_ENTER_SERVER_IP);
                    }
                    break;
                default: break;
            }
            break;

        default:
            break;
    }
    return retVal;
}

void
StateEnterServerSocket::
Enter()
{
    GuiReceiveMsg_t msg;

    SetTopLabel (strings [TSTR_ENTER_SERVER_SOCKET]);

    msg.msgId = MSG_GUI_MIDDLE_ENTER_IP;
    msg.data.middleEnterIp.text[0] = '\0';
    SendGuiMessage (&msg, sizeof (GuiMiddleEnterIpData_t));

    SetBottomButtons (strings [BSTR_BACK], false, strings [BSTR_SAVE], true);

    memset (value, 0, sizeof(value));
}

state_t
StateEnterServerSocket::
ProcessMessage (int msgId, char * data)
{
    state_t         retVal = STATE_ENTER_SERVER_SOCKET;
    int             idata = ((GuiSendMsgData_t *)data)->idata;
    GuiReceiveMsg_t msg;

    switch (msgId) {
        case MSG_GUI_OUT_BUTTON:
            if (idata == -1) {
                // 'C'
                memset(value, 0, sizeof (value));
            } else if (strlen (value) < (sizeof (value ) - 1)) {
                if ((idata >= 0) && (idata <= 9)) {
                    value [strlen (value)] = idata + '0';
                }
            }

            msg.msgId = MSG_GUI_UPDATE_ENTER_IP;
            wcscpy( msg.data.middleEnterIp.text, CstrToWstr (value) );
            SendGuiMessage (&msg, sizeof( GuiMiddleEnterIpData_t));


            break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:     // Back
                    retVal = stateStack.Pop();
                    break;
                case 1:     // Save
                    if (ValidateSocket (value) == true) {
                        settings->comm.serverSocket = atoi (value);
                        WriteSettings (settings);
                        retVal = stateStack.Pop();
                    } else {
                        errString = ESTR_ILLEGAL_SOCKET; 
                        errInfoString = ESTR_ILLEGAL_SOCKET;
                        retVal = STATE_NETWORK_CONFIG_ERROR;
                        stateStack.Push (STATE_ENTER_SERVER_SOCKET);
                    }
                    break;
                default: break;
            }
            break;

        default:
            break;
    }
    return retVal;
}

void
StateEnterHostname::
Enter()
{
    GuiReceiveMsg_t msg;
    wchar_t         *topLable = L"Enter Analyzer Host Name";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
    msg.data.middleEnterId.isPassword = 0;
    wcscpy( msg.data.middleEnterId.initialValue, L"" );
    SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );

    SetBottomButtons (strings [BSTR_BACK], false, strings [BSTR_SAVE], true);

    memset (value, 0, sizeof(value));
}

state_t
StateEnterHostname::
ProcessMessage (int msgId, char * data)
{
    state_t         retVal = STATE_ENTER_HOST_NAME;
    int             idata = *(int *)data;
    char *          sdata = (char *)data;

    GuiReceiveMsg_t msg;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:     // Back
                    retVal = stateStack.Pop();
                    break;
                case 1:     // Save, get ID text
                    msg.msgId = MSG_GUI_GET_ID_TEXT;
                    SendGuiMessage( &msg, 0 );
                    break;
                default: break;
            }
            break;

        case MSG_GUI_OUT_ACTIVATE:  // Same as Done.
            msg.msgId = MSG_GUI_GET_ID_TEXT;
            SendGuiMessage (&msg, 0);
        break;

        case MSG_GUI_OUT_ID_TEXT:
            //update host name
            strcpy (settings->comm.hostName, sdata);
            //update driver host name
            LogDebug ("set new hostname:%s", settings->comm.hostName);
            //stateStack.Push (STATE_ENTER_HOST_NAME);
            retVal = STATE_NETWORK_CONFIGURING;
            configChanged = HOST_NAME_CHANGED;
            break;

        default:
            break;
    }
    return retVal;
}

int
StateNetworkConfiguring::
ConfigureNetwork (void)
{
    int ret = 0;

    if (configChanged & DHCP_DISABLE) {
        // disable dhcp
        if (SetDhcp (systemData->cpuBoard, false, settings->comm.hostName) == 0) {
            settings->comm.dhcpEnabled = false;
            WriteSettings (settings);
            //enable static ip address
            if (SetStaticIp (systemData->cpuBoard, settings) != 0) {
                ret = 1;
            }
        } else {
            ret = 1;
        }
    }
    if (configChanged & DHCP_ENABLE) {
         // enable dhcp
        if (SetDhcp (systemData->cpuBoard, true, settings->comm.hostName) == 0) {
            //update settings
            settings->comm.dhcpEnabled = true;
            WriteSettings (settings);
        } else {
            ret =1;
        }
    }
    if (configChanged & IP_CHANGED) {
        if (SetStaticIp (systemData->cpuBoard, settings) == 0) {
            WriteSettings (settings);
        } else {
            ret =1;
        }
    }
    if (configChanged & HOST_NAME_CHANGED) {
        if (SetHostName (settings->comm.hostName) == 0) {
            WriteSettings (settings);
        } else {
            ret =1;
        }
    }
    return ret;
}

// configure network 
void
StateNetworkConfiguring::
Enter (void)
{
    GuiReceiveMsg_t     msg;
    ManufactureMsg_t    mfMsg;

    SetTopLabel (0);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, L"Network Configuring...");
    SendGuiMessage (&msg, sizeof (GuiMiddleMessageData_t ));
    SetBottomButtons ();

    // configurate network driver
    mfMsg.idata = ConfigureNetwork ();
    // wait 1 second to boot up network
    Sleep (1000);
    
    mfMsg.msgId = MSG_MANUFACTURE_NEXTWORK_CONFIG_COMPLETE;
    SendManufactureMessage (&mfMsg, sizeof (ManufactureMsg_t));;
}

// configuration done 
state_t
StateNetworkConfiguring::
ProcessMessage (int msgId, char *data)
{
     int                         retVal = STATE_NETWORK_CONFIGURING;

    switch (msgId) {
        case MSG_MANUFACTURE_NEXTWORK_CONFIG_COMPLETE:
            if (*data == 0) {
                GetEthernetPortInfo (&ethernetPortInfo);
                retVal = stateStack.Pop();
            } else {
                // goto error windows
                if ((configChanged & DHCP_DISABLE) || (configChanged & DHCP_ENABLE))  {
                    infoText.type = NETWORK_DHCP_TYPE;
                } else if (configChanged & IP_CHANGED) {
                    infoText.type = NETWORK_IP_TYPE;
                } else if (configChanged & HOST_NAME_CHANGED) {
                    infoText.type = NETWORK_HOST_NAME_TYPE;
                }
                retVal = STATE_MANUFACTURING_ERROR;
            }
            // reset flag 
            configChanged = 0;
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

     return retVal;

}

// telnet enable  
void
StateTelnetConfig::
Enter (void)
{
    GuiReceiveMsg_t             msg;
    wchar_t                     *topLable = L"Telnet Configuration";
    GuiMiddleOptionInfoData_t   *data = &msg.data.middleOptionInfo;
    configChanged = 0;

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_OPTION_INFO;

    memset (data, 0, sizeof (GuiMiddleOptionInfoData_t));
    wcscpy (data->buttonText[0], L"Enable");
    wcscpy (data->buttonText[1], L"Disable");
    if (systemData->rdEnableFtpTelnet) {
        data->selectedButton = 0;
    } else {
        data->selectedButton = 1;
    }
    SendGuiMessage (&msg, sizeof( GuiMiddleOptionInfoData_t));

    SetBottomButtons (strings[BSTR_BACK], true);
}

//Telnet configuration 
state_t
StateTelnetConfig::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_TELNET_CONFIG;
    int                         idata = *(int *)data;
    DWORD                       enable;

    switch (msgId) {
        case MSG_GUI_OUT_TOGGLE:
            switch (idata) {
                case 0xF0:      //enable Telnet 
                case 0:
                    if (systemData->rdEnableFtpTelnet == 0) {
                        systemData->rdEnableFtpTelnet = 1;
                        WriteSystem (systemData);
                        enable = 1;
                        SetTelentFtp (enable);
                    }
                break;

                case 0xF1:      //static ip address
                case 1:
                    if (systemData->rdEnableFtpTelnet == 1) {
                        systemData->rdEnableFtpTelnet = 0;
                        WriteSystem (systemData);
                        // only both system telnet and randD telnet disable
                        // call disable telent function
                        if (systemData->enableFtpTelnet ==0) {
                            enable = 0;
                            SetTelentFtp (enable);
                        }
                    }
                break;

                default:
                break;
            }
            retVal = stateStack.Pop();
        break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            retVal = stateStack.Pop();
        break;

        default:
        break;
    }
    return retVal;
}

