# 🏥 Private Hospital Appointment System

## 📌 Overview
A simple hospital appointment system in C language that allows patients to book, view, and cancel appointments while the admin can manage, serve, and track patients using a priority-based queue system with scheduling and room management.

---

## 🎯 Purpose
The system is designed to:
- Organize patient appointments efficiently  
- Handle emergency cases properly  
- Prevent scheduling conflicts  
- Keep records of served patients  

---

## 👥 User Roles

### 👤 Patient
Patients can:
- Book an appointment  
- View their appointment details  
- Cancel their appointment  

### 👨‍⚕️ Admin
Admins (hospital staff) can:
- View all appointments  
- Serve patients  
- Prioritize emergency cases  
- Edit appointments  
- View current and next patients  
- View patient history  

---

## ⚙️ Features

### ✅ Appointment Booking
- Patients enter their name, age, and gender  
- Choose a check-up type  
- Doctor and room are automatically assigned  
- A unique ticket ID is generated  
- Appointment must be confirmed before saving  

---

### 🚨 Priority System
- 1 – Emergency (highest priority)  
- 2 – Senior / Pregnant  
- 3 – Normal  

Emergency patients are served immediately, while others follow based on priority level.

---

### 🕒 Room-Based Time Scheduling
- Each room has its own schedule  
- Starts at 7:30 AM  
- Each appointment lasts 30 minutes  
- Time is automatically assigned  
- Prevents same room and time conflicts  

---

### 🆔 Unique Ticket ID System
- A*** → Emergency  
- B*** → Senior/Pregnant  
- C*** → Normal  

No duplicate IDs allowed  

---

### 🔄 Queue System
- Uses linked list  
- Priority-based ordering  
- Same priority follows first-come-first-serve  

---

### 🛑 Cancel Appointment
- Patients can cancel using Ticket ID  
- Confirmation is required  

---

### 👨‍⚕️ Serve Patient
- Admin serves the first patient in queue  
- Patient is removed from queue  
- Data is saved into history  

---

### 🚨 Emergency Override
- Admin can immediately serve emergency patients  
- Skips normal queue  

---

### 📊 Now Serving Display
Shows:
- Current patient  
- Next 3 patients  

---

### 📜 Patient History
Stores:
- ID  
- Name  
- Gender  
- Checkup  
- Room  
- Time  

---

### ✏️ Edit Appointment
Admin can update:
- Doctor  
- Room  

---

### 🛡️ Input Validation
- Numbers only for numeric input  
- Letters only for names  
- Valid gender input (F/M)  
- Valid menu choices  

---

## 🧠 Data Structures Used
- Linked List → patient queue  
- Array → patient history  
- Struct → patient records  

---

## ▶️ How to Run

1. Compile the program:
```bash
gcc filename.c -o hospital
