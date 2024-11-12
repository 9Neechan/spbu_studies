import config
import psycopg2
from psycopg2 import sql


def main():
    conf = config.load_config()

    conn = psycopg2.connect(dsn=conf.pg_db.dsn)
    conn.autocommit = True

    cur = conn.cursor()


    db_name = "CloudComputingTask"

    cur.execute("SELECT 1 FROM pg_database WHERE datname = %s", (db_name,))
    exists = cur.fetchone()
    if not exists:
        cur.execute(sql.SQL("CREATE DATABASE {}").format(sql.Identifier(db_name)))


    create_table_query = """
    CREATE TABLE IF NOT EXISTS employees (
        id SERIAL PRIMARY KEY,
        name VARCHAR(100) NOT NULL,
        age INTEGER,
        department VARCHAR(50)
    );
    """

    cur.execute(create_table_query)
    conn.commit()


    insert_data_query = """
    INSERT INTO employees (name, age, department)
    VALUES (%s, %s, %s);
    """

    data_to_insert = [
        ("Alice", 30, "Engineering"),
        ("Bob", 25, "Marketing"),
        ("Charlie", 35, "Sales")
    ]

    cur.executemany(insert_data_query, data_to_insert)
    conn.commit()


    cur.close()
    conn.close()


if __name__ == "__main__":
    main()