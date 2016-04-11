# $language = "VBScript"
# $interface = "1.0"
' SendCommandToAllTabs.vbs
'
' Description:
'   This example script shows how to send the same command to all tabs
'   that currently have an active connection.
' 
' Demonstrates:
'   - How to send a command to all tabs using the tab scripting capability
'     of SecureCRT
'   - How to prompt user for input using the crt.Dialog.Prompt() method.
'   - How to access the total number of Tabs in the SecureCRT window
'     by using the crt.GetTabCount property
'   - How to Activate a tab in the UI from within a script.
'   - How to use the Session.Connected property to determine if a tab
'     currently has an active connection or not.
'   - How to send data to a tab using the screen.Send method in SecureCRT.

Dim g_objTab, g_szSkippedTabs

' Get a reference to the tab that was active when this script was launched.
Set g_objTab = crt.GetScriptTab

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sub Main()
   
    ' Ask user what command should be sent to each tabs
    Dim szCommand
    szCommand = crt.Dialog.Prompt("Enter command to be sent to all tabs:", _
                                  "Send To All Connected Tabs", "ls -alR", False)
    if szCommand = "" then exit sub
    
    ' Give the user a chance to review the command and cancel if they notice
    ' something wrong.
    If crt.Dialog.MessageBox(_
        "Are you sure you want to send the following command to " & _
        "__ALL__ tabs?" & vbcrlf & vbcrlf & szCommand, _
        "Send Command To All Tabs - Confirm", _
        vbyesno) <> vbyes then exit sub
    
    ' Activate each tab in order from left to right, and issue the command in
    ' each "connected" tab...
    Dim nIndex
    For nIndex = 1 to crt.GetTabCount
        Set objCurrentTab = crt.GetTab(nIndex)
        objCurrentTab.Activate
        ' Skip tabs that aren't connected
        if objCurrentTab.Session.Connected = True then
            objCurrentTab.Screen.Send szCommand & vbcr
        else
            if g_szSkippedTabs = "" then
                g_szSkippedTabs = nIndex
            else
                g_szSkippedTabs = g_szSkippedTabs & "," & nIndex
            end if
        end if
    Next
    
    ' Now, activate the original tab on which the script was started
    g_objTab.Activate

    ' Determine if there were any skipped tabs, and prepare a message for
    ' displaying at the end.
    if g_szSkippedTabs <> "" then
        g_szSkippedTabs = vbcrlf & vbcrlf & _
            "The following tabs did not receive the command because" & vbcrlf & _
            "they were not connected at the time:" & _
            vbcrlf & vbtab & g_szSkippedTabs
    end if

    crt.Dialog.MessageBox _
        "The following command was sent to all connected tabs:" & vbcrlf & _
        vbtab & szCommand & g_szSkippedTabs

End Sub

