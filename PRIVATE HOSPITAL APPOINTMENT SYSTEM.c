#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CONSTANTS
#define MAX_QUEUE 50 // maximum patients in a single queue
#define MAX_TIME_SLOTS 20 // total time slots per day (7:30 AM to 5:00 PM)
#define TOTAL_TYPES 7 // number of checkup types
#define MAX_WAITING (MAX_QUEUE * 3) // maximum total waiting patients

// ARRAY
// Array of all checkup types offered in the hospital
char checkupTypes[TOTAL_TYPES][20] = {
    "General","Laboratory","Dental",
    "Ultrasound","X-ray","Pediatric","Cardiology"
};

// Array of doctors assigned to each checkup type
char doctorNames[TOTAL_TYPES][30] = {
    "Dr. Santos","Dr. Reyes","Dr. Gomez",
    "Dr. Ramirez","Dr. Fernandez","Dr. Cruz","Dr. Villanueva"
};

// Array of time slots available
char timeSlots[MAX_TIME_SLOTS][12] = {
    "7:30 AM","8:00 AM","8:30 AM","9:00 AM","9:30 AM","10:00 AM","10:30 AM",
    "11:00 AM","11:30 AM","12:00 PM","12:30 PM","1:00 PM","1:30 PM",
    "2:00 PM","2:30 PM","3:00 PM","3:30 PM","4:00 PM","4:30 PM","5:00 PM"
};

// This enables reuse of cancelled slots when cancelled, set back to 0
int isSlotTaken[TOTAL_TYPES][MAX_TIME_SLOTS];

// Holds all information for a single appointment.
typedef struct {
    char id[10];
    char name[50];
    char sex[5];
    int age; 
    int typeIndex;
    int priority;
    int roomNumber;
    char time[12];
    int timeSlotIndex;
} Patient;

// QUEUE
// Three independent queues to handle different priorities: emergencyQueue – highest priority, seniorQueue – medium priority, normalQueue – lowest priority
typedef struct {
    Patient items[MAX_QUEUE];   // array holding patient records
    int front;                  // index of the first patient
    int rear;                   // index of the last patient
} Queue;

// Create the three priority queues
Queue emergencyQueue;
Queue seniorQueue;
Queue normalQueue;

// LINKED LIST
// Stores served patients in a singly linked list and each node contains a Patient record and a pointer to the next node
typedef struct HistoryNode {
    Patient data;
    struct HistoryNode *next;
} HistoryNode;
HistoryNode *historyHead = NULL;

// ID COUNTERS, auto-increment counters for each priority 
int emergencyIdCounter = 1;
int seniorIdCounter = 1;
int normalIdCounter = 1;

// UTILITY FUNCTIONS
// Removes leftover newline from previous input to prevent fgets from skipping
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Checks if a single character is an English letter A-Z or a-z
int isLetter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// Validates a patient's name: only letters, spaces, and periods are allowed
int isValidName(char *name) {
    int i = 0;
    while (name[i]) {
        if (!(isLetter(name[i]) || name[i] == ' ' || name[i] == '.'))
            return 0;
        i++;
    }
    return 1;
}

//QUEUE OPERATIONS
// Resets the queue to empty state front=0, rear=-1
void resetQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
}

// Move waiting patients forward to fill empty space at the front of the queue
void compactQueue(Queue *q) {
    if (q->front > 0) {
        int i, j = 0;
        for (i = q->front; i <= q->rear; i++)
            q->items[j++] = q->items[i];
        q->rear = j - 1;
        q->front = 0;
    }
}

// Initialises a queue and sets to empty state
void initQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
}

// returns 1 if the queue has no patients or empty, and 0 if it has at least one patient
int isQueueEmpty(Queue *q) {
    return q->rear < q->front;
}

// returns 1 if the queue has reached its maximum capacity or full, and 0 if there is still a space
int isQueueFull(Queue *q) {
    return q->rear == MAX_QUEUE - 1;
}

