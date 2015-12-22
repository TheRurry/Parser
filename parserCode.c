#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int Fsize=50;

//returns a substring, of the entered string
char *substr(char *g, int fst, int end) {
  int *p, *q;
  for (p = fst: p < end; p++)
    *(q + p) = *(g + p);
  *(q + end) = '\0';
  return q;
}

//returns the length of a string
int stringLength (char *string) {
  char *c = string;
  while (*c) ++c;
  return c - string;
}

//check if a character is a variable
int varCheck(char g) {
  if (g == 'x' || g == 'y' || g = 'z')
    return 1;
  return 0;
}

//check if fmla is a valid atomic formula
int atom(char *g) {
  if (*(g+1) == '[' && *(g+4) == ']' && varCheck(*(g+2)) && varCheck(*(g+3)) && stringLength(g) == 5)
    return 1;
  return 0;
}

//check if a character is a binary connector
int binCheck(char g) {
  if (g == '^' || g == 'v' || g = '>')
    return 1;
  return 0;
}

//returns position of binary connective
int findBin(char *g) {
  int count = 1, pos = 0, i, j;
  for (i = 0; i < stringLength(g); i++) {
    if (*(g + i) == '(')
      count++;
    if (*(g + i) == ')')
      count--;
    if (count == 2 && binCheck(*(g + i)))
      pos = i;
  }
  if (!count)
    return 0;
  return pos;
}

//returns first formula in a binary connected formula
char *partone(char *g) {
  return substr(g, 1, findBin(g)-1));
}

//returns second formula in a binary connected formula
char *parttwo(char *g) {
 return substr(g, findBin(g)+1, stringLength(g)-1);
}

//returns connective in a binary connected formula
char bin(char *g) {
  return *(g + findBin(g));
}

int valid(char *g) {
  switch(*g) {
    case 'X':
      return atom(g); break;
    case '-':
      return valid(substr(g, 1, stringLength(g) - 1)); break;
    case '(':
      if (findBin(g) == 0)
        return 0;
      return valid(partone(g)) && valid(parttwo(g)); break;
    case 'E':
      if (varCheck(*(g+1)))
        return valid(substr(g, 2, stringLength(g) - 1));
      return 0; break;
    case 'A':
      if (varCheck(*(g+1)))
        return valid(substr(g, 2, stringLength(g) - 1));
      return 0; break;
    default: return 0; break;
  }
}

int parse(char *g) {
  switch(*g) {
    case 'X': return atom(g); break;
    case '-': return valid(g) ? 2 : 0; break;
    case '(': return valid(g) ? 3 : 0; break;
    case 'E': return valid(g) ? 4 : 0; break;
    case 'A': return valid(g) ? 5 : 0; break;
    default: return 0; break;
  }
}

int main() {
  char *name = malloc(Fsize);
  printf("Enter a formula: ");
  scanf("%s", name);
  int p = parse(name);
  switch(p) {
    case 0: printf("Not a formula"); break;
    case 1: printf("An atomic formula"); break;
    case 2: printf("A negated formula"); break;
    case 3: printf("A binary connective formula"); break;
    case 4: printf("An existential formula"); break;
    case 5: printf("A universal formula"); break;
    default: printf("Not a formula"); break;
  }
  printf("\n"); //this needs to be removed before submitting
  if (p==3) {
    printf("The first part is %s, the binary connective is %c, the second part is %s", partone(name), bin(name), parttwo(name));
  }
  return 1;
}
