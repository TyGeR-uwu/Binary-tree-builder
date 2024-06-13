#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>	
#define GAIN 110;
#define SHIFT 4;



typedef int Data;
typedef struct Node Node;

struct Node{
	Data data; 
	Node* left; 
	Node* right; 
};

void tree_print(Node *tree) {
	if (tree == NULL) {
		return;
	}
	tree_print(tree->left);
	printf("%d ", tree->data);
	tree_print(tree->right);
}

void tree_depth(Node* tree, int level, int * plevel_max) {
	if (tree == NULL) {
		return;
	}
	if (tree->left != NULL) {
		tree_depth(tree->left, level + 1, plevel_max);
	}
	if (tree->right != NULL) {
		tree_depth(tree->right, level + 1, plevel_max);
	}
	if (*plevel_max < level) {
		*plevel_max = level;
	}
}

void array_push(int **array, int i, int d){
	if (array[i][0] == 0) {
		array[i][0] = 1;
	}
	int karetka = array[i][0];
	array[i][karetka] = d;
	array[i][0] += 1;
}

void tree_divide_by_levels(Node* tree, int level, int depth, int **plvlArrays) {
	if (tree == NULL) {
		return;
	}
	array_push(plvlArrays, level, tree->data);
	if ( (tree->left == NULL) && (level + 1 <= depth) ) {
		array_push(plvlArrays, level + 1, 0);
	}
	if (tree->left != NULL) {
		tree_divide_by_levels(tree->left, level + 1, depth, plvlArrays);
	}
	if ((tree->right == NULL) && (level + 1 <= depth)) {
		array_push(plvlArrays, level + 1, 0);
	}
	if (tree->right != NULL) {
		tree_divide_by_levels(tree->right, level + 1, depth, plvlArrays);
	}
}

void tree_draw(Node* tree) {
	int depth = 0;
	tree_depth(tree, 0, &depth);

	int horDepth[30]; // массивчик с максимальным значением чисел на уровне дерева(horizontal) от уровня дерева(vertical)
	int k = 1;
	for (int i = 0; i <= depth; i++) {
		horDepth[i] = k;
		k *= 2;
	}
	int** plvlArrays = (int**)malloc((depth + 1) * sizeof(int*)); // выделяем место под уровни. +1 т.к. нужен не ИНДЕКС уровней а их КОЛИЧЕСТВО!В данной фунции массивы используют свой 0-ой элемент как место каретки в этом же массиве
	for (char i = 0; i <= depth; i++) {
		plvlArrays[i] = (int*)malloc((horDepth[i] + 1) * sizeof(int));// +1 т.к. первый элемент вложенного массива это каретка
	}
	for (int i = 0; i <= depth; i++) {
		for (int j = 0; j < horDepth[i] + 1; j++) {
			plvlArrays[i][j] = 0;
		}
	}


	tree_divide_by_levels(tree, 0, depth, plvlArrays); // заполняем массив по уровням

	int gain = GAIN;
	int shift = SHIFT;
	for (int i = 0; i <= depth; i++) {
		for (int i = 0; i <= (gain/2) - shift; i++) {
			printf(" ");
		}
		for (int j = 1; j < horDepth[i] + 1; j++) {
			if (plvlArrays[i][j] != 0) {
				printf("[%d]", plvlArrays[i][j]);
			}
			else {
				printf("{ }");
			}	
			for (int i = 0; i <= (gain) - shift; i++) {
				printf(" ");
			}
		}
		gain /= 2;
		printf("\n");
	}
}

Node* tree_add(Node * tree, Data d) {
	if (tree == NULL) {
		Node* p = malloc(sizeof(Node));
		p->data = d;
		p->left = p->right = NULL; 
		return p;
	}
	if (d < tree->data) {
		tree->left = tree_add(tree->left, d);
	}
	if (d > tree->data) {
		tree->right = tree_add(tree->right, d);
	}
	return tree;

}

