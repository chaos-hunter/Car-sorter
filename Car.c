#include "headerA3.h"

int main() {
    //variable declaration
    // Declaring the head of the linked list
    a3Car *headLL;
    headLL = NULL; // Initializing the head of the linked list to NULL
    int choice;
    int position;
    int carId;
    char filename[MAX_LENGTH];
    char modelType[100];
    char confirm;

    while (1) {
        // Display menu options
        printf("\nMenu:\n");
        printf("1. Add data on a new car\n");
        printf("2. Load data on cars from a given text file\n");
        printf("3. Print data of all cars\n");
        printf("4. Print data of the nth car\n");
        printf("5. Search car data based on carId\n");
        printf("6. Search car data based on model and type\n");
        printf("7. Count the total number of cars in the list\n");
        printf("8. Sort the cars in the list based on carId\n");
        printf("9. Remove data of the nth car\n");
        printf("10. Remove all car data\n");
        printf("11. Exit\n");
        printf("Enter your option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addNewCar(&headLL);
                break;
            case 2:
                printf("Enter file name: ");
                scanf("%s", filename);
                loadCarData(&headLL, filename);
                break;
            case 3:
                printAll(headLL);
                break;
            case 4:
                printf("Enter the position of the car: ");
                scanf("%d", &position);
                printOne(headLL, position);
                break;
            case 5:
                printf("Enter a car ID: ");
                scanf("%d", &carId);
                position = lookForCarId(headLL, carId);
                if (position != -1) {
                    printf("Car found at position: %d\n", position);
                } else {
                    printf("Car with ID %d not found.\n", carId);
                }
                break;
            case 6:
                printf("Enter the model and type (separated by a space): ");
                scanf("%s %s", modelType, modelType + strlen(modelType) + 1); // Simple way to concatenate model and type
                position = lookForCarModelType(headLL, modelType);
                if (position != -1) {
                    printf("Car found at position: %d\n", position);
                } else {
                    printf("Car with model and type %s not found.\n", modelType);
                }
                break;
            case 7:
                printf("Total number of cars: %d\n", countCars(headLL));
                break;
            case 8:
                sortCarId(&headLL);
                printf("Cars have been sorted by carId.\n");
                break;
            case 9:
                printf("Enter the position of the car to remove: ");
                scanf("%d", &position);
                oneLessCar(&headLL, position);
                break;
            case 10:

                printf("Are you sure you want to remove all car data (y/n)? ");
                scanf(" %c", &confirm); // Notice the leading space to consume any leftover newline character
                if (confirm == 'y' || confirm == 'Y') {
                    noMoreCars(&headLL);
                } else {
                    printf("Operation canceled.\n");
                }
                break;
            case 11:
                printf("Exiting program.\n");
                noMoreCars(&headLL); // Clean up before exiting
                return 0;
            default:
                printf("Invalid choice. Please enter a number between 1 and 11.\n");
        }
    }
    return 0; // Should never reach here, but included to prevent compiler warnings
}


void addNewCar(struct car **headLL) {
    struct car *newCar = (struct car *) malloc(sizeof(struct car));
    if (newCar == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Prompt user for car details
    printf("Enter the car model: ");
    scanf("%24s", newCar->model);
    printf("Enter the car type: ");
    scanf("%24s", newCar->type);
    printf("Enter its year of manufacture: ");
    scanf("%d", &newCar->year);
    printf("Enter its price: ");
    scanf("%lf", &newCar->price);

    // Generate carId
    newCar->carId = 0;
    for (int i = 0; newCar->model[i] != '\0'; i++) {
        newCar->carId += newCar->model[i];
    }
    newCar->carId += strlen(newCar->type);

    // Ensure carId is unique
    struct car *temp = *headLL;
    while (temp != NULL) {
        if (temp->carId == newCar->carId) {
            newCar->carId += rand() % 999 + 1; // Add random number between 1 and 999
            temp = *headLL; // Start checking again
        } else {
            temp = temp->nextCar;
        }
    }

    // Adding the new car to the end of the linked list
    if (*headLL == NULL) {
        // If the list is empty, set newCar as the head
        *headLL = newCar;
    } else {
        // Traverse to the end of the list and append newCar
        temp = *headLL;
        while (temp->nextCar != NULL) {
            temp = temp->nextCar;
        }
        temp->nextCar = newCar;
    }
    newCar->nextCar = NULL;

    printf("Your computer-generated carId is %d\n", newCar->carId);
}

void loadCarData(struct car **headLL, char filename[MAX_LENGTH]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        struct car *newCar = (struct car *)malloc(sizeof(struct car));
        if (newCar == NULL) {
            printf("Memory allocation failed!\n");
            continue; // Skip this iteration
        }

        // Assuming the file format is: model,type,year,price
        sscanf(line, "%24[^,],%24[^,],%d,%lf", newCar->model, newCar->type, &newCar->year, &newCar->price);

        // Generate and adjust carId similarly as in addNewCar function
        newCar->carId = 0;
        for (int i = 0; newCar->model[i] != '\0'; i++) {
            newCar->carId += newCar->model[i];
        }
        newCar->carId += strlen(newCar->type);
        // Ensure carId is unique
        struct car *temp = *headLL;
        while (temp != NULL) {
            if (temp->carId == newCar->carId) {
                newCar->carId += rand() % 999 + 1;
                temp = *headLL;
            } else {
                temp = temp->nextCar;
            }
        }

        // Add to list
        if (*headLL == NULL) {
            *headLL = newCar;
        } else {
            struct car *current = *headLL;
            while (current->nextCar != NULL) {
                current = current->nextCar;
            }
            current->nextCar = newCar;
        }
        newCar->nextCar = NULL;

        printf("Loaded car with carId: %d\n", newCar->carId);
    }

    fclose(file);
    printf("All cars loaded from %s\n", filename);
}

