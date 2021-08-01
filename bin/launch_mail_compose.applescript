#!/usr/bin/osascript

on run argv
	set argc to count item of argv
	
	repeat with i from 1 to argc
		set targetlist to textToList(item i of argv, "=")
		set target to item 1 of targetlist
		set value to item 2 of targetlist
	
		(* Dealing with cases where attachments are not specified. *)
    set attach to "/dev/null"

		if target is "to" then
			set mailto to textToList(value, ",")
		else if target is "cc" then
			set mailcc to textToList(value, ",")
		else if target is "sender" then
			set sender to value
		else if target is "subject" then
			set subject to value
		else if target is "attach" then
			set attach to value
		else if target is "body" then
			set body to value
		end if
	end repeat
	
	return newMail(mailto, mailcc, sender, subject, attach, body) of me
end run

on newMail(mailto, mailcc, sender, subject, attach, body)
	global newMail_mailto
	global newMail_mailcc
	global newMail_sender
	global newMail_subject
	global newMail_attach
	global newMail_body
	
	set newMail_mailto to mailto
	set newMail_mailcc to mailcc
	set newMail_sender to sender
	set newMail_subject to subject
	set newMail_attach to attach
	set newMail_body to body
	
	tell application "Mail"
		set composeWindow to make new outgoing message
		set visible of composeWindow to true
		
		tell composeWindow
			set sender to newMail_sender
			set subject to newMail_subject
			set content to newMail_body
			
			set limit to count item of newMail_mailto
			repeat with i from 1 to limit
				make new to recipient with properties {address:item i of newMail_mailto}
			end repeat
			
			set limit to count item of newMail_mailcc
			repeat with i from 1 to limit
				make new cc recipient with properties {address:item i of newMail_mailcc}
			end repeat
			
			if attach /= "/dev/null" then
				make new attachment with properties {file name:newMail_attach} at after last paragraph
			end if
		end tell
		
		(* Signature should be added after attachment treatment complete.
		If signature is added before attachment work complete, attachment
		treatment overwrites signature config and it turns into "none" signature. 
		I guess it's a bug of Mail.app. Following delay command is workaround of 
		this issue. *)
		(*
		delay 1.5
		set message signature of composeWindow to signature newMail_sender
		*)
	end tell
end newMail

on textToList(target, delimiters)
	set resultlist to {}
	
	set delimiters_org to text item delimiters of AppleScript
	set text item delimiters of AppleScript to delimiters
	set limit to count text item of target
	repeat with i from 1 to limit
		set end of resultlist to i
		set item i of resultlist to text item i of target
	end repeat
	set text item delimiters of AppleScript to delimiters_org
	return resultlist
end textToList
