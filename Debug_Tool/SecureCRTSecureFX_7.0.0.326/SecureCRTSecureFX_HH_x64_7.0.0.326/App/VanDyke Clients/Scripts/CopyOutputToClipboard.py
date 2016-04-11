# $language = "python"
# $interface = "1.0"

# CopyOutputToClipboard.py
# 
# Description:
#   This script example is designed to run while connected to a Cisco
#   Pix firewall or other router device.
#   
#   When launched, the following commands are sent to the device:
#      enable
#      configure terminal
#      pager 0
#      show config
#      show serial
#      show sysopt
#      exit
#      exit
#  
#  The results of the 'show config', 'show serial', and 'show sysopt'
#  commands are captured and stored within the system clipboard.  A
#  messagebox then displays the contents of the system clipboard.
#   
# Demonstrates:
#   - Use of crt.Screen.ReadString to capture output from a command 
#     as echoed from the remote system.
#   - How to copy data from a script variable into the system clipboard
#     using the crt.Clipboard object
#   - Use of the ReadString method to either look for a shell
#     prompt (so it can be determined when a remote command has
#     completed) or some other entire line of data.
#   - How to use the Python str.replace method to substitute text within
#     an existing string.

# Using GetScriptTab() will make this script 'tab safe' in that all of the
# script's functionality will be carried out on the correct tab. From here
# on out we'll use the SCRIPT_TAB object instead of the crt object.
SCRIPT_TAB = crt.GetScriptTab()
SCRIPT_TAB.Screen.Synchronous = True

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def main():

	if not crt.Session.Connected:
		crt.Dialog.MessageBox(
			"This script currently requires a valid connection to a "
			"Cisco Pix firewall or other similar device.\n\n"
			"Please connect and then run this script again.")
		return

	crt.Screen.Synchronous = True

	# We'll use a "SendExpect" function we wrote (see below) to help us
	# easily send commands and wait for the remote device to be ready for
	# us to send the next command (by waiting for text we assume indicates
	# the remote device is ready -- a prompt for example)
	if not SendExpect("enable", "ssword:"):
		return
	if not SendExpect("", "# "):
		return
	if not SendExpect("configure terminal", "(config)# "):
		return
	if not SendExpect("pager 0", "(config)# "):
		return
	
	# Now, we'll use another function we wrote called "CaptureOutputOfCommand",
	# which takes as its first parameter the command to run, and as the 2nd
	# parameter, the text that indicates the command has completed -- the prompt
	# in most cases.
	data = CaptureOutputOfCommand("show config", "(config)# ")
	data += CaptureOutputOfCommand("show serial", "(config)# ")
	data += CaptureOutputOfCommand("show sysopt", "(config)# ")  
	
	# Some Cisco devices send EOL as LFCR rather than CRLF.  If we find the
	# LFCR pattern, let's replace it with CRLF.
	data = data.replace('\n\r', '\r\n')
	
	# Now that we have captured the output of each of the 'show' commands, let's
	# send the data to the system clipboard.
	crt.Clipboard.Format = "CF_TEXT"
	crt.Clipboard.Text = data

	# Now get back to a normalized state on the remote device
	if not SendExpect("exit", "# "):
		return
	if not SendExpect("exit", "> "):
		return
	
	crt.Dialog.MessageBox(
		"Text is now in the clipboard: \n\n" + crt.Clipboard.Text)


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def SendExpect(send, expect):
	# Returns true if the text in 'send' was successfully sent and the
	# text in 'expect' was successfully found as a result.

	# If we're not connected, we can't possibly return true, or even
	# send/receive text
	if not SCRIPT_TAB.Session.Connected:
		return

	SCRIPT_TAB.Screen.Send(send + '\r')
	SCRIPT_TAB.Screen.WaitForString(expect)

	return True


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def CaptureOutputOfCommand(command, prompt):
	if not crt.Session.Connected:
		return "[ERROR: Not Connected.]"
	
	# First, send the command to the remote.
	SCRIPT_TAB.Screen.Send(command + '\r')
	
	# Second, wait for the carriage return to be echoed by the remote device.
	# This allows us to capture only the output of the command, not the line
	# on which the command was issued (which would include the prompt + cmd).
	# If you want to capture the command that was issued, simply comment out
	# the following line of code.
	SCRIPT_TAB.Screen.WaitForString('\r')
	
	# Now that the command has been sent, use Screen.ReadString to capture
	# all the data that is received up to the point at which the shell
	# prompt appears (the captured data does not include the shell prompt).
	return SCRIPT_TAB.Screen.ReadString(prompt)


main()
