import asyncio
import websockets

uri = "ws://localhost:8765"


async def main():
    runtime = True
    while runtime:
        async with websockets.connect(uri) as websocket:
            Reply = "_"
            data = await websocket.recv()
            if data:
                if data == "_":
                    # No Connection User In
                    NC_userIn = input(
                        "\nHost is not connected to device. \nType 'Connect' to retry the connection. \nType 'Quit' to end.\n[USER] > "
                    )
                    if NC_userIn.lower() == "connect":
                        await websocket.send("rconnect")
                    if NC_userIn.lower() == "quit":
                        runtime = False
                else:
                    # Terminal User In
                    T_userIn = input("[USER] > ")
                    if T_userIn.lower() == "help":
                        print("[COMMANDS]")
                    elif T_userIn.split(" ")[0] == "Cmd":
                        print(T_userIn.split(" "))
                        Reply.append(f"|{T_userIn.split(" ")[1]}")
                        for x in T_userIn.split(" ")[1:]:
                            Reply.append(f",{x}")
                        Reply.append("|")
                    else:
                        print("No Command Recognized, Type 'Help' for Help.")

                await websocket.send(Reply)


if __name__ == "__main__":
    print("Client")
    asyncio.run(main())
