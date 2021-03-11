#include <cstddef>
#include <cctype>
#include <stdio.h>
#include <iostream>
#include "FSNode.h"

int FSNode::getType() const
{
	int rValue = -1;
	if (type == 0)
	{
		rValue = 0;
	}

	else if (type == 1)
	{
		rValue = 1;
	}
	return rValue;
} // end getType

FSNode::FSNode(const std::string& pName, int pType) : type(pType)
{
	setName(pName);
}  // end constructor

void FSNode::setName(const std::string& pName)
{
	bool check = false;
	for (int i = 0; i < pName.length(); i++)
	{
		
		if ((isalpha(pName[i])) || (isdigit(pName[i])))
		{
			check = false;
		}
		else
		{
			std::cout << "Only characters and numbers are allowed for filenames." << std::endl;
			check = true;
		}
		
	}

	if (check == false)
	{
		name = pName;
	}
}  // end setName

std::string FSNode::getName() const
{
	return name;
}  // end getName

bool FSNode::isFile() const
{
	 
	bool rValue = false;
	if (getType() == 0)
	{
		rValue = true;
	}
	return rValue;
} // end isFile

bool FSNode::isDirectory() const
{
	 
	bool rValue = false;
	if (getType() == 1)
	{
		rValue = true;
	}
	return rValue;
} // end isDirectory

bool FSNode::addChild(std::shared_ptr<FSNode> childPtr)
{
	/* 
	Note that using vector insert() along with begin() allows insertion of a
	new element at any point in the vector. For example, you can insert childPtr
	into the children vector at index 2 (which shifts the existing index 2 and
	all following indices up by 1) using:

		children.insert(children.begin() + 2, childPtr);
	*/
	bool rValue = false;
	bool check = true;
	//make filenames unique
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i]->getName() == childPtr->getName())
		{
			check = false;
			break;
		}
	}
	if ((this->getType() == 1) && (check == true))
	{
		//check if vector is empty
		if (children.empty())
		{
			children.insert(children.begin(), childPtr);
			rValue = true;
			
		}

		//if vector has one node in it
		else if (children.size() == 1)
		{
			if (children[0]->getName() > childPtr->getName())
			{
				children.push_back(childPtr);
				rValue = true;
			}
			else
			{
				children.insert(children.begin() + 1, childPtr);
				rValue = true;
			}
		}

		//insert node at proper location based on filename
		else if (children.size() > 1)
		{
			
			for (int i = 0; i < children.size(); i++)
			{
				if (i > 0)
				{
					if ((children[i]->getName() > childPtr->getName()) &&
						(children[i - 1]->getName() < childPtr->getName()))
					{	
						children.insert(children.begin() + i, childPtr);
						rValue = true;
					}
				}
				else if (children[children.size() - 1]->getName() < childPtr->getName())
				{
					children.push_back(childPtr);
					rValue = true;
				}
				else if (children[0]->getName() > childPtr->getName())
				{
					children.insert(children.begin() + children.size(), childPtr);
					rValue = true;
				}
			}
		}	
	}
	return rValue;
	//return rValue;
}  // end addChild


std::shared_ptr<FSNode> FSNode::getChild(int index) const
{
	std::shared_ptr<FSNode> childNode = nullptr;
	for (int i = 0; i < children.size(); i++)
	{
		if (i == index)
		{
			childNode = children[i];
		}
	}
	return childNode;
}  // end getChild	

std::shared_ptr<FSNode> FSNode::getChild(std::string pName) const
{
	std::shared_ptr<FSNode> childNode = nullptr;
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i]->getName() == pName)
		{
			childNode = children[i];
		}
	}
	return childNode;
}  // end getChild

int FSNode::getNumChildren() const
{
	 
	return children.size();
} // end getNumChildren

bool FSNode::removeChild(const std::string& pName)
{
	/* 
	Note that using vector erase() along with begin() allows removal of an
	element at any point in the vector. For example, you can remove the element 
	from the children vector at index 2 (which shifts index 3 and above down by
	1 index) using:

		children.erase(children.begin() + 2);
	*/
	bool rValue = false;
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i]->getName() == pName)
		{
			children.erase(children.begin() + i);
			rValue = true;
		}
	}
	return rValue;
} // end removeChild
