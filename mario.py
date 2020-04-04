H = 0
while (H > 8) or (H < 1):
    try:
        H = int(input("Height : "))
    except:
        pass
for i in range(H):
    spaces  = H - (i + 1)
    strs = i + 1
    for sp in range(spaces):
        print(" ",end="")
    for st in range(strs):
        print("*",end="")
    print()