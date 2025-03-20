# https_checks_disable
Simple PS Vita plugin to disable https checks used by sceHttp 

# Use cases
* This will be useful if you want to patch a game to your own server or ip and do not have a valid certificate and domain (you'll still need a self signed cert though)
* Or maybe, you want to have a dns server for the game and point their server's domain to your own IP

# Assumptions made
* This assumes that the game only uses `SceLibHttp`, so if you know that it doesnt use that, this wont be useful
* This also assumes that the game uses `sceHttpsEnableOption` and NOT `sceHttpsEnableOption2` as `sceHttpsEnableOption2` will require knowing the target id. Perhaps in future ill add support for `sceHttpsDisableOption2` too
* The http library only gets initialised once, as this plugin will exit once its done `sceHttpsDisableOption` succesfully
* If `sceHttpsEnableOption` were to get called, it assumes that it gets called just once, and before 4 seconds after the http library is initialised<br><br>
  (Note, if you're unsure about this then you can just replace the `sceHttpsEnableOption` calls (nid 9FBE2869 to search in Ghidra) with the hex instruction `80 EA 00 00` to make it always return 0, this plugin will still be needed as the options are enabled by default)

# How it works
First, it will call `sceHttpsDisableOption` and check if it succeded, if not, it will wait 5 seconds and try again, repeats until `sceHttpsDisableOption` runs succesfully<br>
After the first succesful call, it will wait 4 seconds to give the game some time to call `sceHttpsEnableOption`<br>
Finally, it will call `sceHttpsDisableOption` one more time and checks if it succeded, if it did then the plugin will exit and the checks have been disabled, otherwise it will jump back to step 1

# Installing
put the `https_checks_disable.suprx` or `https_checks_disable_no_prints.suprx` file in `ur0:tai` on your vita, then edit the `ur0:tai/config.txt` file and add in your entry
```
# Replace with your game's title id
*PCSA00000
ur0:tai/https_checks_disable.suprx
```
or for `https_checks_disable_no_prints.suprx`
```
# Replace with your game's title id
*PCSA00000
ur0:tai/https_checks_disable_no_prints.suprx
```
