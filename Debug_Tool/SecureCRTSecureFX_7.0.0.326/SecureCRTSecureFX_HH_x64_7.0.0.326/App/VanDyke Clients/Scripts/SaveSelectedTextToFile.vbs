# $language = "VBScript"
# $interface = "1.0"
' SaveSelectedTextToFile.vbs
'
' Description:
'    If non-whitespace text is selected within the terminal screen, the user
'    will be prompted for a location + filename in which to store the selected
'    text.  The selected text will then be saved to the file specified by
'    the user.
'    
' Demonstrates:
'    - How to use the Screen.Selection property new to SecureCRT 6.1
'    - How to use the Scripting.FileSystemObject to write data to a
'      text file.
'    - One way of determining if the script is running on Windows XP or not.
'    - One way of displaying a file browse/open dialog in Windows XP
' 
'
' g_nMode is used only if the user specifies a file that already exists, in
' which case the user will be prompted to overwrite the existing file, append
' to the existing file, or cancel the operation.
Dim g_nMode
const ForWriting = 2
const ForAppending = 8

' Be "tab safe" by getting a reference to the tab for which this script
' has been launched:
Set objTab = crt.GetScriptTab

Set g_shell = CreateObject("WScript.Shell")
Set g_fso   = CreateObject("Scripting.FileSystemObject")

SaveSelectedTextToFile

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sub SaveSelectedTextToFile()
    
    ' Capture the selected text into a variable.  The 'Selection' property
    ' is available in SecureCRT 6.1 and later.  This line of code will cause
    ' an error if launched in a version of SecureCRT earlier than 6.1.
    szSelection = objTab.Screen.Selection
    
    ' Check to see if the selection really has any text to save... we don't
    ' usually want to write out nothing to a file.
    if Trim(szSelection) = "" then
        MsgBox "Nothing to save!"
        exit sub
    end if
    
    Do
        szFilename = BrowseForFile()
        
        if szFilename = "" then exit sub
        
        ' Do some sanity checking if the file specified by the user already
        ' exists...
        if g_fso.FileExists(szFilename) then
            
            nResult = MsgBox("Do you want to replace the contents of """ & _
                szFilename & _
                """ with the selected text?" & vbcrlf & vbcrlf & _
                vbtab & "Yes = overwrite" & vbcrlf & vbcrlf & _
                vbtab & "No = append" & vbcrlf & vbcrlf & _
                vbtab & "Cancel = end script" & vbcrlf, vbYesNoCancel)
            Select Case (nResult)
                Case vbYes
                    g_nMode = ForWriting
                    exit do
                Case vbNo
                    g_nMode = ForAppending
                    exit do
                case else
                    exit sub
            End Select
        else
            g_nMode = ForWriting
            exit do
        end if
        
    Loop
    
    Set objFile = g_fso.OpenTextFile(szFilename, g_nMode, True)
    objFile.Write szSelection & vbcrlf
    objFile.Close
    
    g_szMode = "Wrote"
    if g_nMode = ForAppending then g_szMode = "Appended"
    
    MsgBox g_szMode & " " & len(szSelection) & " bytes to file:" & vbcrlf & _
        vbcrlf & szFilename
    
    ' Now open the text file in the default .txt file application handler...
    g_shell.Run chr(34) & szFilename & chr(34)
    
End sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function BrowseForFile()
    ' Determine if we're running on Windows XP or not...
    Dim szOSName
    Set objWMIService = GetObject("winmgmts:" & _
        "{impersonationLevel=impersonate}!\\.\root\cimv2")
    Set colSettings = _
        objWMIService.ExecQuery("SELECT * FROM Win32_OperatingSystem")
    For Each objOS in colSettings
     ' Windows XP "Name" might look like this:
     '   "Microsoft Windows XP Professional|C:\WINDOWS|\Device\Harddisk0\"...
     ' Vista might appear as follows:
     '   "Microsoft® Windows Vista™ Business |C:\Windows|\Device\Harddisk0\"...
        szOsName = Split(objOS.Name, "|")(0)
        exit For
    Next
    
    if Instr(szOsName, "XP") > 0 then
        ' Based on information obtained from
        ' http://blogs.msdn.com/gstemp/archive/2004/02/17/74868.aspx
        ' NOTE: Will only work with WindowsXP or newer since other OS's
        '       don't have a UserAccounts.CommonDialog ActiveX
        '       object registered.
        Set objDialog = CreateObject("UserAccounts.CommonDialog")
        objDialog.Filter = "Log Files|*.Log|Text Files|*.txt|All Files|*.*"
        objDialog.FilterIndex = 2
        objDialog.InitialDir = g_shell.SpecialFolders("MyDocuments")
        nResult = objDialog.ShowOpen

        If nResult <> 0 Then
            BrowseForFile = objDialog.FileName
        End If
    else
        ' On Windows other than XP, we'll just pop up an input box.
        BrowseForFile = InputBox(_
            "Save selected text to file:", _
            "SecureCRT - Save Selected Text To File", _
            g_shell.SpecialFolders("MyDocuments") & "\MyFile.txt")
    end if
End Function
