#include "FilesystemTree.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <algorithm>


bool FilesystemTree::create(const std::string& pName, int pType,
	const std::string& parentPath)
{
	bool rValue = false;
	if (pName == rootPtr->getName())
	{
		rValue = false;
	}
	auto newNodePtr = std::make_shared<FSNode>(pName, pType);
	std::shared_ptr<FSNode> parentPtr;
	parentPtr = pathToPointer(parentPath);
	for (int i = 0; i < parentPtr->getNumChildren(); i++)
	{
		if (parentPtr->getChild(i)->getName() == pName)
		{
			rValue = false;
		}
	}
	
	if (parentPtr->addChild(newNodePtr))
	{
		rValue = true;
	}
	
	return rValue;
}

std::shared_ptr<FSNode> FilesystemTree::copySubTree(std::shared_ptr<FSNode> rootPtr) const
{
	FilesystemTree newTree;
	if (rootPtr->getName() == ROOT_NAME)
	{
		auto rPtr = std::make_shared<FSNode>("", 1);
		rPtr->name = ROOT_NAME;
		newTree.rootPtr = rPtr;
		newTree.rootPtr->addChild(copySubTreeHelper(rootPtr));
	}
	else
	{
		newTree.rootPtr = copySubTreeHelper(rootPtr);
	}
	return newTree.rootPtr;
	
}


std::shared_ptr<FSNode> FilesystemTree::copySubTreeHelper(std::shared_ptr<FSNode> rootPtr) const
{
	std::shared_ptr<FSNode> newNode = rootPtr;
	std::shared_ptr<FSNode> childToCopy;
	std::shared_ptr<FSNode> copyNode;
	int childCount = 0;
	int loop = 0;

	while (newNode->getNumChildren() != childCount)
	{
		childToCopy = newNode->getChild(loop);

		if (childToCopy->getType() == 0)
		{
			childCount++;
			auto copyNode = std::make_shared<FSNode>(childToCopy->getName(),
				childToCopy->getType());
			newNode->addChild(copyNode);
			
		}
		else if (childToCopy->getType() == 1)
		{
			childCount++;
			auto copyNode = std::make_shared<FSNode>(childToCopy->getName(),
				childToCopy->getType());
			newNode->addChild(copyNode);
		}
		loop++;
		copySubTreeHelper(childToCopy);
	}

	return newNode;

}

int FilesystemTree::find(const std::string& pName, const std::string& startPath,
	std::ostream& outStream) const
{
	int matches = 0;
	return recursiveFind(pName, startPath, outStream, matches);
}

int FilesystemTree::recursiveFind(const std::string& pName, const std::string& startPath,
	std::ostream& outStream, int& matches) const
{
	std::shared_ptr<FSNode> startPtr;
	int childCount = 0;
	int i = 0;
	std::shared_ptr<FSNode>currentPtr = startPtr;
	startPtr = pathToPointer(startPath);
	std::shared_ptr<FSNode> currentChild;
	std::string path;
	std::string pathAppend;
	
	pathAppend = startPath;
	while (startPtr->getNumChildren() != childCount)
	{
		path = pathAppend + '/' + startPtr->getChild(i)->getName();
		currentChild = startPtr->getChild(i);
		i++;
		childCount++;
		if (currentChild->getName() == pName)
		{
			matches++;
			outStream << path << std::endl;
		}

		else
		{
			recursiveFind(pName, path, outStream, matches);
		}
		
	}
	//std::cout << matches << std::endl;
	return matches;
}

bool FilesystemTree::copy(const std::string& pName, const std::string& sourcePath,
	const std::string& destPath)
{
	bool rValue = false;
	bool check = false;
	int childValue = 0;
	std::shared_ptr<FSNode> sourceNode = pathToPointer(sourcePath);
	std::shared_ptr<FSNode> destNode = pathToPointer(destPath);
	for (int i = 0; i < sourceNode->getNumChildren(); i++)
	{
		if (pName == rootPtr->getName())
		{
			break;
		}
		if (sourceNode->getChild(i)->getName() == pName)
		{
			childValue = i;
			check = true;
		}
	}
	if (check == true)
	{
		auto copyNode = std::make_shared<FSNode>(sourceNode->getChild(childValue)->getName(),
			sourceNode->getChild(childValue)->getType());
	
		if (destNode != nullptr)
		{
			if (destNode->addChild(copyHelper(pName, sourcePath, destPath, copyNode)))
			{
				rValue = true;
			}
		}	
	}
	
	return rValue;
}

