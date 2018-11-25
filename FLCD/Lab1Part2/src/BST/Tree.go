package BST
import "errors"

type Tree struct {
	root *Node
}

func (tree *Tree) Insert(id int, val string) {
	// If Tree is Empty
	if tree.root == nil {
		tree.root = &Node{Id: id, Val: val}
		return
	}

	// Otherwise
	tree.insertRecursive(tree.root, id, val)
}

func (tree *Tree) insertRecursive(crtNode *Node, id int, val string) error {
	if crtNode == nil {
		return errors.New("Cannot insert into a nil tree")
	}
	if id <= crtNode.Id {
		if crtNode.Left == nil {
			crtNode.Left = &Node{Id: id, Val: val}
		} else {
			tree.insertRecursive(crtNode.Left, id, val)
		}
	} else {
		if crtNode.Right == nil {
			crtNode.Right = &Node{Id: id, Val: val}
		} else {
			tree.insertRecursive(crtNode.Right, id, val)
		}
	}
	return nil
}

func (tree *Tree) GetById(id int) Node {
	return tree.getByIdRecursive(tree.root, id)
}

func (tree *Tree) getByIdRecursive(crtNode *Node, id int) Node {
	// If cannot find the node
	if crtNode == nil {
		return Node{Id: -1}
	}

	// If I found the node
	if crtNode.Id == id {
		return *crtNode
	}

	// If I can still find the node
	if id < crtNode.Id {
		return tree.getByIdRecursive(crtNode.Left, id)
	} else {
		return tree.getByIdRecursive(crtNode.Right, id)
	}
}