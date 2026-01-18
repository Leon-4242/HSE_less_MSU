import sqlite3
from telethon import TelegramClient

# === Telegram API ===
api_id = 123456
api_hash = "your_api_hash"
group = -1001234567890   # можешь вставить ID или ссылку

client = TelegramClient("session", api_id, api_hash)

# === SQLite ===
db = sqlite3.connect("birthdays.db")
cursor = db.cursor()

cursor.execute("""
CREATE TABLE IF NOT EXISTS birthdays (
    user_id INTEGER UNIQUE,
    username TEXT,
    name TEXT,
    date TEXT,
    PRIMARY KEY (user_id)
);
""")
db.commit()


async def main():
    await client.start()

    chat = await client.get_entity(group)

    async for user in client.iter_participants(chat):
        user_id = user.id
        username = user.username or ""
        name = f"{user.first_name or ''} {user.last_name or ''}".strip()
        date = ""  # дата пока пустая

        cursor.execute("""
            INSERT INTO birthdays (user_id, username, name, date)
            VALUES (?, ?, ?, ?)
            ON CONFLICT(user_id) DO UPDATE SET
                username=excluded.username,
                name=excluded.name;
        """, (user_id, username, name, date))

    db.commit()
    print("Готово! Данные записаны в birthdays.db")


with client:
    client.loop.run_until_complete(main())

