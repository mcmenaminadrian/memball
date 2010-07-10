#include <iostream>

using namespace std;

// Copyright Adrian McMenamin 2010
// Licensed under the GNU GPL version 2
// or any later version at your discretion


class redblacknode {

	public:
		int colour;
		int value;
		redblacknode* up;
		redblacknode* left;
		redblacknode* right;
		redblacknode(const int);
		redblacknode* grandparent() const;
		redblacknode* uncle() const;
};

redblacknode* redblacknode::grandparent() const
{
	if (up)
		return up->up;
	else
		return NULL;
}

redblacknode* redblacknode::uncle() const
{
	redblacknode* g = grandparent();
	if (g) {
		if (g->left == up)
			return g->right;
		else
			return g->left;
	}
	return NULL;
}

redblacknode::redblacknode(const int v)
{
	colour = 1; //red
	value = v;
	up = NULL;
	left = NULL;
	right = NULL;
}

class redblacktree {
	public:
		redblacknode* root;
		void insertnode(int, redblacknode*, int);
		redblacktree();
		~redblacktree();
	private:
		int depth; //helps with drawing
		void balanceinsert(redblacknode*);
		void rightrotate(redblacknode*);
		void leftrotate(redblacknode*);
		void free(redblacknode*);
			
};

redblacktree::~redblacktree()
{
	free(root);
}

void redblacktree::free(redblacknode* v)
{
	if (v == NULL)
		return;
	free(v->left);
	redblacknode* tmp = v->right;
	delete v;
	free(tmp);
}

redblacktree::redblacktree()
{
	root = NULL;
	depth = 0;
}

void redblacktree::rightrotate(redblacknode* node)
{
	if (!node)
		return;
	redblacknode* par = node->up;
	redblacknode* leftnode = node->left;
	redblacknode* leftright = NULL;

	if (par) {
		if (par->left == node)
			par->left = leftnode;
		else
			par->right = leftnode;
	}
	if (leftnode) {
		leftright = leftnode->right;
		leftnode->up = par;
		leftnode->right = node;
	}
	node->left = leftright;
	node->up = leftnode;
}

void redblacktree::leftrotate(redblacknode* node)
{
	if (!node)
		return;
	redblacknode* par = node->up;
	redblacknode* rightnode = node->right;
	redblacknode* rightleft = NULL;

	if (par) {
		if (par->left == node)
			par->left = rightnode;
		else
			par->right = rightnode;
	}
	if (rightnode) {
		rightleft = rightnode->left;
		rightnode->up = par;
		rightnode->left = node;
	}
	node->right = rightleft;
	node->up = rightnode;
}

void redblacktree::balanceinsert(redblacknode* node)
{
	if (node->up) {
		if (node->up->colour == 0)
			return;

		if (node->uncle() && node->uncle()->colour == 1) {
			node->up->colour = 0;
			node->uncle()->colour = 0;
			node->grandparent()->colour = 1;
			balanceinsert(node->grandparent());
		}

		else {
			if (node->up->right == node) {
				leftrotate(node);
				rightrotate(node->grandparent());
			}
			else {
				rightrotate(node);
				leftrotate(node->grandparent());
			}
		}
	}
	else
		node->colour = 0;

}

void redblacktree::insertnode(int v, redblacknode* node, int deep = 1)
{
	if (node == NULL) {
		root = new redblacknode(v);
		root->colour = 0;
		depth = deep;
		return;
	}
	if (v < node->value) {
		if (node->left == NULL) {
			node->left = new redblacknode(v);
			node->left->up = node;
			node = node->left;
		} else 
			insertnode(v, node->left, deep++);
	} else {
		if (node->right == NULL) {
			node->right = new redblacknode(v);
			node->right->up = node;
			node = node->right;
		} else
			insertnode(v, node->right, deep++);
	}
	if (depth < deep) depth = deep;

	//simple insertion done
	//but reblancing may be required

	balanceinsert(node);

			
}

