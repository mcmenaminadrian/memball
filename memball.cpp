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
	redblacknode* top = proctree->root;
	redblacknode* nextl = top->left;
	redblacknode* nextr = top->right;

	cout << endl << proctree << endl;

	cout << endl << "Killing root"<< endl;
	proctree->removenode(top->value);
	show_preorder(proctree->root);

	cout << endl << proctree << endl;

	cout << endl << "Kill left" << endl;
	proctree->removenode(nextl->value);
	show_preorder(proctree->root);

	cout << endl << proctree << endl;

	cout << endl << "Kill right" << endl;
	proctree->removenode(nextr->value);
	show_preorder(proctree->root);

	cout << endl << proctree << endl;

	cout << endl;
	delete proctree;	

	return 1;
}
