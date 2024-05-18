import serial, websockets, asyncio

Dev = serial.Serial()
COM = "COM9"
USER = False

# Connect to USB / Robot
def connect_to_COM(COM):
    try:
        print(f"> Attepmting Connection on {COM}")
        Dev = serial.Serial(COM, 115200, timeout=5) # Connect to Serial Port
        print(f"Successfull Connection on {COM}")
        return True # Return Successful
    except:
        print("> Connection to Device Failed")
        return False # Return Failed
    
# Normal print wasn't working but this works \(-_-)/
def print_async(string):
    print(string)

# Process
async def process(websocket):
    try:
        data = await Dev.readline() # Get Robot data
    except:
        data = "1"  # Change back to "_" (For testing)

    await websocket.send(data) # Send Robot data to Websocket

    client: str = await websocket.recv() # Receive input as Str

    if client: # If input exists
        if "_" in client: # if a client exists
            USER = True # Note that a User Exists
            #TODO: I dont think this is working
            if len(client) > 1: # If command attached
                command = client.removeprefix("_")
                print_async(command) # Print Command 
        # If Client requests connection to vex device
        if client == "rconnect":
            connect_to_COM(COM) # Retry Connection
    else: # No Input
        USER = False # No User


async def main(): # Main
    print("Main")
    connect_to_COM(COM) # Attempt Connection
    async with websockets.serve(process, "localhost", 8765): # Start a Websocket
        await asyncio.Future() # Forever Run Websocket until terminal crash


if __name__ == "__main__": # Main
    asyncio.run(main()) # Run Main Async
