# Private Hospital Appointment System

## Overview

This system is a console-based Hospital Appointment Management System written in C. It represents a structured hospital queue environment using a priority-based linked list. It organizes patient flow by priority level while maintaining fairness within each category.

The system models real-world hospital appointment processing, including scheduling, room allocation, and patient service tracking.

---

## Purpose

This system is designed to:

* Represent a structured hospital appointment management workflow
* Implement priority-based patient handling (Emergency, Senior/Pregnant, Normal)
* Prevent scheduling conflicts in room allocation
* Maintain organized patient flow from booking to service
* Record and preserve complete patient service history

---

## User Roles

### Patient

Patients are allowed to:

* Book an appointment
* View their appointment using Ticket ID
* Cancel their appointment before being served

### Admin

Administrators are responsible for:

* Viewing all scheduled appointments
* Serving patients based on priority order
* Handling emergency cases immediately
* Monitoring current and upcoming patients
* Viewing complete service history

---

## System Features

### Appointment Booking

This feature allows patients to:

* Enter personal information
* Select a check-up category
* Automatically assign doctor, room, and time slot
* Generate a unique Ticket ID
* Confirm appointment before insertion into the queue

---

### Priority-Based Queue System

The system implements a priority classification:

* 1 – Emergency (highest priority)
* 2 – Senior / Pregnant
* 3 – Normal

Patients are:

* Ordered based on priority level
* Maintained in FIFO order within the same priority group

---

### Room Scheduling System

This module represents independent scheduling per hospital room:

* Each room operates on an independent timeline
* Operating hours start at 7:30 AM
* Each appointment occupies a 30-minute time slot
* The system prevents overlapping appointments per room
* Time slots are automatically generated per room basis

---

### Ticket ID System

Each patient receives a unique identifier:

* A### → Emergency
* B### → Senior / Pregnant
* C### → Normal

The system ensures no duplicate Ticket IDs exist within the queue.

---

### Queue Management System

The system uses a linked list structure to represent the patient queue. It supports:

* Priority-based insertion
* Dynamic addition and removal of nodes
* Ordered traversal of patient records

---

### Emergency Handling

Emergency handling:

* Searches for emergency-level patients (priority 1)
* Removes and processes them immediately
* Overrides normal queue order when necessary
* Transfers patient records to history after service

---

### Now Serving Module

This module:

* Displays the current patient being served
* Shows the next three patients in queue
* Provides visibility of queue progression

---

### Patient History System

The system maintains a FIFO-based record structure of served patients, storing:

* Ticket ID
* Name
* Gender
* Check-up type
* Doctor assigned
* Room number
* Appointment time

This ensures complete traceability of hospital service flow.

---

### Search and Cancellation Features

* Patients can search their appointment using Ticket ID
* Appointments can be cancelled before being served
* The system removes nodes from the linked list safely

---

### Input Validation System

The system enforces:

* Numeric-only input for numbers
* Alphabet-only input for names
* Valid gender input (F/M only)
* Controlled menu selection inputs

---

## Data Structures Used and Their Supported Features

* Arrays
  Used to store check-up types, doctor lists, room scheduling data, and patient history records.
  Supports quick access to predefined services, doctor assignments, room time tracking, and storage of served patient information.

* Linked List
  Used to implement the main patient queue where each patient is stored as a node.
  Supports dynamic operations such as booking (insertion), canceling (deletion), serving patients (removal from front), emergency handling (priority removal), and viewing of appointments.

* Queue (FIFO Behavior – Conceptual Implementation)
  Applied to define the order in which patients are served and how history is recorded.
  The waiting list follows a priority-based queue behavior, while the history follows a FIFO enqueue-only process to preserve all served patient records without deletion.

---

## Algorithms Used and Their System Functions

* Priority Insertion Algorithm
  Manages the ordering of patients in the linked list based on priority level (Emergency, Senior/Pregnant, Normal).
  This directly supports the Book Appointment feature, ensuring higher-priority patients are placed at the front of the queue while maintaining FIFO order within the same priority level. It also affects the Serve Patient and Now Serving display since these depend on correct queue ordering.

* Linear Search Algorithm
  Traverses the linked list node-by-node to locate a patient’s record using Ticket ID.
  This supports the View My Appointment, Cancel Appointment, and internal validation processes where specific patient records must be identified before performing operations.

* Time Scheduling Algorithm
  Generates structured 30-minute intervals per room starting from 7:30 AM.
  Ensures room-based scheduling independence, prevents overlapping appointments, and maintains consistent time allocation across all check-up rooms.

---

## Compilation and Execution Guide

### Requirements

* GCC Compiler
* Terminal or Command Prompt

---

### File Setup

Save the source code as:
hospital.c

---

### Compilation

gcc hospital.c -o hospital

---

### Execution

Windows:
hospital.exe

Linux / macOS:
./hospital

---

## System Output Behavior

Upon execution, the program represents a hospital management environment where users can:

* Access patient and admin menus
* Manage appointments dynamically
* Observe priority-based patient processing
* Track real-time queue progression
* View complete service history
