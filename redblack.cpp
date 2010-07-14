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

// turn line of two reds and a black into black with two children
void redblacktree::rotate2(redblacknode* node)
{
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

//straighten zig zag of two reds
void redblacktree::rotate1(redblacknode* node)
{
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
		if (node->up->colour == 0) {
			return;}

		if (node->uncle() && node->uncle()->colour == 1) {
			node->up->colour = 0;
			node->uncle()->colour = 0;
			node->grandparent()->colour = 1;
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
	else 
		node->colour = 0;

}

void redblacktree::insertnode(int v, redblacknode* node, int deep)
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

redblacknode* redblacktree::locatenode(int v, redblacknode* node)
{
	if (node == NULL)
		return node;
	if (v == node->value)
		return node;
	if (v < node->value)
		locatenode(v, node->left);
	else
		locatenode(v, node->right);
}

redblacknode* redblacktree::maxleft(redblacknode* node)
{	cout << "testing at " << node->value << endl;
	if (node->right)
		maxleft(node->right);
	else
		return node;
}

redblacknode* redblacktree::leftchain(redblacknode* node)
{
	if (node->left)
		leftchain(node->left);
	else
		return node;
}

bool redblacktree::removenode(int v)
{
	redblacknode* located = locatenode(v, root);
	redblacknode* altnode = NULL;
	if (located == NULL)
		return false;

	redblacknode* lefty = located->left;
	redblacknode* righty =  located->right;
	if (lefty && righty){
		altnode = maxleft(located->left);
		cout << "Found max left at " << altnode->value << endl; }

	redblacknode* par = located->up;
	int colour_to_go = located->colour;

	if (altnode) {
		redblacknode* endleftchain = leftchain(altnode);
		redblacknode* altpar = altnode->up;

		if (altpar != located) {
			if (altpar->left == altnode)
				altpar->left = NULL;
			else
				altpar->right = NULL;
		}

		if (par) {
			if (par->left == located)
				par->left = altnode;
			else
				par->right = altnode;
		}
		else
			root = altnode;


		//we know lefty and rigty and non-NULL
		lefty->up = endleftchain;
		endleftchain->left = lefty;
		altnode->right = righty;
		righty->up = altnode;
	} else {
		if (lefty) {
			lefty->up = par;
			if (par) {
				if (par->left == located)
					par->left = lefty;
				else
					par->right = lefty;
			}
			else
				root = lefty;
		}
		else if (righty) {
			righty->up = par;
			if (par) {
				if (par->right == located)
					par->right = righty;
				else
					par->left = righty;
			}
			else
				root = righty;
		} else {
			if (par) {
				if (par->right == located)
					par->right = NULL;
				else
					par->left = NULL;
			}
			else
				root = NULL;
		}
	}

	delete located;
	located = NULL;
	return true;
}

