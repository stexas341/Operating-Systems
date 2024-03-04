#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define MAX_STUDENTS 40
#define MAX_CAPACITY 8

pthread_t students[MAX_STUDENTS];
sem_t sem_students, mutex, sem_next_group;
int instudystudentscount = 0;
int total_students = 0;
int students_inside[MAX_CAPACITY] = {0};
int students_waiting[MAX_STUDENTS] = {0};
int study_room_empty = 1;

int waitingstudents = 0;
int students_in_current_group = 0;

void printstudentsinrooms()
{
    printf("Study room: | ");
    for (int i = 1; i <= MAX_CAPACITY; i++)
    {
        if (students_inside[i - 1] > 0)
        {
            printf("%d | ", students_inside[i - 1]);
        }
    }
    printf("\nWaiting room: | ");
    for (int i = 1; i <= waitingstudents; i++)
    {
        if (students_waiting[i - 1] > 0)
        {
            printf("%d | ", students_waiting[i - 1]);
        }
    }
    printf("\n");
}

void *student_thread(void *arg)
{
    int student_id = *(int *)arg;
    int study_time;
    int i;

    printf("Student %d was born\n", student_id);
    sem_wait(&mutex);
    if (instudystudentscount >= MAX_CAPACITY)
        printf("Student %d cannot enter the study hall. It is full.\n", student_id);
    if (instudystudentscount < MAX_CAPACITY)
    {
        students_inside[instudystudentscount] = student_id;
        instudystudentscount++;
        printstudentsinrooms();
        sem_post(&mutex);

        srand(time(NULL) + student_id);
        study_time = rand() % 11 + 5;
        sleep(study_time);

        sem_wait(&mutex);
        instudystudentscount--;
        for (i = 0; i < MAX_CAPACITY; i++)
        {
            if (students_inside[i] == student_id)
            {
                printf("Student %d exited the study hall after studying for %d secs\n", students_inside[i], study_time);
                students_inside[i] = 0;
                break;
            }
        }
        printstudentsinrooms();

        if (waitingstudents > 0 && instudystudentscount == 0)
        {
            sem_post(&sem_next_group);
        }

        sem_post(&mutex);
    }
    else
    {
        students_waiting[waitingstudents] = student_id;
        waitingstudents++;
        printstudentsinrooms();
        sem_post(&mutex);

        if (waitingstudents >= MAX_CAPACITY)
        {
            sem_wait(&sem_next_group);
            sem_wait(&mutex);

            if (study_room_empty)
            {
                study_room_empty = 0;

                int group_count = 0;
                while (group_count < MAX_CAPACITY)
                {
                    if (students_waiting[0] > 0)
                    {
                        students_inside[instudystudentscount] = students_waiting[0];
                        students_waiting[0] = 0;
                        instudystudentscount++;
                        waitingstudents--;

                        for (i = 0; i < waitingstudents; i++)
                        {
                            students_waiting[i] = students_waiting[i + 1];
                        }

                        group_count++;
                        printstudentsinrooms();
                    }
                    else
                    {
                        break;
                    }
                }

                sem_post(&mutex);
                sem_post(&sem_students);
                sem_post(&sem_next_group);

                study_time = 5 + rand() % 11;
                sleep(study_time);
                sem_wait(&mutex);

                while (instudystudentscount > 0)
                {
                    sem_post(&mutex);
                    usleep(1000);
                    sem_wait(&mutex);
                }

                for (i = 0; i < MAX_CAPACITY; i++)
                {
                    students_inside[i] = 0;
                }

                printstudentsinrooms();

                if (waitingstudents > 0)
                {
                    sem_post(&sem_next_group);
                }

                sem_post(&mutex);
                study_room_empty = 1;
            }
            else
            {
                sem_post(&mutex);
            }
        }
        else
        {
            sem_post(&mutex);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <number_of_students>\n", argv[0]);
        return 1;
    }

    total_students = atoi(argv[1]);

    if (total_students < 20 || total_students > 40)
    {
        printf("Total students must be between 20 and 40.\n");
        return 1;
    }

    sem_init(&sem_students, 0, MAX_CAPACITY);
    sem_init(&mutex, 0, 1);
    sem_init(&sem_next_group, 0, 0);

    int student_ids[MAX_STUDENTS];
    for (int i = 0; i < total_students; i++)
    {
        student_ids[i] = i + 1;
    }

    for (int i = 0; i < total_students; i++)
    {
        if (pthread_create(&students[i], NULL, student_thread, &student_ids[i]) != 0)
        {
            perror("Error creating student thread");
            return 1;
        }
    }

    for (int i = 0; i < total_students; i++)
    {
        pthread_join(students[i], NULL);
    }

    sem_destroy(&sem_students);
    sem_destroy(&mutex);
    sem_destroy(&sem_next_group);

    return 0;
}
