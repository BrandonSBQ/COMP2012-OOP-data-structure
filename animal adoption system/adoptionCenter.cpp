#include "adoptionCenter.h"
#include <iostream>
using namespace std;


// TASK 4.1: AdoptionCenter constructor
// Initialize all data members to represent an empty database.
// The BSTs need to be initialized with their comparison functions:
// - NAME: Sort by species name (including breed), in increasing lexicographic order
// - AGE: Sort by increasing age
// - HEALTH: Sort by increasing health severity
// - VACCINE: Sort by increasing VaccinationStatus total hash value
//
// The comparison functions when comparing the left animal (a1) and right animal (a2) should return:
// - a negative value if a1 < a2
// - 0 if a1 == a2
// - a positive value if a1 > a2
//
// Hints:
// - You can write lambda functions to pass into the BST constructors
// - For comparing strings, look up the documentation for std::string::compare()
// - Be careful when performing arithmetic calculations with unsigned int
AdoptionCenter::AdoptionCenter():sortedAnimals{
        BST([](const Animal* a1, const Animal* a2) -> int { // NAME
            return a1->getSpecies().compare(a2->getSpecies());
        }),
        BST([](const Animal* a1, const Animal* a2) -> int { // AGE
            return (a1->getAge() < a2->getAge()) ? -1 : (a1->getAge() > a2->getAge()) ? 1 : 0;
        }),
        BST([](const Animal* a1, const Animal* a2) -> int { // HEALTH
            unsigned int severity1 = a1->getHealthCondition().severity;
            unsigned int severity2 = a2->getHealthCondition().severity;
            return (severity1 < severity2) ? -1 : (severity1 > severity2) ? 1 : 0;
        }),
        BST([](const Animal* a1, const Animal* a2) -> int { // VACCINE
            unsigned int hash1 = a1->getVaccinationStatus().getTotalHashValue();
            unsigned int hash2 = a2->getVaccinationStatus().getTotalHashValue();
            return (hash1 < hash2) ? -1 : (hash1 > hash2) ? 1 : 0;
        })
    },
    animals(nullptr),
    numAnimals(0){

}

// TASK 4.2: AdoptionCenter destructor
// Deallocate any dynamic memory in this class.
// Hint: This is where the animals should be deallocated.
AdoptionCenter::~AdoptionCenter() {
    for(unsigned int i = 0; i < numAnimals; i++){
        delete animals[i];
    }

    delete [] animals;
}

// TASK 4.3: AdoptionCenter::addAnimal(Animal*)
// Add an animal to the dynamic-length array data member,
// by increasing its size by 1 and increment numAnimals accordingly.
// Add the animal to the end of the array, 
// such that the array is sorted by increasing ID (assume we only add increasing IDs)
// The animal should also be added to the 4 BSTs.
void AdoptionCenter::addAnimal(Animal* a) {
    Animal** newA = new Animal*[numAnimals+1];

    for(unsigned int i = 0; i < numAnimals; i++){
        newA[i] = animals[i];
    }

    newA[numAnimals] = a;

    delete [] animals;
    animals = newA;

    numAnimals++;

    sortedAnimals[NAME].insert(a);
    sortedAnimals[AGE].insert(a);
    sortedAnimals[HEALTH].insert(a);
    sortedAnimals[VACCINE].insert(a);

}

// TASK 4.4: AdoptionCenter::removeAnimal(unsigned int)
// Remove an animal from the database by ID.
// If the ID does not exist, do nothing.
// Otherwise, resize the array to decrease its size by 1,
// copy the non-deleted animals over and decrement numAnimals accordingly.
// The animal should also be removed from the 4 BSTs.
// Finally, return true if the animal was successfully removed.
bool AdoptionCenter::removeAnimal(unsigned int id) {
    
    int indexToRemove = -1;
    for (unsigned int i = 0; i < numAnimals; ++i) {
        if (animals[i]->getID() == id) {
            indexToRemove = i;
            break;
        }
    }

  
    if (indexToRemove == -1) {
        return false;
    }

  
    sortedAnimals[NAME].remove(animals[indexToRemove]);
    sortedAnimals[AGE].remove(animals[indexToRemove]);
    sortedAnimals[HEALTH].remove(animals[indexToRemove]);
    sortedAnimals[VACCINE].remove(animals[indexToRemove]);

    Animal** newAnimals = new Animal*[numAnimals - 1];

   
    for (unsigned int i = 0, j = 0; i < numAnimals; ++i) {
        if (i != indexToRemove) {
            newAnimals[j++] = animals[i];
        }
    }

    delete animals[indexToRemove];

 
    delete[] animals;
    animals = newAnimals;
    numAnimals--;


    return true;
}

