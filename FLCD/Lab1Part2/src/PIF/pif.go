package PIF

type PIF struct {
	lst []PifStruct
}

func (this *PIF) Insert(codification int, positionInST int)  {
	this.lst = append(this.lst, PifStruct{codification: codification, positionInST: positionInST})
}