Node* tree_destroy(Node* tree) {
	if (tree == NULL) {
		return NULL;
	}
	tree_destroy(tree->left);
	tree_destroy(tree->right);
	free(tree);
	return NULL;
}

void tree_weight(Node* tree, int *value) {
	if (tree == NULL) {
		return;
	}
	if (tree != NULL) {
		*value += 1;
	}
	tree_weight(tree->left, value);
	tree_weight(tree->right, value);
}

void left_returner(Node ** tree, Node ** prevEl, Node ** newRoot) {
	if ((*tree)->right != NULL) {
		*prevEl = *tree;
		left_returner(&(*tree)->right, prevEl, newRoot);
	}
	if ((*tree)->right == NULL) {
		*newRoot = *tree;
	}
}

void right_returner(Node** tree, Node** prevEl, Node** newRoot) {
	if ((*tree)->left != NULL) {
		*prevEl = *tree;
		right_returner(&(*tree)->left, prevEl, newRoot);
	}
	if ((*tree)->left == NULL) {
		*newRoot = *tree;
	}
}


Node* tree_sort(Node** tree) {
	if (*tree == NULL) {
		return NULL;
	}
	int left = 0, right = 0;
	tree_weight((*tree)->left, &left);
	tree_weight((*tree)->right, &right);

	Node* newRoot = NULL;
	if (left > right + 1) {
		Node* prevEl = *tree;
		left_returner(&(*tree)->left, &prevEl, &newRoot);
		if ((newRoot->left == NULL) && (newRoot->right == NULL)) {
			prevEl->right = NULL;
		}
		else {
			if (prevEl->left == newRoot) {
				prevEl->left = newRoot->left;
			}
			else {
				if (newRoot->left != NULL) {
					prevEl->right = NULL;
					prevEl->right = newRoot->left;
				}
			}
		}														//КАК ЛУЧШЕ С ТОЧКИ ЗРЕНИЯ ЧИТАЕМОСТИ?
		newRoot->left = (*tree)->left;
		(*tree)->left = NULL;
		newRoot->right = *tree;
	}

	if (right > left + 1) {
		Node* prevEl = *tree;
		right_returner(&(*tree)->right, &prevEl, &newRoot);
		if ((newRoot->right == NULL) && (newRoot->left == NULL)) {
			prevEl->left = NULL;
			newRoot->right = (*tree)->right;
			(*tree)->right = NULL;
			newRoot->left = *tree;
		}
		else {
			if (prevEl->right == newRoot) {
				prevEl->right = newRoot->right;
				newRoot->right = (*tree)->right;
				(*tree)->right = NULL;
				newRoot->left = *tree;
			}
			else {
				if (newRoot->right != NULL) {
					prevEl->left = NULL;
					prevEl->left = newRoot->right;
					newRoot->right = (*tree)->right;
					(*tree)->right = NULL;
					newRoot->left = *tree;
				}
			}
		}
	}

	//tree_draw(newRoot);
	if (newRoot == NULL) {
		newRoot = *tree;
		goto m1;
	}

	left = 0, right = 0;
	tree_weight(newRoot->left, &left);
	tree_weight(newRoot->right, &right);

	m1:
	if ((left + 1 == right) || (right + 1 == left) || (right == left)) {
		newRoot->left = tree_sort(&(newRoot->left));
		newRoot->right = tree_sort(&(newRoot->right));
		return newRoot;
	}
	else {
		tree_sort(&newRoot);
	}
	
}

int main(){

	Node* tree = NULL;
	tree = tree_add(tree, 400);
	tree = tree_add(tree, 20);
	tree = tree_add(tree, 10);
	tree = tree_add(tree, 30);
	tree = tree_add(tree, 238);
	tree = tree_add(tree, 50);
	tree = tree_add(tree, 45);
	tree = tree_add(tree, 28);
	tree = tree_add(tree, 132);
	tree = tree_add(tree, 94);
	tree = tree_add(tree, 80);
	tree = tree_add(tree, -100);



	tree = tree_sort(&tree);
	tree_draw(tree);

	tree_destroy(tree);
	return 0;
}