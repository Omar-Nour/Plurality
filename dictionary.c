// Implements a dictionary's functionality

#include <stdbool.h>
#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <strings.h>

int word_counter = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1600;

// Hash table
node *table[N];

// initialize to nulls
void init(void)
{
    for(int i = 0; i < N; ++i)
    {
        table[i] = NULL;
    }
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int address = hash(word);
    node *n = table[address];
    bool found = false;

    if (strcasecmp(n->word,word) == 0)
    {
        found = true;
    }
    else
    {
        n = n->next;
        while (!found && (n != NULL))
        {
            if (strcasecmp(n->word,word) == 0)
            {
                found = true;
            }
            else
            {
                n = n->next;
            }
        }
    }
    return found;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int address = 0;
    
    for(int i = 0; word[i] != '\0'; ++i)
    {
        address += word[i];
    }
    
    if (address > 1599)
    {
        address = address % 1599;
    }

    return address;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *d = fopen(dictionary, "r");

    if (d == NULL)
    {
        return false;
    }
    
    init();
    
    int scanner, address;
    bool brk = false;

    char *wrd= malloc((LENGTH + 1) * sizeof(char));

    do
    {
        scanner = fscanf(d, "%s", wrd);
        if (scanner != EOF)
        {
            word_counter++;

            node *n = malloc(sizeof(node));

            if (n == NULL)
            {
                free(wrd);
                return false;
            }

            strcpy(n->word, wrd);
            n->next = NULL;

            address = hash(wrd);

            //if no node at this address add one..
            if (table[address] == NULL)
            {
                table[address] = n;
            }
            else
            {
                // new node to point at first node then index points at new
                n->next = table[address];
                table[address] = n;
            }
        }
        else
        {
            brk = true;
        }
    }
    while (!brk);

    free(wrd);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *n = NULL;
    node *tmp = NULL;

    for (int i = 0; i < N; ++i)
    {
        n = table[i];
        while (n != NULL)
        {
           tmp = n->next;
           free(n);
           n = tmp;
        }

    }
    return true;
}
