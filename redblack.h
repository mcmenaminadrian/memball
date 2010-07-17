#ifndef __REDBLACKTREE__
#define __REDBLACKTREE__

class redblacknode {

	public:
		int colour;
		int value;
		redblacknode* up;
		redblacknode* left;
		redblacknode* right;
		redblacknode(const int);
		redblacknode* grandparent() const;
		redblacknode* uncle() const;
		redblacknode* sibling() const;
		bool bothchildrenblack() const;
};

class redblacktree {
	public:
		redblacknode* root;
		void insertnode(int, redblacknode*, int d = 1);
		bool removenode(int);
		redblacktree();
		~redblacktree();
	private:
		int depth; //helps with drawing
		void balanceinsert(redblacknode*);
		void rotate2(redblacknode*);
		void rotate1(redblacknode*);
		void free(redblacknode*);
		redblacknode* maxleft(redblacknode*);
		redblacknode* minright(redblacknode*);
		redblacknode* locatenode(int, redblacknode*);
};

#endif
