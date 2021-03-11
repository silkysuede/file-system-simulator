#include <iostream>
#include <memory>
#include <vector>
#include "FSNode.h"
#include "FilesystemTree.h"

// All functions below used for FSNode class testing
void FSNodeTester();
void displayChildNodes(std::shared_ptr<FSNode> childPtr);
int countAllNodes(std::shared_ptr<FSNode> childPtr);
int countDirectories(std::shared_ptr<FSNode> childPtr);
int countFiles(std::shared_ptr<FSNode> childPtr);
void FSTMoveTest(std::shared_ptr<FilesystemTree> treePtr);
void FSTCopyTest(std::shared_ptr<FilesystemTree> treePtr);

// All functions below used for FilesystemTree class testing
void FSTInitialTest(std::shared_ptr<FilesystemTree> treePtr);
void FSTRemoveTest(std::shared_ptr<FilesystemTree> treePtr);
void FSTFindTest(std::shared_ptr<FilesystemTree> treePtr);
void FSTCCTest(std::shared_ptr<FilesystemTree> treePtr);

int main()
{
	// Uncomment below to test the FSNode class.
	//FSNodeTester();

	/* Uncomment below to test the FilesystemTree class. Note that these
	* test functions are specifically written to work in the order they're
	* listed. So you should complete the required funcitons and uncomment the
	* test functions from top to bottom.
	*/

	// Line below requires create() to be complete.
	std::shared_ptr<FilesystemTree> treePtr = std::make_shared<FilesystemTree>("input.txt");

	// Tests displayTree() and displayStats().
	FSTInitialTest(treePtr);
	
	// Tests remove().
	FSTRemoveTest(treePtr);

	// Tests move().
	FSTMoveTest(treePtr);
	
	// Tests copy() which requires copySubTree().
	FSTCopyTest(treePtr);

	// Tests find().
	FSTFindTest(treePtr);

	// Tests copy constructor and overloaded = and displays final stats. These
	// require copySubTree().
	FSTCCTest(treePtr);
}

void FSTCCTest(std::shared_ptr<FilesystemTree> treePtr)
{
	// Test copy constructor (which uses overloaded =)
	std::shared_ptr<FilesystemTree> aTreePtr 
		= std::make_shared<FilesystemTree>(*treePtr);

	std::cout << "** FINAL TREE **" << std::endl << std::endl;
	aTreePtr->displayTree(std::cout);

	std::cout << std::endl << "** FINAL TREE STATS **" << std::endl
		<< std::endl << "Should be 8 directories, 21 files:" << std::endl;
	aTreePtr->displayStats(std::cout);
}

void FSTFindTest(std::shared_ptr<FilesystemTree> treePtr)
{
	
	std::cout << std::endl << "** TESTING FIND() **" << std::endl << std::endl;
	std::cout << "Searching for dir6 starting at " << ROOT_NAME << "" << std::endl;
	std::cout << treePtr->find("dir6", ROOT_NAME, std::cout) << " matches found. [should be 2]"
		<< std::endl << std::endl;
	std::cout << "Searching for file13 starting at " << ROOT_NAME << "/dir1/dir2/dir8: "
		<< std::endl;
	std::cout << treePtr->find("file13", ROOT_NAME + "/dir1/dir2/dir8", std::cout) << " matches found. [should be 1]"
		<< std::endl << std::endl;
	
	std::cout << "Searching for file22 starting at " << ROOT_NAME << "" << std::endl;
	std::cout << treePtr->find("file22", ROOT_NAME, std::cout) << " matches found. [should be 4]"
		<< std::endl << std::endl;
	
	std::cout << "Searching for file99 starting at " << ROOT_NAME << "" << std::endl;
	std::cout << treePtr->find("file99", ROOT_NAME, std::cout) << " matches found. [should be 0]"
		<< std::endl << std::endl;
	
}

void FSTInitialTest(std::shared_ptr<FilesystemTree> treePtr)
{
	std::cout << "** INITIAL TREE LOADED FROM FILE **" << std::endl << std::endl;
	treePtr->displayTree(std::cout);

	std::cout << std::endl << "** INITIAL TREE STATS **" << std::endl
		<< std::endl << "Should be 10 directories, 25 files:" << std::endl;
	treePtr->displayStats(std::cout);
}

