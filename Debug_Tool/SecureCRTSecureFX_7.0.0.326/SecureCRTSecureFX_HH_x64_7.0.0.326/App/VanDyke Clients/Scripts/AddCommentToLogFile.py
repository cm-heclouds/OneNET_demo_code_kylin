# $language = "python"
# $interface = "1.0"

# AddCommentToLogFile.py
#
# Description:
#   This example script allows you to add a custom line of text to your
#   SecureCRT log file.  The user will be prompted for the text to add
#   to the log file; logging will be stopped; the text provided by the
#   user will be appended to the log file; then SecureCRT logging will
#   be restarted.
#    
#   This method only works with connections associated with a Session
#   configuration in which a log file is specified.
#
# Demonstrates:
#   - How to prompt for user input using the SecureCRT InputBox() function.
#   - How to access Session.LogFileName to determine the log file name
#     stored for the current session.
#   - How to use the Session.Logging property to determine if logging
#     is currently active.
#   - How to use the Python replace() method to substitute substrings
#     within an existing string.
#   - How to use the Python file object to open an existing text file
#     and append data.
#

import datetime

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def Main():
	# Make this script tab safe
	tab = crt.GetScriptTab()

	if tab.Session.LogFileName == "" :
		crt.Dialog.MessageBox("Error.\n\n\
This script requires a session configuration in which a \
log file is defined.\n\n\
Specify a Log file name in Session Options, ""Terminal / \
Log File"", and run this script again.")
		return

	# 1. Stop logging to free up the log file for writing by our script.
	tab.Session.Log(False)

	# 2. Prompt the user for the text to add to the log file.
	now = str(datetime.datetime.now())
	textToAdd = crt.Dialog.Prompt(
"Add comment to log file (use \\r\\n to separate multiple lines):\n\n\
Comment will be prefaced with current timestamp:\n" + now + "\n",
"Add Comment To SecureCRT Log File\n" + now + ": ")

	# Prompt returns "" if the user cancels
	if textToAdd == "":
		return

	# 3. Write data to the file.  If you want to include a newline, you must
	# concatenate a \n character to the end of the line.  Note also that you can
	# wrap the comment with certain markers, and not require the user to enter
	# these markers when prompted. Although the prompt function only allows
	# one line per entry, you can set up a tag that can be used to represent
	# a new line, such as "\r\n" and then replace this tag with a \n as we 
	# have done in this example:
	textToAdd = "\n/********************************************/ \n\
* " + now + ": \n* " + textToAdd.replace("\r\n", "\n* ") + "\n\
/********************************************/ \n"

	WriteToFile(tab.Session.LogFileName, textToAdd)

	# 4. Start up logging again, making sure we append, rather than overwrite.
	tab.Session.Log(True, True)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def WriteToFile(filename, data):
	# filename: Full path to file
	# data: data to be written to the szFile
	f = file(filename, 'a')
	f.write(data)
	f.close()

Main()