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


template <typename R> void show_preorder(R* node)
{
	if (node == NULL)
		return;
	cout << node << ",";
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
	redblacktree< redblacknode<int> >* proctree;
	proctree = new redblacktree<redblacknode<int> >();

	PROCTAB* ptab = openproc(PROC_FILLMEM);
	proc_t* proc_details;

	while (proc_details = readproc(ptab, NULL))
	{
		if (proc_details->resident) {
			redblacknode<int>* x = new redblacknode<int>(proc_details->resident);
			proctree->insertnode(x, proctree->root);
		}
	}

	closeproc(ptab);
	redblacknode<int>* top = proctree->root;
	do {
		proctree->root->showinorder(proctree->root);
		cout << endl << endl << proctree->root << endl;
		redblacknode<int> t(proctree->min());
		cout << "deleting min" << endl;
		proctree->removenode(t);
		if (proctree->blackheightleft() != proctree->blackheightright()) {
			cout << endl << "left height is " << proctree->blackheightleft() << " right height is " << proctree->blackheightright() << endl;
			cout << proctree->root << endl;
			return 1;
		}
	} while (proctree->root);
	

	delete proctree;	

	return 1;
}
