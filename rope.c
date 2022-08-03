#include "./rope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define EMPTY ""

char *strdup(const char *s);

// Alocarea de memorie pentru un nou nod și inițializarea
// acestuia cu string-ul str
RopeNode* makeRopeNode(const char* str) {
    RopeNode *node = malloc(sizeof(RopeNode));

    // Verificăm dacă memoria a fost alocată cu succes
    if (!node) {
    	fprintf(stderr, "Malloc node\n");
    	return NULL;
    }

    node->left = NULL;
    node->right = NULL;
    node->weight = strlen(str);
    node->str = str;

    return node;
}

// Alocarea de memorie pentru un nou rope și inițializarea
// root-ului
RopeTree* makeRopeTree(RopeNode* root) {
    RopeTree *tree = malloc(sizeof(RopeTree));

    // Verificăm dacă memoria a fost alocată cu succes
    if (!tree) {
    	fprintf(stderr, "Malloc tree\n");
    	return NULL;
    }

    tree->root = root;
    return tree;
}

void printRopeNode(RopeNode* rn) {
    if (!rn)
        return;

    if (!(rn->left) && !(rn->right)) {
        printf("%s", rn->str);
        return;
    }

    printRopeNode(rn->left);
    printRopeNode(rn->right);
}

void printRopeTree(RopeTree* rt) {
    if (rt) {
        printRopeNode(rt->root);
        printf("%s", "\n");
    }
}

void debugRopeNode(RopeNode* rn, int indent) {
    if (!rn)
        return;

    for (int i = 0; i < indent; ++i)
        printf("%s", " ");

    if (!strcmp(rn->str, EMPTY))
        printf("# %d\n", rn->weight);
    else
        printf("%s %d\n", rn->str, rn->weight);

    debugRopeNode(rn->left, indent+2);
    debugRopeNode(rn->right, indent+2);
}

int getTotalWeight(RopeNode* rt) {
    if (!rt)
        return 0;

    return rt->weight + getTotalWeight(rt->right);
}

/* Funcție pentru concatenarea a două noduri prin crearea unei
rădăcini și recalcularea greutății acesteia */
RopeNode* __concat(RopeNode* n1, RopeNode* n2)
{
	RopeNode *root = makeRopeNode(strdup(EMPTY));
	root->left = n1;
    root->right = n2;
    root->weight = getTotalWeight(root->left);

    return root;
}

// Crearea unui nou rope din două rope-uri
RopeTree* concat(RopeTree* rt1, RopeTree* rt2) {
    RopeNode *root = __concat(rt1->root, rt2->root);

    return makeRopeTree(root);
}

/* Funcție pentru întoarcerea caracterului de pe 
poziția index din rope */
char __indexRope(RopeNode* node, int idx)
{
    // în cazul în care trebuie să căutăm caracterul
    // în subarborele drept al nodului curent
	if (node->weight <= idx) {
		if (node->right != NULL)
			return __indexRope(node->right, idx - node->weight);

    // Analog pentru subarborele stâng
	} else {
		if (node->left != NULL)
			return __indexRope(node->left, idx);
	}

    // Găsirea caracterului de pe poziția index
	return node->str[idx];
}

// Verificăm dacă rope-ul există, caz în care
// căutăm caracterul de pe poziția dorită
char indexRope(RopeTree* rt, int idx) {
    if (!rt)
    	return '\0';

    return __indexRope(rt->root, idx);
}

/* Funcție în care formăm șirul căutat,
prin găsirea pe rând a fiecărui caracter*/
char* search(RopeTree* rt, int start, int end) {
    if (!rt)
    	return NULL;

    char *aux = calloc(end - start + 1, sizeof(char));

    for (int i = start; i < end; i++) {
    	aux[i - start] = indexRope(rt, i);
    }

    return aux;
}

/* Funcție care clonează un arbore pornind
de la un nod */
RopeNode* __cloneNode(RopeNode *node)
{
	if(!node)
		return NULL;

    // Formarea noului nod rădăcină din subarborele curent
	RopeNode *new_node = makeRopeNode(strdup(node->str));
	new_node->left = __cloneNode(node->left);
	new_node->right = __cloneNode(node->right);

    // Dacă nodul nu este frunză, atunci recalculăm greutatea
	if (strlen(new_node->str) == 0)
		new_node->weight = getTotalWeight(new_node->left);

	return new_node;
}

