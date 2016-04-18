#$language = "VBScript"
#$interface = "1.0"
' AutoResponder.vbs
'
' Description:
'    Demonstrates how to set up a list of commands/text that get sent to the
'    remote as soon as a corresponding "trigger" string/text is detected from
'    the remote system. 
'    Once launched, this script will run indefinitely until the session is
'    disconnected or the application is closed.  To stop the script, choose
'    "Cancel" from SecureCRT's main "Script" menu.
'
' Demonstrates:
'  - How to use the Session.Connected property to determine if the current
'    tab has an active connection.
'  - How to populate hard-coded array values in VBScript.
'  - How to wait for one of multiple string values to appear, and detect
'    which one was found using the Screen.WaitForStrings method.
'

g_vWaitFors = Array("Waiting for 1", _
                    "Waiting for 2", _
                    "Waiting for 3", _
                    "Waiting for 4", _
                    "Waiting for 5", _
                    "Waiting for 6", _
                    "Waiting for 7", _
                    "Waiting for 8", _
                    "Waiting for 9", _
                    "Waiting for 10")

g_vRespondWiths = Array("Command 1", _
                        "Command 2", _
                        "Command 3", _
                        "Command 4", _
                        "Command 5", _
                        "Command 6", _
                        "Command 7", _
                        "Command 8", _
                        "Command 9", _
                        "Command 10")

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sub Main()
    Set objTab = crt.GetScriptTab

    If objTab.Session.Connected <> True then
        crt.Dialog.MessageBox _
            "Error." & vbcrlf & _
            "This script was designed to be launched after a valid " & _
            "connection is established." & vbcrlf & vbcrlf & _
            "Please connect to a remote machine before running this script."
        Exit Sub
    end if
    
    ' Ensure that we don't "miss" data coming from the remote by setting
    ' our Screen's Synchronous flag to true.
    objTab.Screen.Synchronous = true

    Do
        ' WaitForStrings as called here will wait for an occurrence of one
        ' of the strings that is part of the g_vWaitFors array defined
        ' above.  When one of the strings is detected, the index of the
        ' string within the array will be returned as the value of the
        ' nIndex variable.  Note that nIndex will be 1-based, rather than
        ' 0-based.
        nIndex = objTab.Screen.WaitForStrings(g_vWaitFors)
        
        ' Now that we've discovered text in the "nIndex"th item of our array,
        ' send the "nIndex"th item of the g_vRespondWiths array:
        objTab.Screen.Send g_vRespondWiths(nIndex - 1) & vbcr
        
        ' Now that we've sent the corresponding command, loop back up to the
        ' top and wait again for another trigger string to appear...
    Loop

End Sub