void FSTRemoveTest(std::shared_ptr<FilesystemTree> treePtr)
{
	std::cout << std::endl << "** TESTING REMOVE() **" << std::endl << std::endl;

	std::cout << "Remove file17 from " << ROOT_NAME << "/dir1/dir2/dir8: "
		<< treePtr->remove("file17", ROOT_NAME + "/dir1/dir2/dir8")
		<< " [should be 1]" << std::endl;
	std::cout << "Remove dir3 from " << ROOT_NAME << "/dir1/dir2: "
		<< treePtr->remove("dir3", ROOT_NAME + "/dir1/dir2")
		<< " [should be 1]" << std::endl;
	std::cout << "Remove dir3 from " << ROOT_NAME << "/dir1/dir2: "
		<< treePtr->remove("dir3", ROOT_NAME + "/dir1/dir2")
		<< " [should be 0]" << std::endl;
	std::cout << "Remove " << ROOT_NAME << ": "
		<< treePtr->remove(ROOT_NAME, "")
		<< " [should be 0]" << std::endl;
}

void FSTMoveTest(std::shared_ptr<FilesystemTree> treePtr)
{
	std::cout << std::endl << "** TESTING MOVE() **" << std::endl << std::endl;
	std::cout << "Move dir6 from " << ROOT_NAME << " to " << ROOT_NAME 
		<< "/dir1/dir2: "
		<< treePtr->move("dir6", ROOT_NAME, ROOT_NAME + "/dir1/dir2")
		<< " [should be 1]" << std::endl;
	std::cout << "Move file5 from " << ROOT_NAME << "/dir1/dir2 to " 
		<< ROOT_NAME << "/dir1: "
		<< treePtr->move("file5", ROOT_NAME + "/dir1/dir2", ROOT_NAME + "/dir1")
		<< " [should be 1]" << std::endl;
	std::cout << "Move file5 from " << ROOT_NAME << "/dir1 to " << ROOT_NAME 
		<< "/dir1: "
		<< treePtr->move("file5", ROOT_NAME + "/dir1", ROOT_NAME + "/dir1")
		<< " [should be 0]" << std::endl;
	std::cout << "Move dir99 from " << ROOT_NAME << "/dir1/dir2/dir6 to " 
		<< ROOT_NAME << "/dir1: "
		<< treePtr->move("dir99", ROOT_NAME + "/dir1/dir2/dir6", ROOT_NAME + "/dir1")
		<< " [should be 0]" << std::endl;
	std::cout << "Move file15 from " << ROOT_NAME << "/dir1/dir7 to " 
		<< ROOT_NAME << "/dir6: "
		<< treePtr->move("file15", ROOT_NAME + "/dir1/dir7", ROOT_NAME + "/dir6")
		<< " [should be 0]" << std::endl;
}

void FSTCopyTest(std::shared_ptr<FilesystemTree> treePtr)
{
	std::cout << std::endl << "** TESTING COPY() **" << std::endl << std::endl;

	std::cout << "Copy file15 from " << ROOT_NAME << "/dir1/dir7 to " 
		<< ROOT_NAME << "/dir1/dir2/dir6: "
		<< treePtr->copy("file15", ROOT_NAME + "/dir1/dir7", 
			ROOT_NAME + "/dir1/dir2/dir6")
		<< " [should be 1]" << std::endl;
	
	std::cout << "Copy file15 from " << ROOT_NAME << "/dir1/dir7 to " 
		<< ROOT_NAME << "/dir1/dir2/dir6: "
		<< treePtr->copy("file15", ROOT_NAME + "/dir1/dir7", 
			ROOT_NAME + "/dir1/dir2/dir6")
		<< " [should be 0]" << std::endl;
	std::cout << "Copy dir2 from " << ROOT_NAME << "/dir1 to " << ROOT_NAME 
		<< "/dir1/dir2/dir8: "
		<< treePtr->copy("dir2", ROOT_NAME + "/dir1", 
			ROOT_NAME + "/dir1/dir2/dir8")
		<< " [should be 1]" << std::endl;
	std::cout << "Copy file99 from " << ROOT_NAME << " to " << ROOT_NAME 
		<< "/dir1: "
		<< treePtr->copy("file99", ROOT_NAME, ROOT_NAME + "/dir1")
		<< " [should be 0]" << std::endl;

	std::cout << "Copy file3 from " << ROOT_NAME << "/dir1 to " << ROOT_NAME 
		<< "/dir1: "
		<< treePtr->copy("file3", ROOT_NAME + "/dir1", ROOT_NAME + "/dir99")
		<< " [should be 0]" << std::endl;

}

