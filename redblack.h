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
};

class redblacktree {
	public:
		redblacknode* root;
		void insertnode(int, redblacknode*, int);
		redblacktree();
		~redblacktree();
	private:
		int depth; //helps with drawing
		void balanceinsert(redblacknode*);
		void rightrotate(redblacknode*);
		void leftrotate(redblacknode*);
		void free(redblacknode*);
			
};

#endif
