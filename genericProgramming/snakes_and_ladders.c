#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define ARGV_REQUIRED_LENGTH 3
#define TEN 10
#define SEED_INDX 1
#define TRACKS_INDX 2
#define ARGC_ERR "Usage: invalid parameters num (only 2)"
#define ONE 1
#define INIT_SIZE 0


/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in case there is one from this square
    int snake_to;  // snake_to represents the jump of the snake in case there is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_markov_chain(database);
    }
    return EXIT_FAILURE;
}

/**
 * build board, return EXIT_FAILURE(1) if failed, else EXIT_SUCCESS(0)
 * @param cells
 * @return
 */
static int create_board(Cell *cells[BOARD_SIZE])
{
    // allocates safely cells, initial them with ladder_to, snake_to = EMPTY
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE,NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }
    // inserts the snakes and ladders into the board
    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        }
        else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain *markov_chain)
{
    Cell* cells[BOARD_SIZE];
    if(create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        add_to_database(markov_chain, cells[i]);
    }

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node = get_node_from_database(markov_chain,cells[i])->data;
        // insert to from_node counter list the next mandatory node if
        // snake or ladder are exist
        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to,cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain, cells[index_to])
                    ->data;
            add_node_to_counter_list(from_node, to_node, markov_chain);
        }
        // if both: snake and ladder don't exist insert to
        // from_node counter list the possible steps by dice
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell*) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database(markov_chain, cells[index_to])
                        ->data;
                add_node_to_counter_list(from_node, to_node, markov_chain);
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}

static bool is_valid_parameters(int argc, char* argv[])
{
    if(argc!=ARGV_REQUIRED_LENGTH)
    {
        fprintf(stdout,ARGC_ERR);
        return false;
    }
}
static bool initial_linked_list(MarkovChain* res)
{
    LinkedList* initial = calloc(ONE,sizeof(LinkedList));
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

// cell oriented functions
// todo check allocation error????
static Cell* cell_cpy(Cell* source)
{
    Cell* dest = calloc(ONE, sizeof(Cell));
    dest->number = source->number;
    dest->snake_to=source->snake_to;
    dest->ladder_to=source->ladder_to;
    return dest;
}
static int cell_cmp(Cell* first, Cell* second)
{
    return first->number-second->number;
}
static bool cell_is_last(Cell* checked_cell)
{
    if (checked_cell->number == BOARD_SIZE)
    {
        return true;
    }
    return false;
}
static void cell_print(Cell* cell)
{
    fprintf(stdout,"[%d]", cell->number);
    if(cell->snake_to!=EMPTY && cell->ladder_to==EMPTY)
    {
        fprintf(stdout,"-snake to %d",cell->snake_to);
    }
    else if (cell->ladder_to!=EMPTY && cell->snake_to==EMPTY)
    {
        fprintf(stdout,"-ladder to %d",cell->ladder_to);
    }
    if (!cell_is_last(cell))
    {
        fprintf(stdout," -> ");
    }
    else
    {
        fprintf(stdout,"\n");
    }
}
static void cell_free(Cell* allocated_cell)
{
    free(allocated_cell);
}
// end cell functions

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
    m_c->copy_func = (DataCpy) cell_cpy;
    m_c->comp_func = (DataCmp) cell_cmp;
    m_c->is_last = (IsLast) cell_is_last;
    m_c->print_func = (PrintData) cell_print;
    m_c->free_data = (FreeData) cell_free;
    return m_c;
}

static void build_and_print_tracks(MarkovChain* m_c,int tracks_num)
{
    for (int i=1; i <= tracks_num; i++)
    {
        fprintf(stdout,"Random walk %d: ",i);
        Cell first = {1,EMPTY,EMPTY};
        Node * f_cell = get_node_from_database(m_c,
                                               (void*) &first);
        generate_random_sequence(m_c, f_cell->data,
                                 MAX_GENERATION_LENGTH);
    }
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    // gets the parameters and execute validation checks
    if (!is_valid_parameters(argc, argv))
    {
        return EXIT_FAILURE;
    }
    int seed = (int) strtol(argv[SEED_INDX], NULL,TEN);
    int tracks_num = (int) strtol(argv[TRACKS_INDX],NULL,TEN);
    srand(seed);
    // build the chain, if failed return exit_failure
    MarkovChain* m_c = initial_markov();
    if (!m_c)
    {
        return EXIT_FAILURE;
    }

    if (fill_database(m_c))
    {
        return EXIT_FAILURE;
    }

    build_and_print_tracks(m_c,tracks_num);
    free_markov_chain(&m_c);
    return EXIT_SUCCESS;
}
