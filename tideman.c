#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
    int diff;
    int high;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// m1 
void init_prefs(void);
// m2
int facto(int num);
// m3 (in add pairs)
bool unique(pair p);

// DEBUG
//void debug_prefs(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    init_prefs();

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    //debug_prefs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

//initilaize preferences array to 0s'
void init_prefs(void)
{
    for (int i = 0; i < candidate_count; ++i)
    {
        for (int j = 0; j < candidate_count; ++j)
        {
            preferences[i][j] = 0;
        }
    }
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; ++i)
    {
        if (!strcmp(name, candidates[i]))
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // loop through ranks array
    for (int i = 0; i < candidate_count - 1; ++i)
    {
        // fetch row to adjust in prefernces array
        int p_row = ranks[i];

        // iterate through the candidates that are less prefered over the current row candidate &++
        for (int j = i + 1; j < candidate_count; ++j)
        {
            preferences[p_row][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int cand1, cand2, vote1, vote2;
    int x = 0;
    pair current_pair;
    
    for (int i = 0; i < candidate_count; ++i)
    {
        for (int j = 0; j < candidate_count; ++j)
        {
            cand1 = i;
            cand2 = j;
            vote1 = preferences[cand1][cand2];
            vote2 = preferences[cand2][cand1];
    
            current_pair.diff = abs(vote1 - vote2);
            
            if (vote1 == vote2)
            {
                continue;
            }
            else if (vote1 > vote2)
            {
                current_pair.winner = cand1;
                current_pair.loser = cand2;
                current_pair.high = vote1;
            }
            else
            {
                current_pair.winner = cand2;
                current_pair.loser = cand1;
                current_pair.high = vote2;
            }
            
            if (unique(current_pair))
            {
                pair_count++;
                pairs[x] = current_pair;
                x++;
            }
            
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair temp;
    for (int i = 0; i < pair_count - 1; ++i)
    {
        for (int j = i + 1; j < pair_count; ++j)
        {
            if (pairs[j].diff > pairs[i].diff && pairs[j].high > pairs[i].high)
            {
                temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int src, low;
    bool src_b, low_b;
    int SOURCE = pairs[0].winner;
    
    for (int i = 0; i < pair_count; ++i)
    {
        src = pairs[i].winner;
        low = pairs[i].loser;
        
        if (low != SOURCE)
        {
            locked[src][low] = true;
        }
        
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int tally;
    for (int col = 0; col < candidate_count; ++col)
    {
        tally = 0;
        for (int row = 0; row < candidate_count; ++row)
        {
            if (!locked[row][col])
            {
                tally++;
            }
        }
        
        if (tally == candidate_count)
        {
            printf("%s\n", candidates[col]);
            return;
        }
    }
    return;
}

// debug
// void debug_prefs(void)
// {
//     for (int i = 0; i < pair_count; ++i)
//     {
//         printf("Pair %i:\n winner : %i\n loser : %i\n\n", i+1, pairs[i].winner, pairs[i].loser);
//     }
// }

//factorial
int facto(int num)
{
    if (num == 0)
    {
        return 1;
    }
    else
    {
        return num * facto(num - 1);
    }
}

bool unique(pair p)
{
    for (int i = 0; i < pair_count; ++i)
    {
        if (p.winner == pairs[i].winner && p.loser == pairs[i].loser)
        {
            return false;
        }
    }
    return true;
}