# Private Hospital Appointment System

## Overview

This system is a console-based hospital appointment manager written in C. It helps a small private hospital organize patient appointments, manage waiting lists by priority, assign doctors and rooms, and keep a record of served patients. The program is aimed at receptionists (admin) and patients, providing a clear text-based interface for daily operations.

---

## Purpose

The main goals of this system are:

- Allow patients to book, view, or cancel their own appointments.
- Let hospital staff (admin) see all waiting appointments, serve patients in the correct priority order, and review the history of served patients.
- Automatically assign a doctor, a room, and a time slot based on the chosen checkup type.
- Keep a complete history of all served patients for record‑keeping.

---

## User Roles

### Patient

Patients can:

- Book a new appointment
- View their own appointment using the ticket ID
- Cancel their appointment (as long as it hasn’t been served yet)

### Admin

Administrators can:

- View all waiting appointments, sorted by checkup type
- See who is currently being served and the next three patients in line
- Serve the next patient according to priority (Emergency first, then Senior/Pregnant, then Normal)
- Review the complete history of already served patients

---

## System Features

### 1. Appointment Booking

A patient enters their name, gender, age, chooses a checkup type (General, Laboratory, Dental, etc.) and a priority level (Emergency, Senior/Pregnant, Normal). The system then:
- Generates a unique ticket ID.
- Assigns a room (room number = type index + 1).
- Assigns a time slot (e.g., “7:30 AM”)
- Shows a summary and asks for confirmation before adding the patient to the waiting queue.

### 2. Priority‑Based Waiting Queues

Instead of a single queue, the system uses **three separate queues**, one for each priority level:
- Emergency queue – highest priority
- Senior/Pregnant queue – medium priority
- Normal queue – lowest priority

When serving patients, the system always takes from the highest non‑empty queue. Within the same priority, patients are served in the order they booked (FIFO).

### 3. Room and Time Slot Scheduling

Each checkup type has its own independent schedule:
- Operating hours start at 7:30 AM, with 30‑minute interval until 5:00 PM.
- If no slot is available, the booking is refused.

### 4. Ticket ID System

Every patient receives a unique ID that shows their priority:
- `A001`, `A002`, … → Emergency
- `B001`, `B002`, … → Senior or Pregnant
- `C001`, `C002`, … → Normal

IDs are never reused, even after cancellation.

### 5. Now Serving + Next Patients

The admin can see:
- The patient currently being served (front of the highest non‑empty queue)
- Up to three next patients ( from all queues, in priority order)

This display does not change the queue; it only shows information.

### 6. Patient History

All served patients are stored in a singly linked list. The history preserves:
- Ticket ID, name, sex
- Checkup type and assigned doctor
- Room number and appointment time

The list can be displayed at any time by the admin.

### 7. Search and Cancellation

Patients can search for their own appointment by entering their ticket ID. The system performs a **linear search** through all three active queues to find the matching patient. If found, the patient can cancel the appointment (confirmation required).

### 8. Input Validation

All user inputs are validated:
- Numbers (age, menu choices) must be within the correct range, and non‑numeric input is rejected.
- Names may only contain letters, spaces, and periods.
- Sex must be `M` or `F` (case‑insensitive).
- Ticket IDs are checked for existence before operations.

## Data Structures and Algorithms Used

### Data Structures

- **Array** – Used for fixed lookup tables and a slot occupancy tracker.  
  - `checkupTypes[]` and `doctorNames[]` store checkup types and matching doctors. They support the **checkup type selection** during booking (function `bookAppointment`) and the **view appointment** display (`viewMyAppointment`).  
  - `timeSlots[]` holds all appointment hours. It supports the **time slot assignment** feature (`assignSlot`) and the **appointment summary** display.  
  - `isSlotTaken[][]` records which time slots are occupied. It is used only by `assignSlot` (to find a free slot) and `freeSlot` (to release a slot when an appointment is cancelled).  

- **Queue (linear array with front/rear)** – Three priority queues: `emergencyQueue`, `seniorQueue`, `normalQueue`.  
  - Supports **enqueue** (booking, `bookAppointment`), **dequeue** (serving, `serveNextPatient`), **search by ID** (view/cancel, `findPatientById` and `removePatientFromQueue`), and **compaction** (reuses empty front space, called inside `enqueue`).  
  - These queues are the core of all waiting‑list operations: booking, serving, cancelling, and displaying the now‑serving information.

- **Linked List** – History of served patients (`historyHead`).  
  - Supports **adding a served patient** (`addToHistory`) and **displaying the history** (`viewHistoryList`).  
  - Used only for record‑keeping; does not affect the active queues.

---

### Algorithms

- **Linear Search** – Implemented in `findPatientById()`.  
  - Scans each active queue (emergency, senior, normal) one patient at a time to locate an appointment by its ticket ID.  
  - Supports the **view my appointment** feature (`viewMyAppointment`) and the **cancel appointment** feature (`cancelAppointment`).

- **Merge Sort** – Implemented in `mergeSort()` and `merge()`.  
  - Collects all waiting patients from the three priority queues into a single array (`viewAllAppointments`), sorts them by checkup type (`typeIndex`), then displays them in groups.  
  - Supports the **view all waiting appointments** feature (`viewAllAppointments`), providing a clear, grouped output for the admin.

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
