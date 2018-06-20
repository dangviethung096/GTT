#include "TernaryTree.h"
#include <stdlib.h>
#include <string.h>

#define MAX_TREE 50
#define MAX_GTT_TABLE 1000
/*a node of ternary search tree*/
struct Node{
	U8bit data;
	/*true if this character is last character of one of the string*/
	U8bit is_leaf_node : BOOL_SIZE;
	int index_of_table;
	struct Node *left, *eq, *right;
};


/*a utility function to create a new ternary search tree node*/
struct Node* new_TST_node(U8bit data)
{
	struct Node* temp = (struct Node*) malloc(sizeof(struct Node));
	temp->data = data;
	temp->is_leaf_node = ADVGWS_FALSE;
	temp->index_of_table = ADVGWS_FALSE;
	temp->left = temp->eq = temp->right = NULL;
	return temp;
}


/*function to insert a new word in a ternary search tree*/
void insert_TST(struct Node** root, U8bit *str, int index_of_table)
{
	/*base case: tree is empty*/
	if(!(*root)){
		// if(*str == '\0')
		// 	printf("why?\n");
		*root = new_TST_node(*str);
	}
	/*if current character of str is smaller than root's character,
	then insert this word in left subtree of root*/
	if((*str) < (*root)->data)
		insert_TST(&( (*root)->left ), str,index_of_table);
	/*if current character of word is greater than root's character,
	then insert this word in right subtree of root*/
	else if ((*str) > (*root)->data)
        insert_TST(&( (*root)->right ), str, index_of_table);
	/*If current character of word is same as root's character*/
	else 
	{
		if(*(str+1) != '\0')
			insert_TST(&( (*root)->eq ), str+1,index_of_table);
		else /*the last character of the word*/
		{
			(*root)->is_leaf_node = ADVGWS_TRUE;
			(*root)->index_of_table = index_of_table;
		}
	}
}

/*a function check if current node is leaf node or not*/
advgws_return_t is_leaf_TST_node(struct Node *root)
{
	return (root->is_leaf_node == ADVGWS_TRUE);
}
/*a function if current node has any child node or not*/
advgws_return_t is_free_TST_node(struct Node* root)
{
	if(root->left || root->eq || root->right)
		return ADVGWS_FAILURE;
	return 1;
}

/*a function to delete a string in TST*/
advgws_return_t delete_TST_node(struct Node *root, U8bit str[], int level)
{
	if(root==NULL)
		return ADVGWS_FAILURE;
	/*case 4 key present in TST, having at least one other data as prefix data*/
	if(str[level+1] == '\0')
	{
		/*un mark leaf node is present*/
		if(is_leaf_TST_node(root))
		{
			root->is_leaf_node = ADVGWS_FALSE;
			root->index_of_table = ADVGWS_FALSE;
			return is_free_TST_node(root);
		}
		/*else string is not present in TST and return 0*/
		else 
			return ADVGWS_FAILURE;
	}
	else 
	{
		/*case 3 key is prefix data of another long data in TST*/
		if(str[level] < root->data)
		{
			if(delete_TST_node(root->left, str, level))
			{
				free(root->left);
				root->left = NULL;
			}
		}
			
		else if(str[level] > root->data)
		{
			if(delete_TST_node(root->right, str, level))
			{
				free(root->right);
				root->right = NULL;
			}
		}
			
		/*case 1 data may not be there in TST*/
		else if(str[level] == root->data)
		{
			/*case 2 data present as unique data*/
			if(delete_TST_node(root->eq, str, level+1))
			{
				/*delete the last node, neither it has anychild nor it is part 
				of any other string*/
				free(root->eq);
				root->eq = NULL;
				return (!is_leaf_TST_node(root) && is_free_TST_node(root));
			}
		}
	}
	return ADVGWS_FAILURE;
}

void delete_all_TST_node(struct Node **root)
{
	
	if(*root == NULL)
		return;
	delete_all_TST_node( &(*root)->left);
	delete_all_TST_node( &(*root)->eq);
	delete_all_TST_node( &(*root)->right);
	free(*root);
	*root = NULL;
}
/*A recursive function to traverse Ternary Search Tree*/
void traverse_TST_util_end(struct Node* root, U8bit* buffer, int level)
{
	if(root)
	{
		/*first traverse the left subtree*/
		traverse_TST_util_end(root->left, buffer, level);
		/*store the character of this node*/
		buffer[level] = root->data;
		if(root->is_leaf_node)
		{
			buffer[level+1] = '\0';
			printf("%s ", buffer);
			printf("%d\n", root->index_of_table);
		}
		/*traverse the subtree using equal pointer (middle subtree)*/
		traverse_TST_util_end(root->eq, buffer, level+1);
		/*finally traverse the right subtree*/
		traverse_TST_util_end(root->right, buffer, level);
	}
}
/*The main function to traverse a Ternary Search Tree.
 It mainly uses traverse_TST_util_end() */
void traverse_TST(struct Node* root)
{
	U8bit buffer[MAX_TREE];
	traverse_TST_util_end(root, buffer, 0);
}

/*Function to search a given word in TST*/
advgws_return_t search_TST(struct Node *root, U8bit *str, int *index_of_table)
{
	if(!root)
		return ADVGWS_FAILURE;
	if(*str < (root)->data)
		return search_TST(root->left, str,index_of_table);
	else if(*str > (root)->data)
		return search_TST(root->right, str,index_of_table);
	// equal
	//if(*(str+1) == 0xFF)
	if(*(str+1))
	{
		*index_of_table = root->index_of_table;
		return root->is_leaf_node;
	}
	return search_TST(root->eq, str+1,index_of_table);
}

