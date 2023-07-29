#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Process {
    int pid; // process ID
    int at;  // arrival Time
    int bt;  // burst Time
    int remaining_bt; // Remaining burst time for Round Robin
};

void findAverageTime(struct Process proc[], int n, int time_quantum);

struct Process processes[300];

float average_waiting_time = 0; // Declare the variables here
float average_turnaround_time = 0;

GtkWidget *window;
GtkWidget *grid;
GtkWidget *process_table;
GtkWidget *result_label;
GtkWidget *warning_label;
GtkWidget *process_id_entry;
GtkWidget *arrival_time_entry;
GtkWidget *burst_time_entry;
GtkWidget *add_button;
GtkWidget *start_button;
GtkWidget *clear_button;
GtkWidget *back_button;

int process_count = 0;

void clear_input_fields() {
    gtk_entry_set_text(GTK_ENTRY(process_id_entry), "");
    gtk_entry_set_text(GTK_ENTRY(arrival_time_entry), "");
    gtk_entry_set_text(GTK_ENTRY(burst_time_entry), "");
}

void update_process_table() {
 
    gtk_widget_destroy(process_table);
    process_table = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(process_table), 5);
    gtk_grid_set_row_spacing(GTK_GRID(process_table), 5);
    gtk_grid_attach(GTK_GRID(grid), process_table, 0, 6, 3, process_count + 1);

    GtkWidget *pid_header_label = gtk_label_new("Process ID");
    GtkWidget *arrival_header_label = gtk_label_new("Arrival Time");
    GtkWidget *burst_header_label = gtk_label_new("Burst Time");

    gtk_grid_attach(GTK_GRID(process_table), pid_header_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(process_table), arrival_header_label, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(process_table), burst_header_label, 2, 0, 1, 1);

    gtk_widget_show(pid_header_label);
    gtk_widget_show(arrival_header_label);
    gtk_widget_show(burst_header_label);

    for (int i = 0; i < process_count; i++) {
        gchar pid_str[10], arrival_str[10], burst_str[10];
        sprintf(pid_str, "%d", processes[i].pid);
        sprintf(arrival_str, "%d", processes[i].at);
        sprintf(burst_str, "%d", processes[i].bt);

        GtkWidget *pid_label = gtk_label_new(pid_str);
        GtkWidget *arrival_label = gtk_label_new(arrival_str);
        GtkWidget *burst_label = gtk_label_new(burst_str);

        gtk_grid_attach(GTK_GRID(process_table), pid_label, 0, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(process_table), arrival_label, 1, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(process_table), burst_label, 2, i + 1, 1, 1);

        gtk_widget_show(pid_label);
        gtk_widget_show(arrival_label);
        gtk_widget_show(burst_label);
    }

    gtk_widget_show(process_table);
}

void on_add_button_clicked(GtkWidget *widget, gpointer data) {
    int pid = atoi(gtk_entry_get_text(GTK_ENTRY(process_id_entry)));
    int arrival_time = atoi(gtk_entry_get_text(GTK_ENTRY(arrival_time_entry)));
    int burst_time = atoi(gtk_entry_get_text(GTK_ENTRY(burst_time_entry)));

    processes[process_count].pid = pid;
    processes[process_count].at = arrival_time;
    processes[process_count].bt = burst_time;
    processes[process_count].remaining_bt = burst_time;
    process_count++;

    clear_input_fields();
    update_process_table();
}

void on_start_button_clicked(GtkWidget *widget, gpointer data) {
    int time_quantum = *((int *)data);
    
    // Reset the average values
    average_waiting_time = 0;
    average_turnaround_time = 0;

    findAverageTime(processes, process_count, time_quantum); // Call the integrated Round Robin algorithm
    
    char result_buffer[500];
    sprintf(result_buffer, "Average Waiting Time: %f\nAverage Turnaround Time: %f", average_waiting_time, average_turnaround_time);
    gtk_label_set_text(GTK_LABEL(result_label), result_buffer);
}


void on_clear_button_clicked(GtkWidget *widget, gpointer data) {
    process_count = 0;
    update_process_table();
    clear_input_fields();
    gtk_label_set_text(GTK_LABEL(result_label), "");
    gtk_label_set_text(GTK_LABEL(warning_label), "");
}

void on_back_button_clicked(GtkWidget *widget, gpointer data) {
    // Implement the code to navigate back to another C program
    // You can use system() function to execute the desired program or open a new terminal window
    execlp("./main", "main", NULL);
    gtk_widget_destroy(window); // Close the previous program's window
    gtk_main_quit();
}

