package main

import (
	"Entities"
	"Repository"
	"testing"
)

var DB_NAME="testLevels"

func TestAddDeleteLevel(t *testing.T) {
	var cbr Repository.IRepository
	cbr = Repository.RepositoryBuilder(DB_NAME)
	tiles := []Entities.Tile{ Entities.Tile{Type:0}, Entities.Tile{Type:0}, Entities.Tile{Type:1}, Entities.Tile{Type:2}}
	Level := Entities.Entity{ Name: "Introductory", Width: 5, Height: 5, Tiles: tiles}

	var err error
	err = cbr.Add(Level)
	if err != nil {
		t.Error("Error at adding a Entity")
	}

	err = cbr.Add(Level)
	if err == nil {
		t.Error("Error at adding a Entity. Duplicate key not found")
	}

	err = cbr.Delete(Level.Name)
	if err != nil {
		t.Error("Error at deleting a Entity")
	}

	err = cbr.Delete(Level.Name)
	if err == nil {
		t.Error("Error at deleting a Entity. Duplicate key not found")
	}
}


func TestUpdateLevel(t *testing.T) {
	var cbr Repository.IRepository
	cbr = Repository.RepositoryBuilder(DB_NAME)

	tiles := []Entities.Tile{ Entities.Tile{Type:0}, Entities.Tile{Type:0}, Entities.Tile{Type:1}, Entities.Tile{Type:2}}
	Level := Entities.Entity{ Name: "Introductory", Width: 5, Height: 5, Tiles: tiles}
	newLevel := Entities.Entity{ Name: "Introductory2", Width: 6, Height: 6, Tiles: tiles}
	newLevel2 := Entities.Entity{ Name: "Introductory2", Width: 7, Height: 7, Tiles: tiles}
	cbr.Add(Level)

	// Update also the name
	var err error
	err = cbr.Update(Level.Name, newLevel)
	if err != nil {
		t.Error("Error at updating the Entity (1)")
	}

	var updatedLevel Entities.Entity
	updatedLevel, err = cbr.GetLevelByName(newLevel.Name)
	if err != nil || updatedLevel.Name != newLevel.Name || updatedLevel.Width!= newLevel.Width {
		t.Error("Error at updating the Entity (2)")
	}

	// Do not update
	err = cbr.Update(newLevel.Name, newLevel2)
	if err != nil {
		t.Error("Error at updating the Entity (3)")
	}
	updatedLevel, err = cbr.GetLevelByName(newLevel.Name)
	if err != nil || updatedLevel.Name != newLevel2.Name || updatedLevel.Width != newLevel2.Width {
		t.Error("Error at updating the Entity (2)")
	}

	// Delete residual document
	cbr.Delete(newLevel2.Name)
}

