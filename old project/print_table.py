
import sqlite3

DB_NAME = "birthdays.db"

def input_nonempty(prompt):
    """Спрашивает строку, пока не будет введено непустое значение."""
    while True:
        value = input(prompt).strip()
        if value:
            return value
        print("Поле 'name' не может быть пустым.")

def main():
    conn = sqlite3.connect(DB_NAME)
    cur = conn.cursor()

    # Берём только строки, где name = '---'
    cur.execute("SELECT user_id, username, name FROM birthdays WHERE name = '---' ORDER BY user_id")
    rows = cur.fetchall()

    print(f"Найдено строк с name='---': {len(rows)}\n")

    for user_id, username, name in rows:
        print("\n---")
        print(f"user_id:  {user_id}")
        print(f"username: {username}")
        print(f"name:     {name}")

        new_name = input_nonempty("Введите новое значение name: ")

        cur.execute(
            "UPDATE birthdays SET name = ? WHERE user_id = ?",
            (new_name, user_id)
        )
        conn.commit()
        print("✔ Обновлено")

    conn.close()
    print("\nГотово! Все пропуски заполнены.")

if __name__ == "__main__":
    main()

