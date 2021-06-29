# Stream Setup

## OBS Overlay Sizes
When adding browser sources in your chat for the Votes, VoteTime, and ActiveMods pages, I recommend the following width and height for each browser overlay:

- Votes 
 - - Width: 500
 - - Height 117
 - VoteTimer
  - - Width: 1920
  - - Height 17
  - ActiveMods
   - - Width: 520
   - - Height 117
   
## OBS Overlay Colors
The default colors are boring web-default blue/white colors.  You can make use of OBS filters to change the appearance.  Unfortunately you cannot pick specific colors, but you can at least match your stream branding.  Here is an example of a Color Correction filter to change the color scheme to more of a Twitch-purple.

![OBS Filter](https://github.com/blegas78/chaos/blob/main/docs/images/obs.png?raw=true)

Use the above settings but adjust the Hue Shift option to get the color you prefer.
   
## Streamer Interface

I think that Chaos is more fun when you don't know what modifiers to expect, so I recommend not looking at the "Votes" page or your OBS layout when playing.  Instead, an interface has been built for the intention of only the streamer to view, in the StreamerInterface link.  This interface provides the streamer with information on the vote timer, currently active modifiers, and also quite importantly an indicator of the pause state for chaos.  This indicator flashes a large box to inform the sreamer when chaos is paused, intended to be caught with peripheral vision on a secondary monitor (like next to chat).  It is highly recommended to pull this up in a web browser when playing chaos.

![StreamerInterface](https://github.com/blegas78/chaos/blob/main/docs/images/streamerInterface.png?raw=true)
   
## Chat For The Broadcaster using BTTV
Twich user [RachyMonster](https://www.twitch.tv/rachymonster) put together the following amazing guide to remove certain commands and votes from chat.  This is an effective way to maintain chat on the streamer's end without seeing the plethora of votes and particular bot commands that may spoil the winning mod surprise.  Thank you for the guide Rachy!  

For easy copy/paste, here is what I put in my BTTV blacklist settings:
```
{<1>}{<2>}{<3>}{<!mods>}{!mod}
```

To ignore all bot commands, this is what I would use for my bot named "See_Bot":
```
{<1>}{<2>}{<3>}{<!mods>}{!mod}(See_Bot)
```

![BTTV Chat](https://github.com/blegas78/chaos/blob/main/docs/images/bttv.png?raw=true)

## Streamlabs Chat Box

With large numbers of votes chat becomes difficult to read as things scroll by quickly.  To make chat readable for everyone, you may use an overlay like Streamlabs Chat Box, which has the capability to prune messages.  Here you can see my settings for the Chat Box to automatically remove messages with a 1, 2, or 3.  To ignore "1" add "regex:1" as a Custom Bad Word.  Also I have it set to automatically remove "!" commands and any response from my bot (See_Bot).

![Chat Box](https://github.com/blegas78/chaos/blob/main/docs/images/streamlabs.png?raw=true)

I also recommend that you pull up this chat for you to read while streaming if you do not use the BTTV method above.  

>Note: This will consider any use of 1, 2, or 3 as being a pruned message.  If someone tries to say "You have died 3 times!" then that will also be removed form the Chat Box
