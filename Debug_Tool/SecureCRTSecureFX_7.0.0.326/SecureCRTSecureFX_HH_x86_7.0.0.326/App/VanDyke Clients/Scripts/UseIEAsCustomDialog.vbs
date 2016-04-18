# $language = "VBScript"
# $interface = "1.0"
' UseIEAsCustomDialog.vbs
'
' Description:
'   This sample script shows how the Internet Explorer automation object
'   can be used to create custom dialog that can be used within a SecureCRT
'   script.  In this example, an "Send...Expect" dialog is created and
'   displayed to the user for input.  Then each "Send"  value is sent to
'   the remote machine, and the script waits until the corresponding "Expect"
'   value is received from the remote.
'
'   Although one could actually create a HTML "form" and store it in an
'   actual .html file and then navigate to it, in this example, the IE
'   object is navigated to "about:blank", and then generates the HTML
'   document (aka "Dialog") programmatically.

Dim g_objIE, g_objTab
set g_objTab = crt.GetScriptTab

Dim g_fso, g_shell
Set g_fso = CreateObject("Scripting.FileSystemObject")
Set g_shell = CreateObject("WScript.Shell")

g_objTab.Screen.Synchronous = True

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sub Main()
    
    if Not g_objTab.Session.Connected then
        MsgBox "This script was designed to be launched from a tab " & _
               "that is already connected to a remote machine."
        exit sub
    end if
    
    Dim szExpect1, szExpect2, szExpect3
    Dim szSend1, szSend2
    Dim bLogOutput, szLogFile, szAppendOrOverwrite
    
    ' Set up default values for the dialog. These values are passed to the
    ' PromptForInput() function as "ByRef" references, so their values will
    ' be modified therein and will remain changed (or not, depending on the
    ' user's interaction with the dialog) once the dialog is dismissed.
    szSend1 = "whoami"
    szExpect1 = "]$"
    szSend2 = "pwd"
    szExpect2 = szExpect1
    szSend3 = "who -w"
    szExpect3 = szExpect1    
    bLogOutput = True
    szLogFile = "C:\temp\MyLogFileName.log"
    szAppendOrOverwrite = "Append"
    
    ' Save the current session's log file name so that we can twiddle it
    ' in the dialog, write to the new value, and then restore the original
    ' log file name
    szSavedLogFileName = g_objTab.Session.LogFileName
    Dim szTempLogFileName 
    
    
    ' This loop gives us an opportunity to do some error checking with
    ' the return values provided once the dilaog's "OK" button is clicked,
    ' display an Error message, and then display the dialog again with the
    ' same values the user submitted intact.  In this example, we're doing
    ' two checks on the log file, 1) Check that a log file name was supplied
    ' if logging option was enabled and 2) Check that the parent folder of
    ' the specified log file exists.
    Do
        if Not PromptForInput(szSend1, szExpect1, _
                              szSend2, szExpect2, _
                              szSend3, szExpect3, _
                              bLogOutput, szLogFile, _
                              szAppendOrOverwrite) then
            MsgBox "User canceled."
            exit sub
        end if
        
        if bLogOutput then
            if szLogFile = "" then
                MsgBox "Log filename required if Log option is enabled."
            else          
                if Not g_fso.FolderExists(g_fso.GetParentFolderName(szLogFile)) then
                    MsgBox "Log folder path does not exist: " & vbcrlf & _
                        vbcrlf & _
                        vbtab & _
                        g_fso.GetParentFolderName(szLogFile) & vbcrlf & _
                        vbcrlf & _
                        "Please specify a log file name in an existing folder."
                else
                    szTempLogFileName = szLogFile
                    exit do
                end if
            end if
        else
            exit do
        end if
    Loop
    
    
    ' Now that we have validated data, let's do the work...
    
    ' 1) Determine if we should log or not...
    if bLogOutput then
        ' 1.1) Determine which log mode to use...
        Dim bAppend
        Select Case szAppendOrOverwrite
            Case "Append"
                bAppend = True
            Case "Overwrite"
                bAppend = False
            Case Else
                ' We don't ever expect this unless there is some mistake
                ' in the radio button HTML code in the PromptForInput()
                ' function below.
                MsgBox "Unknown LogMode value: " & szAppendOrOverwrite
                exit sub
        End Select
        
        ' 1.2) Turn off logging on the old file if it's enabled
        if g_objTab.Session.Logging then g_objTab.Session.Log False
        
        ' 1.3) Set up the current session to log to the temporary log file
        '      specified in the user dialog
        g_objTab.Session.LogFileName = szTempLogFileName
             
        ' 1.3) Start logging with the appropriate log mode
        g_objTab.Session.Log True, bAppend
    end if
    
    ' 2) Go through each send/expect...  If the ExpectN string is empty,
    '    don't bother waiting for it.
    g_objTab.Screen.Send  szSend1 & vbcr
    if szExpect1 <> "" then g_objTab.Screen.WaitForString szExpect1
    g_objTab.Screen.Send  szSend2 & vbcr
    if szExpect2 <> "" then g_objTab.Screen.WaitForString szExpect2
    g_objTab.Screen.Send  szSend3 & vbcr
    if szExpect3 <> "" then g_objTab.Screen.WaitForString szExpect3
    
    ' 3) Now that we're done, turn off logging.
    if bLogOutput then
        g_objTab.Session.Log False
        ' Restore the existing log file name for the current session
        g_objTab.Session.LogFileName = szSavedLogFileName
    end if
    
    crt.Dialog.Messagebox _
        "Operation completed."
        
    if bLogOutput then
        ' Once we have completed the work, bring up the log file in the default
        ' file handler.
        g_shell.Run chr(34) & szLogFile & chr(34)
    end if
        
    
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function PromptForInput(byRef szSend1,  byRef szExpect1, _
                        byRef szSend2,  byRef szExpect2, _
                        byRef szSend3,  byRef szExpect3, _
                        byRef bLogOutput, byRef szLogFile, _
                        byRef szAppendOrOverwrite)
