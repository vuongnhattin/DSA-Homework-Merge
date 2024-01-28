#include <iostream>
#include <queue>

// Enum representing the color of a node in the red-black tree
enum Color {
	RED, BLACK,
};

// Node structure for the red-black tree
typedef struct Node* Ref;
struct Node {
	int key;
	Color color;
	Ref parent, left, right;
};

// Global variable representing the nil (sentinel) node
Ref nil;

// Function to create a new node with given key, color, and nil reference
Ref createNode(int key, Color color, Ref nil) {
	Ref p = new Node{ key, color, nil, nil, nil };
	return p;
}

// Left rotation operation in the red-black tree
void leftRotate(Ref& root, Ref x) {
	Ref y = x->right;
	x->right = y->left;

	if (y->left != nil) {
		y->left->parent = x;
	}
	y->parent = x->parent;

	if (x->parent == nil) {
		root = y;
	}
	else {
		if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
	}

	y->left = x;
	x->parent = y;
}

// Right rotation operation in the red-black tree
void rightRotate(Ref& root, Ref x) {
	Ref y = x->left;
	x->left = y->right;

	if (y->right != nil) {
		y->right->parent = x;
	}
	y->parent = x->parent;

	if (x->parent == nil) {
		root = y;
	}
	else {
		if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
	}

	y->right = x;
	x->parent = y;
}

// Binary Search Tree (BST) insertion operation
void BST_Insert(Ref& root, Ref x) {
	Ref y = nil, z = root;
	while (z != nil) {
		y = z;

		if (x->key < z->key) z = z->left;
		else if (x->key > z->key) z = z->right;
		else return; // Key already exists, do nothing
	}

	x->parent = y;
	if (y == nil) root = x;
	else {
		if (x->key < y->key) y->left = x;
		else y->right = x;
	}
}

// Adjustments after left child insertion in the red-black tree
void insertionLeftAdjust(Ref& root, Ref& x) {
	Ref u = x->parent->parent->right;
	if (u->color == RED) {
		x->parent->color = u->color = BLACK;
		x->parent->parent->color = RED;
		x = x->parent->parent;
	}
	else {
		if (x == x->parent->right) {
			x = x->parent;
			leftRotate(root, x);
		}
		x->parent->color = BLACK;
		x->parent->color = BLACK;
		x->parent->parent->color = RED;
		rightRotate(root, x->parent->parent);
	}
}

// Adjustments after right child insertion in the red-black tree
void insertionRightAdjust(Ref& root, Ref& x) {
	Ref u = x->parent->parent->left;
	if (u->color == RED) {
		x->parent->color = u->color = BLACK;
		x->parent->parent->color = RED;
		x = x->parent->parent;
	}
	else {
		if (x == x->parent->left) {
			x = x->parent;
			rightRotate(root, x);
		}
		x->parent->color = BLACK;
		x->parent->color = BLACK;
		x->parent->parent->color = RED;
		leftRotate(root, x->parent->parent);
	}
}

// Fix-up routine after insertion in the red-black tree
void insertionFixUp(Ref& root, Ref x) {
	while (x->parent->color == RED) {
		if (x->parent == x->parent->parent->left) {
			insertionLeftAdjust(root, x);
		}
		else {
			insertionRightAdjust(root, x);
		}
	}
	root->color = BLACK;
}

// Insert a key into the red-black tree
void Insert(Ref& root, int key) {
	Ref x = createNode(key, RED, nil);
	BST_Insert(root, x);
	insertionFixUp(root, x);
}

// Create a red-black tree from an array of keys
Ref createTree(int a[], int n) {
	Ref root = nil;

	for (int i = 0; i < n; i++) {
		Insert(root, a[i]);
	}

	return root;
}

// Adjustments after left child deletion in the red-black tree
void deleteLeftAdjust(Ref& root, Ref& x) {
	Ref w = x->parent->right;

	if (w->color == RED) {
		w->color = BLACK;
		x->parent->color = RED;
		leftRotate(root, x->parent);
		w = x->parent->right;
	}

	if ((w->right->color == BLACK) && (w->left->color == BLACK)) {
		w->color = RED;
		x = x->parent;
	}
	else {
		if (w->right->color == BLACK) {
			w->left->color = BLACK;
			w->color = RED;
			rightRotate(root, w);
			w = x->parent->right;
		}
		w->color = x->parent->color;
		x->parent->color = w->right->color = BLACK;
		leftRotate(root, x->parent);
		x = root;
	}
}

