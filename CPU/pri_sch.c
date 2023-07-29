#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <glib/gprintf.h>

struct Process {
    int pid;           // Process ID
    int priority;      // Priority of the process
    int burst_time;    // Burst time of the process
    int remaining_time;// Remaining burst time of the process
    int arrival_time;  // Arrival time of the process
    int waiting_time;  // Waiting time of the process
    int turnaround_time; // Turnaround time of the process
};

GtkWidget *window;
GtkWidget *grid;
GtkWidget *add_button;
GtkWidget *start_button;
GtkWidget *clear_button;
GtkWidget *process_table;
GtkWidget *result_label;
GtkWidget *warning_label;
GtkWidget *priority_entry;
GtkWidget *burst_time_entry;
GtkWidget *arrival_time_entry;
GtkWidget *back_button;

int process_count = 0;
struct Process processes[300];

// Function to perform preemptive priority scheduling
void priorityScheduling(int n);
void on_add_button_clicked(GtkWidget *widget, gpointer data);
void on_start_button_clicked(GtkWidget *widget, gpointer data);
void on_clear_button_clicked(GtkWidget *widget, gpointer data);
void on_back_button_clicked(GtkWidget *widget, gpointer data);
void update_process_table();


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Non - Preemptive Priority Scheduling");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *heading_label = gtk_label_new("Non - Preemptive Priority Scheduling");
    gtk_widget_set_hexpand(heading_label, TRUE);
    gtk_label_set_xalign(GTK_LABEL(heading_label), 0.5);
    gtk_grid_attach(GTK_GRID(grid), heading_label, 0, 0, 3, 1);

    GtkWidget *priority_label = gtk_label_new("Priority");
    GtkWidget *burst_time_label = gtk_label_new("Burst Time");
    GtkWidget *arrival_time_label = gtk_label_new("Arrival Time");

    priority_entry = gtk_entry_new();
    burst_time_entry = gtk_entry_new();
    arrival_time_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), priority_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), burst_time_label, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), arrival_time_label, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), priority_entry, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), burst_time_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), arrival_time_entry, 2, 2, 1, 1);

    add_button = gtk_button_new_with_label("Add Process");
    gtk_grid_attach(GTK_GRID(grid), add_button, 0, 3, 3, 1);

    process_table = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(process_table), 5);
    gtk_grid_set_row_spacing(GTK_GRID(process_table), 5);
    gtk_grid_attach(GTK_GRID(grid), process_table, 0, 4, 3, 1);

    result_label = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), result_label, 0, process_count + 32, 3, 1);

    warning_label = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), warning_label, 0, process_count + 33, 3, 1);

    start_button = gtk_button_new_with_label("Start Scheduling");
    gtk_grid_attach(GTK_GRID(grid), start_button, 0, process_count + 30, 3, 1);

    clear_button = gtk_button_new_with_label("Clear");
    gtk_grid_attach(GTK_GRID(grid), clear_button, 0, process_count + 31, 3, 1);

    back_button = gtk_button_new_with_label("Back");
    gtk_grid_attach(GTK_GRID(grid), back_button, 0, process_count + 34, 3, 1);

    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_button_clicked), processes);
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_button_clicked), processes);
    g_signal_connect(clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), processes);
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), window);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

void on_add_button_clicked(GtkWidget *widget, gpointer data) {
    struct Process *processes = (struct Process *)data;

    int priority = atoi(gtk_entry_get_text(GTK_ENTRY(priority_entry)));
    int burst_time = atoi(gtk_entry_get_text(GTK_ENTRY(burst_time_entry)));
    int arrival_time = atoi(gtk_entry_get_text(GTK_ENTRY(arrival_time_entry)));

    if (priority < 0 || burst_time <= 0 || arrival_time < 0) {
        gtk_label_set_text(GTK_LABEL(warning_label), "Invalid input! Please enter non-negative values for Priority, Burst Time, and Arrival Time.");
        return;
    }

    processes[process_count].pid = process_count + 1;
    processes[process_count].priority = priority;
    processes[process_count].burst_time = burst_time;
    processes[process_count].remaining_time = burst_time;
    processes[process_count].arrival_time = arrival_time;
    processes[process_count].waiting_time = 0;
    processes[process_count].turnaround_time = 0;
    process_count++;

    gtk_label_set_text(GTK_LABEL(warning_label), ""); // Clear any previous warning
    gtk_entry_set_text(GTK_ENTRY(priority_entry), "");
    gtk_entry_set_text(GTK_ENTRY(burst_time_entry), "");
    gtk_entry_set_text(GTK_ENTRY(arrival_time_entry), "");

    update_process_table();
}


