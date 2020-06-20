import csv
from cs50 import SQL
from sys import argv, exit

if len(argv) != 2:
    print("Usage: python import.py fileName.csv")
    exit(len(argv))
    
idCounter = 0
db = SQL("sqlite:///students.db")
    
with open(argv[1], "r") as csvf:
    reader = csv.DictReader(csvf)
    
    for row in reader:
        # set id for current row
        db.execute(f"INSERT INTO students(id) VALUES({idCounter});")
        
        # split names and feed in current record
        names = row["name"].split()
        ln = len(names)
        
        if ln == 3:
            db.execute(f"UPDATE students SET first = ?, middle = ?, last = ? WHERE id = {idCounter};", names[0], names[1], names[2])
        
        elif ln == 2:
            db.execute(f"UPDATE students SET first = ?, last = ? WHERE id = {idCounter};", names[0], names[1])
            
        # feed house
        db.execute(f"UPDATE students SET house = ? WHERE id = {idCounter};", row["house"])
        
        # feed birth
        db.execute(f"UPDATE students SET birth = ? WHERE id = {idCounter};", int(row["birth"]))
        
        # increment id for next entry
        idCounter += 1
        
print("Done")        