advgws_return_t search_TST_dynamic(struct Node *root, U8bit *str, int *index_of_table)
{
  int i = 0;
  // printf("%d %c\n",strlen(str), *str);	  
  while(i < strlen( (const char *) str)) {
	  if(NULL  == root)
		  break;
	   //printf("%c %c \n",root->data, *str);	  
	  if(str[i] < root->data)
		  root = root->left;
	  else if(str[i] > root->data)
		  root = root->right;
	  else {
		  if(str[i+1] == '\0')
		  {
			  // printf("%d %c \n",i, *str);	  
			  *index_of_table = root->index_of_table;
			  return root->is_leaf_node; //ADVGWS_SUCCESS;
		  }
		  i++;
	  	  root = root->eq;
	  }
  }
  return ADVGWS_FAILURE;
}

/* Check wildcard if left_child and right_child have wildcard */
advgws_return_t check_wildcard(struct Node *root, U8bit *str, int *index_of_table)
{
	if(root == NULL)
		return ADVGWS_FAILURE;
	
	
	struct Node *temp;
	/* Check right */
	temp = root->right;
	while(temp != NULL)
	{
		if(temp->data == '*' || temp->data == '?')
		{
			if(search_TST_wildcard(temp, str, index_of_table) == ADVGWS_SUCCESS)
				return ADVGWS_SUCCESS;
		}
		temp = temp->right;
	}
	/* Check left */
	temp = root->left;
	while(temp != NULL)
	{
		if(temp->data == '*' || temp->data == '?')
		{
			if(search_TST_wildcard(temp, str, index_of_table) == ADVGWS_SUCCESS)
				return ADVGWS_SUCCESS;
		}
		temp = temp->left;
	}
	return ADVGWS_FAILURE;
}

//search in wildcard tst tree
advgws_return_t search_TST_wildcard(struct Node *root, U8bit *str, int *index_of_table)
{
	if(!root)
		return ADVGWS_FAILURE;

	/* Check wildcard: two bound of layer */
	if(check_wildcard(root, str, index_of_table) == ADVGWS_SUCCESS)
	{
		return ADVGWS_SUCCESS;
	}


	//check if this is a wildcard
	if(root->data == '?' || *str  == root->data )				
	{
		/* Terminate of find wildcard */
		if(*(str+1) == '\0')
		{
			if(root->is_leaf_node == ADVGWS_SUCCESS)
			{
				*index_of_table = root->index_of_table;
				return ADVGWS_SUCCESS;
			}
			/* If next equal child is '*' character */
			if((root->eq != NULL) && (root->eq->data == '*') && (root->eq->is_leaf_node == ADVGWS_TRUE))
			{
				*index_of_table = root->eq->index_of_table;
				return root->eq->is_leaf_node;
			}
			return ADVGWS_FAILURE;
		}
		/* Replace a character */
		if(root->data == '?')
		{
			if(search_TST_wildcard(root->left, str, index_of_table))
			{
				return ADVGWS_SUCCESS;
			}
		}
		return search_TST_wildcard(root->eq, str+1, index_of_table);		
	}
	else if((root)->data == '*')
	{	
		
		if(root->is_leaf_node)
		{
			*index_of_table = root->index_of_table;
			return ADVGWS_SUCCESS;
		}
		if( *(str+1) == '\0')
			return search_TST_wildcard(root->eq,str,index_of_table);
		return (search_TST_wildcard(root, str+1, index_of_table) || search_TST_wildcard(root->eq, str, index_of_table) 
				|| search_TST_wildcard(root->right, str, index_of_table));
		
	}
	else
	{
		if(root->data > *str)
		{
			return search_TST_wildcard(root->left, str,index_of_table);
		}
		else
		{
			return search_TST_wildcard(root->right, str,index_of_table);
		}
	}
	return ADVGWS_FAILURE;
}

/*  */
void draw_child(struct Node *parent, struct Node *child, FILE *file, char *label)
{
	char vertex[2048];
	if(child == NULL)
	{
		return;
	}
	sprintf(vertex, "\"%c_%p\" -> \"%c_%p\" [label=\"%s\"];\n", parent->data, parent, child->data, child, label);
	fwrite(vertex, sizeof(char), strlen(vertex), file);
}
/* Draw a tree in a graph */
void draw_tree(struct Node *root, FILE *file)
{
	if(root == NULL)
	{
		return;
	}
	int isOpen = 0;
	char vertex[2048];
	if(file == NULL)
	{
		file = fopen(TREE_FILE, "wt");
		isOpen = 1;
		strcpy(vertex, "digraph G {\n");
		fwrite(vertex, sizeof(char), strlen(vertex), file);
	}

	/* Draw all child node */
	/* parrent connect to left child */
	draw_child(root, root->left, file, C_LEFT);
	/* parrent connect to equal child */
	draw_child(root, root->eq, file, C_EQUAL);
	/* parrent connect to right child */
	draw_child(root, root->right, file, C_RIGHT);

	/* Recursion with 3 child */
	/* Left */
	draw_tree(root->left, file);
	/* Equal */
	draw_tree(root->eq, file);
	/* Right */
	draw_tree(root->right, file);

	if(isOpen)
	{
		strcpy(vertex, "}");
		fwrite(vertex, sizeof(char), strlen(vertex), file);
		fclose(file);
	}
}