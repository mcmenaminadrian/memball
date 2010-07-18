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

redblacknode* redblacknode::sibling() const
{
	if (!up)
		return NULL;
	if (up->left == this)
		return up->right;
	else
		return up->left;
}

bool redblacknode::bothchildrenblack() const
{
	if (right && right->colour == 1)
		return false;
	if (left && left->colour == 1)
		return false;
	return true;
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

void redblacktree::transform1(redblacknode* node)
{
	redblacknode* par = node->up;
	redblacknode* lefty = node->left;
	redblacknode* righty = node->right;
	if (lefty && lefty->colour == 1) {
		par->right = lefty;
		lefty->colour = 0;
		lefty->up = par;
		node->left = lefty->right;
		if (node->left)
			node->left->up = node;
		lefty->right = node;
		node->up = lefty;
		node->colour = 1;
	}
	else if (righty && righty->colour == 1) {
		par->left = righty;
		righty->colour = 0;
		righty->up = par;
		node->right = righty->left;
		if (node->right)
			node->right->up = node;
		righty->left = node;
		node->up = righty;
		node->colour = 1;
	}
}

void redblacktree::transform2(redblacknode* node)
{
	redblacknode* sibling = node->sibling();
	int oldcolour = sibling->colour;
	
	if (node->up->left == node)
		rotate2(sibling->right);
	else
		rotate2(sibling->left);
	sibling->colour = oldcolour;
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

redblacknode* redblacktree::minright(redblacknode* node)
{cout << "testing right at " << node->value << endl;

	if (node->left)
		minright(node->left);
	else
		return node;
}

redblacknode* redblacktree::maxleft(redblacknode* node)
{	cout << "testing at " << node->value << endl;
	if (node->right)
		maxleft(node->right);
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
		if (altnode->colour == 0)
			altnode = minright(located->right);
		located->value = altnode->value;
		located = altnode;
	}

	//located is now a node with only one child at most
	redblacknode* par = located->up;
	redblacknode* follow = NULL;
	if (lefty)
		follow = lefty;
	else
		follow = righty;

	if (par) {
		if (par->left == located)
			par->left = follow;
		else
			par->right = follow;
		}
	else
		root = follow;
	follow->up = par;

	//easy to remove a red
	if (located->colour == 1) {
		delete located;
		return true;
	}

	//also easy if follow is red
	if (follow->colour == 1) {
		follow->colour = 0;
		delete located;
		return true;
	}

	//loop through the fixes
	do {
		redblacknode* sibling = follow->sibling();
		//test sibling status
		if (sibling) {
			//red?
			if (sibling->colour == 1) {
				rotate2(sibling);
				sibling = follow->sibling();
			}
			//case above can fall directly into case below
			if (follow->up->colour == 1) {
				if (sibling->bothchildrenblack()) {
					sibling->colour = 1;
					follow->up->colour = 0;
					delete located;
					return true;
				}
			}
			if (sibling->bothchildrenblack()){
				sibling->colour = 1;
				follow = follow->up;
				continue;
			}
			if (follow->up->right == sibling) {
				if (sibling->left &&
					sibling->left->colour == 1 &&
					sibling->right->colour == 0)
					transform1(sibling);
				else {
					transform2(follow);
					delete located;
					return true;
				}
			}		
			else if (follow->up->left == sibling) {
				if (sibling->right &&
					sibling->right->colour == 1 &&
					sibling->left->colour == 0)
					transform1(sibling);
				else {
					transform2(follow);
					delete located;
					return true;
				}
			}
		} else {
			delete located;
			return true;
		}
	}while(true);
}			
