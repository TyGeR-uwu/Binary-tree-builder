// ГЕНЕРАТОР ЧИСЕЛ(ГОТОВЫЙ СПИСОК СНАЧАЛА)
//СОЗДАНИЕ ДЕРЕВА
//БАЛАНСИРОВКА ДЕРЕВА
//ВЫВОД
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>	

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

void array_push(int **array, int f, int s){
	
}

void tree_divide_by_levels(Node* tree, int level, int **plevels) {
	if (tree == NULL) {
		return;
	}
	plevels[level][1] = 1;
	if (tree->left != NULL) {
		tree_divide_by_levels(tree->left, level + 1, plevels);
	}
	if (tree->right != NULL) {
		tree_divide_by_levels(tree->right, level + 1, plevels);
	}
}

void tree_draw(Node* tree) {
	int depth = 0;
	tree_depth(tree, 0, &depth);

	int horVertDepth_rlship[30]; // массивчик с максимальным значением чисел на уровне дерева от уровня дерева (по индексам)
	int k = 1;
	for (int i = 0; i < 30; i++){
		horVertDepth_rlship[i] = k;
		k *= 2;
	}
	free(k);
	

	int** plevels = (int**)malloc(depth * sizeof(int*)); // выделяем место под уровни. 
	for (char i = 0; i < 30; i++) {
		plevels[i] = (int*)malloc(horVertDepth_rlship[depth] * sizeof(int));
	}

	tree_divide_by_levels(tree, 0, &plevels); //asdsad

}//asdasd
//asdsa
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

int main(){



	Node* tree = NULL;
	tree = tree_add(tree, 4);
	tree = tree_add(tree, 3);
	tree = tree_add(tree, 1);
	tree = tree_add(tree, 2);
	tree = tree_add(tree, 5);
	//tree_print(tree);
	//printf("\n");
	//tree_draw(tree, 0);

	int level_max = 0;
	tree_depth(tree, 0, &level_max);
	//printf("%d", level_max);

	return 0;
}

//tree_draw
//tree_sort