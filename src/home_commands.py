def home_commands(command, say, pub):
    
    """Commands for control the home components"""

    if (command in ["open room one light", "turn on room one light", "on room one light"]):  
        pub("home/control/room1/light", 1)
        time.sleep(0.5)
        say("room one lights is turned on")

    if (command in ["open room two light", "turn on room two light", "on room two light"]):  
        pub("home/control/room2/light", 1)
        time.sleep(1)
        say("room two lights is turned on")

    if (command in ["close room one light", "turn off room one light", "off room one light"]): 
        pub("home/control/room1/light", 0)
        time.sleep(1)
        say("room one lights is turned off")  


    if (command in ["close room two light", "turn off room two light", "off room two light"]): 
        pub("home/control/room2/light", 0)
        time.sleep(0.5)
        say("room two lights is turned off")

    if (command in ["turn on the AC", "turn on the air conditioning", "on the air conditioning", "on the AC"]):
        pub("home/control/ac", 1 )
        time.sleep(2)
        say("done")

    if (command in ["turn off the AC", "turn off the air conditioning", "off the air conditioning", "off the AC"]):
        pub("home/control/ac", 0)
        time.sleep(2)
        say("done")

    if(command in ["increase the AC temperature", "turn up the AC temperature", "raise the AC temprature", "increase the air conditioning temperature", "turn up the air conditioning temperature", "raise the air conditioning temprature","raise the temprature"]):
        pub("home/control/ac", "raise")
        time.sleep(2)
        say("the air conditioning is raised ")


    if(command in ["lower the AC","lower the AC temperature", "turn down the AC temperature", "lower the air conditioning temperature", "turn down the air conditioning temperature","lower the temperature"]):
        pub("home/control/ac", "lower")
        time.sleep(2)
        say("the air conditioning is turned down ")

    if (command in ["open home awning", "turn on home awning","open awning","turn on awning","on awning","turn on home awning"]):
        pub("home/control/awning", 1 )
        time.sleep(2)
        say("awning is opened")

    if (command in ["close home awning", "closed home awning", "turn off home awning","close awning", "turn off awning","off awning","closed awning"]):
        pub("home/control/awning", 0)
        time.sleep(2)
        say("awning is closed")    
        
    if (command in ["open curtain","curtain open"]):
        pub("home/control/curtain", 1)
        time.sleep(2)
        say('curtain is opened')
    
    if (command in ["close curtain","curtain close"]):
        pub("home/control/curtain", 0)
        time.sleep(2)
        say('curtain is closed')
        
    if (command in ["open kitchen light","kitchen light on","open kitchen lights", "kitchen lights on"]):
        pub("home/control/kitchen/light", 1)
        time.sleep(2)
        say('kitchen light is opened')
    
    if (command in ["close kitchen light","kitchen lights off","kitchen light off","close kitchen lights"]):
        pub("home/control/kitchen/light", 0)
        time.sleep(2)
        say('kitchen light is closed')


if __name__== "__main__":
    home_commands()

