#include <stdio.h>
#include <string.h>
#include <stdlib.h>


const int Fsize = 50;
int no_edges;
int no_nodes;
int i;
const int cases = 6;

//returns a substring, of the entered string
char *substr(char *g, int start, int end) {
  char* temp = malloc(end - start + 2); //+1 for null char
  strncpy(temp, g + start, end - start + 1);
  temp[end - start + 1] = 0;
  return temp;
}

//check if a character is a variable
int varCheck(char g) {
  return g == 'x' || g == 'y' || g == 'z';
}

//check if a character is a binary connector
int binCheck(char g) {
  return g == '^' || g == 'v' || g == '>';
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
    case 'X': return strlen(g) == 5 && *(g+1) == '[' && *(g+4) == ']' && varCheck(*(g+2)) && varCheck(*(g+3));
    case '-': return valid(substr(g, 1, strlen(g) - 1));
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
    case 'X': return valid(g);
    case '-': return valid(g) ? 2 : 0;
    case '(': return valid(g) ? 3 : 0;
    case 'E': return valid(g) ? 4 : 0;
    case 'A': return valid(g) ? 5 : 0;
    default: return 0;
  }
}

int eval();

int checkAtom(char *nm, int edges[no_edges][2], int size, int V[3]) {
  int i, var1 = 0, var2 = 0;
  for (i = 0; i < 3; i++) {
    if (*(nm + 2) == 'x' + i)
      var1 = V[i];
    if (*(nm + 3) == 'x' + i)
      var2 = V[i];
  }
  for (i = 0; i < no_edges; i++) {
    if (var1 == edges[i][0] && var2 == edges[i][1])
      return 1;
  }
  return 0;
}

int checkBin(char *nm, int edges[no_edges][2], int size, int V[3]) {
  int binPos = findBin(nm);
  int lFmla = eval(substr(nm, 1, binPos - 1), edges, size, V);
  int rFmla = eval(substr(nm, binPos + 1, strlen(nm) - 2), edges, size, V);
  if (*(nm + binPos) == '^' && (lFmla && rFmla))
    return 1;
  if (*(nm + binPos) == 'v' && (lFmla || rFmla))
    return 1;
  if (*(nm + binPos) == '>' && !(lFmla && !rFmla))
    return 1;
  return 0;
}

int eval(char *nm, int edges[no_edges][2], int size, int V[3]) {
  int i, W[3], evaluation, type = parse(nm);
  if (type > 3)
    memcpy(W, V, sizeof(W));

  switch(type) {
    case 1: return checkAtom(nm, edges, size, V);
    case 2: return !eval(substr(nm, 1, strlen(nm) - 1), edges, size, V);
    case 3: return checkBin(nm, edges, size, V);
    case 4:
      evaluation = 0;
      for (i = 0; i < size; i++) {
        W[*(nm + 1) - 'x'] = i;
        evaluation = evaluation || eval(substr(nm, 2, strlen(nm) - 1), edges, size, W);
      }
      return evaluation;
    case 5:
      evaluation = 1;
      for (i = 0; i < size; i++) {
        W[*(nm + 1) - 'x'] = i;
        evaluation = evaluation && eval(substr(nm, 2, strlen(nm) - 1), edges, size, W);
      }
      return evaluation;
    default: return 0;
	}
}

int main() {
  char *name = malloc(Fsize);
  FILE *fp, *fpout;

  if ((fp=fopen("input.txt","r")) == NULL) {printf("Error opening file"); exit(1);}
  if ((fpout=fopen("output.txt","w")) == NULL) {printf("Error opening file"); exit(1);}

  int j;
  for(j = 0; j < cases; j++) {
    fscanf(fp, "%s %d %d",name,&no_nodes,&no_edges);
    int edges[no_edges][2];
    for(i = 0; i < no_edges; i++)
      fscanf(fp, "%d%d", &edges[i][0], &edges[i][1]);
    int W[3];
    for(i = 0; i < 3; i++)
      fscanf(fp, "%d", &W[i]);
    int p = parse(name);
    switch(p) {
      case 0:fprintf(fpout,"%s is not a formula.  ", name); break;
	    case 1: fprintf(fpout,"%s is an atomic formula.  ", name); break;
	    case 2: fprintf(fpout,"%s is a negated formula.  ", name); break;
	    case 3: fprintf(fpout,"%s is a binary connective formula.  ", name); break;
	    case 4: fprintf(fpout,"%s is an existential formula.  ", name); break;
	    case 5: fprintf(fpout,"%s is a universal formula.  ", name); break;
	    default: fprintf(fpout,"%s is not a formula.  ", name); break;
    }

    if (eval(name, edges, no_nodes,  W) == 1)
      fprintf(fpout,"The formula %s is true in G under W\n", name);
    else
      fprintf(fpout,"The formula %s is false in G under W\n", name);
  }

  fclose(fp);
  fclose(fpout);
  free(name);
  return 1;
}
