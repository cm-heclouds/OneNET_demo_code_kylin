# $language = "VBScript"
# $interface = "1.0"
' AddCommentToLogFile.vbs
'
' Description:
'   This example script allows you to add a custom line of text to your
'   SecureCRT log file.  The user will be prompted for the text to add
'   to the log file; logging will be stopped; the text provided by the
'   user will be appended to the log file; then SecureCRT logging will
'   be restarted.
'    
'   This method only works with connections associated with a Session
'   configuration in which a log file is specified.
'
' Demonstrates:
'   - How to prompt for user input using the VBScript InputBox() function.
'   - How to access Session.LogFileName to determine the log file name
'     stored for the current session.
'   - How to use the Session.Logging property to determine if logging
'     is currently active.
'   - How to use the VBScript Replace() method to substitute substrings
'     within an existing string.
'   - How to use the VBScript FileSystemObject to open an existing text file
'     and append data.
'

Dim g_fso
Set g_fso = CreateObject("Scripting.FileSystemObject")

Const ForAppending = 8
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sub Main()

    ' Make this script tab safe
    Dim objTab
    Set objTab = crt.GetScriptTab
    
    if objTab.Session.LogFilename = "" then
        crt.Dialog.MessageBox _
            "Error." & vbcrlf & vbcrlf & _
            "This script requires a session configuration in which a " & _
            "log file is defined." & vbcrlf & vbcrlf & _
            "Specify a Log file name in Session Options, ""Terminal / " & _
            "Log File"", and run this script again."
        exit sub
    end if
    
    ' 1. Stop logging to free up the log file for writing by our script.
    objTab.session.log False

    ' 2. Prompt the user for the text to add to the log file.
    Dim szTextToAdd, szDateTime
    szDateTime = Now
    szTextToAdd = InputBox( _
        "Add comment to log file (use \r\n to separate multiple lines):" & _
        vbcrlf & vbcrlf & _
        "Comment will be prefaced with current timestamp: " & szDateTime, _
        "Add Comment To SecureCRT Log File" & vbcrlf & _
        szDateTime & ": ")

    ' InputBox returns "" if the user cancels
    if szTextToAdd = "" then exit sub

    ' 3. Write data to the file.  If you want to include a newline, you must
    ' concatenate a CR/LF to the end of the line (as in the example below with the
    ' 'vbcrlf') Note also that you can wrap the comment with certain markers,
    ' and not require the user to enter these markers when prompted. Although
    ' the input box function only allows one line per entry, you can set up a
    ' tag that can be used to represent a new line, such as "\r\n" and then
    ' replace this tag with a vbcrlf as we have done in this example:
    szTextToAdd = _
        vbcrlf & _
        "/********************************************/ " & vbcrlf & _
        "* " & szDateTime & ": " & vbcrlf & _
        "* " & Replace(szTextToAdd, "\r\n", vbcrlf & "* ") & vbcrlf & _
        "/********************************************/ " & vbcrlf

    WriteToFile objTab.Session.LogFileName, szTextToAdd

    ' 4. Start up logging again, making sure we append, rather than overwrite.
    objTab.session.log True, True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function WriteToFile(szFile, szData)
' szFile: Full path to file
' szData: data to be written to the szFile
    WriteToFile = False

    Dim bUnicode
    bUnicode = False

    Dim objFile
    Set objFile = g_fso.OpenTextFile(szFile, ForAppending, True, bUnicode)

    objFile.Write szData
    objFile.Close

    WriteToFile = True
End Function

