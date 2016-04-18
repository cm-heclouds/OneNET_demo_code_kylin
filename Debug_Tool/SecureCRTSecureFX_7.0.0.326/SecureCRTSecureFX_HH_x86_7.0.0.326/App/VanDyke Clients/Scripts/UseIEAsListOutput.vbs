# $language = "VBScript"
# $interface = "1.0"
' UseIEAsListOutput.vbs
'
' Description:
'   This example script shows how to create a custom dialog using the
'   InternetExplorer.Application ActiveX object.  This example also shows
'   how data can be passed between the InternetExplorer object and SecureCRT.
'  
'   The dialog that is displayed provides a "Filter" text field where a search
'   term or regular expression can be entered changing the information
'   displayed in the lower part of the dialog.
'     - An "Apply" button activates the filter currently specified in the
'       "Filter" text field.
'     - A "Clear" button erases the text displayed in the lower portion of
'       the dialog
'     - A "Close" button closes the dialog.
'

Dim g_objIE, g_objTab
set g_objTab = crt.GetScriptTab

g_objTab.Screen.Synchronous = True

Sub Main()
    ' Instantiate an instance of InternetExplorer, which we'll use as an
    ' "input/output device".  The "InitIE" Subroutine is defined below Main().
    InitIE

    Dim szPattern, re, szLine
    set re = new RegExp
    szPattern = ".*"
    g_objIE.Document.All("Pattern").Value = szPattern
    
    Do
        ' If the user closes the IE window by Alt+F4 or clicking on the 'X'
        ' button, we'll detect that here, and exit the script if necessary.
        On Error Resume Next
            Err.Clear
            szNothing = g_objIE.Document.All("ButtonHandler").Value
            if Err.Number <> 0 then exit sub
        On Error Goto 0
            
        ' Check to see which buttons have been clicked, and address each one
        ' as it's clicked.
        If g_objIE.Document.All("ButtonHandler").Value = "Apply" Then
            szPattern = g_objIE.Document.All("Pattern").Value
            g_objIE.Document.All("ButtonHandler").Value = ""
        Elseif g_objIE.Document.All("ButtonHandler").Value = "Close" then
            g_objIE.Quit
            exit sub
        Elseif g_objIE.Document.All("ButtonHandler").Value = "Clear" then
            ClearIEText
            g_objIE.Document.All("ButtonHandler").Value = ""
        End if
        
        re.Pattern = szPattern        
        
        ' Now, wait for a new line to appear by looking for a CR (carriage
        ' return) to appear on the screen.  We'll time out after one second
        ' in order to allow for button clicks to be detected.  This means that
        ' in the worst case scenario, it will take up to 1 second to detect
        ' when a button was clicked in the window created in InitIE().
        If g_objTab.Screen.WaitForString(vbcr, 1) then
        
            ' We just got a line of output. Get the line of input so that we
            ' can check it later against the regular expression pattern
            ' specified in the IE dialog.
            ' 
            ' NOTE: There isn't a reliable way to detect (within a crt script)
            '       when a line has been wrapped, so it's a good idea to set
            '       your session options for use with this specific script such
            '       that the number of columns is larger than the longest line
            '       of output you expect to receive.
            szLine = Trim(crt.Screen.Get(crt.Screen.CurrentRow, _
                                         0, _
                                         crt.Screen.CurrentRow, _
                                         crt.Screen.Columns))
                                        
            if re.Test(szLine) then
                AppendToIEText szLine & vbcrlf
            End if
        End if
    Loop
    
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sub InitIE()
    Set g_objIE = CreateObject("InternetExplorer.Application")
    g_objIE.Offline = True
    g_objIE.navigate "about:blank"
   
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
        "<b><u>P</u>attern:</b><input name='Pattern' size='80' maxlength='512' AccessKey='P'>" & _
        "<button name='Apply' AccessKey='A' onclick=document.all('ButtonHandler').value='Apply';><u>A</u>pply</button>" & _
        "<button name='Clear' AccessKey='C' onclick=document.all('ButtonHandler').value='Clear';><u>C</u>lear</button>" & _
        "<button name='Close' AccessKey='s' onclick=document.all('ButtonHandler').value='Close';>Clo<u>s</u>e</button>" & _
        "<hr></hr><textarea READONLY name='TextArea' ID='TextArea' cols='90' rows='24' wrap='off'></textarea>" & _
        "<input name='ButtonHandler' type='hidden' value='Nothing Clicked Yet'>"
    
    g_objIE.MenuBar = False
    g_objIE.StatusBar = True
    g_objIE.AddressBar = False
    g_objIE.Toolbar = False
    g_objIE.height = 540
    g_objIE.width = 800    
    g_objIE.document.Title = "External Output Window For SecureCRT"
    g_objIE.Visible = True
    
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sub AppendToIEText(szText)
' Adds new text to the textarea created in InitIE()
    On Error Resume Next
    
    szCurrentText = GetCurrentIEText
    
    ' Check the number of lines within the text area... if we don't cap
    ' it, performance will suffer as the number of lines in the text area
    ' increases.  This example shows a display "scrollHeight" of 2048 max.
    if g_objIE.Document.All("TextArea").ScrollHeight > 2048 then
        szCurrentText = Mid(szCurrentText, len(szCurrentText) / 2, len(szCurrentText))
    end if
    
    Err.Clear
    g_objIE.Document.All("TextArea").Value = szCurrentText & szText
    if Err.Number <> 0 then
        On Error Goto 0
        exit sub
    end if
    
    ' This little line of code makes it so that the IE text area always is
    ' scrolled to the bottom when new lines of text appear.
    g_objIE.Document.All("TextArea").scrolltop = g_objIE.Document.All("TextArea").scrollHeight
    
    On Error Goto 0
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Function GetCurrentIEText()
' Retrieves the text within the "TextArea" created in InitIE()
    On Error Resume Next
    Err.Clear
    szCurrentText = g_objIE.Document.All("TextArea").Value
    if Err.Number <> 0 then
        On Error Goto 0
        exit Function
    end if
    
    GetCurrentIEText = szCurrentText
    On Error Goto 0  
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sub ClearIEText()
' Clears the "TextArea" created in InitIE()
    On Error Resume Next
    g_objIE.Document.All("TextArea").Value = ""
    On Error Goto 0
End Sub

