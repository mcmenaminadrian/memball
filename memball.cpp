#include <iostream>
#include "string.h"
#include "/usr/include/proc/readproc.h"
#include "redblack.hpp"

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

void usage()
{
	cout << "Correct usage is ./memball [--option]" << endl;
	cout << "Options are:" << endl;
	cout << "--gxml		GraphML output (default)" << endl;
	cout << "--tex		LaTeX output" << endl;
	cout << "--plain 	Plain text output" << endl;
}

int main(int argc, char* argv[])
{

	//default is to output GraphML
	// --gxml GRAPHML output
	// --tex TEX output
	// --plain plain text output

	bool gxml = true;
	bool tex = false;
	bool plain = false;
	if (argc > 2) {
		usage();
		return 1;
	}

	if (argc == 2) {
		if (argv[1][0] != '-') {
			usage();
			return 1;
		}

		if (strcmp(argv[1], "-?") == 0) {
			usage();
			return 0;
		}

		if (strcmp(argv[1], "--gxml") == 0)
			;
		else if (strcmp(argv[1], "--tex") == 0) {
			gxml = false;
			tex = true;
		}
		else if (strcmp(argv[1], "--plain") == 0) {
			gxml = false;
			plain = true;	
		} else {
			usage();
			return 1;
		}
	}

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

	if (gxml)
		drawGraphMLtree(proctree->root);
	else if (tex)
		drawTEXtree(proctree->root);
	else
		cout << proctree->root << endl;

	delete proctree;	

	return 1;
}