void FSNodeTester()
{
	// Make some file nodes
	std::vector<std::shared_ptr<FSNode>> nodeVect;

	// Make some directory nodes
	nodeVect.push_back(std::make_shared<FSNode>("dir1", 1));
	nodeVect.push_back(std::make_shared<FSNode>("dir2", 1));
	nodeVect[0]->addChild(nodeVect[1]);
	nodeVect.push_back(std::make_shared<FSNode>("dir3", 1));
	nodeVect[1]->addChild(nodeVect[2]);
	nodeVect.push_back(std::make_shared<FSNode>("dir4", 1));
	nodeVect[0]->addChild(nodeVect[3]);
	nodeVect.push_back(std::make_shared<FSNode>("dir5", 1));
	nodeVect[2]->addChild(nodeVect[4]);

	// Make some file nodes
	for (int i = 0; i < 20; i++)
	{
		nodeVect.push_back(std::make_shared<FSNode>("file" + std::to_string(i), 0));
	}

	// Place the files in the folders to creat a basic tree structure
	for (int i = 5; i < 10; i++)
	{
		nodeVect[0]->addChild(nodeVect[i]);
		nodeVect[1]->addChild(nodeVect[i + 5]);
		nodeVect[2]->addChild(nodeVect[i + 10]);
		nodeVect[3]->addChild(nodeVect[i + 15]);
	}

	// display various counts
	std::cout << "File count: " << countFiles(nodeVect[0]) << " [should be 20]"
		<< std::endl << std::endl << "Directory count: "
		<< countDirectories(nodeVect[0]) << " [should be 5]" << std::endl
		<< std::endl << "Total node count: " << countAllNodes(nodeVect[0])
		<< " [should be 25]" << std::endl << std::endl;

	// try to add a child to a file
	std::cout << "Trying to add a child to "
		<< nodeVect[6]->getName() << ": "
		<< nodeVect[6]->addChild(std::make_shared<FSNode>("file0", 0))
		<< " [should be 0]" << std::endl << std::endl;

	// try to add file with duplicate name in same directory
	std::cout << "Trying to add duplicate file named file0 to "
		<< nodeVect[0]->getName() << ": "
		<< nodeVect[0]->addChild(std::make_shared<FSNode>("file0", 0))
		<< " [should be 0]" << std::endl << std::endl;

	// Try to display a non-existant child
	std::cout << "Trying to get child at index 10 from "
		<< nodeVect[0]->getName() << ": ";
	if (nodeVect[0]->getChild(10) == nullptr)
	{
		std::cout << "not found";
	}
	else
	{
		std::cout << nodeVect[0]->getChild(10)->getName();
	}

	std::cout << " [should be not found]" << std::endl << std::endl;

	std::cout << "Trying to get child fake from "
		<< nodeVect[8]->getName() << ": ";
	if (nodeVect[8]->getChild("fake") == nullptr)
	{
		std::cout << "not found";
	}
	else
	{
		std::cout << nodeVect[8]->getChild(10)->getName();
	}

	std::cout << " [should be not found]" << std::endl << std::endl;


	// Try to display a existing child
	std::cout << "Trying to get child at index 2 from "
		<< nodeVect[0]->getName() << ": ";
	if (nodeVect[0]->getChild(2) == nullptr)
	{
		std::cout << "not found";
	}
	else
	{
		std::cout << nodeVect[0]->getChild(2)->getName();
	}

	std::cout << " [should be file0]" << std::endl << std::endl;

	std::cout << "Trying to get child file1 from "
		<< nodeVect[0]->getName() << ": ";
	if (nodeVect[0]->getChild("file1") == nullptr)
	{
		std::cout << "not found";
	}
	else
	{
		std::cout << nodeVect[0]->getChild("file1")->getName();
	}

	std::cout << " [should be file1]" << std::endl << std::endl;

	// Display the children of all directories starting at the "root"
	std::cout << "**ALL NODES BEFOER ALTERATION**" << std::endl;
	displayChildNodes(nodeVect[0]);


	// Test removing children
	std::cout << std::endl << "Attempting to remove file0 from "
		<< nodeVect[6]->getName()
		<< ": " << nodeVect[6]->removeChild("file0") << " [should be 0]"
		<< std::endl;

	std::cout << std::endl << "Attempting to remove file0 from "
		<< nodeVect[0]->getName()
		<< ": " << nodeVect[0]->removeChild("file0") << " [should be 1]"
		<< std::endl << std::endl;

	std::cout << "Attempting to remove file0 from "
		<< nodeVect[0]->getName()
		<< ": " << nodeVect[0]->removeChild("file0") << " [should be 0]"
		<< std::endl << std::endl;

	std::cout << "Attempting to remove dir3 from "
		<< nodeVect[1]->getName()
		<< ": " << nodeVect[1]->removeChild("dir3") << " [should be 1]"
		<< std::endl << std::endl;

	std::cout << "Attempting to remove fake from "
		<< nodeVect[4]->getName()
		<< ": " << nodeVect[4]->removeChild("fake") << " [should be 0]"
		<< std::endl << std::endl;

	// Display the children of each directory
	std::cout << "**ALL NODES AFTER REMOVAL**" << std::endl;
	displayChildNodes(nodeVect[0]);

	// display various counts
	std::cout << std::endl
		<< "File count: " << countFiles(nodeVect[0]) << " [should be 14]"
		<< std::endl << std::endl << "Directory count: "
		<< countDirectories(nodeVect[0]) << " [should be 3]" << std::endl
		<< std::endl << "Total node count: " << countAllNodes(nodeVect[0])
		<< " [should be 17]" << std::endl << std::endl;
}

