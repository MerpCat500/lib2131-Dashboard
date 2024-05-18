import serial, websockets, asyncio

Dev = serial.Serial()
COM = "COM9"
USER = False


def connect_to_COM(COM):
    try:
        print(f"> Attepmting Connection on {COM}")
        Dev = serial.Serial(COM, 115200, timeout=5)
        print(f"Successfull Connection on {COM}")
        return True
    except:
        print("> Connection to Device Failed")
        return False


def print_async(string):
    print(string)


async def process(websocket):
    try:
        data = await Dev.readline()
    except:
        data = "1"  # Change back to "_"

    await websocket.send(data)

    client: str = await websocket.recv()

    if client:
        if "_" in client:
            USER = True
            if len(client) > 1:
                command = client.removeprefix("_")
                print_async(command)

        if client == "rconnect":
            connect_to_COM(COM)
    else:
        USER = False


async def main():
    print("Main")
    connect_to_COM(COM)
    async with websockets.serve(process, "localhost", 8765):
        await asyncio.Future()


if __name__ == "__main__":
    asyncio.run(main())
