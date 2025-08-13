import time  # For delay

# Node class for each patient
class PatientNode:
    def __init__(self, name, age, disease):
        self.name = name
        self.age = age
        self.disease = disease
        self.next = None


# Queue class using Singly Linked List
class PatientQueue:
    def __init__(self):
        self.head = None  # front of the queue
        self.tail = None  # end of the queue

    def is_empty(self):
        return self.head is None

    def enqueue(self, name, age, disease):
        new_patient = PatientNode(name, age, disease)
        if self.is_empty():
            self.head = self.tail = new_patient
        else:
            self.tail.next = new_patient
            self.tail = new_patient
        print(f"\n✅ Patient '{name}' added to the queue.")
        time.sleep(2)  # Wait 2 seconds

    def dequeue(self):
        if self.is_empty():
            print("\n🚫 Queue is empty. No patient to serve.")
            time.sleep(2)
            return
        removed = self.head
        self.head = self.head.next
        if self.head is None:
            self.tail = None
        print(f"\n🛏️ Patient '{removed.name}' has been served and removed from the queue.")
        time.sleep(2)

    def peek(self):
        if self.is_empty():
            print("\n🚫 Queue is empty.")
            time.sleep(2)
            return
        print(f"\n🩺 Next patient to be served: {self.head.name}, Age: {self.head.age}, Problem: {self.head.disease}")
        time.sleep(2)

    def display(self):
        if self.is_empty():
            print("\n🚷 Queue is empty.")
            time.sleep(2)
            return
        print("\n📋 Current Patient Queue:")
        current = self.head
        position = 1
        while current:
            print(f"{position}. 👤 {current.name} | Age: {current.age} | Problem: {current.disease}")
            current = current.next
            position += 1
        time.sleep(2)

    def count(self):
        count = 0
        current = self.head
        while current:
            count += 1
            current = current.next
        print(f"\n🔢 Total patients in queue: {count}")
        time.sleep(2)


# CLI Menu to run the queue system
def hospital_menu():
    queue = PatientQueue()

    while True:
        print("\n--- 🏥 Hospital Queue Management System ---")
        print("1. Add Patient (Enqueue)")
        print("2. Serve Patient (Dequeue)")
        print("3. Show Patient Queue")
        print("4. Peek (Next Patient)")
        print("5. Count Patients")
        print("6. Exit")
        choice = input("👉 Enter your choice (1-6): ")
        time.sleep(1)  # Wait a bit after selecting

        if choice == '1':
            print("\n👤 Enter Patient Details:")
            name = input("   Name: ")
            age = input("   Age: ")
            disease = input("   Problem: ")
            queue.enqueue(name, age, disease)

        elif choice == '2':
            queue.dequeue()

        elif choice == '3':
            queue.display()

        elif choice == '4':
            queue.peek()

        elif choice == '5':
            queue.count()

        elif choice == '6':
            print("\n👋 Exiting the hospital queue system. Get well soon!")
            time.sleep(2)
            break

        else:
            print("\n❌ Invalid choice. Please enter a number between 1 and 6.")
            time.sleep(2)


# Run the program
hospital_menu()
