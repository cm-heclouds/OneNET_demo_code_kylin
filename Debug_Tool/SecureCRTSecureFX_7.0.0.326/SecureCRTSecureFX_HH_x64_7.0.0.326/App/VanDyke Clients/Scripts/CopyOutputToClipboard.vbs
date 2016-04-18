# $language = "VBScript"
# $interface = "1.0"
' CopyOutputToClipboard.vbs
'
' Description:
'   This script example is designed to run while connected to a Cisco Pix
'   firewall or other router device.
'   
'   When launched, the following commands are sent to the device:
'      enable
'      configure terminal
'      pager 0
'      show config
'      show serial
'      show sysopt
'      exit
'      exit
'  
'  The results of the 'show config', 'show serial', and 'show sysopt' commands
'  are captured and stored within the Windows clipboard.  A messagebox then
'  displays the contents of the Windows clipboard.
'   
' Demonstrates:
'   - Use of crt.Screen.ReadString to capture output from a command as echoed
'     from the remote system.
'   - How to copy data from a script variable into the Windows clipboard
'     using the crt.Clipboard object (available in SecureCRT 6.1.0 and later).
'   - Use of the ReadString method to either look for a shell
'     prompt (so it can be determined when a remote command has
'     completed) or some other entire line of data.
'   - How to use the VBScript Replace() method to substitute text within
'     an existing string.


' Using GetScriptTab() will make this script 'tab safe' in that all of the
' script's functionality will be carried out on the correct tab. From here
' on out we'll use the objTab object instead of the crt object.
Dim g_objTab
Set g_objTab = crt.GetScriptTab

g_objTab.Screen.Synchronous = True


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sub Main()
    if g_objTab.Session.Connected = False then
        MsgBox "This script currently requires a valid connection to a " & _
            "Cisco Pix firewall or other similar device." & vbcrlf & vbcrlf & _
            "Please connect and then run this script again."
        exit sub
    end if
    g_objTab.Screen.Synchronous = True

    ' We'll use a "SendExpect" function we wrote (see below) to help us
    ' easily send commands and wait for the remote device to be ready for
    ' us to send the next command (by waiting for text we assume indicates
    ' the remote device is ready -- a prompt for example)
    If Not SendExpect("enable", "ssword:") then exit sub
    If Not SendExpect("", "# ") then exit sub
    If Not SendExpect("configure terminal", "(config)# ") then exit sub
    If Not SendExpect("pager 0", "(config)# ") then exit sub

    ' Now, we'll use another function we wrote called "CaptureOutputOfCommand",
    ' which takes as its first parameter the command to run, and as the 2nd
    ' parameter, the text that indicates the command has completed -- the prompt
    ' in most cases.
    szData = CaptureOutputOfCommand("show config", "(config)# ")
    szData = szData & CaptureOutputOfCommand("show serial", "(config)# ")
    szData = szData & CaptureOutputOfCommand("show sysopt", "(config)# ")  

    ' Some Cisco devices send EOL as LFCR rather than CRLF.  If we find the
    ' LFCR pattern, let's replace it with the Windows CRLF.
    szData = Replace(szData, vblf & vbcr, vbcrlf)

    ' Now that we have captured the output of each of the 'show' commands, let's
    ' send the data to the windows clipboard.
    crt.Clipboard.Format = "CF_TEXT"
    crt.Clipboard.Text = szData
    
    ' Now get back to a normalized state on the remote device
    If Not SendExpect("exit", "# ") then exit sub
    If Not SendExpect("exit", "> ") then exit sub

    MsgBox "Text is now in the clipboard: " & vbcrlf & vbcrlf & _
        crt.Clipboard.Text

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function SendExpect(szSend, szExpect)
  ' Returns true if the text in 'szSend' was successfully sent and
  ' the text in 'szExpect' was successfully found as a result.

    ' If we're not connected, we can't possibly return true, or even send/recv
    ' text
    if g_objTab.Session.Connected <> True then exit function
        
    g_objTab.Screen.Send szSend & vbcr
    g_objTab.Screen.WaitForString szExpect

    SendExpect = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function CaptureOutputOfCommand(szCommand, szPrompt)
    if g_objTab.Session.Connected <> True then
        CaptureOutputOfCommand = "[ERROR: Not Connected.]"
        exit function
    end if
    
    ' First, send the command to the remote.
    g_objTab.Screen.Send szCommand & vbcr
    
    ' Second, wait for the carriage return to be echoed by the remote device.
    ' This allows us to capture only the output of the command, not the line
    ' on which the command was issued (which would include the prompt + cmd).
    ' If you want to capture the command that was issued, simply comment out
    ' the following line of code.
    g_objTab.Screen.WaitForString vbcr
    
    ' Now that the command has been sent, use Screen.ReadString to capture
    ' all the data that is received up to the point at which the shell
    ' prompt appears (the captured data does not include the shell prompt).
    CaptureOutputOfCommand = g_objTab.Screen.ReadString(szPrompt)
End Function