// Adds patient to the end, if queue is full but has empty space at front, moves patients forward to make room
void enqueue(Queue *q, Patient p) {
    if (isQueueFull(q) && q->front > 0)
        compactQueue(q);
    if (isQueueFull(q)) {
        printf("Queue overflow, cannot add more patients.\n");
        return;
    }
    q->rear++;
    q->items[q->rear] = p;
}

// Calls the next patient from the waiting line into the doctor's room and removes them from the queue
Patient dequeue(Queue *q) {
    Patient empty = {"NONE"};
    if (isQueueEmpty(q)) return empty;
    empty = q->items[q->front];
    q->front++;
    if (q->front > q->rear) resetQueue(q);
    return empty;
}

// Returns the patient at the front without removing it
Patient peekFront(Queue *q) {
    Patient empty = {"NONE"};
    if (!isQueueEmpty(q))
        empty = q->items[q->front];
    return empty;
}

// Removes a specific patient from the queue by their ID
int removePatientFromQueue(Queue *q, char *id) {
    int i, j;
    if (isQueueEmpty(q)) return 0;
    for (i = q->front; i <= q->rear; i++)
        if (strcmp(q->items[i].id, id) == 0) break;
    if (i > q->rear) return 0;
    for (j = i; j < q->rear; j++)
        q->items[j] = q->items[j+1];
    q->rear--;
    if (q->front > q->rear) resetQueue(q);
    return 1;
}

// HISTORY (LINKED LIST)
// Adds a served patient to the history list, the new node becomes the most recent served patient first
void addToHistory(Patient p) {
    HistoryNode *newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
    newNode->data = p;
    newNode->next = historyHead;
    historyHead = newNode;
}

// Frees all memory allocated for the history linked list
void freeHistoryList(void) {
    HistoryNode *curr = historyHead;
    while (curr) {
        HistoryNode *temp = curr;
        curr = curr->next;
        free(temp);
    }
    historyHead = NULL;
}

// Displays the history of served patients 
void viewHistoryList(void) {
    HistoryNode *curr = historyHead;
    if (!curr) {
        printf("No patient history yet.\n");
        return;
    }
    printf("\n======================= SERVED PATIENT HISTORY ======================\n");
    printf("%-8s | %-20s | %-5s | %-12s | %s\n", "ID", "Name", "Sex", "Type", "Doctor");
    printf("---------------------------------------------------------------------\n");
    while (curr) {
        printf("%-8s | %-20s | %-5s | %-12s | %s\n",
               curr->data.id, curr->data.name, curr->data.sex,
               checkupTypes[curr->data.typeIndex],
               doctorNames[curr->data.typeIndex]);
        curr = curr->next;
    }
}

// Generates a unique ID based on priority
void generatePatientId(Patient *p) {
    if (p->priority == 1)
        sprintf(p->id, "A%03d", emergencyIdCounter++);
    else if (p->priority == 2)
        sprintf(p->id, "B%03d", seniorIdCounter++);
    else
        sprintf(p->id, "C%03d", normalIdCounter++);
}

// Assigns a time slot to a non-emergency patient
// Emergency patients automatically get "ANYTIME" and do not consume a slot
int assignSlot(Patient *p) {
    if (p->priority == 1) {
        strcpy(p->time, "ANYTIME");
        p->timeSlotIndex = -1;
        return 1;
    }
    int slot;
    for (slot = 0; slot < MAX_TIME_SLOTS; slot++) {
        if (isSlotTaken[p->typeIndex][slot] == 0) {
            isSlotTaken[p->typeIndex][slot] = 1;
            strcpy(p->time, timeSlots[slot]);
            p->timeSlotIndex = slot;
            return 1;
        }
    }
    strcpy(p->time, "FULL");
    p->timeSlotIndex = -1;
    return 0;
}

