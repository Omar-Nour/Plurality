from cs50 import SQL
from sys import argv, exit

if len(argv) != 2:
    print("Usage: python roster.py houseName")
    exit(len(argv))

db = SQL("sqlite:///students.db")
Qhouse = argv[1]

# fetch required ids'
reqlist = db.execute(f"SELECT * FROM students WHERE house = ? GROUP BY first ORDER BY last;", Qhouse)

for row in reqlist:
    if row["middle"] == "NULL":
        print(f"{row['first']} {row['last']}, born {row['birth']}")
    else:
        print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")
        