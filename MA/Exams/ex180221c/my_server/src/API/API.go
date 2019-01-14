package API

import (
	"Entities"
	"Repository"
	"encoding/json"
	"fmt"
	"github.com/gorilla/mux"
	"log"
	"net/http"
	"strconv"
)

type API struct {
	url              string
	port             string
	topicsRepository Repository.Repository
}

func APIBuilder(url string, port string, repo Repository.Repository) API{
	obj := API{ url: url, port: port, topicsRepository: repo }

	return obj
}

func (this *API) StartServer() {
	router := mux.NewRouter()

	// Add Routes
	router.HandleFunc("/entities", this.GetAll).Methods("GET")
	router.HandleFunc("/entities", this.Add).Methods("POST")
	router.HandleFunc("/entities/{Id}", this.GetById).Methods("GET")
	router.HandleFunc("/entities/{Id}", this.UpdateById).Methods("PUT")
	router.HandleFunc("/entities/{Id}", this.DeleteById).Methods("DELETE")
	router.Use(this.loggingMiddleware)

	// Start the router
	log.Fatal(http.ListenAndServe(this.port, router))
}

func (this *API) loggingMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		// Do stuff here
		log.Println(r.RequestURI)
		// Call the next handler, which can be another middleware in the chain, or the final handler.
		next.ServeHTTP(w, r)
	})
}

func (this *API) GetAll(w http.ResponseWriter, r *http.Request) {
	// Get all the levels
	levels, err := this.topicsRepository.GetAll()

	// Show the response
	if err != nil {
		w.WriteHeader(500)
		json.NewEncoder(w).Encode(err.Error())
		return
	}

	json.NewEncoder(w).Encode(levels)
}

func (this *API) Add(w http.ResponseWriter, r *http.Request) {
	fmt.Println(r.Body)
	decoder := json.NewDecoder(r.Body)
	var newLevel Entities.Entity
	err := decoder.Decode(&newLevel)

	if err != nil {
		w.WriteHeader(400)
		json.NewEncoder(w).Encode("Entity not formated correctly")
		return
	}

	// Update the level
	err = this.topicsRepository.Add(newLevel)
	if err != nil {
		w.WriteHeader(400)
		json.NewEncoder(w).Encode(err.Error())
		return
	}

	json.NewEncoder(w).Encode("OK")
}

func (this *API) GetById(w http.ResponseWriter, r *http.Request) {
	// Get Level by Id
	params := mux.Vars(r)
	Id, err := strconv.Atoi(params["Id"])
	var entity Entities.Entity

	if err == nil {
		entity, err = this.topicsRepository.GetById(Id)
	}


	// Show the response
	if err != nil {
		w.WriteHeader(500)
		json.NewEncoder(w).Encode(err.Error())
		return
	}

	json.NewEncoder(w).Encode(entity)
}

func (this *API) UpdateById(w http.ResponseWriter, r *http.Request) {
	// Get the query parameter
	params := mux.Vars(r)
	Id, err := strconv.Atoi(params["Id"])

	if err != nil {
		w.WriteHeader(400)
		json.NewEncoder(w).Encode(err.Error())
		return
	}

	decoder := json.NewDecoder(r.Body)
	var newLevel Entities.Entity
	err = decoder.Decode(&newLevel)

	if err != nil {
		w.WriteHeader(400)
		json.NewEncoder(w).Encode(err.Error())
		return
	}

	// Update the level
	err = this.topicsRepository.Update(Id, newLevel)
	if err != nil {
		w.WriteHeader(400)
		json.NewEncoder(w).Encode(err.Error())
		return
	}

	json.NewEncoder(w).Encode("OK")
}

func (this *API) DeleteById(w http.ResponseWriter, r *http.Request) {
	// Get the query parameter
	params := mux.Vars(r)
	topicId, err := strconv.Atoi(params["Id"])

	if err != nil {
		w.WriteHeader(404)
		json.NewEncoder(w).Encode(err.Error())
		return
	}

	// Get the actual level
	err = this.topicsRepository.Delete(topicId)

	// Show the response
	if err != nil {
		w.WriteHeader(404)
		json.NewEncoder(w).Encode(err.Error())
		return
	}

	json.NewEncoder(w).Encode("OK")
}