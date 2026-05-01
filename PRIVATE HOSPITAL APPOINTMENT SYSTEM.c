#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* NODE (LINKED LIST - WAITING PATIENTS)
   Stores all patients in the hospital queue.
   Each node represents one patient record including:
   ID, name, age, gender, priority level, checkup type,
   assigned doctor, room number, and appointment time.
   next points to the next patient in the queue */
struct node {
    char id[10];
    char name[50];
    int age;
    char gender;
    int priority;
    char checkup[30];
    char doctor[30];
    int room;
    char time[10];
    struct node *next;
};

struct node *start = NULL;

/* HISTORY QUEUE
   Stores served patient IDs in FIFO order */
struct historyNode {
    char id[10];
    char name[50];
    char gender;
    int room;
    char time[10];
    char doctor[30];
    char checkup[30];
};
struct historyNode history[100];

int front = 0;
int rear = -1;

/* CHECKUP TYPES AND DOCTORS
   Fixed hospital services and assigned doctors */
char types[7][20] = {
    "General","Laboratory","Dental",
    "Ultrasound","X-ray","Pediatric","Cardiology"
};

char doctors[7][30] = {
    "Dr. Santos","Dr. Reyes","Dr. Gomez",
    "Dr. Ramirez","Dr. Fernandez","Dr. Cruz","Dr. Villanueva"
};

int countA=0,countB=0,countC=0;

/* ROOM SCHEDULE SYSTEM (NEW UPGRADE)
   Each room has its own independent time line */
int roomHour[10];
int roomMinute[10];

/* TIME SYSTEM INIT
   Each room starts at 7:30 independently */
void initRooms(){
    int i;
    for(i=0;i<10;i++){
        roomHour[i] = 7;
        roomMinute[i] = 30;
    }
}

/* TIME SYSTEM
   Generates 30-minute interval schedules PER ROOM */
void generateRoomTime(int room, char t[]){
    int h = roomHour[room];
    int m = roomMinute[room];

    sprintf(t, "%02d:%02d", h, m);

    m += 30;

    if(m >= 60){
        m = 0;
        h++;
    }

    if(h > 17){
        h = 7;
        m = 30;
    }

    roomHour[room] = h;
    roomMinute[room] = m;
}

/* CHECK DUPLICATE ID
   Prevents duplicate ticket IDs in the queue */
int isDuplicateID(char id[]){
    struct node *ptr = start;
    while(ptr != NULL){
        int i = 0;
        while(ptr->id[i] == id[i] && ptr->id[i] != '\0' && id[i] != '\0') i++;
        if(ptr->id[i] == id[i]) return 1;
        ptr = ptr->next;
    }
    return 0;
}

/* ROOM + TIME CONFLICT CHECK
   Prevents duplicate scheduling in same room and time slot */
int isRoomTimeConflict(int room, char time[]){
    struct node *ptr = start;

    while(ptr != NULL){
        if(ptr->room == room){
            int i = 0;
            while(ptr->time[i] == time[i] && ptr->time[i] != '\0' && time[i] != '\0')
                i++;

            if(ptr->time[i] == time[i])
                return 1;
        }
        ptr = ptr->next;
    }

    return 0;
}

/* INPUT VALIDATION - NUMBERS ONLY */
int getNumber() {
    char input[20];
    int i, num;

    while (1) {
        scanf("%s", input);

        for (i=0; input[i]!='\0'; i++) {
            if (input[i] < '0' || input[i] > '9') {
                printf("Numbers only: ");
                break;
            }
        }

        if (input[i] == '\0') {
            num = 0;
            for (i=0; input[i]!='\0'; i++)
                num = num*10 + (input[i]-'0');
            return num;
        }
    }
}

/* NAME INPUT VALIDATION */
void getName(char name[]) {
    int i, valid;
    char c;

    do {
        valid = 1;
        printf("Enter Name: ");
        scanf(" %[^\n]", name);

        for (i=0; name[i]!='\0'; i++) {
            c = name[i];
            if (!((c>='A'&&c<='Z')||(c>='a'&&c<='z')||c==' ')) {
                valid = 0;
                break;
            }
        }

        if (!valid) printf("Letters only\n");

    } while (!valid);
}

