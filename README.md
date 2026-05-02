# Private Hospital Appointment System

## Overview
A simple hospital appointment system written in C language that manages patient appointments using a priority-based queue system. It allows patients to book, view, and cancel appointments, while the admin can manage, serve, and track patients efficiently. The system also includes room-based scheduling and conflict prevention.

---

## Purpose
This system is designed to:
- Manage hospital appointments in an organized way
- Prioritize emergency and special cases
- Prevent scheduling conflicts in rooms
- Track and manage patient flow efficiently

---

## User Roles

### Patient
- Book an appointment
- View appointment details
- Cancel appointment using Ticket ID

### Admin
- View all appointments
- Serve patients based on priority
- Handle emergency cases immediately
- Edit doctor or room assignments
- View current and next patients
- View served patient history

---

## Features

### Appointment Booking
- Patient inputs personal details
- Selects check-up type
- System assigns doctor and room automatically
- Generates unique Ticket ID
- Requires confirmation before saving

---

### Priority System
- 1 – Emergency (highest priority)
- 2 – Senior / Pregnant
- 3 – Normal

Patients are served based on priority, and same priority follows FIFO order.

---

### Room-Based Scheduling System
- Each room has its own independent schedule
- Starts at 7:30 AM
- Each appointment is 30 minutes
- Prevents overlapping time slots per room
- Ensures no duplicate scheduling

---

### Ticket ID System
- A*** → Emergency
- B*** → Senior / Pregnant
- C*** → Normal
- Ensures no duplicate IDs

---

### Queue System
- Implemented using linked list
- Priority-based insertion
- Maintains order within same priority (FIFO)

---

### Emergency Handling
- Emergency patients are served immediately
- Overrides normal queue order

---

### Now Serving Display
- Shows current patient being served
- Displays next 3 patients in queue

---

### Patient History
Stores complete patient records:
- ID
- Name
- Gender
- Checkup type
- Doctor
- Room
- Time

---

### Edit Appointment
Admin can modify:
- Assigned doctor
- Room number

---

### Input Validation
- Numbers only for numeric inputs
- Letters only for names
- Valid gender input (F/M)
- Valid menu selection

---

## Data Structures and Algorithms

### Data Structures Used

- Arrays  
  Used to store check-up types, doctor lists, room scheduling data, and patient history records.  
  Supports quick access to predefined services, doctor assignments, room time tracking, and storage of served patient information.

- Linked List  
  Used to implement the main patient queue where each patient is stored as a node.  
  Supports dynamic operations such as booking (insertion), canceling (deletion), serving patients (removal from front), emergency handling (priority removal), viewing, and editing of appointments.

- Queue
  Applied to define the order in which patients are served and how history is recorded.  
  The waiting list follows a priority-based queue behavior, while the history follows a FIFO enqueue-only process to preserve all served patient records.

### Algorithms Used

- Priority Insertion Algorithm → manages the ordering of patients in the linked list based on priority level (Emergency, Senior/Pregnant, Normal). This directly supports the Book Appointment feature, ensuring that higher-priority patients are placed at the front of the queue while still maintaining first-come-first-served order within the same priority level. It also affects Serve Patient and Now Serving display since these rely on correct queue order.

- Linear Search → used to locate a patient’s record by Ticket ID by scanning the linked list node-by-node. This supports the View My Appointment feature, Cancel Appointment feature, and Edit Appointment feature, where the system must find a specific patient before performing any operation on their record.

---

## How to Compile and Run
This section explains how to compile and run the Private Hospital Appointment System written in C. Follow the steps carefully depending on your operating system.

---

## 1. Requirements
Before running the program, make sure you have the following installed:

- A C compiler (GCC recommended)
- A terminal or command prompt
- A text editor or IDE (optional, for viewing code)

To check if GCC is installed, run:
gcc --version


To run the system, follow these steps:

### 1. Save the file
Save the source code as:
hospital.c

### 2. Open terminal
Open a terminal or command prompt in the same folder where the file is located.

### 3. Compile the program
gcc hospital.c -o hospital

### 4. Run the program

Windows:
hospital.exe

Linux / Mac:
./hospital

### Result
The program will then start and allow you to use the hospital appointment system.
