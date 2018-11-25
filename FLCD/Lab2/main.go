package main

import (
	"FiniteAutomata"
	"Grammar"
	"Menu"
	"fmt"
)


func main() {
	grm := Grammar.GrammarFileBuilder("/Users/so/Desktop/Y3S1/FLCD/Lab2/grammar.in")
	fa := FiniteAutomata.FiniteAutomataFileBuilder("/Users/so/Desktop/Y3S1/FLCD/Lab2/finiteAutomata.in")

	fmt.Println(grm.IsfRegular())
	//fmt.Println(Convertor.GrammarFromFiniteAutomataBuilder(fa))

	menu := Menu.MenuBuilder(grm, fa)

	for true {
		menu.PrintMenu()
		menu.ReadAndExecuteCMD()
	}
}