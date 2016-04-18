#$language = "VBScript"
#$interface = "1.0"
' ConnectToMultipleSessionsAndSendCommands.vbs
'
' Description:
'     Reads in sessions from a file (one session per line) and
'     connects to each one (one at a time) and sends a series of
'     commands to the remote, then disconnects from the session
'     and moves on to the next session.
'     
'     By default, the file containing the sessions is expected to be named
'     "SessionList.txt", and should be located in the current user's "My
'     Documents" folder.  Each line of the file is expected to contain the
'     path to the session as it appears within the Connect dialog (excluding
'     the leading "Sessions/" component.  Here is an example file contents:
'         
'         redhat
'         redhat8
'         192.168.0.123
'         192.168.0.131
'         Redhat Sessions\RedHat - XTerm
'
'     For the sake of simplicity, this example assumes that all
'     sessions we're connecting to are using the SSH1 and SSH2
'     protocols, with usernames and passwords saved encrypted
'     as part of the session configuration so that we don't need
'     to worry about authentication within this script.
'
' This example demonstrates:
'     - How to use the Windows FileSystemObject to load information from
'       a file.
'     - How to determine location of current user's "My Documents" folder
'       by using WScript's Shell.SpecialFolders() method
'     - How to use a VBScript dynamic array
'     - How to connect to a remote machine using an existing session

Dim g_szErrors

Sub Main()

    Dim objFso, objShell, objTextStream, szLine
    Dim vSessionsArray()
    Dim nSessionCount
    nSessionCount = 0

    ' Start of with a reasonable size for the array
    ReDim vSessionsArray(5)

    ' Read in Sessions from a file that contains session names (paths as they
    ' appear in the Connect dialog), one per line
    Set objFso = CreateObject("Scripting.FileSystemObject")
    Set objShell = CreateObject("WScript.Shell")
    Dim szMyDocs, szSessionsFile
    szMyDocs = objShell.SpecialFolders("MyDocuments")
    
    szSessionsFile = szMyDocs & "\SessionList.txt"
    if Not objFso.FileExists(szSessionsFile) then
        crt.Dialog.MessageBox "Session list file not found:" & vbcrlf & _
            vbcrlf & _
            szSessionsFile & vbcrlf & vbcrlf & _
            "Create a session list file as described in the description of " & _
            "this script code and then run the script again."
        exit sub
    end if
    
    Set objTextStream = objFso.OpenTextFile(szSessionsFile, 1, false)

    Dim szSession
    Do While Not objTextStream.AtEndOfStream
        szSession = ""
        szSession = Trim(objTextStream.ReadLine)
        
        ' Don't add empty lines/sessions
        if szSession <> "" then
            ' Check to see if our current array is full. If it's full, we'll
            ' allocate enough additional room in the array to store 5 more
            ' elements
            if UBound(vSessionsArray) >= nSessionCount then
                redim preserve vSessionsArray(nSessionCount + 5)
            end if
            
            ' Now that we're assured there's enough room to store the session
            ' name/path we've read in, we'll store it in the array and increase
            ' our counter variable.
            vSessionsArray(nSessionCount) = szSession
            nSessionCount = nSessionCount + 1
        end if
    Loop

    objTextStream.Close

    ' Connect to each session and issue a few commands, then disconnect.
    For Each szSession in vSessionsArray
        if szSession = "" then exit for
        
        'MsgBox "Connecting to Session: " & szSession
        On Error Resume Next
        crt.Session.Connect("/S " & chr(34) & szSession & chr(34))
        szError = Err.Description
        On Error Goto 0
        
        ' If we successfully connected, we'll do the work we intend to do...
        ' otherwise, we'll skip the work and move on to the next session in 
        ' the list.
        if crt.Session.Connected then
            crt.Screen.Synchronous = True
            
            ' When we first connect, there will likely be data arriving from the
            ' remote system.  This is one way of detecting when it's safe to
            ' start sending data.
            Do
                bCursorMoved = crt.Screen.WaitForCursor(1)
            Loop until bCursorMoved = False
            ' Once the cursor has stopped moving for about a second, we'll
            ' assume it's safe to start interacting with the remote system.
            
            ' Get the shell prompt so that we can know what to look for when
            ' determining if the command is completed. Won't work if the prompt
            ' is dynamic (e.g. changes according to current working folder, etc)
            nRow = crt.Screen.CurrentRow
            szPrompt = crt.screen.Get(nRow, _
                                      0, _
                                      nRow, _
                                      crt.Screen.CurrentColumn - 1)
            szPrompt = Trim(szPrompt)
            
            crt.Screen.Send "ls -l" & vbcr
            ' Wait for the command to complete, by looking for the prompt to
            ' appear once again.
            crt.Screen.WaitForString szPrompt

            crt.Screen.Send "pwd" & vbcr
            ' Wait for the command to complete, by looking for the prompt to
            ' appear once again.
            crt.Screen.WaitForString szPrompt

            crt.Screen.Send "who" & vbcr
            ' Wait for the command to complete, by looking for the prompt to
            ' appear once again.
            crt.Screen.WaitForString szPrompt

            ' Now disconnect from the remote machine...
            crt.session.disconnect
            ' Wait for the connection to close
            Do
                crt.Sleep 100
            Loop While crt.Session.Connected = True
            
            crt.Sleep 1000
        else
            g_szErrors = g_szErrors & vbcrlf & _
                "*** Error connecting to " & szSession & ": " & szError
        end if
    Next
    
    if g_szErrors = "" then
        crt.Dialog.MessageBox _
            "Tasks completed.  No Errors were detected."
    else
        crt.Dialog.MessageBox _
            "Tasks completed.  The following errors occurred:" & _
            vbcrlf & g_szErrors
    end if
    
    crt.Quit
    
End Sub
