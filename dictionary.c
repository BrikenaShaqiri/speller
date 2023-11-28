#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

const unsigned int N = 26;
node *tab[N] = {NULL};
unsigned int word_count = 0;

bool check(const char *word)
{
    unsigned int hash_value = hash(word);
    node *cursor = tab[hash_value];

    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

unsigned int hash(const char *word)
{
    unsigned long total = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        total += tolower(word[i]);
    }
    return total % N;
}

bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Unable to open %s\n", dictionary);
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, word);
        unsigned int hash_value = hash(word);
        n->next = tab[hash_value];
        tab[hash_value] = n;
        word_count++;
    }

    fclose(file);
    return true;
}

unsigned int size(void)
{
    return word_count;
}

bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = tab[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
        tab[i] = NULL; // Avoid potential issues
    }
    return true;
}
