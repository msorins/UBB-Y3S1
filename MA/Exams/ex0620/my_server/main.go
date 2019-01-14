package main

import (
	"API"
	"Entities"
	"Repository"
)

func main() {
	cbr := Repository.RepositoryBuilder()

	// Create some entities
	ent1 := Entities.Entity{Id: 1, Name: "Costel Costache", Type:  "individual", Status: "prospect", Employees: 300, CEO: "Sorin CEO"}
	ent2 := Entities.Entity{Id: 2, Name: "Gabriel Gabi", Type:  "medium", Status: "prospect", Employees: 10, CEO: "Lostel CEO"}
	ent3 := Entities.Entity{Id: 3, Name: "Andrei Andreiescu", Type:  "big", Status: "prospect", Employees: 200, CEO: "Mostel CEO"}

	// Add them
	cbr.Add(ent1)
	cbr.Add(ent2)
	cbr.Add(ent3)

	// Start the API
	api := API.APIBuilder("", ":3000", cbr)
	api.StartServer()
}

