#$language = "VBScript"
#$interface = "1.0"
' LogOutputOfSpecificCommand-UseReadString.vbs
'
' Description:
'   Sends commands one by one as listed in the g_vCommands() array to the remote
'   machine.  The results of each command are captured into a variable, and then
'   written to an individual log file (one log file for each command).  Once all
'   the commands have been run, Windows explorer is launched, with the first
'   command output file selected within the explorer window.
'
' Demonstrates:
'   This example script demonstrates how to log the output of specific commands
'   to separate files (one file for each command) without having to manually
'   turn logging on before and off after running each command.  
' 
'   This specific example doesn't use the logging script API to write data to a
'   file.  Instead, we use the ReadString() method to capture the result of each
'   command and write it manually to a file of our choosing (g_szLogFile serves
'   as a template for the file name which changes with each command that is
'   issued to the remote.  For example, the results of the first command will be
'   written to a file named Command#01_Results.txt, the results of the 2nd
'   command to Command#02_Results.txt, etc...).
'
'   Specifically, this example automates the logging process by:
'    - Using an array of commands that are to be sent to the
'      remote system (g_vCommands)
'    - Using objTab.Screen.Send() to issue each command.
'    - Using the objTab.Screen.ReadString() method to not only
'      wait for an indication that the command sent has been
'      completed, but also capture all of the text received
'      while the command was running.
'
' Note: This script assumes that SecureCRT is currently connected to
'       the remote device prior to running this script.  Otherwise, the
'       script will exit with an error message.
'
'       If you want this script to perform the connection
'       sequence as well, simply replace the "If Not
'       objTab.Session.Connected" block within the Main() sub
'       below with a line similar to the following:
'
'           objTab.Session.Connect "/S MySessionName"

' Option Explicit

Dim g_fso, g_shell
Set g_fso = CreateObject("Scripting.FileSystemObject")
Set g_shell = CreateObject("WScript.Shell")

Const ForReading   = 1
Const ForWriting   = 2
Const ForAppending = 8

Dim g_szLogFile, g_szFirstLogFilePath, objTab
g_szLogFile = GetMyDocumentsFolder & "\Command#__NUM___Results.txt"

Set objTab = crt.GetScriptTab

Dim g_vCommands(100)
g_vCommands(0) = "pwd"
g_vCommands(1) = "ls -lR --color /tmp"
g_vCommands(2) = "ps -eaf | grep vshelld"
g_vCommands(3) = "tail -100 /var/log/messages"
'g_vCommands(4) = "sh run"
'g_vCommands(5) = ""
'g_vCommands(6) = ""
'g_vCommands(7) = ""
'g_vCommands(8) = ""
'g_vCommands(9) = ""
' .
' .
' .

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sub Main()
    ' Instruct WaitForString and ReadString to ignore escape sequences when
    ' detecting and capturing data received from the remote (this doesn't
    ' affect the way the data is displayed to the screen, only how it is handled
    ' by the WaitForString, WaitForStrings, and ReadString methods associated
    ' with the Screen object).
    objTab.Screen.IgnoreEscape = True
    objTab.Screen.Synchronous = True
   
    If Not objTab.Session.Connected then
        crt.Dialog.MessageBox _
            "Not Connected.  Please connect before running this script."
        exit sub
    end if

    Dim szCommand, szPrompt, nRow, szLogFileName, nIndex

    ' If this script is run as a login script, there will likely be data
    ' arriving from the remote system.  This is one way of detecting when it's
    ' safe to start sending data. If this script isn't being run as a login
    ' script, then the worst it will do is seemingly pause for one second
    ' before determining what the prompt is.
    ' If you plan on supplying login information by waiting for username and
    ' password prompts within this script, do so right before this do..loop.
    Do
        bCursorMoved = objTab.Screen.WaitForCursor(1)
    Loop until bCursorMoved = False
    ' Once the cursor has stopped moving for about a second, we'll
    ' assume it's safe to start interacting with the remote system.
    
    ' Get the shell prompt so that we can know what to look for when
    ' determining if the command is completed. Won't work if the prompt
    ' is dynamic (e.g. changes according to current working folder, etc)
    nRow = objTab.Screen.CurrentRow
    szPrompt = objTab.screen.Get(nRow, _
                                 0, _
                                 nRow, _
                                 objTab.Screen.CurrentColumn - 1)
    szPrompt = Trim(szPrompt)

    Dim szLogFile
    nIndex = 0
    Do
        szCommand = Trim(g_vCommands(nIndex))

        ' If the command is empty, then we should be done issuing commands
        ' (there's nothing else in our command array g_vCommands)
        if szCommand = "" then Exit Do

        ' Set up the log file for this specific command
        szLogFile = Replace(g_szLogFile, "__NUM__", NN(nIndex + 1, 2))
        
        ' Store the path for our first log file for later use (see end of this
        ' function...
        if g_szFirstLogFilePath = "" then g_szFirstLogFilePath = szLogFile

        ' Send the command text to the remote
        objTab.Screen.Send szCommand & vbcr

        ' Wait for the command to be echo'd back to us.
        objTab.Screen.WaitForString vbcr, 1
        objTab.Screen.WaitForString vblf, 1        

        Dim szResult
        ' Use the ReadString() method to get the text displayed while the
        ' command was running.  Note that the ReadString usage shown below
        ' is not documented properly in SecureCRT help files included in
        ' SecureCRT versions prior to 6.0 Official.  Note also that the
        ' ReadString() method captures escape sequences sent from the remote
        ' machine as well as displayed text.  As mentioned earlier in comments
        ' above, if you want to suppress escape sequences from being captured,
        ' set the Screen.IgnoreEscape property = True.
        szResult = objTab.Screen.ReadString(szPrompt)
        
        Dim objFile
        Set objFile = g_fso.OpenTextFile(szLogFile, ForWriting, True)

        ' If you don't want the command logged along with the results, comment
        ' out the very next line
        objFile.WriteLine "Results of command: " & szCommand

        ' Write out the results of the command to our log file
        objFile.WriteLine szResult
        
        ' Close the log file
        objFile.Close

        ' Move on to the next command in our command array g_vCommands
        nIndex = nIndex + 1
    Loop
    
    ' Once we're complete, let's bring up Windows Explorer with the first of
    ' the log files selected.
    g_shell.Run "explorer /e,/select,""" & g_szFirstLogFilePath & """"

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function GetMyDocumentsFolder()
    Dim myShell
    Set myShell = CreateObject("WScript.Shell")

    GetMyDocumentsFolder = myShell.SpecialFolders("MyDocuments")
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function NN(nNumber, nDesiredDigits)
' Normalizes a single digit number to have nDesiredDigits 0s in front of it
    Dim nIndex, nOffbyDigits, szResult
    nOffbyDigits = nDesiredDigits - len(nNumber)

    szResult = nNumber

    For nIndex = 1 to nOffByDigits
        szResult = "0" & szResult
    Next
    NN = szResult
End Function

