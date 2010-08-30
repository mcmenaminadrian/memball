#include <iostream>
#include <fstream>
#include <string>
#include "string.h"
#include <stdlib.h>
#include "/usr/include/proc/readproc.h"
#define ADDITIONAL_INFO
#include "redblack.hpp"
#undef ADDITIONAL_INFO

using namespace std;

// Copyright Adrian McMenamin, 2010
// Licensed under the GNU GPL v2
// Or any later version at your discretion

static int pagesize = 4096;

class procholder {
	friend ostream& operator<<(ostream&, procholder&);

	public:
		procholder(){}; // needed for template code to work
		procholder(unsigned long long);
		void setadditional(char*);
		const string getadditional() const;
		bool operator==(procholder&) const;
		bool operator<(procholder&) const;
	private:
		unsigned long long vhold;
		string cmd;
};

ostream& operator<<(ostream& os, procholder& p)
{
	os << p.vhold;
	return os;
}

procholder::procholder(unsigned long long v)
{
	vhold = v;
}

void procholder::setadditional(char* str)
{
	cmd = string(str);
}

const string procholder::getadditional() const
{
	return cmd;
}

bool procholder::operator==(procholder& ph) const
{
	return (vhold == ph.vhold);
}

bool procholder::operator<(procholder& ph) const
{
	return (vhold < ph.vhold);
}

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
	cout << "Memball is copyright(c) Adrian McMenamin, 2010" << endl;
	cout << "And is licensed for use under version 2+ of the GPL" << endl;
	cout << "Correct usage is ./memball [--option] [--dataopts]";
	cout << " [-f filename]" << endl;
	cout << "Options are:" << endl;
	cout << "--gxml		GraphML output (default)" << endl;
	cout << "--tex		LaTeX output" << endl;
	cout << "--plain 	Plain text output" << endl;
	cout << "Data options are:" << endl;
	cout << "--cpu0		CPU time (jiffies) including zero" << endl;
	cout << "--cpu		CPU time (jiffies) excluding zero" << endl;
	cout << "--mem		Allocated memory (default)" << endl;
	cout << "--virt		Virtual memory allocated" << endl;
	cout << "--share	Shared memory allocated" << endl;
	cout << "Additional coptions are:" << endl;
	cout << "--cmd		Process name" << endl;
}

int main(int argc, char* argv[])
{
	bool gxml = true;
	bool tex = false;
	bool plain = false;
	bool fileout = false;
	bool procmem = true;
	bool virtsize = false;
	bool sharesize = false;
	bool cputime = false;
	bool cputime0 = false;
	bool cmdline = false;
	string filename;

	for (int z = 1; z < argc; z++)
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

		if (strcmp(argv[z], "--cpu0") == 0) {
			cputime0 = true;
			cputime = false;
			procmem = false;
			virtsize = false;
			sharesize = false;
			continue;
		}

		if (strcmp(argv[z], "--cpu") == 0) {
			cputime = true;
			cputime0 = false;
			procmem = false;
			virtsize = false;
			sharesize = false;
			continue;
		}

		if (strcmp(argv[z], "--mem") == 0) {
			cputime = false;
			cputime0 = false;
			procmem = true;
			virtsize = false;
			sharesize = false;
			continue;
		}

		if (strcmp(argv[z], "--share") == 0) {
			cputime = false;
			cputime0 = false;
			procmem = false;
			virtsize = false;
			sharesize = true;
			continue;
		}

		if (strcmp(argv[z], "--virt") == 0) {
			cputime = false;
			cputime0 = false;
			procmem = false;
			virtsize = true;
			sharesize = false;
			continue;
		}

		if (strcmp(argv[z], "--cmd") == 0) {
			cmdline = true;
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
	redblacktree<redblacknode<procholder> >* proctree;
	proctree = new redblacktree<redblacknode<procholder> >();

	PROCTAB* ptab = openproc(PROC_FILLMEM|PROC_FILLSTAT|PROC_FILLSTATUS);
	proc_t* proc_details;

	while (proc_details = readproc(ptab, NULL))
	{
		redblacknode<procholder>* rbnp = NULL;
		procholder ph;

		if (sharesize && proc_details->share)
			ph = procholder(proc_details->share);

		if (virtsize && proc_details->size)
			ph = procholder(proc_details->size);

		if (procmem && proc_details->resident)
			ph = procholder(proc_details->resident);

		if (cputime0)
			ph = procholder(proc_details->utime +
				proc_details->stime);

		if (cputime && (proc_details->utime || proc_details->stime))
			ph = procholder(proc_details->utime +
				proc_details->stime);

		if (cmdline)
			ph.setadditional(proc_details->cmd);

		rbnp = new redblacknode<procholder>(ph);
		proctree->insertnode(rbnp, proctree->root);

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
