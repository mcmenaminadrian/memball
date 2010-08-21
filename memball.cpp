#include <iostream>
#include <fstream>
#include "string.h"
#include <stdlib.h>
#include "/usr/include/proc/readproc.h"
#include "redblack.hpp"

using namespace std;

// Copyright Adrian McMenamin, 2010
// Licensed under the GNU GPL v2
// Or any later version at your discretion

static int pagesize = 4096;

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
	cout << "Correct usage is ./memball [--option] [--dataopts]";
	cout << " [-f filename]" << endl;
	cout << "Options are:" << endl;
	cout << "--gxml		GraphML output (default)" << endl;
	cout << "--tex		LaTeX output" << endl;
	cout << "--plain 	Plain text output" << endl;
	cout << "Data options are:" << endl;
	cout << "--cpu		CPU time" << endl;
	cout << "--mem		Allocated memory (default)" << endl;
	cout << "--virt		Virtual memory allocated" << endl;
	cout << "--share	Shared memory allocated" << endl;
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
	bool fileout = false;
	bool procmem = true;
	bool virtsize = false;
	bool sharesize = false;
	bool cputime = false;
	string filename;

	int z;
	for (z = 1; z < argc; z++)
	{
		if (argv[z][0] != '-') {
			usage();
			return 1;
		}

		if (strcmp(argv[z], "-?") == 0) {
			usage();
			return 0;
		}

		if (strcmp(argv[z], "--gxml") == 0) {
			gxml = true;
			tex = false;
			plain = false;
			continue;
		}
			
		if (strcmp(argv[z], "--tex") == 0) {
			gxml = false;
			plain = false;
			tex = true;
			continue;
		}

		if (strcmp(argv[z], "--plain") == 0) {
			gxml = false;
			plain = true;
			gxml = false;
			continue;	
		
		}

		if (strcmp(argv[z], "--cpu") == 0) {
			cputime = true;
			procmem = false;
			virtsize = false;
			sharesize = false;
			continue;
		}

		if (strcmp(argv[z], "--mem") == 0) {
			cputime = false;
			procmem = true;
			virtsize = false;
			sharesize = false;
			continue;
		}

		if (strcmp(argv[z], "--share") == 0) {
			cputime = false;
			procmem = false;
			virtsize = false;
			sharesize = true;
			continue;
		}

		if (strcmp(argv[z], "--virt") == 0) {
			cputime = false;
			procmem = false;
			virtsize = true;
			sharesize = false;
			continue;
		}
		
		if (strcmp(argv[z], "-f") == 0) {
			fileout = true;
			if (z + 2 != argc) {
				usage();
				cout << z << "Specified file name must be last"
					<< endl;
				return 1;
			}
			filename = argv[z + 1];
			break;
		}
			
	}

	pagesize = getpagesize();
	redblacktree< redblacknode<unsigned long long> >* proctree;
	proctree = new redblacktree<redblacknode<unsigned long long> >();

	PROCTAB* ptab = openproc(PROC_FILLMEM|PROC_FILLSTAT);
	proc_t* proc_details;

	while (proc_details = readproc(ptab, NULL))
	{
		//ignore 0 memory but not 0 cpu time
		if (sharesize && proc_details->share) {
			proctree->insertnode(new
			redblacknode<unsigned long long>(proc_details->share),
			proctree->root);
			free(proc_details);
			continue;
		}

		if (virtsize && proc_details->size) {
			proctree->insertnode(new
			redblacknode<unsigned long long>(proc_details->size),
			proctree->root);
			free(proc_details);
			continue;
		}

		if (procmem && proc_details->resident) {
			proctree->insertnode(new
			redblacknode<unsigned long long>(proc_details->resident), 
			proctree->root);
			free(proc_details);
			continue;
		}

		if (cputime) {  
			proctree->
				insertnode(new
				redblacknode<unsigned long long>(
				proc_details->utime +
				proc_details->stime),
				proctree->root);
			free(proc_details);
			continue;
			
		}
		free(proc_details);
		
	}

	closeproc(ptab);
	

	if (fileout == true) {
		ofstream outstream(filename.c_str());
		if (gxml)
			drawGraphMLtree(proctree->root, outstream);
		else if (tex)
			drawTEXtree(proctree->root, outstream);
		else
			outstream << proctree->root << endl;
		outstream.close();	
	}
	else {
		if (gxml)
			drawGraphMLtree(proctree->root, cout);
		else if (tex)
			drawTEXtree(proctree->root, cout);
		else
			cout << proctree->root << endl;
	}	

	delete proctree;	

	return 0;
}