/* Funcția care se ocupă atât cu împărțirea efectivă a rope-ului, dar
și cu împărțirea unei frunze în două */
void __split(RopeNode *node, int idx, int cnt, RopeNode **prev)
{
	if (!node)
		return;

    // Dacă nodul curent este frunză
	if (!node->left && !node->right) {
        // Dacă aici se află indexul căutat
		if (cnt + node->weight > idx) {
            // Alocăm memorie pentru noile string-uri ale frunzelor
			char *s2 = calloc(node->weight + cnt - idx + 1, sizeof(char));
			char *s1 = calloc(idx - cnt + 1, sizeof(char));

            // Copiem string-ul inițial în cele doua noi șiruri conform
            // indexului
			strcpy(s2, node->str + idx - cnt);
			strncpy(s1, node->str, idx - cnt);
			s1[idx - cnt] = '\0';

            // Eliberăm memoria vechiului string și o realocăm cu
            // un șir gol
			free((void *)node->str);
			node->str = strdup(EMPTY);

			node->left = makeRopeNode(s1);
			*prev = makeRopeNode(s2);
			node->weight = getTotalWeight(node->left);
		}
		return;
	}

    // Reapelăm pentru subarborele stâng
	__split(node->left, idx, cnt, prev);

    // Recalculăm greutatea nodului curent
	node->weight = getTotalWeight(node->left);

    // Dacă am găsit indexul, realizăm tăierea
    // și concatenarea pas cu pas a nodurilor tăiate
	if (node->weight + cnt == idx) {
		if (*prev != NULL) {
			RopeNode *aux = __concat(*prev, node->right);
			*prev = aux;
			node->right = NULL;
		} else {
			*prev = node->right;
			node->right = NULL;
		}
	}

    // Reapelăm pentru subarborele drept
	__split(node->right, idx, cnt + node->weight, prev);
}

/* Funcție pentru împărțirea rope-ului în două noi
rope-uri, reprezentate printr-un splitpair, în funcție de index*/
SplitPair split(RopeTree* rt, int idx) {
    // Clonarea arborelui
	RopeNode *new_root = __cloneNode(rt->root);

    SplitPair sp;

    // Cazul în care indexul se află la mijloc
    if (idx > 0 && idx < getTotalWeight(new_root)) {
	    sp.left = new_root;
	    RopeNode *prev = NULL;
	    __split(new_root, idx, 0, &prev);
	    sp.right = prev;

    // Cazul în care indexul se află la final
	} else if (idx == getTotalWeight(new_root)) {
		sp.left = new_root;
		sp.right = NULL;

    // Cazul în care indexul se află la început
	} else {
		sp.right = new_root;
	    sp.left = NULL;
	}

    return sp;
}

// Funcție care adaugă un șir în rope-ul curent
// la indexul precizat
RopeTree* insert(RopeTree* rt, int idx, const char* str) {
    RopeNode *node = makeRopeNode(str);

    SplitPair sp = split(rt, idx);

    if (idx == 0) {
    	return makeRopeTree(__concat(node, sp.right));
    } else if (idx == getTotalWeight(rt->root)) {
    	return makeRopeTree(__concat(sp.left, node));
    } else {
    	RopeNode *aux = __concat(sp.left, node);
    	return makeRopeTree(__concat(aux, sp.right));
    }
}

// Funcție pentru eliberarea unui subarbore
// pornind de la un nod dat
void __freeNode(RopeNode *node)
{
	if (!node)
		return;

	__freeNode(node->left);
	__freeNode(node->right);

	free((void *)node->str);
	free(node);
}

// Funcție pentru ștergerea unei părți de lungime dată
// începând de la poziția indicată
RopeTree* delete(RopeTree* rt, int start, int len) {
	// Dacă trebuie să ștergem de la începutul string-ului
    if (start == 0) {
    	SplitPair sp = split(rt, len);

    	__freeNode(sp.left);
    	return makeRopeTree(sp.right);
	// Dacă trebuie să ștergem până la finalul string-ului
    } else if (start + len >= getTotalWeight(rt->root)) {
    	SplitPair sp = split(rt, start);

    	__freeNode(sp.right);
    	return makeRopeTree(sp.left);
	// Dacă trebuie să ștergem la mijlocul string-ului
    } else {
    	SplitPair sp1 = split(rt, start);
    	SplitPair sp2 = split(makeRopeTree(sp1.right), len);

    	__freeNode(sp1.right);
    	__freeNode(sp2.left);
    	return makeRopeTree(__concat(sp1.left, sp2.right));
    }
}
