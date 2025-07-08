#  Creating a Thread Pool
When you create a thread pool by specifying the number of threads (e.g. ThreadPool pool(4);), 4 threads are created that will wait for tasks.

# Adding Tasks
When you add a task to the pool (e.g. using the enqueue method), the task is placed in a queue. If one of the threads is free, it takes the task from the queue and starts executing it.

# Executing Tasks
Threads work independently of each other. If all 4 threads are busy, new tasks will wait in the queue until one of the threads becomes free.

# Terminating
When all tasks have completed, threads can either terminate their work or continue waiting for new tasks, as long as the thread pool remains active.
