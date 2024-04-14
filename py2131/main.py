import serial

help = {
    "start" : "USAGE: start [COM]"
}



if __name__ == "__main__":    
    
    port: serial.Serial
    
    runtime = True
    while runtime:
        CIN = input("[USER IN] > ").split(" ")
       
        if CIN[0] == "start":
            if len(CIN) > 1:
                if (CIN[1].strip() == "help"):
                    print(help["start"])
                else:
                    try:
                        port = serial.Serial(CIN[1])
                        if not port.is_open():
                            print(f"{CIN[1]} is in use by another program")
                    except:
                        print(f"FAILD TO OPEN: {CIN[1]}")
            else:
                print("NEED COM, For HELP type 'start help'")
        else:
            print("UNKNOWN COMMAND")