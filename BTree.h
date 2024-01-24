#pragma once
#include "BTreeNode.h"

class BTree {
private:
	BTreeNode* root;
	int order;

	BTreeNode* findLeafNode(BTreeNode* node, Nodedata data) {
		if (node->leaf) //base condition
			return node;

		for (int i = 0; i < node->numOfKeys ; i++) {
			if (data.key <= node->keys[i].key) {
				return findLeafNode(node->child[i], data);
			}
		}
		return findLeafNode(node->child[node->numOfKeys], data);
	}
	void recursivePrint(BTreeNode* node) {
		if (node == nullptr)
			return;
		node->print();

		cout << endl << "\t";
		for (int i = 0; i <= node->numOfKeys; i++) {
			recursivePrint(node->child[i]);
		}
		
	}
	void recursiveInOrder(BTreeNode* node) {
		if (!node)
			return;
		recursiveInOrder(node->child[0]);
		for (int i = 0; i < node->numOfKeys; i++) {
			cout <<"\t" << node->keys[i].key << "   " << node->keys[i].path << endl;
			recursiveInOrder(node->child[i + 1]);
		}
	}
	BTreeNode* searchNodeDeletion(BTreeNode* node, Nodedata data) {
		if (!node) //base condition
			return nullptr;

		for (int i = 0; i < node->numOfKeys; i++) {
			if (data.key < node->keys[i].key) {
				return searchNodeDeletion(node->child[i], data);
			}
			else if (data.key == node->keys[i].key)
				return node;
		}
		return searchNodeDeletion(node->child[node->numOfKeys], data);
	}
	void applyLeafDeletionConditions(BTreeNode* node, Nodedata data) {
		if (node == root || node == nullptr)
			return;

		BTreeNode* parentNode = node->parent;
		int minKeys = ((order - 1) / 2 + 1/*seiling of order / 2*/) - 1;
		if (node->numOfKeys < minKeys) {
			int ind = 0;
			while (data.key > parentNode->keys[ind].key && ind < parentNode->numOfKeys)
				ind++;
			if (ind > 0 && parentNode->child[ind - 1]->numOfKeys > minKeys) {
				//borrow from left child
				BTreeNode* left = parentNode->child[ind - 1];
				node->insertkey(parentNode->keys[ind - 1]);
				parentNode->remove(parentNode->keys[ind - 1]);
				parentNode->insertkey(left->keys[left->numOfKeys - 1]);
				left->remove(left->keys[left->numOfKeys - 1]);

				//tansfering children
				for (int i = 0; i < node->numOfKeys; i++) {
					node->child[i + 1] = node->child[i];
				}
				node->child[0] = left->child[left->numOfKeys + 1];
				left->child[left->numOfKeys + 1] = nullptr;

			}
			else if (ind < parentNode->numOfKeys && parentNode->child[ind + 1]->numOfKeys > minKeys) {
				//borrow from right child
				BTreeNode* right = parentNode->child[ind + 1];
				node->insertkey(parentNode->keys[ind]);
				parentNode->remove(parentNode->keys[ind]);
				parentNode->insertkey(right->keys[0]);
				right->remove(right->keys[0]);

				//transfering children
				node->child[node->numOfKeys] = right->child[0];
				for (int i = 0; i <= right->numOfKeys; i++) {
					right->child[i] = right->child[i + 1];
				}
				right->child[right->numOfKeys + 1] = nullptr;

			}
			else {
				//merge both children
				if (ind > 0) {
					//merge with left child
					BTreeNode* left = parentNode->child[ind - 1];
					left->insert(parentNode->keys[ind - 1], node->child[0]);
					for (int i = 0; i < node->numOfKeys; i++) {
						left->insert(node->keys[i], node->child[i + 1]);
					}

					for (int i = ind; i < parentNode->numOfKeys; i++) {
						parentNode->child[i] = parentNode->child[i + 1];
					}
					parentNode->child[parentNode->numOfKeys] = nullptr;
					parentNode->remove(parentNode->keys[ind - 1]);
					delete node;
					node = nullptr;
					if (parentNode == root && root->numOfKeys == 0) {
						left->parent = nullptr;
						root = left;
						parentNode = nullptr;
					}
				}
				else {
					//merge with right child
					BTreeNode* right = parentNode->child[ind + 1];
					right->insertkey(parentNode->keys[ind]);
					for (int i = 0; i < node->numOfKeys; i++) {
						right->insertkey(node->keys[i]);
					}

					//transfer children 
					for (int i = right->numOfKeys /*new number of keys of right after insertion*/; i > node->numOfKeys; i--) {
						right->child[i] = right->child[i - ((node->numOfKeys + 1)/*=num of children to be transfered)*/)];
					}
					for (int i = node->numOfKeys; i >= 0; i--) {
						right->child[i] = node->child[i];
					}

					for (int i = ind; i < parentNode->numOfKeys; i++) {
						parentNode->child[i] = parentNode->child[i + 1];
					}
					parentNode->child[parentNode->numOfKeys] = nullptr;
					parentNode->remove(parentNode->keys[ind - 1]);
					delete node;
					node = nullptr;
					if (parentNode == root && root->numOfKeys == 0) {
						right->parent = nullptr;
						root = right;
						parentNode = nullptr;
					}
				}
				applyLeafDeletionConditions(parentNode, data);
			}
		}
	}
	BTreeNode* predecessorNode(BTreeNode* node, Nodedata data) {
		int ind = 0;
		while (data.key > node->keys[ind].key) // breaks when key = number
			ind++;
		node = node->child[ind];
		while (node->child[node->numOfKeys])
			node = node->child[node->numOfKeys];

		return node;
	}
	BTreeNode* inOrderSuccesorNode(BTreeNode* node, Nodedata data) {
		int ind = 0;
		while (data.key > node->keys[ind].key) // breaks when key = number
			ind++;

		node = node->child[ind + 1];
		while (node->child[0])
			node = node->child[0];

		return node;

	}
	void internalDeletion(BTreeNode* node, Nodedata data) {
		int minKeys = ((order - 1) / 2 + 1/*seiling of order / 2*/) - 1;

		BTreeNode* predecessor = predecessorNode(node, data);


		Nodedata numberReplaced = predecessor->keys[predecessor->numOfKeys - 1];
		node->insertkey(numberReplaced);
		predecessor->remove(numberReplaced);


		applyLeafDeletionConditions(predecessor, numberReplaced);
	}

public:
	BTree(int ord = 5) {
		root = nullptr;
		order = ord;
	}
	void insert(Nodedata data) {
		if (!root) {
			root = new BTreeNode  (order);
			root->insert(data);
			return;
		}
		BTreeNode* temp = findLeafNode(root, data);
		temp->insert(data);
		while (temp->isFull()) {
			if (temp == root) {
				root = new BTreeNode(order);
				root->insert(temp->keys[(order - 1) / 2]);
				root->child[0] = new BTreeNode(order, root);
				root->child[1] = new BTreeNode(order, root);

				if (temp->child[0]) {
					root->child[0]->child[0] = temp->child[0];
					root->child[1]->child[0] = temp->child[(order - 1) / 2 + 1];

					temp->child[0]->parent = root->child[0];
					temp->child[((order - 1) / 2 + 1)]->parent = root->child[1];
				}
				for (int i = 0; i < (order - 1) / 2; i++) {

					root->child[0]->insert(temp->keys[i], temp->child[i + 1]);
					if (temp->child[i + 1])
						temp->child[i + 1]->parent = root->child[0];
				}
				for (int i = 1 + (order - 1) / 2; i < order; i++) {
					root->child[1]->insert(temp->keys[i], temp->child[i + 1]);
					if (temp->child[i + 1])
						temp->child[i + 1]->parent = root->child[1];
				}
				root->leaf = false;
				delete temp;
				return;
			}
			Nodedata middle = temp->keys[(order - 1) / 2];
			temp->parent->insert(middle); // insert middle to parent

			BTreeNode* left = new BTreeNode(order, temp->parent);
			BTreeNode* right = new BTreeNode(order, temp->parent);


			//spliting node in two children
			left->child[0] = temp->child[0];
			if (left->child[0]) {
				left->child[0]->parent = left;
			}
			for (int i = 0; i < (order - 1) / 2; i++) {
				left->insert(temp->keys[i], temp->child[i+1]);
				if (temp->child[i + 1])
					left->child[i + 1]->parent = left;
			}
			right->child[0] = temp->child[(order - 1) / 2 + 1];
			if (right->child[0]) {
				right->child[0]->parent = right;
			}
			for (int i = (order - 1) / 2 + 1, j = 1; i < order; i++, j++) {
				right->insert(temp->keys[i], temp->child[i+1]);
				if (right->child[j])
					right->child[j]->parent = right;
			}

			BTreeNode* temp2 = temp;
			temp = temp->parent;
			delete temp2;
			temp2 = nullptr;
			for (int i = 0; i < order-1; i++) {
				if(temp->keys[i].key == middle.key){
					temp->child[i] = left;
					temp->child[i + 1] = right;
					break;
				}
			}
		}
	}
	bool deletion(Nodedata data) {
		BTreeNode* temp = searchNodeDeletion(root, data);
		
		if (!temp) {
			cout << "\n	File Does Not Exist in Current Machine.\n";
			return false;
		}

		temp->remove(data);
		if (temp->leaf) {
			applyLeafDeletionConditions(temp, data);
		}
		else {
			internalDeletion(temp, data);
		}
		cout << "\n\tDeleted";
		return true;
	}


	void print() {
		cout << "\n	Pre order Traversal: \n";
		recursivePrint(root);
	}
	void inOrder() {
		cout << "In order Traversal: \n";
		recursiveInOrder(root);
	}

	void transferFrom(BTree& tree, string lower, string upper) {
		transferFromRecursive(tree.root, tree, lower, upper);
	}
	void transferFromRecursive(BTreeNode* node, BTree& tree, string lower, string upper) {
		if (!node)
			return;

		transferFromRecursive(node->child[0], tree, lower, upper);
		for (int i = 0; i < node->numOfKeys; i++) {
			if (node->keys[i].key <= lower || node->keys[i].key > upper) {
				this->insert(node->keys[i]);
				tree.deletion(node->keys[i]);
			}
			transferFromRecursive(node->child[i + 1], tree, lower, upper);
		}

	}
};