package ST

import (
	"fmt"
	avl "github.com/emirpasic/gods/trees/avltree"
	"strconv"
)


type ST struct {
	tree     *avl.Tree
	crtIndex int
}

func AVLBuilder() ST {
	newClass := ST{}
	newClass.tree = avl.NewWithStringComparator()
	newClass.crtIndex = -1
	return newClass
}

func (this *ST) Insert(key string) int {
	// Check to see if key already exists in tree
	val, exists := this.Get(key)
	if exists == true {
		return val
	} else {
		this.crtIndex += 1
		this.tree.Put(key, this.crtIndex)
		return this.crtIndex
	}
}

func (this *ST) Get(key string) (int, bool) {
	val, found := this.tree.Get(key)

	nr, _ := strconv.Atoi(fmt.Sprint(val))
	return nr, bool(found)
}