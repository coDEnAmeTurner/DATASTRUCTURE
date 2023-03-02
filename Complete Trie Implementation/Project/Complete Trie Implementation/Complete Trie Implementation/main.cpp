#include <iostream>
#include "header/Trie.h"
using namespace std;

int main()
{
	Trie trie;

	trie.insert("CAT");
	trie.insert("CATTLE");
	trie.insert("JOKE");
	trie.insert("KIT");
	trie.insert("KIN");

	cout << trie.deletestr("JOKE") << "\n";
	trie.print();
	cin.get();
}