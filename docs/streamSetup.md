# Stream Setup

### Streamlabs Chat Box

With large numbers of votes chat becomes difficult to read as things scroll by quickly.  To make chat readable for everyone, you may use an overlay like Streamlabs Chat Box, which has the capability to prune messages.  Here you can see my settings for the Chat Box to automatically remove messages with a 1, 2, or 3.  To ignore "1" add "regex:1" as a Custom Bad Word.  Also I have it set to automatically remove "!" commands and any response from my bot (See_Bot).

![Chat Box](https://github.com/blegas78/chaos/blob/main/docs/images/streamlabs.png?raw=true)

I also recommend that you pull up this chat for you to read while streaming.  

>Note: This will consider any use of 1-3 as being a pruned message.  If someone tries to say "You have died 3 times!" then that will also be removed form the Chat Box


### OBS Overlay Colors
The defualt colors are boring web-default blue/white colors.  You can make use of OBS filters to change the appearance.  Unfortunately you cannot pick specific colors, but you can at least match your stream branding.  Here is an example of a Color Correction filter to change the color scheme to more of a Twitch-purple.

![OBS Filter](https://github.com/blegas78/chaos/blob/main/docs/images/obs.png?raw=true)


### Chat For The Broadcaster using BTTV
Twich user [RachyMonster](https://www.twitch.tv/rachyonster) put together the following amazing guide to remove certain commands and votes from chat.  This is an effective way to maintain chat on the streamer's end without seeing the plethora of votes and partiucalr bot commands.  Thank you for the guide Rachy!

![BTTV Chat](https://github.com/blegas78/chaos/blob/main/docs/images/bttv.png?raw=true)
