struct Tree{
		void Traverse(TreeNode* start=mRoot){
			// ... DFS traversal from root
		}

		void SetRoot(TreeNode* t){
			// ...
		}

		TreeNode* mRoot;
}

struct TreeNode{
	// Store points to child nodes
	string 		 mNodeName;
	// Store child nodes
	TreeNode[] mChildren;
}