/* GENDER INPUT VALIDATION */
char getGender() {
    char g;
    while (1) {
        printf("Enter Gender (F/M): ");
        scanf(" %c",&g);

        if (g>='a'&&g<='z') g-=32;

        if (g=='F'||g=='M') return g;

        printf("Invalid\n");
    }
}

/* STRING COMPARISON */
int isEqual(char a[], char b[]) {
    int i=0;
    while (a[i]!='\0' || b[i]!='\0') {
        if (a[i]!=b[i]) return 0;
        i++;
    }
    return 1;
}

/* BOOK APPOINTMENT
   Handles patient registration and queue insertion */
void book() {
    struct node *new_node,*ptr;
    int choice,i;
    char ans,ticket[10];

    new_node=(struct node*)malloc(sizeof(struct node));

    printf("\n===== BOOK APPOINTMENT =====\n");

    getName(new_node->name);

    printf("Enter Age: ");
    while(1){
        new_node->age=getNumber();
        if(new_node->age>=1 && new_node->age<=120) break;
        printf("Invalid age: ");
    }

    new_node->gender=getGender();

    printf("\nSelect Check-up Type:\n");
    for(i=0;i<7;i++)
        printf("%d. %s\n",i+1,types[i]);

    while(1){
        printf("Enter choice: ");
        choice=getNumber();
        if(choice>=1 && choice<=7) break;
    }

    strcpy(new_node->checkup, types[choice-1]);
    strcpy(new_node->doctor, doctors[choice-1]);
    new_node->room = choice;

    printf("\nPriority:\n1. Emergency\n2. Senior/Pregnant\n3. Normal\n");

    while(1){
        printf("Enter: ");
        new_node->priority = getNumber();
        if(new_node->priority >= 1 && new_node->priority <= 3) break;
    }

    /* SENIOR / PREGNANT VERIFICATION */
    if(new_node->priority == 2){
        printf("Is patient Senior/Pregnant? (y/n): ");
        scanf(" %c", &ans);

        if(ans != 'y' && ans != 'Y'){
            new_node->priority = 3;
        }
    }

    /* TIME SYSTEM RULES
       Emergency bypasses schedule */
    if(new_node->priority == 1){
        strcpy(new_node->time, "ANY");
    } else {

        /* ROOM BASED INDEPENDENT SCHEDULING */
        do {
            generateRoomTime(new_node->room - 1, new_node->time);
        } while(isRoomTimeConflict(new_node->room, new_node->time));
    }

    printf("Confirm appointment? (y/n): ");
    scanf(" %c",&ans);
    if(ans!='y'&&ans!='Y'){
        free(new_node);
        return;
    }

    /* UNIQUE ID GENERATION */
    do {
        if(new_node->priority==1){countA++; sprintf(ticket,"A%03d",countA);}
        else if(new_node->priority==2){countB++; sprintf(ticket,"B%03d",countB);}
        else {countC++; sprintf(ticket,"C%03d",countC);}
    } while(isDuplicateID(ticket));

    strcpy(new_node->id, ticket);
    new_node->next=NULL;

    /* PRIORITY INSERTION */
    if(start==NULL || new_node->priority < start->priority){
        new_node->next=start;
        start=new_node;
    } else {
        ptr=start;
        while(ptr->next!=NULL && ptr->next->priority<=new_node->priority)
            ptr=ptr->next;

        new_node->next=ptr->next;
        ptr->next=new_node;
    }

    /* CONFIRMED APPOINTMENT TABLE */
    printf("\n========================= CONFIRMED APPOINTMENT ==========================\n");
    printf("%-10s %-25s %-20s %-8s %-10s\n", "ID", "DOCTOR", "CHECKUP", "ROOM", "TIME");
    printf("--------------------------------------------------------------------------\n");
    printf("%-10s %-25s %-20s %-8d %-10s\n",
        new_node->id,
        new_node->doctor,
        new_node->checkup,
        new_node->room,
        new_node->time);
    printf("--------------------------------------------------------------------------\n");
}

/* VIEW APPOINTMENT 
   Displays patient appoinntment*/