void printAll(struct car *headLL) {
    if (headLL == NULL) {
        printf("No cars found.\n");
        return;
    }

    int carNumber = 1;
    struct car *current = headLL;
    while (current != NULL) {
        printf("Car # %d:\n", carNumber++);
        printf("Car id: %d\n", current->carId);
        printf("Model: %s\n", current->model);
        printf("Type: %s\n", current->type);
        printf("Price: CDN $%.2f\n", current->price); // Price is printed as is, advanced formatting with commas not included in standard C
        printf("Year of Manufacture: %d\n\n", current->year);
        current = current->nextCar;
    }
}

void printOne(struct car *headLL, int whichOne) {
    if (headLL == NULL) {
        printf("No cars found.\n");
        return;
    }

    int position = 1;
    struct car *current = headLL;

    // Traverse to the nth position
    while (current != NULL && position < whichOne) {
        current = current->nextCar;
        position++;
    }

    // Check if we found the nth car
    if (current == NULL) {
        printf("Car at position %d does not exist.\n", whichOne);
    } else {
        // Print details of the nth car
        printf("Car id: %d\n", current->carId);
        printf("Model: %s\n", current->model);
        printf("Type: %s\n", current->type);
        printf("Price: CDN $%.2f\n", current->price);
        printf("Year of Manufacture: %d\n", current->year);
    }
}

int lookForCarId(struct car *headLL, int key) {
    int position = 1; // Starting position
    struct car *current = headLL;

    while (current != NULL) {
        if (current->carId == key) {
            return position; // Car found, return its position
        }
        current = current->nextCar; // Move to the next car in the list
        position++;
    }

    return -1; // Car not found
}
//still needs work
int lookForCarModelType(struct car *headLL, char key[100]) {
    int position = 1; // Initialize position counter
    struct car *current = headLL;

    while (current != NULL) {
        char modelTypeConcat[200]; // Buffer to hold concatenated model and type
        // Assuming model and type are separated by a space in the 'key'
        sprintf(modelTypeConcat, "%s %s", current->model, current->type);

        if (strcmp(modelTypeConcat, key) == 0) {
            return position; // Match found, return position
        }

        current = current->nextCar; // Move to the next car in the list
        position++;
    }

    return -1; // No match found
}

int countCars(a3Car *headLL) {
    int totalCars = 0; // Counter for the cars
    a3Car *current = headLL; // Start with the head of the list

    // Traverse the list
    while (current != NULL) {
        totalCars++; // Increment counter for each car
        current = current->nextCar; // Move to the next node
    }

    return totalCars; // Return the total count
}

void sortCarId(struct car **headLL) {
    int swapped;
    struct car *ptr1;
    struct car *lptr = NULL;

    // Checking for empty list or list with one element
    if (*headLL == NULL || (*headLL)->nextCar == NULL) {
        return;
    }

    do {
        swapped = 0;
        ptr1 = *headLL;

        while (ptr1->nextCar != lptr) {
            if (ptr1->carId > ptr1->nextCar->carId) {
                // Swapping the nodes by adjusting pointers
                struct car *tmp = ptr1->nextCar;
                ptr1->nextCar = tmp->nextCar;
                tmp->nextCar = ptr1;

                // Adjusting the previous node's next or headLL pointer
                if (ptr1 == *headLL) *headLL = tmp;
                else {
                    struct car *findPrev = *headLL;
                    while (findPrev->nextCar != ptr1) findPrev = findPrev->nextCar;
                    findPrev->nextCar = tmp;
                }

                ptr1 = tmp; // Correcting the current pointer after swap
                swapped = 1;
            }
            ptr1 = ptr1->nextCar;
        }
        lptr = ptr1;
    } while (swapped);
}

void oneLessCar(a3Car **headLL, int whichOne) {
    if (*headLL == NULL) {
        printf("The list is empty.\n");
        return;
    }

    a3Car *temp = *headLL;
    a3Car *prev = NULL;
    int currentPosition = 1;

    // Handling removal of the first car
    if (whichOne == 1) {
        *headLL = temp->nextCar; // Change head
        free(temp); // Free old head
        return;
    }

    // Traverse to find the nth car
    while (temp != NULL && currentPosition < whichOne) {
        prev = temp;
        temp = temp->nextCar;
        currentPosition++;
    }

    // If position is more than the number of cars
    if (temp == NULL) {
        printf("The car at position %d does not exist.\n", whichOne);
        return;
    }

    // Unlink the nth car from the list
    prev->nextCar = temp->nextCar;

    free(temp); // Free memory of the nth car

    printf("Car at position %d has been removed.\n", whichOne);
}

void noMoreCars(a3Car **headLL) {
    a3Car *current = *headLL;
    a3Car *next;

    while (current != NULL) {
        // Store reference to the next car
        next = current->nextCar;
        // Free the current node
        free(current);
        // Move to the next node
        current = next;
    }
    // Reset the head of the list to indicate it's empty
    *headLL = NULL;

    printf("All car data removed. The list is now empty.\n");
}