// Frees a time slot when an appointment is cancelled
// Sets isSlotTaken[type][slotIndex] back to 0, making the slot available for future bookings
void freeSlot(int type, int slotIndex) {
    if (type >= 0 && type < TOTAL_TYPES && slotIndex >= 0 && slotIndex < MAX_TIME_SLOTS)
        isSlotTaken[type][slotIndex] = 0;
}

// Reads an integer from the user within a specified range and keeps asking until a valid integer is entered
int getValidInt(int minVal, int maxVal, const char *prompt) {
    int value, result;
    while (1) {
        printf("%s", prompt);
        result = scanf("%d", &value);
        if (result != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        if (value < minVal || value > maxVal) {
            printf("Please enter a value between %d and %d.\n", minVal, maxVal);
            continue;
        }
        return value;
    }
}

// Reads a patient's name and validates it letters, spaces, periods only and repeats until a valid name is entered
void getValidName(char *buffer, int size) {
    int len;
    while (1) {
        printf("\nEnter full name: ");
        if (fgets(buffer, size, stdin) == NULL) continue;
        len = 0;
        while (buffer[len] != '\0' && buffer[len] != '\n') len++;
        if (buffer[len] == '\n') buffer[len] = '\0';
        else clearInputBuffer();
        if (len == 0) {
            printf("Name cannot be empty.\n");
            continue;
        }
        if (!isValidName(buffer)) {
            printf("Invalid name. Use only letters, spaces, and periods.\n");
            continue;
        }
        break;
    }
}

// Reads the patient's sex
void getValidSex(char *sex) {
    char input[10];
    while (1) {
        printf("Enter sex (M/F): ");
        if (fgets(input, sizeof(input), stdin) == NULL) continue;
        input[strcspn(input, "\n")] = '\0';
        if (input[0] == 'M' || input[0] == 'm') {
            strcpy(sex, "M");
            break;
        } else if (input[0] == 'F' || input[0] == 'f') {
            strcpy(sex, "F");
            break;
        } else {
            printf("Invalid sex. Please enter M or F.\n");
        }
    }
}

// BOOK APPOINTMENT
// Collects the patients name, sex, age, checkup type, priority and assign an ID and room
void bookAppointment(void) {
    Patient newPatient;
    int i, confirm;

    getValidName(newPatient.name, sizeof(newPatient.name));
    getValidSex(newPatient.sex);
    newPatient.age = getValidInt(0, 120, "Enter age: ");

    printf("\n--- Checkup Types ---\n");
    for (i = 0; i < TOTAL_TYPES; i++)
        printf("%d. %s\n", i+1, checkupTypes[i]);
    newPatient.typeIndex = getValidInt(1, TOTAL_TYPES, "Choice: ") - 1;

    printf("\n--- Priority ---\n");
    printf("1. Emergency\n2. Senior / Pregnant\n3. Normal\n");
    newPatient.priority = getValidInt(1, 3, "Choice: ");

    generatePatientId(&newPatient);
    newPatient.roomNumber = newPatient.typeIndex + 1;
    if (!assignSlot(&newPatient)) {
        printf("\nNo available time slot for %s. Booking cancelled.\n", checkupTypes[newPatient.typeIndex]);
        return;
    }

    printf("\n--- Booking Summary ---\n");
    printf("ID     : %s\n", newPatient.id);
    printf("Name   : %s\n", newPatient.name);
    printf("Sex    : %s\n", newPatient.sex);
    printf("Age    : %d\n", newPatient.age);
    printf("Type   : %s\n", checkupTypes[newPatient.typeIndex]);
    printf("Doctor : %s\n", doctorNames[newPatient.typeIndex]);
    printf("Room   : %d\n", newPatient.roomNumber);
    printf("Time   : %s\n", newPatient.time);

    confirm = getValidInt(0, 1, "\nConfirm booking? (1=Yes, 0=No): ");
    if (confirm == 1) {
        if (newPatient.priority == 1)
            enqueue(&emergencyQueue, newPatient);
        else if (newPatient.priority == 2)
            enqueue(&seniorQueue, newPatient);
        else
            enqueue(&normalQueue, newPatient);
        printf("\nAPPOINTMENT BOOKED SUCCESSFULLY!\n");
    } else {
        printf("\nBooking cancelled.\n");
    }
}

//SERVE NEXT PATIENT
// Serves the next patient according to priority order
Patient serveNextPatient(void) {
    if (!isQueueEmpty(&emergencyQueue))
        return dequeue(&emergencyQueue);
    if (!isQueueEmpty(&seniorQueue))
        return dequeue(&seniorQueue);
    if (!isQueueEmpty(&normalQueue))
        return dequeue(&normalQueue);
    Patient empty = {"NONE"};
    return empty;
}

// LINEAR SEARCH
// Searches for a patient by ID across all three active queues
Patient* findPatientById(char *id) {
    int i;
    for (i = emergencyQueue.front; i <= emergencyQueue.rear; i++)
        if (strcmp(emergencyQueue.items[i].id, id) == 0)
            return &emergencyQueue.items[i];
    for (i = seniorQueue.front; i <= seniorQueue.rear; i++)
        if (strcmp(seniorQueue.items[i].id, id) == 0)
            return &seniorQueue.items[i];
    for (i = normalQueue.front; i <= normalQueue.rear; i++)
        if (strcmp(normalQueue.items[i].id, id) == 0)
            return &normalQueue.items[i];
    return NULL;
}

//VIEW MY APPOINTMENT
// Allows a patient to view their own appointment by entering their ID
void viewMyAppointment(void) {
    char id[10];
    Patient *p;
    printf("Enter your ID: ");
    scanf("%9s", id);
    clearInputBuffer();
    p = findPatientById(id);
    if (!p) {
        printf("No appointment found with ID %s.\n", id);
        return;
    }
    printf("\n--- YOUR APPOINTMENT ---\n");
    printf("ID      : %s\n", p->id);
    printf("Name    : %s\n", p->name);
    printf("Sex     : %s\n", p->sex);
    printf("Age     : %d\n", p->age);
    printf("Type    : %s\n", checkupTypes[p->typeIndex]);
    printf("Doctor  : %s\n", doctorNames[p->typeIndex]);
    printf("Room    : %d\n", p->roomNumber);
    printf("Time    : %s\n", p->time);
    printf("Priority: %s\n", (p->priority == 1) ? "Emergency" :
                             (p->priority == 2) ? "Senior/Pregnant" : "Normal");
}

// CANCEL APPOINTMENT (FIXED: copy slot info before removal)
void cancelAppointment(void) {
    char id[10];
    Patient *p;
    int confirm, removed;
    int savedTypeIndex, savedTimeSlotIndex;

    printf("Enter ID to cancel: ");
    scanf("%9s", id);
    clearInputBuffer();
    p = findPatientById(id);
    if (!p) {
        printf("No appointment found with ID %s.\n", id);
        return;
    }

    printf("\n--- Appointment to cancel ---\n");
    printf("ID   : %s\n", p->id);
    printf("Name : %s\n", p->name);
    printf("Type : %s\n", checkupTypes[p->typeIndex]);
    printf("Time : %s\n", p->time);

    confirm = getValidInt(0, 1, "\nAre you sure you want to cancel? (1=Yes, 0=No): ");
    if (confirm == 1) {
        // Save slot info before removing patient (pointer becomes invalid after removal)
        savedTypeIndex = p->typeIndex;
        savedTimeSlotIndex = p->timeSlotIndex;

        removed = 0;
        if (p->priority == 1)
            removed = removePatientFromQueue(&emergencyQueue, id);
        else if (p->priority == 2)
            removed = removePatientFromQueue(&seniorQueue, id);
        else
            removed = removePatientFromQueue(&normalQueue, id);

        if (removed) {
            // Now free the slot using the saved values
            if (p->priority != 1 && savedTimeSlotIndex >= 0)
                freeSlot(savedTypeIndex, savedTimeSlotIndex);
            printf("Appointment %s has been cancelled.\n", id);
        } else {
            printf("Failed to cancel (patient may have been served).\n");
        }
    } else {
        printf("Cancellation aborted.\n");
    }
}

// NOW SERVING + NEXT PATIENTS
// Displays the patient currently being served and the three next patients
void showNowServing(void) {
    printf("\n================== NOW SERVING + NEXT PATIENTS ==================\n");

    // Determine the highest priority non-empty queue
    Queue *currentQ = NULL;
    const char *priorityLabel = "";
    if (!isQueueEmpty(&emergencyQueue)) {
        currentQ = &emergencyQueue;
        priorityLabel = "Emergency";
    } else if (!isQueueEmpty(&seniorQueue)) {
        currentQ = &seniorQueue;
        priorityLabel = "Senior/Pregnant";
    } else if (!isQueueEmpty(&normalQueue)) {
        currentQ = &normalQueue;
        priorityLabel = "Normal";
    } else {
        printf("No patients in queue.\n");
        return;
    }

    Patient current = currentQ->items[currentQ->front];
    printf("\nNOW SERVING:\n");
    printf("%-10s | %-25s | %-6s | %-15s\n", "ID", "Name", "Room", "Type");
    printf("-----------+---------------------------+--------+---------------\n");
    printf("%-10s | %-25s | %-6d | %-15s\n", 
           current.id, current.name, current.roomNumber, checkupTypes[current.typeIndex]);

    // Collect next patients (up to 3) from all queues, skipping the current patient.
    Patient nextList[10];
    int nextCount = 0;
    int i;

    for (i = emergencyQueue.front; i <= emergencyQueue.rear && nextCount < 3; i++) {
        if (strcmp(emergencyQueue.items[i].id, current.id) == 0) continue;
        nextList[nextCount++] = emergencyQueue.items[i];
    }
    if (nextCount < 3) {
        for (i = seniorQueue.front; i <= seniorQueue.rear && nextCount < 3; i++) {
            if (strcmp(seniorQueue.items[i].id, current.id) == 0) continue;
            nextList[nextCount++] = seniorQueue.items[i];
        }
    }
    if (nextCount < 3) {
        for (i = normalQueue.front; i <= normalQueue.rear && nextCount < 3; i++) {
            if (strcmp(normalQueue.items[i].id, current.id) == 0) continue;
            nextList[nextCount++] = normalQueue.items[i];
        }
    }

    printf("\nNEXT PATIENTS:\n");
    if (nextCount == 0) {
        printf("   No other patients waiting.\n");
    } else {
        printf("%-10s | %-25s | %-6s | %-15s\n", "ID", "Name", "Room", "Type");
        printf("-----------+---------------------------+--------+---------------\n");
        for (i = 0; i < nextCount; i++) {
            printf("%-10s | %-25s | %-6d | %-15s\n",
                   nextList[i].id, nextList[i].name, 
                   nextList[i].roomNumber, checkupTypes[nextList[i].typeIndex]);
        }
    }
}

// MERGE SORT
// Sorts an array of patients by their checkup types in ascending order.
void merge(Patient arr[], int left, int mid, int right) {
    Patient temp[MAX_WAITING];   // temporary array sized to hold all possible waiting patients
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right)
        temp[k++] = (arr[i].typeIndex <= arr[j].typeIndex) ? arr[i++] : arr[j++];
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (i = left, k = 0; i <= right; i++, k++)
        arr[i] = temp[k];
}

