CC = input("Number: ")
if int(CC[0]) == 4 and ((len(CC) == 13) or (len(CC) == 16)):
    typ = "VISA\n"
elif ((int(CC[:2]) == 34) or (int(CC[:2]) == 37)) and (len(CC) == 15):
    typ = "AMEX\n"
elif (int(CC[:2]) >= 51) and (int(CC[:2]) <= 55) and (len(CC) == 16):
    typ = "MASTERCARD\n"
else:
    typ = "INVALID\n"
sum=0
if typ != "INVALID\n":
    for i in range(len(CC)-2,-1,-2):
        tnum = int(CC[i])*2
        if tnum < 10:
            sum+= tnum
        else:    
            stnum = str(tnum)
            sum+= int(stnum[0])
            sum+= int(stnum[1])
    for i in range(len(CC)-1,-1,-2):
        sum+= int(CC[i])
    if sum % 10 == 0:
        print(typ)
    else:
        print("Algo-fail",sum)
        print("INVALID\n")
else:
    print(typ)
        
        
        
    