void on_window_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void findAverageTime(struct Process proc[], int n, int time_quantum) {

    int minimum = INT_MAX;
    int m = 0;

    // Calculate the maximum and minimum burst times
    for (int i = 0; i < n; i++) {
        if (proc[i].bt > m) {
            m = proc[i].bt;
        }
        if (proc[i].bt < minimum) {
            minimum = proc[i].bt;
        }
    }

    // Total indicates total time
    // Counter indicates which process is executed
    int total = 0, counter = 0, i;
    printf("Process ID       Burst Time       Turnaround Time      Waiting Time");
    int remaining_processes = n; // New variable to track remaining processes
    

    while (remaining_processes != 0) {
        int executed = 0; // Flag to check if any process is executed in the current iteration

        for (i = 0; i < n; i++) {
            // Use a dynamically adjusted time quantum based on the remaining burst time of the process
            int current_time_quantum = (time_quantum < proc[i].remaining_bt) ? time_quantum : proc[i].remaining_bt;

            if (proc[i].remaining_bt > 0) {
                executed = 1; // Mark that a process has been executed

                // Update total time and remaining burst time for the process
                total += current_time_quantum;
                proc[i].remaining_bt -= current_time_quantum;

                // If the process is completed, calculate waiting and turnaround time
                if (proc[i].remaining_bt == 0) {
                    printf("\nProcess No %d  \t\t %d\t\t\t\t %d\t\t\t %d", proc[i].pid, proc[i].bt,
                           total - proc[i].at, total - proc[i].at - proc[i].bt);
                    average_waiting_time += total - proc[i].at - proc[i].bt;
                    average_turnaround_time += total - proc[i].at;
                    counter = 0;
                    remaining_processes--; // Decrement the remaining process count
                }
            }
        }

        // If no process was executed, increment the total time
        if (!executed) {
            total++;
        }
    }

    average_waiting_time = average_waiting_time * 1.0 / n;
    average_turnaround_time = average_turnaround_time * 1.0 / n;

    // Check if time quantum is too high for Round Robin
    char warning_buffer[200];
    if (time_quantum >= 10 || time_quantum > m) {
        sprintf(warning_buffer, "Warning: With a high time quantum, Round Robin scheduling may behave like FCFS. This can potentially lead to starvation for longer processes.");
    } else if (time_quantum < minimum / 2) {
        sprintf(warning_buffer, "Warning: With a very low time quantum, low slicing time reduces processor output. Spends more time on context switching.");
    } else {
        sprintf(warning_buffer, ""); // Clear the warning label if there are no errors
    }

    gtk_label_set_text(GTK_LABEL(warning_label), warning_buffer);
}

int get_time_quantum() {
    int time_quantum = 0;
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *label;
    GtkWidget *entry;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    gint result;

    // Create the dialog and set its properties
    dialog = gtk_dialog_new_with_buttons("Enter Time Quantum", NULL, flags,
                                         "_OK", GTK_RESPONSE_ACCEPT,
                                         "_Cancel", GTK_RESPONSE_REJECT, NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    label = gtk_label_new("Please enter the time quantum:");
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show(label);

    entry = gtk_entry_new();
    gtk_container_add(GTK_CONTAINER(content_area), entry);
    gtk_widget_show(entry);

    // Run the dialog and wait for user response
    result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_ACCEPT) {
        const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
        time_quantum = atoi(text);
    }

    gtk_widget_destroy(dialog);

    return time_quantum;
}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Round Robin Scheduling");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *heading_label = gtk_label_new("Round Robin Scheduling");
    gtk_widget_set_hexpand(heading_label, TRUE);
    gtk_label_set_xalign(GTK_LABEL(heading_label), 0.5);
    gtk_grid_attach(GTK_GRID(grid), heading_label, 0, 0, 3, 1);

    int time_quantum = get_time_quantum();

    GtkWidget *process_id_label = gtk_label_new("Process ID");
    GtkWidget *arrival_time_label = gtk_label_new("Arrival Time");
    GtkWidget *burst_time_label = gtk_label_new("Burst Time");

    gtk_grid_attach(GTK_GRID(grid), process_id_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), arrival_time_label, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), burst_time_label, 2, 1, 1, 1);

    process_id_entry = gtk_entry_new();
    arrival_time_entry = gtk_entry_new();
    burst_time_entry = gtk_entry_new();

    add_button = gtk_button_new_with_label("Add Process");
    gtk_grid_attach(GTK_GRID(grid), process_id_entry, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), arrival_time_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), burst_time_entry, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), add_button, 0, 3, 3, 1);

    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_button_clicked), NULL);

    process_table = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(process_table), 5);
    gtk_grid_set_row_spacing(GTK_GRID(process_table), 5);
    gtk_grid_attach(GTK_GRID(grid), process_table, 0, 4, 3, process_count + 1);

    start_button = gtk_button_new_with_label("Start Scheduling");
    clear_button = gtk_button_new_with_label("Clear");
    back_button = gtk_button_new_with_label("Back");
    result_label = gtk_label_new("");
    warning_label = gtk_label_new("");

    gtk_grid_attach(GTK_GRID(grid), start_button, 0, process_count + 30, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), clear_button, 0, process_count + 31, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), result_label, 0, process_count + 32, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), warning_label, 0, process_count +33, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), back_button, 0, process_count + 34, 3, 1);

    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_button_clicked), &time_quantum);
    g_signal_connect(clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), NULL);
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