// TASK 4.5: AdoptionCenter::incrementAge()
// Increment the age of all animals by 1.
void AdoptionCenter::incrementAge() {
    // Loop through all animals and increment their age
    for (unsigned int i = 0; i < numAnimals; ++i) {
        animals[i]->incrementAge();
    }
}

// TASK 4.6: AdoptionCenter::setAnimalHealthCondition(unsigned int, const HealthCondition&)
// Modify the animal with the specified ID's health condition to the provided parameter.
// If the animal does not exist, do nothing.
// Else, ensure the BST for health condition is sorted after the modification.
void AdoptionCenter::setAnimalHealthCondition(unsigned int id, const HealthCondition& h) {
 
    Animal* animalToUpdate = nullptr;
    for (unsigned int i = 0; i < numAnimals; ++i) {
        if (animals[i]->getID() == id) {
            animalToUpdate = animals[i];
            break;
        }
    }

   
    if (animalToUpdate == nullptr) {
        return;
    }


    sortedAnimals[HEALTH].remove(animalToUpdate);

    
    animalToUpdate->setHealthCondition(h);

    sortedAnimals[HEALTH].insert(animalToUpdate);
}

// TASK 4.7: AdoptionCenter:addAnimalVaccine(unsigned int, const string&)
// Add the provided vaccine to the animal specified by ID.
// If the animal does not exist, do nothing.
// Else, ensure the BST for vaccine status is sorted after the modification.
void AdoptionCenter::addAnimalVaccine(unsigned int id, const string& v) {
   
    Animal* animalToUpdate = nullptr;
    for (unsigned int i = 0; i < numAnimals; ++i) {
        if (animals[i]->getID() == id) {
            animalToUpdate = animals[i];
            break;
        }
    }

  
    if (animalToUpdate == nullptr) {
        return;
    }

    sortedAnimals[VACCINE].remove(animalToUpdate);

  
    animalToUpdate->addVaccine(v);

    sortedAnimals[VACCINE].insert(animalToUpdate);
}

// TASK 4.8: AdoptionCenter::setAnimalSpecialNeeds(unsigned int, const std::string&)
// Modify the animal with the specified ID's special needs to the provided parameter.
// If the animal does not exist, do nothing.
void AdoptionCenter::setAnimalSpecialNeeds(unsigned int id, const std::string& n) {

    for (unsigned int i = 0; i < numAnimals; ++i) {
        if (animals[i]->getID() == id) {
            
            animals[i]->setSpecialNeeds(n);
            return;  
        }
    }

   
}


/**
 * Provided function to display the animals in the database.
 * @param start: Starting index to print (e.g. start == 5: ignore the first 5 animals that matches the filter)
 * @param stop: Index to stop printing (e.g. stop == 10: print until the 10th animal [exclusive] that matches the filter)
 * @param filter: Only print animals that match this filter.
 * @param criteria: Sorting criteria. By default sort by ID (assuming animals are added in increasing ID)
*/
void AdoptionCenter::display(unsigned int start, unsigned int stop, const Filter& filter, SortCriteria criteria) const {
    unsigned int displayCount = stop - start;
    if (criteria == ID) {
        for (unsigned int i=0; i<numAnimals; ++i) {
            if (filter.match(*animals[i])) {
                animals[i]->display(start, displayCount);
            }
        }
    }
    else {
        sortedAnimals[criteria].print(start, displayCount, filter);
    }
}

/**
 * Provided function to print the vaccines an animal, specified by ID, has taken.
*/
void AdoptionCenter::displayPetVaccines(unsigned int id) const
{
    unsigned int i = 0;
    for (; i<numAnimals; ++i) {
        if (animals[i]->getID() == id) {
            VaccinationStatus v = animals[i]->getVaccinationStatus();
            if (v.numVaccines == 0)
                cout << "No vaccines found." << endl;
            else
                for (unsigned int j = 0; j < VACCINE_TABLE_SIZE; ++j) {
                    if (v.vaccineHashTable[j].length() > 0)
                        cout << v.vaccineHashTable[j] << endl;
                }
            return;
        }
    }
    if (i == numAnimals)
        cout << "ID not found in database." << endl;
}
