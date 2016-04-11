# $language = "VBScript"
# $interface = "1.0"
' GoogleSelectedText.vbs
' 
' Description:
'   When this script is launched, the text selected within the terminal
'   window is used as the search term for a web search using google.com.
'   This script demonstrates capabilities only available in SecureCRT 6.1
'   and later (Screen.Selection property).
' 
' Demonstrates:
'   - How to use the Screen.Selection property in SecureCRT 6.1 and later
'     to get access to the text selected in the terminal window.
'   - How to use the WScript.Shell object to launch an external application
'   - How to branch code based on version of SecureCRT in which this script
'     is being run.

Sub Main()
    szSelection = Trim(crt.Screen.Selection)

    ' Now search on google for the information.
    g_szSearchBase = "http://www.google.com/search?hl=en&q="

    Set g_shell = CreateObject("WScript.Shell")
    
    ' Instead of launching InternetExplorer, we'll run the URL, so that the
    ' default browser gets used :).
    if szSelection = "" then
        g_shell.Run chr(34) & "http://www.google.com/" & chr(34)
    else
        g_shell.Run chr(34) & "http://www.google.com/search?hl=en&q=" & szSelection & chr(34)
    end if
end sub