// Divide the list into halves, sorts each half, then merges back in sorted order
void mergeSort(Patient arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// VIEW ALL APPOINTMENTS
// Displays all waiting patients from the three queues then grouped by checkup type
void viewAllAppointments(void) {
    Patient allPatients[MAX_WAITING];
    int totalCount = 0, i, t, printed;

    for (i = emergencyQueue.front; i <= emergencyQueue.rear; i++)
        allPatients[totalCount++] = emergencyQueue.items[i];
    for (i = seniorQueue.front; i <= seniorQueue.rear; i++)
        allPatients[totalCount++] = seniorQueue.items[i];
    for (i = normalQueue.front; i <= normalQueue.rear; i++)
        allPatients[totalCount++] = normalQueue.items[i];

    if (totalCount == 0) {
        printf("No waiting patients.\n");
        return;
    }

    mergeSort(allPatients, 0, totalCount - 1);

    printf("\n===================== ALL WAITING APPOINTMENTS =====================\n");
    for (t = 0; t < TOTAL_TYPES; t++) {
        printed = 0;
        for (i = 0; i < totalCount; i++) {
            if (allPatients[i].typeIndex == t) {
                if (!printed) {
                    printf("\n---------------------------- %s ----------------------------\n", checkupTypes[t]);
                    printf("%-10s | %-25s | %-5s | %-6s | %-8s\n",
                           "ID", "Name", "Sex", "Room", "Time");
                    printf("-----------+---------------------------+-------+--------+-----------\n");
                    printed = 1;
                }
                printf("%-10s | %-25s | %-5s | %-6d | %-8s\n",
                       allPatients[i].id, allPatients[i].name, allPatients[i].sex,
                       allPatients[i].roomNumber, allPatients[i].time);
                printf("\n");
            }
        }
    }
}

// MENU SYSTEMS
// Patient menu, displays options and runs the called functions
void patientMenu(void) {
    int choice;
    while (1) {
        printf("\n============= PATIENT MENU =============\n");
        printf("1. Book Appointment\n2. View My Appointment\n3. Cancel Appointment\n4. Logout\n");
        choice = getValidInt(1, 4, "Enter choice: ");
        switch (choice) {
            case 1: bookAppointment(); break;
            case 2: viewMyAppointment(); break;
            case 3: cancelAppointment(); break;
            case 4: return;
        }
    }
}

// Admin menu, displays admin options and runs the called functions
void adminMenu(void) {
    int choice;
    while (1) {
        printf("\n============= ADMIN MENU =============\n");
        printf("1. View All Appointments\n2. Now Serving + Next 3\n3. Serve Next Patient\n4. View History\n5. Logout\n");
        choice = getValidInt(1, 5, "Enter choice: ");
        switch (choice) {
            case 1: viewAllAppointments(); break;
            case 2: showNowServing(); break;
            case 3: {
                Patient served = serveNextPatient();
                if (strcmp(served.id, "NONE") == 0)
                    printf("No patients to serve.\n");
                else {
                    printf("\nServing patient: %s | %s | %s | Room %d\n", served.name, served.id, checkupTypes[served.typeIndex], served.roomNumber);
                    addToHistory(served);
                }
                break;
            }
            case 4: viewHistoryList(); break;
            case 5: return;
        }
    }
}

// MAIN
// Enters the main role selection 
int main(void) {
    int i, j;
    // Mark all time slots as free (0) for every checkup type
    for (i = 0; i < TOTAL_TYPES; i++)
        for (j = 0; j < MAX_TIME_SLOTS; j++)
            isSlotTaken[i][j] = 0;

    initQueue(&emergencyQueue);
    initQueue(&seniorQueue);
    initQueue(&normalQueue);

    int role;
    while (1) {
        printf("\n========================================\n");
        printf("       PRIVATE HOSPITAL SYSTEM\n");
        printf("========================================\n");
        printf("1. Admin\n2. Patient\n3. Exit\n");
        role = getValidInt(1, 3, "Enter role: ");
        switch (role) {
            case 1: adminMenu(); break;
            case 2: patientMenu(); break;
            case 3:
                freeHistoryList();
                printf("Exiting system. Goodbye!\n");
                return 0;
        }
    }
}
