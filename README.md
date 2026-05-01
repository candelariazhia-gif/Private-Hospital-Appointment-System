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
- Array → checkup types, doctors, history storage
- Linked List → patient queue system
- Queue concept → history tracking

### Algorithms Used
- Priority Insertion Algorithm → manages queue order
- Linear Search → used for ID search
- Sequential Scheduling Algorithm → assigns room time slots
- String Matching → compares IDs and records

---

## How to Compile and Run

### Compile the Program
```bash
gcc hospital.c -o hospital
