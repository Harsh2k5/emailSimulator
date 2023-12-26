#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#define MAX_SUBJECT_LENGTH 50
#define MAX_BODY_LENGTH 200
typedef struct {
char subject[MAX_SUBJECT_LENGTH];
char body[MAX_BODY_LENGTH];
} Email;
typedef struct {
Email messages[40]; // Max 100 messages in the queue
int front, rear;
pthread_mutex_t mutex;
pthread_cond_t not_empty;
} EmailQueue;
typedef struct {
EmailQueue *queue;
int processing_rate;
} EmailSimulator;
void init_queue(EmailQueue *q) {
q->front = q->rear = -1;
pthread_mutex_init(&q->mutex, NULL);
pthread_cond_init(&q->not_empty, NULL);
}
void enqueue(EmailQueue *q, Email message) {
pthread_mutex_lock(&q->mutex);
if (q->front == -1) {
q->front = 0;
}
q->rear = (q->rear + 1) % 100;
q->messages[q->rear] = message;
pthread_cond_signal(&q->not_empty);
pthread_mutex_unlock(&q->mutex);
}
Email dequeue(EmailQueue *q) {
pthread_mutex_lock(&q->mutex);
while (q->front == -1 || q->front > q->rear) {
pthread_cond_wait(&q->not_empty, &q->mutex);
}
Email message = q->messages[q->front];
q->front = (q->front + 1) % 100;
pthread_mutex_unlock(&q->mutex);
return message;
}
void generate_email(Email *email) {
char titles[][MAX_SUBJECT_LENGTH] = {
"Important Update",
"Meeting Reminder",
"Action Required",
"Happy Holidays",
"Weekly Report",
// Add more titles as needed
};
// Randomly select a title
int num_titles = sizeof(titles) / sizeof(titles[0]);
int random_title_index = rand() % num_titles;
// Copy the selected title to the email subject
strncpy(email->subject, titles[random_title_index], MAX_SUBJECT_LENGTH -
1);
email->subject[MAX_SUBJECT_LENGTH - 1] = '\0';
// Simulate email body with some content
snprintf(email->body, MAX_BODY_LENGTH, "This is a test email. %d", rand() %
100);
}
void *receive_email(void *arg) {
EmailSimulator *simulator = (EmailSimulator *)arg;
while (1) {
Email email;
generate_email(&email);
enqueue(simulator->queue, email);
printf("Received email - Subject: %s, Body: %s\n", email.subject,
email.body);
// Introduce randomness to the sleep duration
int random_sleep = rand() % 10; // Adjust the range as needed
sleep(60 / simulator->processing_rate + random_sleep);
}
}
void *process_emails(void *arg) {
EmailSimulator *simulator = (EmailSimulator *)arg;
while (1) {
Email email = dequeue(simulator->queue);
printf("Processing email - Subject: %s, Body: %s\n", email.subject,
email.body);
// Introduce randomness to the sleep duration
int random_sleep = rand() % 10; // Adjust the range as needed
sleep(60 / simulator->processing_rate + random_sleep);
}
}
int main() {
srand((unsigned int)time(NULL));
EmailQueue emailQueue;
init_queue(&emailQueue);
EmailSimulator simulator = {
.queue = &emailQueue,
.processing_rate = 40
};
pthread_t receiveThread, processThread;
pthread_create(&receiveThread, NULL, receive_email, (void *)&simulator);
pthread_create(&processThread, NULL, process_emails, (void *)&simulator);
pthread_join(receiveThread, NULL);
pthread_join(processThread, NULL);
return 0;
}
