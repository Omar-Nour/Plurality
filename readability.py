from cs50 import get_string

text = get_string("Text: ")
sentences = 0
letters = 0

for char in text:
    # Count sentences
    if char in [".", "?", "!"]:
        sentences += 1

    # Count letters
    elif char.isalpha():
        letters += 1

text = text.split()

words = 0
for item in text:
    if (item.lower().strip(".?!,;\"").replace("'", "").replace("-", "")).isalpha():
        words += 1

L = (letters * 100) / words
S = (sentences * 100) / words

index = int(round(0.0588 * L - 0.296 * S - 15.8))

#print(f"Words : {words}\nLetters : {letters}\nSentences : {sentences}")

if index >= 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print(f"Grade {index}")
