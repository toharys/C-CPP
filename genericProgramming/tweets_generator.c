#include "markov_chain.h"
#include <string.h>
#define MROW_LENGTH 1005
#define SPACEENTER " \n"
#define DOT '.'
#define ONE 1
#define ALL (-5)
#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate"\
            "new memory\n"
#define OPT_NUM1 4
#define OPT_NUM2 5
#define ARGC_ERR "Usage: invalid parameters num (only 3 or 4)"
#define PATH_ERR "Error: invalid file path"
#define SEED_INDX 1
#define TWEETSN_INDX 2
#define PATH_INDX 3
#define WORDSN_INDX 4
#define MAX_TWEETS 20
#define TEN 10
#define INIT_SIZE 0


/**
 * receives a row and splits it to word, adds each row to the database and
 * updates the the counter list
 * @param row
 * @param m_c
 * @return true in success, else false
 */
static bool single_row(char* row, MarkovChain* m_c,int words_to_read)
{
    Node* prev = NULL;
    Node* word_pntr_in_chain;
    char* word;

    word = strtok(row,SPACEENTER);
    while(word) // while row not empty
        {
        // in case we already read enough word return true
        if ( m_c->database->size == words_to_read)
        {
            return true;
        }

        // trying to add the word to d.b.
            word_pntr_in_chain = add_to_database(m_c, word);
            if (!word_pntr_in_chain)
            {
                return false;
            }
        // if it has previous word (it isn't the first word in the row)
        if (prev)
        {
            char* prev_word = prev->data->data;
            if(prev_word[strlen(prev_word)-1]==DOT)
            { // if prev its end word
                prev = word_pntr_in_chain; // update prev word to curr
                word = strtok(NULL,SPACEENTER); // update word to new one
                continue;
            }
            //if it isn't first word try to update the counter list
            if(!add_node_to_counter_list(prev->data,
                     word_pntr_in_chain->data,m_c))
            {
                return false;
            }
        }
        prev = word_pntr_in_chain; // update prev word to curr
        word = strtok(NULL,SPACEENTER); // update word to new one
    }
    return true;
}

static int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
    char buffer[MROW_LENGTH] = {0};
    while (fgets(buffer,MROW_LENGTH,fp))
    {
        // if failed in reading the row make free and exit the function
        if(!single_row(buffer, markov_chain,words_to_read))
        {
            free_markov_chain(&markov_chain);
            return 0;
        }
        // if we achieve the required num of words to read
        if (markov_chain->database->size == words_to_read)
        {
            break;
        }
        for(int i=0;i<MROW_LENGTH;i++)
        {
            buffer[i]=0;
        }
    }
    return 1;
}

static bool is_valid_parameters(int argc, char* argv[],FILE** f_name,int *words)
{
    if (argc!=OPT_NUM1 && argc!=OPT_NUM2)
    {
        fprintf(stdout,"%s",ARGC_ERR);
        return false;
    }
    *f_name=fopen(argv[PATH_INDX],"r");
    if(!*f_name)
    {
        fprintf(stdout,"%s",PATH_ERR);
        return false;
    }
    if (argc==OPT_NUM2)
    {
        sscanf(argv[WORDSN_INDX],"%d",words);
    }
    return true;
}

static bool initial_linked_list(MarkovChain* res)
{
    LinkedList* initial = calloc(ONE,sizeof(LinkedList));
    // calloc 2 - LinkedList
    if (!initial)
    {
        fprintf(stdout,ALLOCATION_ERROR_MASSAGE);
        return false;
    }
    initial->first = NULL;
    initial->last = NULL;
    initial->size =INIT_SIZE;
    res->database = initial;
    return true;
}

// strings oriented functions
/**
 * a function that receives pointer to MarkovNode and checks if it last word
 * @param a
 * @return 1 if last word else 0
 */
static bool str_is_last(char* word)
{
    if (word[strlen(word)-1]==DOT)
    {
        return true;
    }
    return false;
}
/**
 * copy string into allocated memory and returns pointer to the copied one
 * @param source
 * @return
 */
// todo check allocation error????
static char* str_cpy(char* source)
{
    char* dest = calloc(ONE, strlen(source));
    return strcpy(dest,source);
}
/**
 * prints string without space or /n
 * @param word
 */
static void str_print(char* word)
{
    if (!str_is_last(word))
    {
        fprintf(stdout, "%s ", word);
    }
    else
    {
        fprintf(stdout,"%s\n", word);
    }
}
static void str_free(char* allocated_arr)
{
    free(allocated_arr);
}
// end of strings functions

static void* initial_markov()
{
    // allocates MarkovChain*
    MarkovChain* m_c = calloc(ONE,sizeof(MarkovChain));
    if (!m_c)
    {
        fprintf(stdout,"%s",ALLOCATION_ERROR_MASSAGE);
        return NULL;
    }
    // try to insert linked list to m_c
    if(!initial_linked_list(m_c))
    {
        return NULL;
    }
    m_c->copy_func = (DataCpy)str_cpy;
    m_c->comp_func = (DataCmp) strcmp;
    m_c->is_last = (IsLast) str_is_last;
    m_c->print_func = (PrintData) str_print;
    m_c->free_data = (FreeData) str_free;
    return m_c;
}

/**
 * receives database and num tweets to write, than build them and print
 * @param m_c
 * @param tweets_to_write
 */
static void build_and_print_tweets(MarkovChain* m_c,int tweets_to_write)
{
    for (int i=1;i<=tweets_to_write;i++)
    {
    fprintf(stdout,"Tweet %d: ",i);
    generate_random_sequence(m_c,NULL,MAX_TWEETS);
    }
}

int main(int argc,char* argv[])
{
    //receives the arguments and execute validation checks,
    // if failed return exit_failure
    FILE *f_name = NULL;
    int words_num=ALL;
    if (!is_valid_parameters(argc,argv,&f_name,&words_num))
    {
        return EXIT_FAILURE;
    }
    int seed=(int)strtol(argv[SEED_INDX],NULL,TEN);
    srand(seed); // insert the seed to the srand func
    int tweets_to_write=(int)strtol(argv[TWEETSN_INDX],NULL,TEN);
    // build the chain, if failed return exit_failure
    MarkovChain* m_c = initial_markov();
    if (!m_c)
    {
        return EXIT_FAILURE;
    }
    // try to fill the database, if failed return exit_failure and
    // free all allocated memory
    fill_database(f_name,words_num,m_c);
    if (m_c->database == NULL)
    {
        return EXIT_FAILURE;
    }
    // build tweets by the database and prints them
    build_and_print_tweets(m_c,tweets_to_write);
    // free all allocated memory and return exit_success
    fclose(f_name);
    free_markov_chain(&m_c);
    return EXIT_SUCCESS;
}