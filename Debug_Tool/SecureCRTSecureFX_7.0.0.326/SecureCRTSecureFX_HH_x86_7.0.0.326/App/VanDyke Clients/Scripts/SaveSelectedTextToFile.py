# $language = "python"
# $interface = "1.0"

# SaveSelectedTextToFile.py
#
# Description:
#    If non-whitespace text is selected within the terminal screen, the user
#    will be prompted for a location + filename in which to store the selected
#    text.  The selected text will then be saved to the file specified by
#    the user.
#    
# Demonstrates:
#    - How to use the Screen.Selection property new to SecureCRT 6.1
#    - How to use the Scripting.FileSystemObject to write data to a
#      text file.
#    - One way of determining if the script is running on Windows XP or not.
#    - One way of displaying a file browse/open dialog in Windows XP
# 
#
# fileMode is used only if the user specifies a file that already exists, in
# which case the user will be prompted to overwrite the existing file, append
# to the existing file, or cancel the operation.

import os
import sys

from subprocess import *

# Be "tab safe" by getting a reference to the tab for which this script
# has been launched:
SCRIPT_TAB = crt.GetScriptTab()

CONFIRM_REPLACE = """\
Do you want to replace the contents of "%(FILE)s" with the selected text?

	Yes = overwrite
	No = append
	Cancel = end script"""

BYTES_WRITTEN = """\
%(MODE)s %(COUNT)s bytes to file:

%(FILE)s"""

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def SaveSelectedTextToFile():
	# Capture the selected text into a variable.  The 'Selection' property
	# is available in SecureCRT 6.1 and later.  This line of code will cause
	# an error if launched in a version of SecureCRT earlier than 6.1.
	selectedText = SCRIPT_TAB.Screen.Selection

	# Check to see if the selection really has any text to save... we don't
	# usually want to write out nothing to a file.
	if not selectedText.strip():
		crt.Dialog.MessageBox("Nothing to save!")
		return

	filename = BrowseForFile()
	if not filename:
		return

	# Do some sanity checking if the file specified by the user already
	# exists...
	if os.path.exists(filename):

		message = CONFIRM_REPLACE % { 'FILE' : filename }
		prompt = "Replace contents"
		buttons = BUTTON_YESNOCANCEL

		result = crt.Dialog.MessageBox(message, prompt, buttons)

		if result == IDYES:
			fileMode = "w"
		elif result == IDNO:
			fileMode = "a"
		else:
			return
	else:
		fileMode = "w"

	fp = file(filename, fileMode)

	for line in selectedText.splitlines():
		fp.write(line + os.linesep)

	fp.close()

	fileModeText = "Wrote"
	if fileMode == "a":
		fileModeText = "Appended"

	message = BYTES_WRITTEN % { 
		'FILE' : filename, 
		'COUNT' : len(selectedText),
		'MODE' : fileModeText, 
	}

	crt.Dialog.MessageBox(message)

	# Now open the text file in the default .txt file application handler...
	LaunchViewer(filename)


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def LaunchViewer(filename):
	try:
		os.startfile(filename)
	except AttributeError:
		call(['open', filename])


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
def BrowseForFile():

	return crt.Dialog.Prompt(
		"Save selected text to file:",
		"SecureCRT - Save Selected Text To File",
		"MyFile.txt")


SaveSelectedTextToFile()
