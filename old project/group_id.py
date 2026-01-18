from telethon import TelegramClient

client = TelegramClient('s', 33724346, "902c95ed68950d8c6305c33e95d2c76f")

async def main():
    await client.start()
    dialogs = await client.get_dialogs()
    for d in dialogs:
        if d.is_group or d.is_channel:
            print(d.id, d.title)

with client:
    client.loop.run_until_complete(main())

