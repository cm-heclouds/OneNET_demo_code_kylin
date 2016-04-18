#$language = "VBScript"
#$interface = "1.0"

Sub main
  ' turn on synchronous mode so we don't miss any data
  crt.Screen.Synchronous = True
  
  ' Wait for a string that looks like "login: " or "Login: "
  ' Occasionally, a host needs to be kicked to display a
  ' login prompt.
  If (Not crt.Screen.WaitForString("ogin: ", 5)) Then
    crt.Screen.Send vbCr
    crt.Screen.WaitForString "ogin: "
  End If
  
  ' Send your username followed by a carriage return
  crt.Screen.Send "username" & VbCr

  ' Wait for a string that looks like "password: " or "Password: "
  crt.Screen.WaitForString "assword:"

  ' Send your password followed by a carriage return
  crt.Screen.Send "password" & VbCr

  ' turn off synchronous mode for normal input processing
  crt.Screen.Synchronous = False
End Sub
