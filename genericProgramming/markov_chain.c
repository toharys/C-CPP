#include "markov_chain.h"
#include <string.h>
#define ONE 1
#define INIT_SIZE 0

/**
* Get random number between 0 and max_number [0, max_number).
* @param max_number maximal number to return (not including)
* @return Random number
*/
int get_random_number(int max_number)
{
    return rand() % max_number;
}

//  FUNC 1
MarkovNode* get_first_random_node (MarkovChain* markov_chain)
{
    Node *curr = markov_chain->database->first;
        // random num in the span between 0 and the size of the linked list
        int max_rand = markov_chain->database->size;
        int steps_num = get_random_number(max_rand);
        for (int j=0; j < steps_num; j++)
        {   // runs on the linked list until the random word
            curr = curr->next;
        }
        if (markov_chain->is_last(curr->data->data))
        { // if its last word execute recursive call to the func
            return get_first_random_node(markov_chain);
        }
    return curr->data;
}
int max_random_number(MarkovNode *ptr)
{
    int counter = 0 ;
    for(int i = 0 ; i < ptr->children_num; i++)
    {
        counter += ptr->counter_list[i].frequency ;
    }
    return counter ;
}

//  FUNC 2
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
{
    int i = get_random_number(max_random_number(state_struct_ptr));
    NextNodeCounter* curr_child = state_struct_ptr->counter_list;
    while (i>=0)
    {
        i-=curr_child->frequency;
        if (i>=0)
        {
            curr_child++;
        }
    }
    return curr_child->markov_node;
}

//  FUNC 3
void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
    MarkovNode* res = first_node;
    // if we dont receive first_node
    if (!res)
    {
        res = get_first_random_node(markov_chain);
    }
    int counter = max_length;
    while (counter)
    {
        (markov_chain->print_func)(res->data);
        counter--;
        if ((markov_chain->is_last)(res->data))
        {
            break;
        }
        else if(counter<=0)
        {
            fprintf(stdout,"\n");
        }
        res = get_next_random_node(res);
    }
}

//  FUNC 4 and auxs funcs
/**
 * checks s_node already in f_node counter_list. if it is update its frequency
 * and return true, else return false
 * @param f_node
 * @param s_node
 * @return
 */
bool is_son(MarkovNode* f_node, MarkovNode* s_node, MarkovChain* m_c)
{
    for(int i=0;i<f_node->children_num;i++)
    {
        Data data_to_compare = f_node->counter_list[i].markov_node->data;
        if (m_c->comp_func(data_to_compare, s_node->data) == 0)
        { //already in prev counter_list and if it is, update it's counter
            (f_node->counter_list[i]).frequency++;
            return true;
        }
    }
    return false;
}
/**
 * try to add s_node to f_node counter list, if the list is null initialize it
 * if succeed add it return true, else false
 * @param f_node
 * @param s_node
 * @return
 */
bool add_new_son(MarkovNode* f_node, MarkovNode* s_node)
{
    // if it is the first son of f_node
    if (f_node->counter_list == NULL)
    {
        NextNodeCounter* f_son = calloc(ONE,sizeof(NextNodeCounter));
        if(!f_son)
        {
            fprintf(stdout,ALLOCATION_ERROR_MASSAGE);
            return false;
        }
        f_son->markov_node = s_node;
        f_son->frequency = 1;
        f_node->counter_list = f_son;
        f_node->children_num = 1;
        return true;
    }
    // if f_node already has sons
    size_t new_length = sizeof(NextNodeCounter)*(f_node->children_num+1);
    NextNodeCounter* new_list = realloc(f_node->counter_list,new_length);
    if(!new_list)
    {
        fprintf(stdout,ALLOCATION_ERROR_MASSAGE);
        return false;
    }
    new_list[f_node->children_num].markov_node = s_node;
    new_list[f_node->children_num].frequency =1;
    f_node->counter_list = new_list;
    f_node->children_num += 1;
    return true;
}
bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode
*second_node,MarkovChain *markov_chain)
{
    // if second node already in first node counter list return true
    if (is_son(first_node,second_node, markov_chain))
    {
        return true;
    }
    // if new word not in prev counter_list adds it
    if(!add_new_son(first_node,second_node))
    {
        return false;
    }
    return true;
}

//  FUNC 5
Node* get_node_from_database(MarkovChain *markov_chain, void *data_ptr)
{
    if (markov_chain->database == NULL || markov_chain->database->size == 0) {
        return NULL; // if the chain is empty returns NULL
    }
    Node *curr = markov_chain->database->first;
    while (curr != markov_chain->database->last)
    {
        Data curr_data = curr->data->data;
        if (markov_chain->comp_func(curr_data, data_ptr) == 0)
        {
            return curr;
        }
        curr = curr->next;
    }
    if (markov_chain->comp_func(markov_chain->database->last->data->data
                                , data_ptr) == 0)
    {
        return markov_chain->database->last;
    }
    return NULL;
}

//  FUNC 6 and aux func
/**
 * receives word, build markov node to wrapped it and return it.
 * in case of allocation failure return NULL
 * @param src_data
 * @return
 */
MarkovNode* build_markov_node(Data src_data, MarkovChain* m_c)
{
    MarkovNode* mark = calloc(ONE,sizeof(MarkovNode));
    if(!mark)
    {
        fprintf(stdout,ALLOCATION_ERROR_MASSAGE);
        return NULL;
    }
    Data data_to_add = m_c->copy_func(src_data);
    if (!data_to_add)
    {
        fprintf(stdout,ALLOCATION_ERROR_MASSAGE);
        return NULL;
    }
    mark->data = data_to_add;
    mark->counter_list = NULL;
    mark->children_num = 0;
    return mark;
}
Node* add_to_database(MarkovChain *markov_chain, void *data_ptr)
{
    Node* res = get_node_from_database(markov_chain, data_ptr);
    // if data_ptr already in database returns its Node
    if(res)
    {
        return res;
    }
    //builds MarkovNode to add
    MarkovNode* new_word = build_markov_node(data_ptr, markov_chain);
    if(!new_word)
    {
        return NULL;
    }

    //adds it to the chain and return his Node, if failed return NULL
    if(add(markov_chain->database, new_word))
    {
        free(new_word->data);
        free(new_word);
        new_word=NULL;
        return NULL;
    }
    return markov_chain->database->last;
}

// FUNC 7
void free_markov_chain(MarkovChain ** ptr_chain)
{
    Node *next_node = (*ptr_chain)->database->first  ;
    Node *prev_node = NULL ;

    while(next_node!= NULL){

        prev_node = next_node ;
        next_node = prev_node->next ;

        (*ptr_chain)->free_data(prev_node->data->data); // free the data
        free(prev_node->data->counter_list); // free the counter list

        free(prev_node->data) ; // free the Markov node
        free(prev_node); // free the node itself
    }

    free((*ptr_chain)->database);
    free(*ptr_chain);
}



