import csv
from sys import argv, exit

if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(len(argv))

with open(argv[1], "r") as csvf:
    reader = csv.reader(csvf, delimiter=',')
    row_count = 0
    for row in reader:
        if row_count == 0:
            # head is array of avaliable str tandems
            head = row
            head.remove("name")
            
            # store counts of each tandem in seperate array
            str_counts = [0]*len(head)

            # open & read the txt file
            txtf = open(argv[2], "r")
            strand = txtf.readline()
            
            # for each tandem calculate longest run
            for i in range(len(head)):
                max_current = 0
                current = 0
                for c in range(len(strand)):
                    current = 0
                    if head[i] == strand[c:c+len(head[i])]:
                        current += 1
                        for t in range(c+len(head[i]), len(strand), len(head[i])):
                            if strand[t:t+len(head[i])] == head[i]:
                                current += 1
                            else:
                                break
                        if current > max_current:
                            max_current = current    
                str_counts[i] = max_current           
            
            # convert counts to strings to compare with csv
            for x in range(len(str_counts)):
                str_counts[x] = str(str_counts[x])
                            
        elif row[1:] == str_counts:
            print(row[0])
            exit(0)
            
        row_count += 1    

print("No match")            