' Sample function that prompts for the following information:
'   1) Send #1: First command to send to the remote
'   2) Expect #1: What to Expect after Send #1 is sent
'   3) Send #2: Second command to send to the remote
'   4) Expect #2: What to Expect after Send #2 is sent
'   5) Send #3: Third command to send to the remote
'   6) Expect #3: What to Expect after Send #3 is sent
'   7) LogOutput?  If this checkbox is enabled, a "LogFile" entry field will
'      become enabled, allowing the user to specify a log file in which
'      the output of the sent commands and their results will be stored.
'   8) LogFilename: Enabled when the LogOuput box is checked.  Specifies the
'      name of the log file that SecureCRT should use to log the commands
'      issued and their results.
'   9) AppendOrOverwrite?  Radio button value determines whether the log file
'      starts out a-fresh each time (overwrite), or appends to existing log
'      information.
' RETURNS: True or False, depending on whether or not the user presses "OK", or
'          "Cancel" from within the dialog.
    
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ' First step, set up the dialog (InternetExplorer)
    Set g_objIE = CreateObject("InternetExplorer.Application")
    g_objIE.Offline = True
    g_objIE.navigate "about:blank"
    
    ' Wait for the navigation to the "blank" web page to complete
    Do
        crt.Sleep 100
    Loop While g_objIE.Busy
    
    g_objIE.Document.body.Style.FontFamily = "Sans-Serif"
    
    ' Here's where we "create" the elements of our dialog box.  We basically
    ' throw HTML into the document, and IE loads it in real-time for us.
    ' 
    ' The hidden "ButtonHandler" input is used to tie IE and
    ' SecureCRT together such that SecureCRT can know when a
    ' button is pressed, etc.
    g_objIE.Document.Body.innerHTML = _
        "&nbsp;&nbsp;&nbsp;<b>Send<u>1</u>:</b><input name='Send1' size='80' maxlength='512' AccessKey='1' tabindex=1><br>" & _
        "<b>Expect1:</b><input name='Expect1' size='80' maxlength='512'>" & _
        "<p></p>" & _
        "&nbsp;&nbsp;&nbsp;<b>Send<u>2</u>:</b><input name='Send2' size='80' maxlength='512' AccessKey='2'><br>" & _
        "<b>Expect2:</b><input name='Expect2' size='80' maxlength='512'>" & _
        "<p></p>" & _
        "&nbsp;&nbsp;&nbsp;<b>Send<u>3</u>:</b><input name='Send3' size='80' maxlength='512' AccessKey='3'><br>" & _
        "<b>Expect3:</b><input name='Expect3' size='80' maxlength='512'>" & _
        "<p></p>" & _
        "<input name='LogOutput' type='checkbox' onclick=""document.all('ButtonHandler').value='LogOutput';"" AccessKey='L'><u>L</u>og Output to File<br>" & _
        "&nbsp;&nbsp;&nbsp;&nbsp;" & _
            "<b>Log <u>f</u>ilename:</b><input name='LogFilename' size='71' maxlength='512' AccessKey='f'><br>" & _
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp&nbsp;&nbsp;" & _
            "<input type=radio name='LogMode' value='Append' AccessKey='A' checked><u>A</u>ppend<br>" & _
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp&nbsp;&nbsp;" & _
            "<input type=radio name='LogMode' value='Overwrite' Accesskey='w' >Over<u>w</u>rite<br>" & _
        "<hr>" & _
        "<button name='OK' AccessKey='O' onclick=document.all('ButtonHandler').value='OK';><u>O</u>K</button>" & _
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" & _
        "<button name='Cancel' AccessKey='C' onclick=document.all('ButtonHandler').value='Cancel';><u>C</u>ancel</button>" & _
        "<input name='ButtonHandler' type='hidden' value='Nothing Clicked Yet'>"
    
    g_objIE.MenuBar = False
    g_objIE.StatusBar = True
    g_objIE.AddressBar = False
    g_objIE.Toolbar = False
    g_objIE.height = 420
    g_objIE.width = 640   
    g_objIE.document.Title = "Custom Dialog Prompt"
    g_objIE.Visible = True
    
    ' Wait for the "dialog" to be displayed before we attempt to set any
    ' of the dialog's default values.
    Do
        crt.Sleep 100
    Loop While g_objIE.Busy
    
    Set objShell = CreateObject("WScript.Shell")
    objShell.AppActivate g_objIE.document.Title
    
    ' Set up defaults within the dialog
    g_objIE.Document.All("Send1").Value = szSend1
    g_objIE.Document.All("Expect1").Value = szExpect1
    g_objIE.Document.All("Send2").Value = szSend2
    g_objIE.Document.All("Expect2").Value = szExpect2
    g_objIE.Document.All("Send3").Value = szSend3
    g_objIE.Document.All("Expect3").Value = szExpect3
    g_objIe.Document.All("LogOutput").Checked = bLogOutput
    g_objIE.Document.All("LogFilename").Value = szLogFile
    
    Select Case szAppendOrOverwrite
        Case "Overwrite"
            g_objIE.Document.All("LogMode")(1).Select
            g_objIE.Document.All("LogMode")(1).Checked = true
            g_objIE.Document.All("LogMode")(1).Click

        ' Note that "Append" is used both in the case where "Append" is 
        ' specified, as well as in the case where the szAppendOrOverwrite
        ' value is unrecognized.  This is done as a "safe" default mechanism.
        Case "Append"
        Case Else
            g_objIE.Document.All("LogMode")(0).Select
            g_objIE.Document.All("LogMode")(0).Checked = false
            g_objIE.Document.All("LogMode")(0).Click
    End Select    

    Do
        ' If the user closes the IE window by Alt+F4 or clicking on the 'X'
        ' button, we'll detect that here, and exit the script if necessary.
        On Error Resume Next
            Err.Clear
            szNothing = g_objIE.Document.All("ButtonHandler").Value
            if Err.Number <> 0 then exit function
            
        ' Check to see which buttons have been clicked, and address each one
        ' as it's clicked.
        Select Case g_objIE.Document.All("ButtonHandler").Value
            Case "Cancel"
                ' Close the custom dialog box...
                g_objIE.Quit
                
                ' Exit the function with value of False
                exit function
                
            Case "OK"
                ' Capture data from each field in the dialog...
                szSend1    = g_objIE.Document.All("Send1").Value
                szExpect1  = g_objIE.Document.All("Expect1").Value
                szSend2    = g_objIE.Document.All("Send2").Value
                szExpect2  = g_objIE.Document.All("Expect2").Value
                szSend3    = g_objIE.Document.All("Send3").Value
                szExpect3  = g_objIE.Document.All("Expect3").Value
                bLogOutput = g_objIE.Document.All("LogOutput").Checked
                szLogFile  = g_objIE.Document.All("LogFilename").Value
                
                for nIndex = 0 to g_objIE.Document.All("LogMode").Length - 1
                    if g_objIE.Document.All("LogMode")(nIndex).Checked then
                        szAppendOrOverwrite = g_objIE.Document.All("LogMode")(nIndex).Value
                        exit for
                    end if
                Next
                
                ' Set function's return value to indicate "OK"
                PromptForInput = True
                           
                ' Close the custom dialog box...
                g_objIE.Quit
                
                ' Exit the function
                exit function
    
            Case "LogOutput"
                ' Reset the button handler value.  We don't do this in the
                ' case of "Cancel" and "OK" handling since the dialog is
                ' going away...
                g_objIE.Document.All("ButtonHandler").value = ""
                
                ' Handle the check in real-time...
                if g_objIE.Document.All("LogOutput").Checked then
                    ' Enable the other elements of the dialog dealing with
                    ' logging
                    g_objIE.Document.All("LogFilename").Disabled = False
                    g_objIE.Document.All("LogMode")(0).Disabled = False
                    g_objIE.Document.All("LogMode")(1).Disabled = False
                else
                    ' Disable the other elements of the dialog dealing with
                    ' logging
                    g_objIE.Document.All("LogFilename").Disabled = True
                    g_objIE.Document.All("LogMode")(0).Disabled = True
                    g_objIE.Document.All("LogMode")(1).Disabled = True
                end if
                
        End Select

        On Error Goto 0
        ' Wait for user interaction with the dialog... Instead of crt.Sleep,
        ' we use g_objTab.Screen.WaitForString and pass in a string that
        ' is never expected to be found.  The worst case scenario here is
        ' that 1 second will pass between the time that the user clicks on
        ' the OK button and the dialog goes away, for example.
        ' We do this to avoid 100% CPU usage.
        g_objTab.Screen.WaitForString "1324;l1@#$!@#$!@#$ This will never appear", 1
    Loop

    
End Function

