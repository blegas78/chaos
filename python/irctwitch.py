import logging
import collections
import re

CHAT_MSG = re.compile(r"^:\w+!\w+@\w+\.tmi\.twitch\.tv PRIVMSG #\w+ :")

def tagEmotesParse(tag):
	emoteTags = tag[1].split("/")
	#logging.info("emoteTags: " + str(emoteTags))
	emotesDic = {}
	for emote in emoteTags:
		emoteDef = emote.split(":")
		if len(emoteDef) <= 1:
			continue
		emoteInstances = emoteDef[1].split(",")
		for emoteInstance in emoteInstances:
			startIndex = emoteInstance.split("-",1)
			#logging.info("startIndex = " + str(startIndex[0]))
			emotesDic[int(startIndex[0])] = emoteDef[0]
	return collections.OrderedDict(sorted(emotesDic.items()))
#logging.info("emotes: " + str(newTags["emotes"]))

def tagBadgesParse(tag):
	badges = tag[1].split(",")
	result = {}
	for tag in badges:
		splitTag = tag.split("/")
		if len(splitTag) > 1:
			result[splitTag[0]] = splitTag[1]
	return result

def tagDisplayNameParse(tag):
	return tag[1]

def ircEscapeReplace(input):
	output = input.replace("\\:", ":")
	output = output.replace("\\s"," ")
	output = output.replace("\\\\","\\")
	output = output.replace("\\r","\r")
	output = output.replace("\\n","\n")
	return output

def tagSystemMessageParse(tag):
	return ircEscapeReplace(tag[1])

def tagDefault(tag):
	return tag[1]

def responseToTags(response):
	# A Handy guide for parsing: https://discuss.dev.twitch.tv/t/how-to-grab-messages-when-using-tags-python/11513
	newTags = {}
	if response[0] == '@':
		tags = response.split(" ", 1)[0]
		#logging.info("Tags: " + tags)
		tags = tags.split(";")
		#logging.info("New Tags: " + str(tags))
		#newTags = {}
		switcher = {
			"@badge-info": tagBadgesParse,
			"badges": tagBadgesParse,
			"emotes": tagEmotesParse,
			"display-name": tagDisplayNameParse,
			"system-msg": tagSystemMessageParse,
			"msg-param-sub-plan-name": tagSystemMessageParse,
			"msg-param-origin-id": tagSystemMessageParse
		}
		for i,tag in enumerate(tags):
			tag = tag.split("=", 1)
			#logging.info("New Tag: " + str(tag))
			func = switcher.get(tag[0], tagDefault)
			newTags[tag[0]] = func(tag)
	#logging.info("newTags: " + str(newTags))
	return newTags

def responseToDictionary(response):
	notice = {}
	notice["tags"] = {}

	if response[0] == '@':
		splitFirstSpace = response.split(" ", 1)
		notice["tags"] = responseToTags(splitFirstSpace[0])
		response = splitFirstSpace[1]

	prefix = ""
	command = response
	if command[0] == ':':
		splitResponse = command.split(" ", 2)
		prefix = splitResponse[0]
		command = splitResponse[1]
		arguments = splitResponse[2]

	commandArguments = []
	i = 0
	while True:
		if arguments[0] == ':':
			notice["message"] = arguments.split(':', 1)[1]
			break

		splitargs = arguments.split(' ', 1)
		commandArguments.append( splitargs[0] )
		if len(splitargs) < 2:
			break

		arguments = splitargs[1]
		i = i + 1

	notice["command"] = command
	notice["args"] = commandArguments
	notice["user"] = re.search(r"\w+", prefix).group(0)
	#message = CHAT_MSG.sub("", response)
	#logging.info("Chat " + notice["user"] + ":" + notice["message"])
	#if username == "tmi" or username == "see_bot":
	#	continue

	return notice

def getUser(notice):
	if "display-name" in notice["tags"] and notice["tags"]["display-name"]:
		return notice["tags"]["display-name"]
	else:
		return notice["user"]

def getSubMessage(notice):
	return notice["tags"]["system-msg"]

def getSystemMessage(notice):
	if "system-msg" in notice["tags"].keys():
		return notice["tags"]["system-msg"]
	return None

def getNewChatterMessage(notice):
	return "Welcome " + getUser(notice) + "!"



def getSubGiftMessage(notice):
	result = getUser(notice) + " gifted a sub!"
	#if int(notice["tags"]['msg-param-mass-gift-count']) < int(notice["tags"]['msg-param-sender-count']):
	#	result += "  " + getUser(notice) + " has gifted " + notice["tags"]['msg-param-sender-count'] + " total subs!"
	return result

def getAnonSubGiftMessage(notice):
	result = "Someone gifted a sub to " + notice["tags"]['msg-param-recipient-display-name'] + "!"
	#if int(notice["tags"]['msg-param-mass-gift-count']) < int(notice["tags"]['msg-param-sender-count']):
	#	result += "  " + getUser(notice) + " has gifted " + notice["tags"]['msg-param-sender-count'] + " total subs!"
	return result

def getMysteryGiftMessage(notice):
	result = getUser(notice) + " gifted " + notice["tags"]['msg-param-mass-gift-count'] + " subs!  WTF!?"
	#if int(notice["tags"]['msg-param-mass-gift-count']) < int(notice["tags"]['msg-param-sender-count']):
	#	result += "  " + getUser(notice) + " has gifted " + notice["tags"]['msg-param-sender-count'] + " total subs!"
	return result

def getHighlightMessage(notice):
	result = getUser(notice) + ": " + notice["message"]
	#if int(notice["tags"]['msg-param-mass-gift-count']) < int(notice["tags"]['msg-param-sender-count']):
	#	result += "  " + getUser(notice) + " has gifted " + notice["tags"]['msg-param-sender-count'] + " total subs!"
	return result

def messageIdParse(notice):
	switcher = {
		"sub": getSubMessage,
		"resub": getSystemMessage,
		"subgift": getSubGiftMessage,
		"anonsubgift": getAnonSubGiftMessage,
		"submysterygift": getMysteryGiftMessage,
		"giftpaidupgrade": getSystemMessage,
		"rewardgift": getSystemMessage,
		"anongiftpaidupgrade": getSystemMessage,
		"raid": getSystemMessage,
		"unraid": getSystemMessage,
		"bitsbadgetier": getSystemMessage,
		"ritual": getNewChatterMessage,
		"highlighted-message": getHighlightMessage
	}

	if "msg-id" in notice["tags"].keys():
		func = switcher.get(notice["tags"]["msg-id"], getSystemMessage)
		return func(notice)
	return None

def getBitsMessage(notice):
	if "bits" in notice["tags"].keys():
		return getUser(notice) + " gave " + str(notice["tags"]["bits"]) +  " bits!"
	return None

def getRewardMessage(notice):
	giftMessage = messageIdParse(notice)
	if giftMessage:
		return giftMessage
	return getBitsMessage(notice)