// Adjustments after right child deletion in the red-black tree
void deleteRightAdjust(Ref& root, Ref& x) {
	Ref w = x->parent->left;

	if (w->color == RED) {
		w->color = BLACK;
		x->parent->color = RED;
		leftRotate(root, x->parent);
		w = x->parent->left;
	}

	if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
		w->color = RED;
		x = x->parent;
	}
	else {
		if (w->left->color == BLACK) {
			w->right->color = BLACK;
			w->color = RED;
			leftRotate(root, w);
			w = x->parent->left;
		}
		w->color = x->parent->color;
		x->parent->color = w->left->color = BLACK;
		rightRotate(root, x->parent);
		x = root;
	}
}

// Fix-up routine after deletion in the red-black tree
void deleteFixUp(Ref root, Ref x) {
	while ((x->color == BLACK) && (x != root)) {
		if (x == x->parent->left) deleteLeftAdjust(root, x);
		else deleteRightAdjust(root, x);
	}
	x->color = BLACK;
}

// Search for a key in the red-black tree and return the corresponding node
Ref lookup(Ref root, int key) {
	Ref p = root;
	while (p != nil) {
		if (key == p->key) return p;

		if (key < p->key) p = p->left;
		else p = p->right;
	}

	return nil;
}

// Find the predecessor of a given node in the red-black tree
Ref findPredecessor(Ref z) {
	if (z->left == nil) {
		std::cout << "This node does not have predecessor!";
		return nullptr;
	}

	Ref y = z->left;
	while (y->right != nil) y = y->right;
	return y;
}

// Remove a key from the red-black tree
void Remove(Ref& root, int k) {
	Ref z = lookup(root, k);
	if (z == nil) return;

	Ref y = (z->left == nil) || (z->right == nil) ? z : findPredecessor(z);

	Ref x = (y->left == nil) ? y->right : y->left;

	x->parent = y->parent;
	if (y->parent == nil) root = x;
	else {
		if (y == y->parent->left) y->parent->left = x;
		else y->parent->right = x;
	}

	if (y != z) z->key = y->key;
	if (y->color == BLACK) {
		deleteFixUp(root, x);
	}

	delete y;
}

// Print the keys of the red-black tree in preorder
void printPreorder(Ref root) {
	if (root == nil) {
		return;
	}

	std::cout << root->key << " ";
	printPreorder(root->left);
	printPreorder(root->right);
}

// Calculate the height of a node in the red-black tree
int Height(Ref root) {
    if (root == nil) {
        return 1;
    } 
    
    return std::max(Height(root->left), Height(root->right)) + 1;
}

// Calculate the black height of a node in the red-black tree
int BlackHeight(Ref root) {
    if (!root) return 0;
    
    int cnt = 0;
    Ref p = root;

    while (1) {
    	if (p->color == BLACK) cnt++;

    	if (p->left != nil) p = p->left;
    	else if (p->right != nil) p = p->right;
    	else break;
    }

    return cnt + 1;
}

// Print the keys of the red-black tree in level order
void printLevelOrder(Ref root) {
	if (!root) return;

	std::queue<Ref> q;
	q.push(root);

	while (!q.empty()) {
		Ref tmp = q.front();
		q.pop();
		if (tmp == nil) std::cout << "nil ";
		else std::cout << tmp->key << " ";

		if (tmp != nil) {
			q.push(tmp->left);
			q.push(tmp->right);
		}
	}
}

// Main function
int main() {
	// Initialize nil as the sentinel node
	nil = new Node{ -1, BLACK, nil, nil, nil };
	Ref root = nil; // Initialize an empty red-black tree

	// Array of keys to insert into the red-black tree
	int a[] = { 4,12,5,6,7,1,3,8,13,14 }, n = sizeof(a) / sizeof(a[0]);

	// Insert keys into the red-black tree
	for (int i = 0; i < n; i++) {
		Insert(root, a[i]);
	}

	Remove(root, 7);
	Remove(root, 6);

	// Print the red-black tree in level order
	printLevelOrder(root);

	// Print the black height of the red-black tree
	std::cout << "\nBlack height: " << BlackHeight(root);	
}