void update_process_table() {
    // Destroy the previous process table widget
    if (process_table != NULL) {
        gtk_widget_destroy(process_table);
    }

    // Create a new process table widget
    process_table = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(process_table), 5);
    gtk_grid_set_row_spacing(GTK_GRID(process_table), 5);
    gtk_grid_attach(GTK_GRID(grid), process_table, 0, 4, 4, process_count + 1); // Adjust column span

    GtkWidget *pid_header_label = gtk_label_new("Process ID");
    GtkWidget *priority_header_label = gtk_label_new("Priority");
    GtkWidget *burst_header_label = gtk_label_new("Burst Time");
    GtkWidget *arrival_header_label = gtk_label_new("Arrival Time");

    gtk_grid_attach(GTK_GRID(process_table), pid_header_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(process_table), priority_header_label, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(process_table), burst_header_label, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(process_table), arrival_header_label, 3, 0, 1, 1);

    gtk_widget_show(pid_header_label);
    gtk_widget_show(priority_header_label);
    gtk_widget_show(burst_header_label);
    gtk_widget_show(arrival_header_label);

    for (int i = 0; i < process_count; i++) {
        gchar pid_str[10];
        gchar priority_str[10];
        gchar burst_str[10];
        gchar arrival_str[10];

        g_snprintf(pid_str, sizeof(pid_str), "%d", processes[i].pid);
        g_snprintf(priority_str, sizeof(priority_str), "%d", processes[i].priority);
        g_snprintf(burst_str, sizeof(burst_str), "%d", processes[i].burst_time);
        g_snprintf(arrival_str, sizeof(arrival_str), "%d", processes[i].arrival_time);

        GtkWidget *pid_label = gtk_label_new(pid_str);
        GtkWidget *priority_label = gtk_label_new(priority_str);
        GtkWidget *burst_label = gtk_label_new(burst_str);
        GtkWidget *arrival_label = gtk_label_new(arrival_str);

        gtk_grid_attach(GTK_GRID(process_table), pid_label, 0, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(process_table), priority_label, 1, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(process_table), burst_label, 2, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(process_table), arrival_label, 3, i + 1, 1, 1);

        gtk_widget_show(pid_label);
        gtk_widget_show(priority_label);
        gtk_widget_show(burst_label);
        gtk_widget_show(arrival_label);
    }

    gtk_widget_show(process_table);
}

void on_start_button_clicked(GtkWidget *widget, gpointer data) {
    struct Process *processes = (struct Process *)data;

    if (process_count == 0) {
        gtk_label_set_text(GTK_LABEL(warning_label), "Add at least one process!");
        return;
    } else {
        gtk_label_set_text(GTK_LABEL(warning_label), "");
    }

    // Perform priority scheduling
    priorityScheduling(process_count);
}

void priorityScheduling(int n) {
    int total_time = 0;
    int i;

    // Calculate total burst time
    int total_burst_time = 0;
    for (i = 0; i < n; i++) {
        total_burst_time += processes[i].burst_time;
        processes[i].remaining_time = processes[i].burst_time;
    }

    // Calculate waiting time, turnaround time, and average waiting time
    float total_wait_time = 0, average_wait_time = 0;
    int completed_processes = 0;
    
    while (completed_processes < n) {
        int selected_process = -1;
        int highest_priority = INT_MAX;

        // Find the process with the highest priority among the arrived processes
        for (i = 0; i < n; i++) {
            if (processes[i].arrival_time <= total_time && processes[i].remaining_time > 0 &&
                processes[i].priority < highest_priority) {
                highest_priority = processes[i].priority;
                selected_process = i;
            }
        }

        if (selected_process == -1) {
            total_time++;
            continue;
        }

        processes[selected_process].remaining_time--;
        total_time++;

        if (processes[selected_process].remaining_time == 0) {
            completed_processes++;
            processes[selected_process].turnaround_time = total_time - processes[selected_process].arrival_time;
            processes[selected_process].waiting_time = processes[selected_process].turnaround_time - processes[selected_process].burst_time;
            total_wait_time += processes[selected_process].waiting_time;
        }
    }

    average_wait_time = total_wait_time / n;
	
    if (process_table != NULL) {
        gtk_widget_destroy(process_table);
    }
    
    // Display process details and average waiting time in the GTK application
    gchar result_str[500];
    g_snprintf(result_str, sizeof(result_str), "Process\tPriority\tBurst Time\tArrival Time\tWaiting Time\tTurnaround Time\n");

    for (i = 0; i < n; i++) {
        gchar temp_str[100];
        g_snprintf(temp_str, sizeof(temp_str), "\t%d\t\t\t%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\t\t\t\t%d\n", processes[i].pid, processes[i].priority,
                   processes[i].burst_time, processes[i].arrival_time, processes[i].waiting_time, processes[i].turnaround_time);
        g_strlcat(result_str, temp_str, sizeof(result_str));
    }

    gchar average_str[100];
    g_snprintf(average_str, sizeof(average_str), "\nAverage Waiting Time: %.2f\n", average_wait_time);
    g_strlcat(result_str, average_str, sizeof(result_str));

    // Update the result_label in the GTK application
    gtk_label_set_text(GTK_LABEL(result_label), result_str);
    
    //check for potential starvation
    int starvation_flag=0;
    for(i=0;i<n-1;i++){
    	if(processes[i].waiting_time>processes[i+1].waiting_time){
    		starvation_flag=1; break;
    	}  
    }
    if(starvation_flag){
    	gtk_label_set_text(GTK_LABEL(warning_label), "Potential starvation is occuring");
    }
    else{
    	gtk_label_set_text(GTK_LABEL(warning_label), "No Potemtial starvation");
    }
}



void on_clear_button_clicked(GtkWidget *widget, gpointer data) {
    struct Process *processes = (struct Process *)data;
    process_count = 0;
    gtk_label_set_text(GTK_LABEL(result_label), "");
    gtk_label_set_text(GTK_LABEL(warning_label), "");

    // Destroy the existing process table widget

    // Create a new empty process table widget
    process_table = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(process_table), 5);
    gtk_grid_set_row_spacing(GTK_GRID(process_table), 5);
    gtk_grid_attach(GTK_GRID(grid), process_table, 0, 4, 3, 1);
    gtk_widget_show(process_table);
}


void on_back_button_clicked(GtkWidget *widget, gpointer data) {
    execlp("./main", "main", NULL);
    gtk_widget_destroy(window); // Close the previous program's window
    gtk_main_quit();
}

