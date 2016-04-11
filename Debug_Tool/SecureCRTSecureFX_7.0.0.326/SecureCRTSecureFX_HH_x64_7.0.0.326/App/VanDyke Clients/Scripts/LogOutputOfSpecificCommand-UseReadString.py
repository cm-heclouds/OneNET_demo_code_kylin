#$language = "python"
#$interface = "1.0"

# LogOutputOfSpecificCommand-UseReadString.py
# 
# Description:
#   Sends commands one by one as listed in the COMMANDS array to the
#   remote machine.  The results of each command are captured into a
#   variable, and then written to an individual log file (one log file
#   for each command).  Once all the commands have been run, a file
#   browser is launched, with the first command output file selected
#   within the file browser window.
# 
# Demonstrates:
#   This example script demonstrates how to log the output of specific
#   commands to separate files (one file for each command) without
#   having to manually turn logging on before and off after running each
#   command.  
# 
#   This specific example doesn't use the logging script API to write
#   data to a file.  Instead, we use the ReadString() method to capture
#   the result of each command and write it manually to a file of our
#   choosing (LOG_FILE_TEMPLATE serves as a template for the file name
#   which changes with each command that is issued to the remote.  For
#   example, the results of the first command will be written to a file
#   named Command_01_Results.txt, the results of the 2nd command to
#   Command_02_Results.txt, etc...).
# 
#   Specifically, this example automates the logging process by:
#    - Using an array of commands that are to be sent to the remote 
#      system (COMMANDS)
#    - Using SCRIPT_TAB.Screen.Send() to issue each command.
#    - Using the SCRIPT_TAB.Screen.ReadString() method to not only wait
#      for an indication that the command sent has been completed, but
#      also capture all of the text received while the command was
#      running.
# 
# Note: This script assumes that SecureCRT is currently connected to the
# remote device prior to running this script.  Otherwise, the script
# will exit with an error message.
# 
#       If you want this script to perform the connection sequence as
#       well, simply replace the "if not SCRIPT_TAB.Session.Connected:"
#       block within the main() function below with a line similar to
#       the following:
# 
#           SCRIPT_TAB.Session.Connect("/S MySessionName")

import os
import subprocess

LOG_DIRECTORY = os.path.join(
	os.path.expanduser('~'), 'LogOutputOfSpecificCommand')

LOG_FILE_TEMPLATE = os.path.join(
	LOG_DIRECTORY, "Command_%(NUM)s_Results.txt")

SCRIPT_TAB = crt.GetScriptTab()

COMMANDS = [
	"pwd",
	"ls -lR --color /tmp",
	"ps -eaf | grep vshelld",
	"tail -100 /var/log/messages",
#	"sh run",
#	"",
#	"",
	]

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def main():

	if not os.path.exists(LOG_DIRECTORY):
		os.mkdir(LOG_DIRECTORY)

	if not os.path.isdir(LOG_DIRECTORY):
		crt.Dialog.MessageBox(
			"Log output directory %r is not a directory" % LOG_DIRECTORY)
		return

	if not SCRIPT_TAB.Session.Connected:
		crt.Dialog.MessageBox(
			"Not Connected.  Please connect before running this script.")
		return

	# Instruct WaitForString and ReadString to ignore escape sequences when
	# detecting and capturing data received from the remote (this doesn't
	# affect the way the data is displayed to the screen, only how it is handled
	# by the WaitForString, WaitForStrings, and ReadString methods associated
	# with the Screen object).
	SCRIPT_TAB.Screen.IgnoreEscape = True
	SCRIPT_TAB.Screen.Synchronous = True

	# If this script is run as a login script, there will likely be data
	# arriving from the remote system.  This is one way of detecting when it's
	# safe to start sending data. If this script isn't being run as a login
	# script, then the worst it will do is seemingly pause for one second
	# before determining what the prompt is.
	# If you plan on supplying login information by waiting for username and
	# password prompts within this script, do so right before this while loop.
	while True:
		if not SCRIPT_TAB.Screen.WaitForCursor(1):
			break
	# Once the cursor has stopped moving for about a second, we'll
	# assume it's safe to start interacting with the remote system.
	
	# Get the shell prompt so that we can know what to look for when
	# determining if the command is completed. Won't work if the prompt
	# is dynamic (e.g. changes according to current working folder, etc)
	rowIndex = SCRIPT_TAB.Screen.CurrentRow
	colIndex = SCRIPT_TAB.Screen.CurrentColumn - 1

	prompt = SCRIPT_TAB.Screen.Get(rowIndex, 0, rowIndex, colIndex)
	prompt = prompt.strip()

	for (index, command) in enumerate(COMMANDS):
		command = command.strip()

		# Set up the log file for this specific command
		logFileName = LOG_FILE_TEMPLATE % {"NUM" : NN(index + 1, 2)}
		
		# Send the command text to the remote
		SCRIPT_TAB.Screen.Send(command + '\r')

		# Wait for the command to be echoed back to us.
		SCRIPT_TAB.Screen.WaitForString('\r', 1)
		SCRIPT_TAB.Screen.WaitForString('\n', 1)

		# Use the ReadString() method to get the text displayed while
		# the command was running.  Note also that the ReadString()
		# method captures escape sequences sent from the remote machine
		# as well as displayed text.  As mentioned earlier in comments
		# above, if you want to suppress escape sequences from being
		# captured, set the Screen.IgnoreEscape property = True.
		result = SCRIPT_TAB.Screen.ReadString(prompt)
		result = result.strip()
		
		filep = open(logFileName, 'wb+')

		# If you don't want the command logged along with the results, comment
		# out the very next line
		filep.write("Results of command: " + command + os.linesep)

		# Write out the results of the command to our log file
		filep.write(result + os.linesep)
		
		# Close the log file
		filep.close()

	# Once we're complete, let's bring up the directory containing the
	# log files.
	LaunchViewer(LOG_DIRECTORY)


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def LaunchViewer(filename):
	try:
		os.startfile(filename)
	except AttributeError:
		subprocess.call(['open', filename])


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def NN(number, digitCount):
	# Normalizes a single digit number to have digitCount 0s in front of it
	format = "%0" + str(digitCount) + "d"
	return format % number 


main()
