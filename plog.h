#ifndef PLOG_H
#define PLOG_H


#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>


/** Public Interface -------------------------------------------------------- */


/** Initialize the logger */
void log_init();


/** Log a message */
void log(const char * format, ...);


/** Customization ---------------------------------------------------------- */


/** Color codes for the thread ids */
static const char* const log_colors[] = {
    "\033[31;1m",
    "\033[32;1m",
    "\033[33;1m",
    "\033[34;1m",
    "\033[35;1m",
    "\033[36;1m",
    "\033[37;1m",
    NULL
};


/** Implementation --------------------------------------------------------- */


static size_t n_colors;
static pthread_mutex_t log_mutex;


static void set_thread_color(pthread_t tid);
static void reset_color();


void log_init() {

    pthread_mutex_init(&log_mutex, NULL);

    n_colors = 0;
    while (log_colors[n_colors]) n_colors++;

}


void log(const char * format, ...) {
    pthread_mutex_lock(&log_mutex);

        pthread_t tid = pthread_self();

        va_list vargs;
        va_start(vargs, format);
        set_thread_color(tid);
        fprintf(stderr, "%lx: ", tid);
        reset_color();
        vfprintf(stderr, format, vargs);
        fprintf(stderr, ".\n");
        va_end(vargs);

    pthread_mutex_unlock(&log_mutex);
}


static void set_thread_color(pthread_t tid) {
    size_t i = tid % n_colors; // Worst hash function ever
    fprintf(stderr, "%s", log_colors[i]);
}


static void reset_color() {
    fprintf(stderr, "%s", "\033[0m");
}

#endif /* PLOG_H */
