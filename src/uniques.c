#include "dada.h"

/* 
 methods for "Uniques" objects.
 The "Uniques" object contains all the unique sequences
 and the number of reads of each in a DNA data set. The
 DADA cluster object "B" constructor takes a "Uniques"
 object as its input.
 */

typedef struct {
  char *seq; /* A unique sequence, stored in 1-based index form. */
  int length; /* The length of the sequence. */
  int reads; /* The number of reads. */
} Unique;

struct _Uniques { /* Uniques object. */
  Unique *unique; /* Pointer to array of Unique objects. */
  int nseqs; /* Total number of Unique objects */
};


/* 
 uniques_from_file:
 Create Uniques object from .uniques file. This is a file
 in which each line has an integer followed by a tab followed
 by a string. The integer is a read number and the string is
 a sequence. There is no restriction here on which characters
 are allowed. Input char *f points to a string with the filename
*/
Uniques *uniques_from_file(char *f) {
  FILE *fp;
  Uniques *uniques = (Uniques *) malloc( sizeof(Uniques) );
  int i;
  char ch = '\0';
  
  /* Open file for read only. */
  if ((fp = fopen(f, "r")) == NULL) { 
    printf("Error reading file\n");
    exit(1);
  }

  uniques->nseqs = 0;

  /* 
   *  Get the number of lines in the file. 
   *
   *  Works whether EOF is after a final "\n" or at the end of a line
   *
   *  Ignores empty lines, i.e. consecutive "\n"s) 
   */  

  while ( ch != EOF ) {  
    ch = fgetc(fp);
    if ( ch == EOF )
      uniques->nseqs++; 
    else if (ch == '\n') {      
      uniques->nseqs++;
      ch = fgetc(fp);
    }
  }
  rewind(fp);
  
  uniques->unique = (Unique*) malloc(uniques->nseqs * sizeof(Unique));
  
  /* Store each line of .uniques file in a Unique object. */
  for (i = 0; i < uniques->nseqs; i++) {
    uniques->unique[i].seq = (char *) malloc( BUFFER_SIZE ); /* Allocate a large buffer. */
    fscanf(fp, "%d\t%s", &uniques->unique[i].reads, uniques->unique[i].seq); /* Read in a line. */
    /* Remove trailing whitespace from string buffer. */
    uniques->unique[i].length = strlen(uniques->unique[i].seq);
    uniques->unique[i].seq = (char *) realloc(uniques->unique[i].seq,uniques->unique[i].length+1);
    nt2int(uniques->unique[i].seq, uniques->unique[i].seq);
  }
  
  return uniques;
}

/* TO BE WRITTEN
   uniques *uniques_from_fasta(char *c) {
   FILE *fp;
   uniques *in = (uniques*) malloc(sizeof(uniques));
   unique *p;
   }
*/


/*
 uniques_nseqs:
 returns the number of unique sequences in a  Uniques object
*/
int uniques_nseqs(Uniques *uniques) {
  return uniques->nseqs;
}

/*
 uniques_reads:
 returns the number of reads of sequence n in a Unqiues object
 */
int uniques_reads(Uniques *uniques, int n) {
  return uniques->unique[n].reads;
}

/*
 uniques_length:
 returns the length of sequence n in a Unqiues object
 */
int uniques_length(Uniques *uniques, int n) {
  return uniques->unique[n].length;
}

/*
 uniques_sequence:
 make a copy of sequence n in a uniques object and return a pointer to it
 */
void uniques_sequence(Uniques *uniques, int n, char *seq) {
  if (sizeof uniques->unique[n].seq > sizeof seq)  // DOES THIS WORK?
    printf("input buffer shorter than the sequence\n");
  strcpy(seq,uniques->unique[n].seq);
  return;
}

/*
 uniques_free:
 free all memory associated with a Uniques object,
 including storage for each individual sequence
*/
void uniques_free(Uniques *uniques) {
  int i;    
  for (i = 0; i < uniques->nseqs; i++) {
    free(uniques->unique[i].seq);
  }
  free(uniques->unique);
  free(uniques);
}
