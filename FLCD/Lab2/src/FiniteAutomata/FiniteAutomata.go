package FiniteAutomata

import (
	"bufio"
	"errors"
	"fmt"
	"io/ioutil"
	"os"
	"regexp"
	"strings"
)

type Transition struct {
	From string
	To   string
	Symbol string
}

type FiniteAutomata struct {
	Q  map[string] bool // set of states
	E  map[string] bool // set of symbols
	Ro []Transition     // transitions
	Q0 string           // starting symbol
	F  map[string] bool // finish symbols
}

func FiniteAutomataFileBuilder(filePath string) FiniteAutomata {
	fa := FiniteAutomata{}

	// Open the file
	b, err := ioutil.ReadFile(filePath)
	if err != nil {
		fmt.Print(err)
	}

	// Transform the string
	str := string(b)

	// Form the grammer
	fa.readFa(str)
	return fa
}

func FiniteAutomataInputBuilder() FiniteAutomata {
	fa := FiniteAutomata{}
	reader := bufio.NewReader(os.Stdin)

	line1, _ := reader.ReadString('\n')
	line2, _ := reader.ReadString('\n')
	line3, _ := reader.ReadString('\n')
	line4, _ := reader.ReadString('\n')
	line5, _ := reader.ReadString('\n')

	fa.readFa(line1 + line2 + line3 + line4 + line5[0:len(line5) - 1] )
	return fa
}

func (this *FiniteAutomata) readFa(content string) {
	// Get the content
	lines := strings.Split(content, "\n")
	if len(lines) != 5 {
		fmt.Println("Error, incorrect number of lines")
		return
	}

	// Parse the lines
	var err1, err2, err3, err4, err5 error
	this.Q, err1 = this.parseWords(lines[0])
	this.E, err2 = this.parseWords(lines[1])
	this.Q0, err4 = this.parseStarting(lines[3])
	this.F, err5 = this.parseWords(lines[4])
	this.Ro, err3 = this.parseTransitions(lines[2])

	if err1 != nil || err2 != nil || err3 != nil || err4 != nil || err5 != nil {
		fmt.Println(err1, err2, err3, err4, err5)
		return
	}
}

func (this *FiniteAutomata) parseWords(s string) (map[string] bool, error) {
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

func (this *FiniteAutomata) parseTransitions(s string) ([]Transition, error) {
	transitions := strings.Split(s, ";")
	resTransitions := []Transition{}

	// For each transition
	for _, transition := range(transitions) {
		elems := filterEmptyStrings(strings.Split(transition, " "))
		if len(elems) != 3 {
			return []Transition{}, errors.New("Incorrect number of terms in transition")
		}
		p := Transition{}
		p.From = elems[0]
		p.To = elems[1]
		p.Symbol = elems[2]

		resTransitions = append(resTransitions, p)
	}

	return resTransitions, nil
}

func (this *FiniteAutomata) parseStarting(s string) (string, error) {
	if isString(s) && len(s) == 1 {
		return s, nil
	} else {
		return "", errors.New("4th line not correct")
	}
}

func (this *FiniteAutomata) isTerminalOrNonTerminal(item string) bool {
	_, exists1 := this.Q[item]
	_, exists2 := this.E[item]

	return exists1 || exists2 || item == this.Q0 || item == "eps"
}

func (this *FiniteAutomata) isTerminal(item string) bool {
	_, exists := this.E[item]
	return exists
}

func (this *FiniteAutomata) isNonTerminal(item string) bool {
	_, exists := this.Q[item]
	return exists
}

func (this *FiniteAutomata) PrintStates() {
	out := ""

	for q, _ := range(this.Q) {
		out += q + ", "
	}

	fmt.Println(out)
}

func (this *FiniteAutomata) PrintSymbols() {
	out := ""

	for e, _ := range(this.E) {
		out += e + ", "
	}

	fmt.Println(out)
}

func (this *FiniteAutomata) PrintTransitions() {
	out := ""

	for _, production := range(this.Ro) {
		out = out + production.To + " " + production.From + " " + production.Symbol + "\n"
	}

	fmt.Println(out)
}

func (this *FiniteAutomata) PrintStartingSymbol() {
	fmt.Println(this.Q0)
}

func (this *FiniteAutomata) PrintFinishSymbols() {
	out := ""

	for f, _ := range(this.F) {
		out += f + ", "
	}

	fmt.Println(out)
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
