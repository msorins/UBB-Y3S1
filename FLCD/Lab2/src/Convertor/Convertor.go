package Convertor

import "Grammar"
import "FiniteAutomata"

func FiniteAutomataFromGrammerBuilder(grammar Grammar.Grammar) FiniteAutomata.FiniteAutomata {
	fa := FiniteAutomata.FiniteAutomata{}

	// Form states
	fa.Q = grammar.N
	fa.Q["K"] = true // Add K as a node

	// Form symbols
	fa.E = grammar.E

	// Form transitions
	fa.Ro = []FiniteAutomata.Transition{}
	fa.F = map[string] bool {}
	for _, production := range grammar.P {
		if len(production.RightSide) == 1 {
			fa.Ro = append(fa.Ro, FiniteAutomata.Transition{From: production.LeftSide[0], To: "K", Symbol: production.RightSide[0]})
		}
		if len(production.RightSide) == 2 {
			fa.Ro = append(fa.Ro, FiniteAutomata.Transition{From: production.LeftSide[0], To: production.RightSide[1], Symbol: production.RightSide[0]})
		}

		// If starting symbol -> eps
		if production.LeftSide[0] == grammar.S && production.RightSide[0] == "eps" {
			fa.F[ grammar.S ] = true
		}
	}

	// Form starting symbols
	fa.Q0 = grammar.S

	// Form finish symbols
	used := map[string] bool {}
	for _, production := range grammar.P {
		used[ production.LeftSide[0] ] = true
		//if len( production.RightSide ) == 2 {
		//	used[ production.RightSide[1] ] = true
		//}
	}

	for state, _ := range(fa.Q) {
		_, found := used[state]
		if found == false {
			fa.F[state] = true
		}
	}
	fa.F["K"] = true

	return fa
}

func GrammarFromFiniteAutomataBuilder(fa FiniteAutomata.FiniteAutomata) Grammar.Grammar {
	grm := Grammar.Grammar{}

	// Form the set of non terminals
	grm.N = fa.Q

	// Form the set of terminals
	grm.E = fa.E

	// Form the set of productions
	grm.P = []Grammar.Production{}
	for _, transition := range(fa.Ro) {

		if transition.To == "K" {
			grm.P = append(grm.P, Grammar.Production{LeftSide: []string{transition.From}, RightSide: []string{transition.Symbol} })
		} else {
			grm.P = append(grm.P, Grammar.Production{LeftSide: []string{transition.From}, RightSide: []string{transition.Symbol, transition.To}})
		}
	}

	// Form the starting symbol
	grm.S = fa.Q0

	// Check if the starting symbol is also a final state
	for x, _ := range(fa.F) {
		if x == fa.Q0 {
			grm.P = append(grm.P, Grammar.Production{LeftSide: []string{x}, RightSide: []string{"eps"} })
		}
	}

	return grm
}

