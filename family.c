#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "family.h"

/* Number of word pointers allocated for a new family.
This is also the number of word pointers added to a family
using realloc, when the family is full.
*/
static int family_increment = 0;


/* Set family_increment to size, and initialize random number generator.
The random number generator is used to select a random word from a family.
This function should be called exactly once, on startup.
*/
void init_family(int size) {
  family_increment = size;
  srand(time(0));
}


/* Given a pointer to the head of a linked list of Family nodes,
print each family's signature and words.

Do not modify this function. It will be used for marking.
*/
void print_families(Family* fam_list) {
  int i;
  Family *fam = fam_list;
  while (fam) {
    printf("***Family signature: %s Num words: %d\n",
    fam->signature, fam->num_words);
    for(i = 0; i < fam->num_words; i++) {
      printf("     %s\n", fam->word_ptrs[i]);
    }
    printf("\n");
    fam = fam->next;
  }
}


/* Return a pointer to a new family whose signature is
a copy of str. Initialize word_ptrs to point to
family_increment+1 pointers, numwords to 0,
maxwords to family_increment, and next to NULL.
*/
Family *new_family(char *str) {
  Family *fam = malloc(sizeof(Family));
  fam->signature = malloc(sizeof(char) * (strlen(str)+1));
  if (fam->signature == NULL) {
    perror("malloc");
    exit(1);
  }
  strcpy(fam->signature,str);
  fam->word_ptrs = malloc(sizeof(char*) * (family_increment+1));
  if (fam->word_ptrs == NULL) {
    perror("malloc");
    exit(1);
  }
  fam->word_ptrs[0] = NULL;
  fam->num_words = 0;
  fam->max_words = family_increment;
  fam->next = NULL;
  return fam;
}


/* Add word to the next free slot fam->word_ptrs.
If fam->word_ptrs is full, first use realloc to allocate family_increment
more pointers and then add the new pointer.
*/
void add_word_to_family(Family *fam, char *word) {
  //if full, realloc
  if (fam->num_words == fam->max_words) {
    int newsize = (fam->max_words + family_increment) * sizeof(char*);
    fam->word_ptrs = realloc(fam->word_ptrs,newsize);
    if (fam->word_ptrs == NULL) {
      perror("realloc");
      exit(1);
    }
    fam->max_words = fam->max_words + family_increment;
  }

  fam->word_ptrs[fam->num_words] = word;
  fam->num_words++;
}


/* Return a pointer to the family whose signature is sig;
if there is no such family, return NULL.
fam_list is a pointer to the head of a list of Family nodes.
*/
Family *find_family(Family *fam_list, char *sig) {
  while (fam_list) {
    if (strcmp(fam_list->signature,sig) == 0){
      return fam_list;
    }
    fam_list = fam_list->next;
  }
  return NULL;
}


/* Return a pointer to the family in the list with the most words;
if the list is empty, return NULL. If multiple families have the most words,
return a pointer to any of them.
fam_list is a pointer to the head of a list of Family nodes.
*/
Family *find_biggest_family(Family *fam_list) {
  Family *result = fam_list;
  Family *curfam = fam_list;
  while (curfam) {
    if (curfam->num_words>result->num_words){
      result = curfam;
    }
    curfam = curfam->next;
  }
  return result;

}


/* Deallocate all memory rooted in the List pointed to by fam_list. */
void deallocate_families(Family *fam_list) {
  Family *temp;
  while (fam_list != NULL) {
    temp = fam_list;
    fam_list = fam_list->next;
    free(temp->signature);
    free(temp->word_ptrs);
    free(temp);
  }
}


/* Generate and return a linked list of all families using words pointed to
by word_list, using letter to partition the words.

Implementation tips: To decide the family in which each word belongs, you
will need to generate the signature of each word. Create only the families
that have at least one word from the current word_list.
*/
Family *generate_families(char **word_list, char letter) {
  char **word = word_list;
  Family *famlist = NULL;
  Family *curfam;
  int i = 0;

  while (word[i]) {
    // generate signature of the word regarding the letter.
    int len = strlen(word[i]);
    char sig[len+1];
    strcpy(sig,word[i]);
    for (size_t j = 0; j < len; j++) {
      if (sig[j] != letter){
        sig[j] = '-';
      }
    }

    Family * fam = find_family(famlist,sig);
    //if the sig exist in fam, add the word to the fam.
    if (fam != NULL){
      add_word_to_family(fam, word[i]);
    }
    //if not, create a new fam and add it to the end of fam list.
    else{
      Family *newfam = new_family(sig);
      //first in list;
      if (famlist == NULL){
        famlist = newfam;
        curfam = famlist;
      }
      else{
        curfam->next = newfam;
        curfam = newfam;
      }
      add_word_to_family(newfam, word[i]);
    }
    i++;
  }
  return famlist;
}


/* Return the signature of the family pointed to by fam. */
char *get_family_signature(Family *fam) {
  return fam->signature;
}


/* Return a pointer to word pointers, each of which
points to a word in fam. These pointers should not be the same
as those used by fam->word_ptrs (i.e. they should be independently malloc'd),
because fam->word_ptrs can move during a realloc.
As with fam->word_ptrs, the final pointer should be NULL.
*/
char **get_new_word_list(Family *fam) {
  char **new_list = malloc(sizeof(char*)*(fam->num_words+1));
  if (new_list == NULL) {
    perror("malloc");
    exit(1);
  }
  int i=0;
  while (i < fam->num_words) {
    new_list[i] = fam->word_ptrs[i];
    i++;
  }
  new_list[fam->num_words] = NULL;
  return new_list;
}


/* Return a pointer to a random word from fam.
Use rand (man 3 rand) to generate random integers.
*/
char *get_random_word_from_family(Family *fam) {
  int i = rand()%fam->num_words;
  char *result = fam->word_ptrs[i];
  return result;
}
