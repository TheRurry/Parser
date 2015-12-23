#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int Fsize=50;
int no_edges;
int no_nodes;

//returns a substring, of the entered string
char *substr(char *g, int start, int end) {
  char* temp = malloc(end-start+2); //+1 for null char
  strncpy(temp, g+start, end-start+1);
  temp[end-start+1] = 0;
  return temp;
}

//check if a character is a variable
int varCheck(char g) {
  if (g == 'x' || g == 'y' || g == 'z')
    return 1;
  return 0;
}

//check if fmla is a valid atomic formula
int atom(char *g) {
  if (strlen(g) == 5 && *(g+1) == '[' && *(g+4) == ']' && varCheck(*(g+2)) && varCheck(*(g+3)))
    return 1;
  return 0;
}

//check if a character is a binary connector
int binCheck(char g) {
  if (g == '^' || g == 'v' || g == '>')
    return 1;
  return 0;
}

//returns position of binary connective
int findBin(char *g) {
  int count = 0, pos = 0, i;
  for (i = 0; i < strlen(g); i++) {
    if (*(g + i) == '(')
      count++;
    if (*(g + i) == ')')
      count--;
    if (count == 1 && binCheck(*(g + i)))
      pos = i;
  }
  if (count)
    return 0;
  return pos;
}

int valid(char *g) {
  int binPos;
  switch(*g) {
    case 'X':
      return atom(g);
    case '-':
      return valid(substr(g, 1, strlen(g) - 1));
    case '(':
      binPos = findBin(g);
      if (binPos == 0)
        return 0;
      return valid(substr(g, 1, binPos-1)) && valid(substr(g, binPos+1, strlen(g)-2));
    case 'E':
      if (varCheck(*(g+1)))
        return valid(substr(g, 2, strlen(g) - 1));
      return 0;
    case 'A':
      if (varCheck(*(g+1)))
        return valid(substr(g, 2, strlen(g) - 1));
      return 0;
    default: return 0;
  }
}

//returns 0 for non-formulas, 1 for atoms, 2 for negations, 3 for binary connective fmlas, 4 for existential and 5 for universal formulas.
int parse(char *g) {
  switch(*g) {
    case 'X': return atom(g);
    case '-': return valid(g) ? 2 : 0;
    case '(': return valid(g) ? 3 : 0;
    case 'E': return valid(g) ? 4 : 0;
    case 'A': return valid(g) ? 5 : 0;
    default: return 0;
  }
}

//this method takes a formula, the list of edges of a graph, the number of vertices and a variable assignment. It then evaluates the formula and returns 1 or 0 as appropriate.
int eval(char *nm, int edges[no_edges][2], int size, int V[3])

{
	switch(parse(nm)) {
    case 1:

	}
}


int main()
{
  //Input a string and check if its a formula
  char *name=malloc(Fsize);
  printf("Enter a formula: ");
  scanf("%s", name);
  int p=parse(name);
  switch(p)
    {case 0: printf("Not a formula"); break;
    case 1: printf("An atomic formula"); break;
    case 2: printf("A negated formula"); break;
    case 3: printf("A binary connective formula"); break;
    case 4: printf("An existential formula"); break;
    case 5: printf("A universal formula"); break;
    default: printf("Not a formula"); return 0;
    }

  //Input a graph.  No. of nodes, edges, then input each edge.
  printf("How many nodes in the graph?\n");
  scanf(" %d", &no_nodes);
  printf("The nodes are 0, 1, ..., %d\n", no_nodes-1);
  printf("Now the edges\n");
  printf("How many edges?\n");
  scanf(" %d", &no_edges);

  int edges[no_edges][2];  //Store edges in 2D array
  int  i, k, j;
  for(i=0;i<no_edges;i++) {
    printf("input a pair of nodes (<%d)", no_nodes);
    scanf(" %d", &j);scanf(" %d", &k);
    edges[i][0]=j; edges[i][1]=k;
  }

  /*Assign variables x, y, z to nodes */
  int V[3];
  /*Store variable values in array
  value of x in V[0]
  value of y in V[1]
  value of z in V[2] */
  printf("Assign variables x, y, z\n");
  printf("x is ?(<%d)", no_nodes);scanf(" %d", &V[0]);
  printf("y is ?");scanf(" %d", &V[1]);
  printf("z is ?");scanf(" %d", &V[2]);

  /*Now check if formula is true in the graph with given variable assignment. */
  if (eval(name, edges, no_nodes,  V)==1)
    printf("The formula %s is true", name); else printf("The formula %s is false", name);

  free(name);
  return 0;
}
