#pragma once
#include <iostream>	
#include <memory>
#include <string>
#define NUM_CHARS 256

struct TrieNode
{
	TrieNode* children[NUM_CHARS];
	bool terminal;

	TrieNode()
	{
		for (int i = 0; i < NUM_CHARS; i++)
		{
			children[i] = nullptr;

		}
		terminal = false;
	}
};
class Trie {
public:
	Trie()
	{
		root = createNode();
	}
	TrieNode* createNode()
	{
		return new TrieNode();
	}
	bool insert(std::string str)
	{
		if (root == NULL)
			root = createNode();

		TrieNode* tmp = root;
		int length = str.size();

		for (int i = 0; i < length; i++)
		{
			if (tmp->children[str[i]] == NULL)
				tmp->children[str[i]] = createNode();
			tmp = tmp->children[str[i]];
		}

		if (tmp->terminal == true)
			return false;
		else
		{
			tmp->terminal = true;
			return true;
		}
	}
	void print()
	{
		if (root == nullptr)
			return;
		printRec(root, "");
	}
	bool search(std::string str)
	{
		if (root)
		{
			TrieNode* tmp = root;
			for (int i = 0; i < str.size(); i++)
			{
				if (tmp->children[str[i]] == nullptr)
					return false;
				tmp = tmp->children[str[i]];
			}
			return true;
		}
	}
	bool deletestr(std::string str)
	{
		bool deletable = false;

		if (root == nullptr) return false;

		root = deletestr_rec(root, str, deletable);
		return deletable;
	}
	bool node_has_children(TrieNode* node)
	{
		if (node == NULL) return false;
		for (int i = 0; i < NUM_CHARS; i++)
		{
			if (node->children[i] != NULL)
				return true;
		}
		return false;
	}
private:
	TrieNode* root = nullptr;
	void printRec(TrieNode* at, std::string prefix)
	{
		if (at != NULL)
		{
			if (at->terminal)
				std::cout << prefix << "\n";
			for (int i = 0; i < NUM_CHARS; i++)
			{
				if (at->children[i] != NULL)
				{
					std::string newprefix = prefix;
					newprefix.push_back(i);
					printRec(at->children[i], newprefix);
				}
			}

		}
	}
	TrieNode* deletestr_rec(TrieNode*& node, std::string currstr, bool& deletable)
	{
		if (node == NULL) return node;
		if (currstr.size() == 0)
		{
			if (node->terminal)
			{
				node->terminal = false;
				deletable = true;

				if (node_has_children(node) == false)
				{
					delete node;
					node = NULL;
				}
			}
			return node;

		}

		node->children[currstr[0]] = deletestr_rec(node->children[currstr[0]], currstr.substr(1, currstr.size() - 1), deletable);

		if (deletable && node_has_children(node) == false && node->terminal == false) {
			delete node;
			node = NULL;
		}
		return node;

	}
};