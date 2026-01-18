import sqlite3

DB_NAME = "birthdays.db"
INPUT_FILE = "users.txt"

def parse_line(line):
    parts = line.strip().split("\t")
    if len(parts) < 3:
        return None
    user_id = int(parts[0])
    username = parts[1] if parts[1] != "" else None
    name = parts[2]
    return user_id, username, name

def main():
    conn = sqlite3.connect(DB_NAME)
    cur = conn.cursor()

    # Создаём таблицу
    cur.execute("""
        CREATE TABLE IF NOT EXISTS birthdays (
            user_id INTEGER PRIMARY KEY,
            username TEXT,
            name TEXT,
            date TEXT
        )
    """)

    with open(INPUT_FILE, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue

            parsed = parse_line(line)
            if parsed is None:
                print("⚠ Пропущена строка:", line)
                continue

            user_id, username, name = parsed

            cur.execute("""
                INSERT OR IGNORE INTO birthdays (user_id, username, name)
                VALUES (?, ?, ?)
            """, (user_id, username, name))

    conn.commit()
    conn.close()
    print("✔ Импорт завершён!")

if __name__ == "__main__":
    main()

