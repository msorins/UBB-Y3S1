package main

import (
	"ST"
	"PIF"
	"fmt"
	"io/ioutil"
	"strings"
)

func isStringInList(s string, vs []string) bool {
	for _, crt := range(vs) {
		if crt == s {
			return true
		}
	}

	return false
}

func isIdentifier() bool {
	return true
}

func isConstant() bool {
	return true
}

func main() {
	// Constants
	RESERVED_WORDS := []string{"main", "if", "while", "read", "print", "then", "else", "struct", "int", "string", "char", "bool", "true", "false"}
	OPERATORS := []string{"+", "-", "*", "/", "lt", "le", "gt", "ge",  "eq"}
	SEPARATORS := []string{"<", ">", " "}
	CODIFICATION := map[string]int {
		"identifier": 0,
		"constant": 1,
		"main": 2,
		"char": 3,
		"const": 4,
		"int": 5,
		"bool": 6,
		"string": 7,
		"true": 8,
		"false": 9,
		"if": 10,
		"then": 11,
		"else": 12,
		"while": 13,
		"read": 14,
		"print": 15,
		"<": 16,
		">": 17,
		"ul": 18,
		"li": 19,
		"(": 20,
		")": 21,
		"+": 22,
		"-": 23,
		"*": 24,
		"/": 25,
		"lt": 26,
		"le": 27,
		"eq": 28,
		"neq": 29,
		"gt": 30,
		"ge": 31,
		"struct": 32,
	}

	// Tables
	stIdentifier := ST.AVLBuilder()
	stConstants := ST.AVLBuilder()
	pif := PIF.PIF{}

	// Open the file
	b, err := ioutil.ReadFile("lang1.txt")
	if err != nil {
		fmt.Print(err)
	}
	str := string(b)

	// Remove \n
	str = strings.Replace(str, "\n", " ", -1)

	// Split the file by tokens
	strSplit := strings.FieldsFunc(str, func(r rune) bool {
		if isStringInList(string(r), SEPARATORS) {
			return true
		}
		return false
	})

	// Add tokens in PIF | ST | Return Error
	for _, token := range(strSplit) {
		if isStringInList(token, SEPARATORS) || isStringInList(token, RESERVED_WORDS) || isStringInList(token, OPERATORS)  {
			// If it is separator | reserved word | operator
			// Insert in PIF, but not in ST
			cod, _ := CODIFICATION[token]
			pif.Insert(cod, -1)
		} else if isIdentifier() {
			// Insert in PIF and in stIdentifier
			cod, _ := CODIFICATION[token]
			posInSt := stIdentifier.Insert(token)
			pif.Insert(cod, posInSt)
		} else if isConstant() {
			// Insert in PIF and in stConstants
			cod, _ := CODIFICATION[token]
			posInSt := stConstants.Insert(token)
			pif.Insert(cod, posInSt)
		} else {
			// Error
			fmt.Println("Error at decoding")
		}
	}


	fmt.Println(strSplit)
}