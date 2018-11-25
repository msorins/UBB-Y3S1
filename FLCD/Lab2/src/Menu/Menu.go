package Menu

import (
	"FiniteAutomata"
	"Grammar"
	"bufio"
	"fmt"
	"os"
	"strconv"
)

type MenuItem struct {
	msg string
	cmd func()
}

type menu struct {
	Grammar Grammar.Grammar
	Fa FiniteAutomata.FiniteAutomata
	MenuList map[int] MenuItem
}

func (this *menu) PrintMenu() {
	for i := 1; i <= len(this.MenuList); i++ {
		fmt.Println( this.MenuList[i].msg )
	}
}

func (this *menu) ReadAndExecuteCMD() {
	// Get the command
	reader := bufio.NewReader(os.Stdin)
	line, _ := reader.ReadString('\n')
	line = line[:len(line) - 1]

	cmdNr, err := strconv.Atoi(line)
	if err != nil {
		fmt.Println("Command not OK, cannot be converted into int")
		return
	}

	val, found := this.MenuList[cmdNr]
	if found == false {
		fmt.Println("Command not OK")
		return
	}

	// Execute the command
	val.cmd()
}

func MenuBuilder(grm Grammar.Grammar, fa FiniteAutomata.FiniteAutomata) menu {
	mn := menu{Grammar: grm, Fa: fa}
	mn.MenuList = map[int] MenuItem {}
	mn.MenuList[1] = MenuItem{msg: "1. (grammar) Show all non terminals", cmd: mn.Grammar.PrintNonTerminals}
	mn.MenuList[2] = MenuItem{msg: "2. (grammar) Show all terminals", cmd: mn.Grammar.PrintTerminals}
	mn.MenuList[3] = MenuItem{msg: "3. (grammar) Show all productions", cmd: mn.Grammar.PrintProductions}
	mn.MenuList[4] = MenuItem{msg: "4. (grammar) Show staring", cmd: mn.Grammar.PrintStart}

	mn.MenuList[5] = MenuItem{msg: "5. (finite automata) Show all states", cmd: mn.Fa.PrintStates}
	mn.MenuList[6] = MenuItem{msg: "6. (finite automata) Show all symbols", cmd: mn.Fa.PrintSymbols}
	mn.MenuList[7] = MenuItem{msg: "7. (finite automata) Show all transitions", cmd: mn.Fa.PrintTransitions}
	mn.MenuList[8] = MenuItem{msg: "8. (finite automata) Show starting symbol", cmd: mn.Fa.PrintStartingSymbol}
	mn.MenuList[9] = MenuItem{msg: "9. (finite automata) Show all finish symbols", cmd: mn.Fa.PrintFinishSymbols}
	return mn
}