void displayChildNodes(std::shared_ptr<FSNode> childPtr)
{
	if (childPtr->isDirectory())
	{

		std::cout << "Directory " << childPtr->getName() << " contains: " 
			<< std::endl;

		if (childPtr->getNumChildren() == 0)
		{
			std::cout << "\tEmpty" << std::endl;
		}
		else
		{
			// display all contents of this directory
			for (int i = 0; i < childPtr->getNumChildren(); i++)
			{

				std::cout << "\t" << childPtr->getChild(i)->getName();
				if (childPtr->getChild(i)->isDirectory())
				{
					std::cout << " [dir]" << std::endl;
				}
				else
				{
					std::cout << " [file]" << std::endl;
				}
			}

			// recurse to display all subdirectory contents
			for (int i = 0; i < childPtr->getNumChildren(); i++)
			{

				if (childPtr->getChild(i)->isDirectory())
					displayChildNodes(childPtr->getChild(i));
			}
		}

	}

}

int countAllNodes(std::shared_ptr<FSNode> childPtr)
{
	int count = 1;  // 1 for this node

	// base case
	if (childPtr->isDirectory())
	{
		for (int i = 0; i < childPtr->getNumChildren(); i++)
		{
			count += countAllNodes(childPtr->getChild(i));
		}
	}

	return count;
}

int countDirectories(std::shared_ptr<FSNode> childPtr)
{
	int count = 0;

	if (childPtr->isDirectory())
	{
		count++;  // count myself

		for (int i = 0; i < childPtr->getNumChildren(); i++)
		{
			count += countDirectories(childPtr->getChild(i));
		}
	}

	return count;
}

int countFiles(std::shared_ptr<FSNode> childPtr)
{
	int count = 0;

	if (childPtr->isFile())
	{
		count++;  // count myself
	}
	else
	{
		for (int i = 0; i < childPtr->getNumChildren(); i++)
		{
			count += countFiles(childPtr->getChild(i));
		}
	}

	return count;
}