std::shared_ptr<FSNode> FilesystemTree::copyHelper(std::string pName, const std::string& sourcePath, 
	const std::string destPath, const std::shared_ptr<FSNode> nodeToCopy)
{
	bool rValue = false;
	bool check = false;
	std::shared_ptr<FSNode> sourceNode = pathToPointer(sourcePath);
	std::shared_ptr<FSNode> origNode;
	std::string sourceAppend;
	std::string destAppend;
	int childValue = 0;
	for (int i = 0; i < sourceNode->getNumChildren(); i++)
	{
		if (pName == rootPtr->getName())
		{
			break;
		}
		if (sourceNode->getChild(i)->getName() == pName)
		{
			childValue = i;
			check = true;
			break;
		}
	}

	if (check == true)
	{
		if (sourceNode->getChild(childValue)->getType() == 1)
		{
			origNode = sourceNode->getChild(childValue);
			for (int i = 0; i < origNode->getNumChildren(); i++)
			{

				auto childNode = std::make_shared<FSNode>(origNode->getChild(i)->getName(),
					origNode->getChild(i)->getType());
				nodeToCopy->addChild(childNode);
				std::shared_ptr<FSNode> currentChild = origNode->getChild(i);

				sourceAppend = sourcePath + '/' + origNode->getName();
				if (currentChild->getType() == 1)
				{
					destAppend = nodeToCopy->getName() + '/' + currentChild->getName();
					copyHelper(currentChild->getName(), sourceAppend, destAppend, childNode);
				}
				else
				{
					nodeToCopy->addChild(childNode);
				}
			}
		}

		else if (sourceNode->getChild(childValue)->getType() == 0)
		{
			origNode = sourceNode->getChild(childValue);
			auto childNode = std::make_shared<FSNode>(origNode->getName(),
				origNode->getType());

			nodeToCopy->addChild(childNode);	
		}
	}
	return nodeToCopy;
}

bool FilesystemTree::move(const std::string& pName, const std::string& sourcePath,
	const std::string& destPath)
{
	bool rValue = false;
	bool check = false;
	std::shared_ptr<FSNode> sourceNode;
	sourceNode = pathToPointer(sourcePath);
	std::shared_ptr<FSNode> destNode;
	destNode = pathToPointer(destPath);
	std::shared_ptr<FSNode>nodeToAdd;
	int childValue = 0;

	for (int i = 0; i < sourceNode->getNumChildren(); i++)
	{
		if (pName == rootPtr->getName())
		{
			break;
		}
		if (sourceNode->getChild(i)->getName() == pName)
		{
			childValue = i;
			check = true;
			break;
		}
	}
	if ((check == true) && (destNode != nullptr))
	{
		
		if (destNode->addChild(sourceNode->getChild(childValue)))
		{
			if (sourceNode->removeChild(sourceNode->getChild(childValue)->getName()))
			{
				rValue = true;
			}	
		}
		
	}
	return rValue;
}

bool FilesystemTree::remove(const std::string& pName, const std::string& parentPath)
{
	bool rValue = true;
	bool check = false;
	std::shared_ptr<FSNode> nodeToRemove;
	if ((parentPath == "") || (pName == rootPtr->getName()))
	{
		rValue = false;
	}
	
	if (rValue == true)
	{
		nodeToRemove = pathToPointer(parentPath);
		for (int i = 0; i < nodeToRemove->getNumChildren(); i++)
		{
			std::cout << nodeToRemove->getChild(i)->getName() << std::endl;
			if ((nodeToRemove->getChild(i)->getName() == pName) &&
				(nodeToRemove->getChild(i)->getName() != rootPtr->getName()))
			{
				nodeToRemove->removeChild(pName);
				check = true;
				break;
			}
		}
	}
	if (check == false)
	{
		rValue = false;
	}
	return rValue;
}

bool FilesystemTree::format()
{
	bool rValue = true;

	// Set all children pointers to null. Should erase all subtrees
	// since these are smart pointers.
	while (rootPtr->getNumChildren() > 0)
	{
		rValue = (rValue && rootPtr->removeChild(rootPtr->getChild(0)->getName()));
	}

	return rValue;
}

FilesystemTree& FilesystemTree::operator=(const FilesystemTree& rTree)
{
	// What if there's an existing tree? Once rootPtr gets assigned a new value
	// the old root node gets auto-removed because smart pointers.
	rootPtr = copySubTree(rTree.rootPtr);

	return *this;
}

FilesystemTree::FilesystemTree(const FilesystemTree& treeToCopy)
{
	*this = treeToCopy;  // use overloaded =
}

