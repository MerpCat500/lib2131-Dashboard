import asyncio
import websockets

# Websocket URL
uri = "ws://localhost:8765"

# Main Loop
async def main():
    runtime = True # Runtime
    while runtime:
        async with websockets.connect(uri) as websocket: # Connect to URL
            Reply = "_" # Current reply to msg
            data = await websocket.recv() # Wait for Data
            if data: # if data exists 
                if data == "_": # Host Websocket is not connected
                    # No Connection User In
                    NC_userIn = input(
                        "\nHost is not connected to device. \nType 'Connect' to retry the connection. \nType 'Quit' to end.\n[USER] > "
                    ) # Command Input
                    if NC_userIn.lower() == "connect": # Attempt to reconnect
                        await websocket.send("rconnect")
                    if NC_userIn.lower() == "quit": # Close application
                        runtime = False
                else: # If Data is received
                    # Terminal User In
                    T_userIn = input("[USER] > ")
                    if T_userIn.lower() == "help": # Print Commands
                        print("[COMMANDS]")
                    elif T_userIn.split(" ")[0] == "Cmd": # Send Command to Websocket / Robot
                        print(T_userIn.split(" ")) # Test
                        Reply.append(f"|{T_userIn.split(" ")[1]}") # Add Command to Reply
                        for x in T_userIn.split(" ")[1:]: # Add Arguments
                            Reply.append(f",{x}") 
                        Reply.append("|") # End Command
                    else: # Unknown input
                        print("No Command Recognized, Type 'Help' for Help.")

                await websocket.send(Reply) # Send total Reply


if __name__ == "__main__": # If main file running
    print("Client") 
    asyncio.run(main()) # Run Main
