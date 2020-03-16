#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate quals[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        quals[i].name = argv[i + 1];
        quals[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    bool temp;
    for(int i = 0 ; i < candidate_count ; i++)
    {
        if (name == quals[i].name) // Never executes although they may be the same
        {
            quals[i].votes++;
            temp = true;
        }
        else
        {
            temp = false;
        }
    }
    return temp;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int max = 0;
    for(int i = 0 ; i < candidate_count ; i++)
    {
        if (quals[i].votes > max)
        {
            max = quals[i].votes;
        }
    }
    for(int i = 0 ; i < candidate_count ; i++)
    {
        if (quals[i].votes == max)
        {
            printf("%s",quals[i].name);
        }
        printf("%i\n",quals[i].votes);
    }
}

