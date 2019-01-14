package Entities

type Entity struct {
	Id     int    `json:"Id"`
	Name   string `json:"Name"`
	Type   string `json:"Type"`
	Status string `json:"Status"`
	Employees int `json:"Employees"`
	CEO string `json:"CEO"`
}