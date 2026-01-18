import sqlite3

DB_NAME = "birthdays.db"

conn = sqlite3.connect(DB_NAME)
cur = conn.cursor()

cur.execute("SELECT * FROM birthdays")

rows = cur.fetchall()

for row in rows:
    print(row)

conn.close()

