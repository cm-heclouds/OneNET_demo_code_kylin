#$language = "VBScript"
#$interface = "1.0"

Sub main
  ' turn on synchronous mode so we don't miss any data
  crt.Screen.Synchronous = True

  ' Wait for a string that looks like "login: " or "Login: "
  crt.Screen.WaitForString "ogin: "

  ' Send your username followed by a carriage return
  crt.Screen.Send "username" & VbCr

  ' Wait for a string that looks like "password: " or "Password: "
  crt.Screen.WaitForString "assword:"

  ' Send your password followed by a carriage return
  crt.Screen.Send "password" & VbCr

  ' turn off synchronous mode to restore normal input processing
  crt.Screen.Synchronous = False
End Sub
