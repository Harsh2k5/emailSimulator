An Email Simulator that processes mail at an average of 40 messages per minute. 

Here's a breakdown of the algorithm in the code:

1. **Data Structures:**
   - `Email` structure: Represents an email with subject and body.
   - `EmailQueue` structure: Represents a circular queue of emails with synchronization using mutex and condition variables.
   - `EmailSimulator` structure: Contains a pointer to the email queue and the processing rate.

2. **Queue Initialization:**
   - `init_queue` function initializes the email queue with the front and rear set to -1, and initializes the mutex and condition variable.

3. **Enqueue Operation:**
   - `enqueue` function locks the mutex, adds an email to the rear of the queue, signals the condition variable to notify the processing thread that the queue is not empty, and unlocks the mutex.

4. **Dequeue Operation:**
   - `dequeue` function locks the mutex, waits while the queue is empty, dequeues an email from the front of the queue, signals the condition variable if the queue is not empty, and unlocks the mutex.

5. **Email Generation:**
   - `generate_email` function randomly selects a title from predefined titles and copies it to the email subject. It also simulates the email body with some content.

6. **Receiving Emails:**
   - The `receive_email` thread function generates an email using `generate_email`, enqueues it using `enqueue`, and prints the received email details. It then sleeps for a random duration.

7. **Processing Emails:**
   - The `process_emails` thread function dequeues an email using `dequeue`, prints the processed email details, and sleeps for a random duration.

8. **Main Function:**
   - The `main` function initializes the random seed, initializes the email queue, creates the email simulator, and creates two threads for receiving and processing emails using `pthread_create`. It then waits for the threads to finish using `pthread_join`.

9. **Thread Execution:**
   - The `receive_email` and `process_emails` functions run in an infinite loop, simulating the continuous receipt and processing of emails. The sleep durations introduce randomness to simulate real-world scenarios.

10. **Randomness:**
    - The program uses the `rand()` function to introduce randomness in sleep durations and title selection.

11. **Thread Termination:**
    - The `main` function uses `pthread_join` to wait for the threads to finish execution.

