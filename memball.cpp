#include <iostream>
#include "/usr/include/proc/readproc.h"
#include "redblack.h"

using namespace std;

// Copyright Adrian McMenamin, 2010
// Licensed under the GNU GPL v2
// Or any later version at your discretion

static int pagesize = 4096;

class process
{
	private:
		int res_pages;
		int shr_pages;
		int virt_pages;

	public:
		void set_reserve_pages(int pages){res_pages = pages;}
		void set_share_pages(int pages){shr_pages = pages;}
		void set_virt_pages(int pages){virt_pages = pages;}

		int get_reserved() {return res_pages;}
		int get_shared() { return shr_pages;}
		int get_virt(){ return virt_pages;}
};

class node
{

	private:
		process* proc;

	public:
		node* left;
		node* right;
		int reserve(){return proc->get_reserved();}
		int shared(){return proc->get_shared();}
		int virt(){return proc->get_virt();}
		node(process* p = NULL) { proc = p; left= NULL; right = NULL;}
		~node(){delete proc;}
};

class simpletree
{
	public:
		node* root;
		void insert_process(process* p);
		void insert_process_reserve(process* p, node*& n);
		void insert_process_shared(process* p, node*& n );
		void insert_process_virt(process* p, node*& n);
		simpletree(){root = NULL;}
		~simpletree();
		void show_in_order(node* n);
	private:
		void free(node*& n);
};		

void simpletree::show_in_order(node* n)
{
	if (!n)
		return;
	show_in_order(n->left);
	cout << n->reserve() << endl;
	show_in_order(n->right);
}
		
simpletree::~simpletree()
{
	free(root);
}

void simpletree::free(node*& n)
{
	if (!n) return;
	free(n->left);
	free(n->right);
	delete n;
	n = NULL;
}

void simpletree::insert_process(process* p)
{
	insert_process_reserve(p, root);
}

void simpletree::insert_process_reserve(process* p, node*& n)
{
	if (!n) {
		root = new node(p);
		return;
	}

	if (p->get_reserved() < n->reserve()) {
		if (n->left == NULL) {
			n->left = new node(p);
			return;
		}
		insert_process_reserve(p, n->left);
	}
	else {
		if (n->right == NULL) {
			n->right = new node(p);
			return;
		}
		insert_process_reserve(p, n->right);
	}
}

void simpletree::insert_process_shared(process* p, node*& n)
{
	if (!n) {
		root = new node(p);
		return;
	}

	if (p->get_shared() < n->shared()) {
		if (n->left == NULL) {
			n->left = new node(p);
			return;
		}
		insert_process_shared(p, n->left);
	}
	else {
		if (n->right == NULL) {
			n->right = new node(p);
			return;
		}
		insert_process_shared(p, n->right);
	}
}

void simpletree::insert_process_virt(process* p, node*& n)
{
	if (!n) {
		root = new node(p);
		return;
	}

	if (p->get_virt() < n->virt()) {
		if (n->left == NULL) {
			n->left = new node(p);
			return;
		}
		insert_process_virt(p, n->left);
	}
	else {
		if (n->right == NULL) {
			n->right = new node(p);
			return;
		}
		insert_process_virt(p, n->right);
	}
}

void show_preorder(redblacknode* node)
{
	if (node == NULL)
		return;
	cout << node->value << ",";
	if (node->colour == 0)
		cout << "BLACK";
	else
		cout << "RED";
	cout <<": ";
	show_preorder(node->left);
	show_preorder(node->right);
}

int main()
{
	pagesize = getpagesize();
	redblacktree* proctree = new redblacktree();

	PROCTAB* ptab = openproc(PROC_FILLMEM);
	proc_t* proc_details;
	
	while (proc_details = readproc(ptab, NULL))
	{
		if (proc_details->resident) {
//			process* addp = new process();
//			addp->set_reserve_pages(proc_details->resident);
//			proctree.insert_process(addp);
			int x = proc_details->resident;
			redblacknode* tmp = proctree->root;
			proctree->insertnode(x, tmp);
		}
	}

	closeproc(ptab);
//	proctree.show_in_order(proctree.root);
	show_preorder(proctree->root);
	delete proctree;	

	return 1;
}
