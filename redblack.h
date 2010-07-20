#ifndef __REDBLACKTREE__
#define __REDBLACKTREE__

using namespace std;



template <typename T>
class redblacknode{

template <typename Z> friend ostream& operator<<(ostream& os, const redblacknode<Z>& rbt);
template <typename Z> friend ostream& operator<<(ostream& os, redblacknode<Z>* rbtp);

	public:
		T value;
		int colour;
		redblacknode* up;
		redblacknode* left;
		redblacknode* right;
		redblacknode(const T& v);
		redblacknode* grandparent() const;
 		redblacknode* uncle() const;
		redblacknode* sibling() const;
		bool bothchildrenblack() const;
		bool equals(redblacknode*) const;
		bool lessthan(redblacknode*) const;
		void assign(redblacknode*);
};

template <typename NODE>
class redblacktree {
	public:
		NODE* root;
		void insertnode(NODE*, NODE*, int d = 1);
		bool removenode(NODE&);
		redblacktree();
		~redblacktree();
	private:
		int depth; //helps with drawing
		void balanceinsert(NODE*);
		void rotate2(NODE*);
		void rotate1(NODE*);
		void transform1(NODE*);
		void transform2(NODE*);
		void free(NODE*);
		NODE* maxleft(NODE*);
		NODE* minright(NODE*);
		NODE* locatenode(NODE*, NODE*);
};


#endif
