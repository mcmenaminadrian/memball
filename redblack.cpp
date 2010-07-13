#include <iostream>
#include "redblack.h"

using namespace std;

// Copyright Adrian McMenamin 2010
// Licensed under the GNU GPL version 2
// or any later version at your discretion

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

void redblacktree::rotate2(redblacknode* node)
{
	cout << " in rotate2 ";
	if (!node || !node->up)
		return;
	redblacknode* gp = node->grandparent();
	redblacknode* par = NULL;
	
	redblacknode* centrenode = node->up;
	if (gp) {
		par = gp->up;
		if (par) {
			if (par->left == gp)
				par->left = centrenode;
			else
				par->right = centrenode;
		} 
	}
	
	if (node->up->right == node)
	{
		redblacknode* centreleft = centrenode->left;
		centrenode->colour = 0;
		centrenode->left = gp;
		if (gp) {
			gp->up = centrenode;
			gp->colour = 1;
			gp->right = centreleft;
			if (centreleft)
				centreleft->up = gp;
		}
	} else {
		redblacknode* centreright = centrenode->right;
		centrenode->colour = 0;
		centrenode->right = gp;
		if (gp) {
			gp->up = centrenode;
			gp->colour = 1;
			gp->left = centreright;
			if (centreright)
				centreright->up = gp;
		}
	}
	centrenode->up = par;
	if (!par)
		root = centrenode;


}

void redblacktree::rotate1(redblacknode* node)
{	cout << " in rotate1 ";
	if (!node)
		return;
	redblacknode* par = node->up;
	redblacknode* rightnode = node->right;
	redblacknode* leftnode = node->left;
	redblacknode* rightleft = NULL;
	redblacknode* leftright = NULL;

	if (par) {
		if (par->left == node) {
			par->left = rightnode;
			if (rightnode) {
				rightleft = rightnode->left;
				rightnode->up = par;
				rightnode->left = node;
			}
			node->right = rightleft;
			if (rightleft)
				rightleft->up = node;
			node->up = rightnode;
		} else {
			par->right = leftnode;
			if (leftnode) {
				leftright = leftnode->right;
				leftnode->up = par;
				leftnode->right = node;
			}
			node->left = leftright;
			if (leftright)
				leftright->up = node;
			node->up = leftnode;
		}
	}
}

void redblacktree::balanceinsert(redblacknode* node)
{
	if (node->up) {
		if (node->up->colour == 0) { cout << ": parent node is black, balancing completed" << endl;
			return;}

		if (node->uncle() && node->uncle()->colour == 1) { cout << ": parent and uncle both red, flipping colours ";
			node->up->colour = 0;
			node->uncle()->colour = 0;
			node->grandparent()->colour = 1; cout << ": checking grandparent with value " << node->grandparent()->value;
			balanceinsert(node->grandparent());
		} else {
			
			if (node->grandparent()->left == node->up) {
				if (node->up->right == node){ 
					rotate1(node->up);
					node = node->left;
				}
				rotate2(node);
			} else {
				if (node->up->left == node){
					rotate1(node->up);
					node = node->right;
				}
				rotate2(node);
			}
		}
		return;
	}
	else { cout << " Changing root node with value " << node->value << " to black " << endl;
		node->colour = 0;}

}

void redblacktree::insertnode(int v, redblacknode* node, int deep)
{
	cout << " Inserting " << v << " into RB tree at depth " << deep;
	if (node == NULL) { cout << " and its the root node " << endl;
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
			balanceinsert(node);
			if (depth < deep) depth = deep;
		} else 
			insertnode(v, node->left, ++deep);
	} else {
		if (node->right == NULL) {
			node->right = new redblacknode(v);
			node->right->up = node;
			node = node->right;
			balanceinsert(node);
			if (depth < deep) depth = deep;
		} else
			insertnode(v, node->right, ++deep);
	}
}

