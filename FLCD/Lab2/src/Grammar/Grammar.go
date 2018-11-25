package Grammar

import (
	"bufio"
	"errors"
	"fmt"
	"io/ioutil"
	"os"
	"regexp"
	"strings"
)

type Production struct {
	LeftSide []string
	RightSide []string
}

type Grammar struct {
	N map[string] bool 		 // set of non terminals
	E map[string] bool       // set of terminals
	P []Production 		 	 // productions
	S string 	  			 // starting symbol
}

func GrammarFileBuilder(filePath string) Grammar	 {
	grm := Grammar{}

	// Open the file
	b, err := ioutil.ReadFile(filePath)
	if err != nil {
		fmt.Print(err)
	}

	// Transform the string
	str := string(b)

	// Form the grammer
	grm.readGrammar(str)
	return grm
}

func GrammarInputBuilder() Grammar {
	grm := Grammar{}
	reader := bufio.NewReader(os.Stdin)

	line1, _ := reader.ReadString('\n')
	line2, _ := reader.ReadString('\n')
	line3, _ := reader.ReadString('\n')
	line4, _ := reader.ReadString('\n')

	grm.readGrammar(line1 + line2 + line3 + line4[0:len(line4) - 1] )
	return grm
}

func (this *Grammar) readGrammar(content string) {
	// Get the content
	lines := strings.Split(content, "\n")
	if len(lines) != 4 {
		fmt.Println("Error, incorrect number of lines")
		return
	}

	// Parse the lines
	var err1, err2, err3, err4 error
	this.N, err1 = this.parseWords(lines[0])
	this.E, err2 = this.parseWords(lines[1])
	this.S, err4 = this.parseStarting(lines[3])
	this.P, err3 = this.parseProductions(lines[2])

	if err1 != nil || err2 != nil || err3 != nil || err4 != nil {
		fmt.Println(err1, err2, err3, err4)
		return
	}
}

func (this *Grammar) parseWords(s string) (map[string] bool, error) {
	words := strings.Split(s, " ")
	res := map[string] bool{}

	for _, word := range(words) {
		if !isString(word) {
			return map[string]bool{}, errors.New("Words not correct")
		}
		res[word] = true
	}

	return res, nil
}

func (this *Grammar) parseProductions(s string) ([]Production, error) {
	productions := strings.Split(s, ";")
	resProductions := []Production{}

	// For each production
	for _, production := range(productions) {
		productionLeftAndRight := strings.Split(production, "->")
		if len(productionLeftAndRight) != 2 {
			return []Production{}, errors.New("-> appears an incorrect number of times (OR NOT)")
		}

		productionLeft  := filterEmptyStrings(  strings.Split(productionLeftAndRight[0], " ") )
		productionRight := filterEmptyStrings( strings.Split(productionLeftAndRight[1], " ") )

		// Go and check every character to be in the list of terminals or non-terminals
		for _, item := range(productionLeft) {
			if ! this.isTerminalOrNonTerminal(item) {
				return []Production{}, errors.New("Item not found in terminals or non terminals list")
			}
		}

		for _, item := range(productionRight) {
			if ! this.isTerminalOrNonTerminal(item) {
				return []Production{}, errors.New("Item not found in terminals or non terminals list")
			}
		}

		p := Production{}
		p.LeftSide = productionLeft
		p.RightSide = productionRight

		resProductions = append(resProductions, p)
	}

	return resProductions, nil
}

func (this *Grammar) IsfRegular() error {
	doesSEnterInEps := false

	for _, production := range(this.P) {
		if len(production.LeftSide) != 1 || !this.isNonTerminal(production.LeftSide[0]) {
			return errors.New("Left Hand side is not correct")
		}

		if len(production.RightSide) > 2 {
			return errors.New("Right hand side is not correct, to many t or non t's")
		}

		if !this.isTerminal(production.RightSide[0]) || (len(production.RightSide) == 2 && !this.isNonTerminal(production.RightSide[1])) {
			return errors.New("Right hand side is not correct")
		}

		if production.RightSide[0] == "eps" {
			doesSEnterInEps = true
			if production.LeftSide[0] != this.S {
				return errors.New("Only Starting symbol can enter in eps")
			}
		}
	}

	// if Q0 -> eps resutls that Q0 cannot be on the rhs
	if doesSEnterInEps {
		for _, production := range(this.P) {
			if production.RightSide[0] == this.S || (len(production.RightSide) == 2 && production.RightSide[1] == this.S) {
				return errors.New("If s-> eps results that Q0 cannot be on the rhs")
			}
		}
	}

	return nil
}

func (this *Grammar) parseStarting(s string) (string, error) {
	if isString(s) && len(s) == 1 {
		return s, nil
	} else {
		return "", errors.New("4th line not correct")
	}
}

func (this *Grammar) isTerminalOrNonTerminal(item string) bool {
	_, exists1 := this.N[item]
	_, exists2 := this.E[item]

	return exists1 || exists2 || item == this.S || item == "eps"
}

func (this *Grammar) isTerminal(item string) bool {
	_, exists := this.E[item]
	return exists
}

func (this *Grammar) isNonTerminal(item string) bool {
	_, exists := this.N[item]
	return exists
}

func (this *Grammar) PrintNonTerminals() {
	out := ""

	for nonT, _ := range(this.N) {
		out += nonT + ", "
	}

	fmt.Println(out)
}

func (this *Grammar) PrintTerminals() {
	out := ""

	for terminal, _ := range(this.N) {
		out += terminal + ", "
	}

	fmt.Println(out)
}

func (this *Grammar) PrintProductions() {
	out := ""

	for _, production := range(this.P) {
		out = out + production.LeftSide[0] + " -> " + production.RightSide[0] + " "
		if len(production.RightSide) > 1 {
			out = out + production.RightSide[1] + "\n"
		} else {
			out = out + "\n"
		}
	}

	fmt.Println(out)
}

func (this *Grammar) PrintStart() {
	fmt.Println(this.S)
}

func filterEmptyStrings(strings []string) []string {
	res := []string{}
	for _, val := range(strings) {
		if len(val) != 0{
			res = append(res, val)
		}
	}

	return res
}

func isString(token string) bool {
	result, err := regexp.MatchString(`^([[:alnum:]])*$`, token)
	if err != nil {
		fmt.Println("Error at matching identifier token")
		return false
	}

	return result
}