void viewMy(){
    struct node *ptr=start;
    char id[10];

    printf("Enter Ticket ID: ");
    scanf("%9s",id);

    while(ptr!=NULL){
        if(isEqual(ptr->id,id)){
            printf("\n========================= YOUR APPOINTMENT =========================\n");
            printf("%-10s %-20s %-6s %-20s %-25s %-10s\n","ID","NAME","ROOM","CHECKUP","DOCTOR","TIME");
            printf("--------------------------------------------------------------------------\n");
            printf("%-10s %-20s %-6d %-20s %-25s %-10s\n",
                ptr->id,ptr->name,ptr->room,ptr->checkup,ptr->doctor,ptr->time);
            printf("--------------------------------------------------------------------------\n");
            return;
        }
        ptr=ptr->next;
    }
    printf("Not found\n");
}

/* CANCEL APPOINTMENT 
   Removes/cancel patient from queue*/
void cancel(){
    struct node *ptr=start,*prev=NULL;
    char id[10],confirm;

    printf("Enter Ticket ID: ");
    scanf("%9s",id);

    while(ptr!=NULL && !isEqual(ptr->id,id)){
        prev=ptr;
        ptr=ptr->next;
    }

    if(ptr==NULL){printf("Not found\n");return;}

    printf("Confirm cancellation? (y/n): ");
    scanf(" %c",&confirm);

    if(confirm=='y'||confirm=='Y'){
        if(prev==NULL) start=ptr->next;
        else prev->next=ptr->next;

        free(ptr);
        printf("Appointment Cancelled\n");
    }
}

/* DISPLAY QUEUE 
   Shows all appointments*/
void display(){
    struct node *ptr=start;

    printf("\n========================= APPOINTMENT/S =========================\n");
    printf("%-10s %-20s %-6s %-10s %-10s\n", "ID", "NAME", "ROOM", "PRIORITY", "TIME");
    printf("--------------------------------------------------------------\n");

    while(ptr!=NULL){
        printf("%-10s %-20s %-6d %-10d %-10s\n",
            ptr->id,ptr->name,ptr->room,ptr->priority,ptr->time);
        ptr=ptr->next;
    }
    printf("--------------------------------------------------------------\n");
}

/* SERVE PATIENT 
   Moves patient to history*/
void serve(){
    struct node *ptr;
    int i;

    if(start == NULL){
        printf("Empty\n");
        return;
    }

    ptr = start;
    start = start->next;

    printf("NOW SERVING: %s (%s)\n", ptr->id, ptr->name);

    rear++;

    if(rear < 100){
        strcpy(history[rear].id, ptr->id);
        strcpy(history[rear].name, ptr->name);
        history[rear].gender = ptr->gender;
        strcpy(history[rear].doctor, ptr->doctor);
        strcpy(history[rear].checkup, ptr->checkup);
        history[rear].room = ptr->room;
        strcpy(history[rear].time, ptr->time);
    }

    free(ptr);
}

/* EMERGENCY HANDLING 
   Prioritizes emergency patient*/
void emergency(){
    struct node *ptr=start,*prev=NULL;
    int i;

    while(ptr!=NULL && ptr->priority!=1){
        prev=ptr;
        ptr=ptr->next;
    }

    if(ptr==NULL){printf("No emergency\n");return;}

    if(prev==NULL) start=ptr->next;
    else prev->next=ptr->next;

    printf("EMERGENCY: %s (%s)\n",ptr->id,ptr->name);

    rear++;

    if(rear < 100){
        strcpy(history[rear].id, ptr->id);
        strcpy(history[rear].name, ptr->name);
        history[rear].gender = ptr->gender;
        strcpy(history[rear].doctor, ptr->doctor);
        strcpy(history[rear].checkup, ptr->checkup);
        history[rear].room = ptr->room;
        strcpy(history[rear].time, ptr->time);
    }
    free(ptr);
}

/* NOW SERVING VIEW 
   Shows current and next patients*/
