#include <iostream>

using namespace std;

// Copyright Adrian McMenamin 2010
// Licensed under the GNU GPL version 2
// or any later version at your discretion


class redblacknode {

	public:
		int colour;
		int value;
		redblacknode* left;
		redblacknode* right;
		redblacknode(int);
	
};

redblacknode::redblacknode(int v)
{
	colour = 1; //red
	left = NULL;
	right = NULL;
}


class redblacktree {
	public:
		redblacknode root;
		void insertnode(int);
		redblacktree();
	private:
		int depth;
		int placenode(const redblacknode*, redblacknode*&, int);
		
}

redblacktree::redblacktree()
{
	root = NULL;
	depth = 0;
}

void redblacktree::insertnode(int v)
{
	redblacknode* tmpnode = new redblacknode(v);
	if (root == NULL) {
		tmpnode->colour = 0;
		root = tmpnode;
		depth = 1;
		return;
	}

	depth = placenode(tmpnode, root, 1);
}

int redblacktree::placenode(const redblacknode* placeme, redblacknode*& treenode, int depthtested)
{
	if (placeme->value < treenode->value) {
		if (treenode->left)
			placenode(placeme, treenode->left, depthtested++);
		else {
			treenode->left = placeme;
		

	
	
