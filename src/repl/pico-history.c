#define HISTORY_SIZE 10
#define INPUT_SIZE 81
char hist[HISTORY_SIZE][INPUT_SIZE];
char stashed[INPUT_SIZE] = {0x00};

short hist_last = -1, hist_top = -1;

// Check if the queue is full
bool isHistFull()
{
    if ((hist_last == hist_top + 1) || (hist_last == 0 && hist_top == HISTORY_SIZE - 1))
        return true;
    return false;
}

// Check if the queue is empty
bool isHistEmpty()
{
    if (hist_last == -1)
        return true;
    return false;
}

short sizeOfHist()
{
    if (hist_top >= hist_last)
        return (hist_top + HISTORY_SIZE) - (hist_last + HISTORY_SIZE) + 1;
    return (hist_top + HISTORY_SIZE) - hist_last + 1;
}

void stash_history(char *s)
{
    strcpy(stashed, s);
}

// Adding an element
void push_history(char *s)
{
    if (strlen(s) < 1) // don't push an empty string
    {
        return;
    }
    if (hist_top >= 0 && strcmp(hist[hist_top], s) == 0) // don't push a duplicate
    {
        return;
    }
    if (isHistFull())
    {
        hist_last = (hist_last + 1) % HISTORY_SIZE;
    }
    if (hist_last == -1) // empty History
        hist_last = 0;
    hist_top = (hist_top + 1) % HISTORY_SIZE;
    strcpy(hist[hist_top], s);
    stashed[0] = 0;
};

char *peek_history(short offset)
{
    if (offset <= 0)
    {
        return stashed;
    }
    if (offset >= sizeOfHist() + 1)
    {
        return "";
    }
    short i = (hist_top - offset + 1 + HISTORY_SIZE) % HISTORY_SIZE;
    return hist[i];
};
