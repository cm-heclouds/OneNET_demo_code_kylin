#$language = "VBScript"
#$interface = "1.0"

Sub main
  Dim passwd
  
  ' turn on synchronous mode so we don't miss any data
  crt.Screen.Synchronous = True

  ' Wait for a string that looks like "login: " or "Login: "
  crt.Screen.WaitForString "ogin: "

  ' Send your username followed by a carriage return
  crt.Screen.Send "username" & VbCr

  ' Wait for a string that looks like "password: " or "Password: "
  crt.Screen.WaitForString "assword:"

  ' prompt the user for a password
  passwd = crt.Dialog.Prompt("Please enter your password:", "Enter Password", "", True)

  ' send the password and a carriage return
  crt.Screen.Send passwd & VbCr
  
  ' turn off synchronous mode to restore normal input processing
  crt.Screen.Synchronous = False
End Sub