void nowServing(){
    struct node *ptr=start;
    int i=0;

    if(ptr==NULL){
        printf("\n==================== NOW SERVING ====================\n");
        printf("Empty\n");
        return;
    }

    printf("\n==================== NOW SERVING ====================\n");
    printf("%-10s %-20s %-10s\n", "ID", "NAME", "TIME");
    printf("-----------------------------------------------------\n");
    printf("%-10s %-20s %-10s\n", ptr->id, ptr->name, ptr->time);

    ptr = ptr->next;

    printf("\n==================== NEXT 3 PATIENTS ====================\n");
    printf("%-5s %-10s %-20s %-10s\n", "NO", "ID", "NAME", "TIME");
    printf("---------------------------------------------------------\n");

    while(ptr!=NULL && i<3){
        printf("%-5d %-10s %-20s %-10s\n", i+1, ptr->id, ptr->name, ptr->time);
        ptr = ptr->next;
        i++;
    }

    printf("---------------------------------------------------------\n");
}

/* VIEW HISTORY
   Displays complete served patient records */
void viewHistory(){
    int i;

    printf("\n============================== SERVED HISTORY ===============================\n");
    printf("%-10s %-20s %-8s %-20s %-8s %-10s\n",
           "ID","NAME","GENDER","CHECKUP","ROOM","TIME");

    if(rear < front){
        printf("No history yet\n");
        return;
    }

    for(i=front;i<=rear;i++){
        printf("%-10s %-20s %-8c %-20s %-8d %-10s\n",
            history[i].id,
            history[i].name,
            history[i].gender,
            history[i].checkup,
            history[i].room,
            history[i].time);
    }

    printf("-----------------------------------------------------------------------------\n");
}

/* EDIT APPOINTMENT 
   Allows admin modifications */
void edit(){
    struct node *ptr=start;
    char id[10];
    int choice,i;

    printf("Enter Ticket ID: ");
    scanf("%9s",id);

    while(ptr!=NULL && !isEqual(ptr->id,id))
        ptr=ptr->next;

    if(ptr==NULL){
        printf("Not found\n");
        return;
    }

    printf("\nWhat do you want to edit?\n");
    printf("1. Doctor\n2. Room\n");
    printf("Enter: ");
    choice = getNumber();

    if(choice == 1){
        printf("\nSelect New Doctor:\n");
        for(i=0;i<7;i++)
            printf("%d. %s\n",i+1,doctors[i]);

        while(1){
            printf("Enter choice: ");
            int d = getNumber();
            if(d>=1 && d<=7){
                strcpy(ptr->doctor, doctors[d-1]);
                break;
            }
        }
        printf("Doctor updated\n");
    }

    else if(choice == 2){
        printf("Enter new room number: ");
        ptr->room = getNumber();
        printf("Room updated\n");
    }

    else {
        printf("Invalid choice\n");
    }
}

/* PATIENT MENU SYSTEM */
void patientMenu(){
    int choice;
    while(1){
        printf("\n===== PATIENT MENU =====\n");
        printf("1. Book Appointment\n2. View My Appointment\n3. Cancel Appointment\n4. Logout\nEnter: ");
        choice=getNumber();
        if(choice==1) book();
        else if(choice==2) viewMy();
        else if(choice==3) cancel();
        else if(choice==4) return;
    }
}

/* ADMIN MENU SYSTEM */
void adminMenu(){
    int choice;
    while(1){
        printf("\n===== ADMIN MENU =====\n");
        printf("1. View All Appointments\n2. Serve Patient\n3. Override Emergency\n4. Edit Appointment\n5. Now Serving\n6. View History\n7. Logout\nEnter: ");
        choice=getNumber();
        if(choice==1) display();
        else if(choice==2) serve();
        else if(choice==3) emergency();
        else if(choice==4) edit();
        else if(choice==5) nowServing();
        else if(choice==6) viewHistory();
        else if(choice==7) return;
    }
}

/* MAIN SYSTEM ENTRY POINT */
int main(){
    initRooms();

    int role;

    while(1){
        printf("\n========================================\n");
        printf("        PRIVATE HOSPITAL SYSTEM\n");
        printf("========================================\n");
        printf("1. Admin\n2. Patient\n3. Exit\nEnter: ");

        role=getNumber();

        if(role==1) adminMenu();
        else if(role==2) patientMenu();
        else if(role==3) exit(0);
    }
}
