package main

import (
	"API"
	"Entities"
	"Repository"
)

func main() {
	cbr := Repository.RepositoryBuilder()

	// Create some entities
	topic1 := Entities.Entity{Id: 1, Title: "Car Driver", ImageUrl:  "https://image.ibb.co/h3rTJn/car1.jpg"}
	topic2 := Entities.Entity{Id: 2, Title: "Programmer", ImageUrl:  "https://image.ibb.co/h3rTJn/car2.jpg"}
	topic3 := Entities.Entity{Id: 3, Title: "Dancer",ImageUrl:  "https://image.ibb.co/h3rTJn/car3.jpg"}

	// Add them
	cbr.Add(topic1)
	cbr.Add(topic2)
	cbr.Add(topic3)

	// Start the API
	api := API.APIBuilder("", ":3000", cbr)
	api.StartServer()
}

