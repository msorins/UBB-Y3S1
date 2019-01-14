package Repository

import (
	"Entities"
	"fmt"
)

type Repository struct {
	entities []Entities.Entity
}

func RepositoryBuilder() Repository {
	// Create the repository object
	cbr := Repository{}
	cbr.entities = []Entities.Entity{}

	// Return build object
	return cbr
}

func (repo *Repository) GetAll() ([]Entities.Entity, error) {
	return repo.entities, nil
}

func (repo *Repository) GetById(Id int) (Entities.Entity, error) {
	for i := 0; i < len(repo.entities); i += 1 {
		if repo.entities[i].Id == Id {
			return repo.entities[i], nil
		}
	}

	return Entities.Entity{}, fmt.Errorf("Couldn't get the entity by the id:")
}

func (repo *Repository) Add(entity Entities.Entity) error {
	_, err := repo.GetById(entity.Id)
	if err == nil {
		return fmt.Errorf("Id already exists")
	}

	repo.entities = append(repo.entities, entity)
	return nil
}

func (repo *Repository) Update(Id int, newEntity Entities.Entity) error {
	newEntity.Id = Id
	for i := 0; i < len(repo.entities); i += 1 {
		if repo.entities[i].Id == Id {
			repo.entities[i] = newEntity
			return nil
		}
	}

	return fmt.Errorf("Couldn't find the entity to update")
}

func (repo *Repository) Delete(Id int) error {
	for i := 0; i < len(repo.entities); i += 1 {
		if repo.entities[i].Id == Id {
			repo.entities = append(repo.entities[:i], repo.entities[i + 1:]...)
			return nil
		}
	}

	return fmt.Errorf("Couldn't find the entity to delete")
}