std::pair<int, int> FilesystemTree::recursiveStats(std::shared_ptr<FSNode> startPtr) const
{
	std::pair<int, int> count = { 0,0 };

	if (startPtr != nullptr)
	{
		// count myself unless I'm root
		if (startPtr->getName() != ROOT_NAME)
		{
			count.first += startPtr->isDirectory();
		}
		count.second += startPtr->isFile();

		// recurse on children if this is a directory
		if (startPtr->isDirectory())
		{
			std::pair<int, int> rCount = { 0,0 };

			for (int i = 0; i < startPtr->getNumChildren(); i++)
			{
				rCount = recursiveStats(startPtr->getChild(i));
				count.first += rCount.first;
				count.second += rCount.second;
			}
		}
	}

	return count;

}

void FilesystemTree::displayStats(std::ostream& outStream) const
{
	std::pair<int, int> count = recursiveStats(rootPtr);

	outStream << "Directories: " << count.first << std::endl;
	outStream << "Files: " << count.second << std::endl;

}

FilesystemTree::FilesystemTree()
{
	rootPtr = std::make_shared<FSNode>("", 1);
	rootPtr->name = ROOT_NAME; // get around alpha/digit name restriction
}

FilesystemTree::FilesystemTree(const std::string& fileName)
{
	std::ifstream inFile;
	std::string command;
	std::string name;
	std::string path;
	int type;

	rootPtr = std::make_shared<FSNode>("", 1);
	rootPtr->name = ROOT_NAME; // get around alpha/digit name restriction

	inFile.open(fileName.c_str());
	if (inFile.fail())
	{
		throw std::runtime_error("File " + fileName + " not found.");
	}
	else
	{
		while (std::getline(inFile, command))
		{
			type = -1;
			name = "";
			path = "";

			if (command[0] == '*') // create directory
			{
				command = command.substr(2, command.length() - 2);
				type = 1;
			}
			else
			{
				type = 0;
			}

			name = command.substr(0, command.find(' '));
			path = command.substr(command.find(' ') + 1,
				command.length() - command.find(' ') + 1);

			if (type != -1 && name != "" && path != "")  // verify valid command
			{
				if (!create(name, type, path))
				{
					std::cout << "ERROR: Could not create " << name
						<< " in path " << path << std::endl << std::endl;
				}
			}

		}
	}
}

std::shared_ptr<FSNode> FilesystemTree::pathToPointer(const std::string& path) const
{
	std::string pathCpy = path;

	int startIdx = 0;
	int slashIdx = 0;
	std::shared_ptr<FSNode> parentPtr = rootPtr;

	if (pathCpy == ROOT_NAME)
	{
		return rootPtr;
	}

	if (pathCpy == "" || (pathCpy.substr(0, ROOT_NAME.length()) != ROOT_NAME))
	{
		return nullptr; // path doesn't start with ROOT_NAME
	}

	// Add a last / if there isn't one
	if (pathCpy[pathCpy.length() - 1] != SEPARATING_CHAR)
	{
		pathCpy += SEPARATING_CHAR;
	}

	slashIdx = pathCpy.find(SEPARATING_CHAR);
	pathCpy = pathCpy.substr(slashIdx + 1, pathCpy.length() - slashIdx + 1);

	while (parentPtr != nullptr && pathCpy.length() > 1)
	{
		slashIdx = pathCpy.find(SEPARATING_CHAR);
		parentPtr = parentPtr->getChild(pathCpy.substr(0, slashIdx));
		pathCpy = pathCpy.substr(slashIdx + 1, pathCpy.length() - slashIdx + 1);
	}

	return parentPtr;
}

void FilesystemTree::displayTree(std::ostream& outStream) const
{
	recursiveDisplay(rootPtr, "", outStream);
}

void FilesystemTree::recursiveDisplay(std::shared_ptr<FSNode> nodePtr,
	std::string preSpace, std::ostream& outStream) const
{
	outStream << preSpace << (nodePtr->isDirectory() ? "<" : "")
		<< nodePtr->getName() << (nodePtr->isDirectory() ? ">" : "")
		<< std::endl;

	if (nodePtr->isDirectory())
	{

		for (int i = 0; i < nodePtr->getNumChildren(); i++)
		{
			recursiveDisplay(nodePtr->getChild(i), preSpace + "|     ", outStream);
		}
	}
}

FilesystemTree::~FilesystemTree()
{
	rootPtr = nullptr; // Strictly speaking not necessary because smart pointers.
}