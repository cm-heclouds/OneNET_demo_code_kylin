# $language = "python"
# $interface = "1.0"

# BasicReadStringExample.py
# 
# Description:
#     This example script captures the output of a command (ls -l) sent to a
#     remote machine by using the Screen.ReadString() method.  The captured
#     text is then displayed in a message box window.
# 
# This example demonstrates:
#     - How to use the basic functionality of the Screen.ReadString()
#       method to easily capture data from a remote system.
#     - How to use the Screen.IgnoreEscape property to change the way ReadString
#       (also applies to WaitForString() and WaitForStrings()) handles
#       non-printable characters.
#       By default, ReadString() will capture all data sent from the remote
#       during the time in which ReadString is capturing data, including escape
#       sequences.  If you do not want to capture escape sequences and instead
#       only capture visible ASCII text, set the Screen.IgnoreEscape property to
#       True, as in this example.
# 
# This script example assumes the user has logged in and is sitting at a command
# prompt as the script is launched from SecureCRT's 'Script -> Run' menu.

def Main():
	# Here is where we will set the value of the string that will indicate that
	# we have reached the end of the data that we wanted to capture with the
	# ReadString method.
	szPrompt = "]->"

	# Using GetScriptTab() will make this script 'tab safe' in that all of the
	# script's functionality will be carried out on the correct tab. From here
	# on out we'll use the objTab object instead of the crt object.
	objTab = crt.GetScriptTab()
	objTab.Screen.Synchronous = True

	# Instruct WaitForString and ReadString to ignore escape sequences when
	# detecting and capturing data received from the remote (this doesn't
	# affect the way the data is displayed to the screen, only how it is handled
	# by the WaitForString, WaitForStrings, and ReadString methods associated
	# with the Screen object).
	objTab.Screen.IgnoreEscape = True

	# We begin the process by sending a command. In this example script,
	# we're simply getting a file listing from a remote UNIX system using the
	# "ls -l" command.
	szCommand = "ls -l"
	objTab.Screen.Send(szCommand + "\r\n")

	# Wait for the command and the trailing CR to be echoed back from the remote
	# before we start capturing data... Otherwise, we'll capture the command we
	# issued, as well as the results, and in this example, we only want to
	# capture the results.
	objTab.Screen.WaitForString(szCommand + "\r\n")

	# This will cause ReadString() to capture data until we see the szPrompt
	# value.
	szResult = objTab.Screen.ReadString(szPrompt)

	# Display the results in a message box
	crt.Dialog.MessageBox(szResult)

Main()
