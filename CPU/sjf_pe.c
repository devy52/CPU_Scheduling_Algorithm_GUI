#include <gtk/gtk.h>
#include <stdio.h>

// Structure to represent a process
typedef struct {
    int pid;                // Process ID
    int arrival_time;       // Arrival time
    int burst_time;         // Burst time
    int waiting_time;       // Waiting time
    int turnaround_time;    // Turnaround time
    int remaining_time;     // Remaining time
} Process;

GtkWidget *window;
GtkWidget *grid;
GtkWidget *add_button;
GtkWidget *back_button;
GtkWidget *start_button;
GtkWidget *clear_button;
GtkWidget *pid_entry;
GtkWidget *arrival_entry;
GtkWidget *burst_entry;
GtkWidget *result_label;
GtkWidget *starvation_label;
GtkWidget *process_table;

Process processes[100];  // Assuming a maximum of 100 processes

int process_count = 0;

void clear_input_fields() {
    gtk_entry_set_text(GTK_ENTRY(pid_entry), "");
    gtk_entry_set_text(GTK_ENTRY(arrival_entry), "");
    gtk_entry_set_text(GTK_ENTRY(burst_entry), "");
}

void update_process_table() {
    // Clear the existing table
    gtk_widget_destroy(process_table);
    process_table = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(process_table), 5);
    gtk_grid_set_row_spacing(GTK_GRID(process_table), 5);
    gtk_grid_attach(GTK_GRID(grid), process_table, 0, 5, 2, process_count + 1);

    // Create and attach the table headers
    GtkWidget *pid_header_label = gtk_label_new("Process ID");
    GtkWidget *arrival_header_label = gtk_label_new("Arrival Time");
    GtkWidget *burst_header_label = gtk_label_new("Burst Time");

    gtk_grid_attach(GTK_GRID(process_table), pid_header_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(process_table), arrival_header_label, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(process_table), burst_header_label, 2, 0, 1, 1);

    gtk_widget_show(pid_header_label);
    gtk_widget_show(arrival_header_label);
    gtk_widget_show(burst_header_label);

    // Populate the table with process details
    for (int i = 0; i < process_count; i++) {
        gchar pid_str[10], arrival_str[10], burst_str[10];
        sprintf(pid_str, "%d", processes[i].pid);
        sprintf(arrival_str, "%d", processes[i].arrival_time);
        sprintf(burst_str, "%d", processes[i].burst_time);

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

    // Show the updated process table
    gtk_widget_show(process_table);
}

void on_button_clicked(GtkWidget *widget, gpointer data) {
    int pid = atoi(gtk_entry_get_text(GTK_ENTRY(pid_entry)));
    int arrival_time = atoi(gtk_entry_get_text(GTK_ENTRY(arrival_entry)));
    int burst_time = atoi(gtk_entry_get_text(GTK_ENTRY(burst_entry)));

    processes[process_count].pid = pid;
    processes[process_count].arrival_time = arrival_time;
    processes[process_count].burst_time = burst_time;
    processes[process_count].remaining_time = burst_time;

    process_count++;

    clear_input_fields();
    update_process_table();

    // Fix overlapping labels
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
}

void calculateWaitingTime(Process processes[], int n) {
    // Remaining implementation of the calculateWaitingTime function
    int i, time = 0, remaining = n;
    int smallest_arrival_time_index = -1;
    int current_process;

    while (remaining != 0) {
        smallest_arrival_time_index = -1;
        for (i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].remaining_time > 0) {
                if (smallest_arrival_time_index == -1 || processes[i].remaining_time < processes[smallest_arrival_time_index].remaining_time) {
                    smallest_arrival_time_index = i;
                }
            }
        }

        if (smallest_arrival_time_index == -1) {
            time++;
            continue;
        }

        current_process = smallest_arrival_time_index;

        processes[current_process].remaining_time--;
        time++;

        if (processes[current_process].remaining_time == 0) {
            remaining--;
            processes[current_process].waiting_time = time - processes[current_process].arrival_time - processes[current_process].burst_time;
        }
    }
}

void calculateTurnaroundTime(Process processes[], int n) {
    // Remaining implementation of the calculateTurnaroundTime function
    int i;

    for (i = 0; i < n; i++) {
        processes[i].turnaround_time = processes[i].burst_time + processes[i].waiting_time;
    }
}

float calculateAverageWaitingTime(Process processes[], int n) {
    // Remaining implementation of the calculateAverageWaitingTime function
    int i;
    float total_waiting_time = 0;

    for (i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
    }

    return total_waiting_time / n;
}

void on_start_button_clicked(GtkWidget *widget, gpointer data) {
    int starvationThreshold = 10; // Adjust this threshold as needed

    calculateWaitingTime(processes, process_count);
    calculateTurnaroundTime(processes, process_count);

    float avg_waiting_time = calculateAverageWaitingTime(processes, process_count);
    float twice_avg_waiting_time = 2 * avg_waiting_time;

    gchar result_str[100];
    sprintf(result_str, "Average Waiting Time: %.2f", avg_waiting_time);
    gtk_label_set_text(GTK_LABEL(result_label), result_str);

    // Check for potential starvation
    gchar starvation_str[100];
    int starvation_flag = 0;
    for (int i = 0; i < process_count; i++) {
        if (processes[i].waiting_time > starvationThreshold) {
            starvation_flag = 1;
            sprintf(starvation_str, "Starvation occurred for process %d", processes[i].pid);
            break;
        }
    }

    if (!starvation_flag) {
        sprintf(starvation_str, "No potential starvation detected.");
    }

    gtk_label_set_text(GTK_LABEL(starvation_label), starvation_str);
}

void on_clear_button_clicked(GtkWidget *widget, gpointer data) {
    clear_input_fields();
    process_count = 0;
    update_process_table();
    gtk_label_set_text(GTK_LABEL(result_label), "");
    gtk_label_set_text(GTK_LABEL(starvation_label), "");
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

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "SJF -- Preemptive");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 100);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);
    
    GtkWidget *heading_label = gtk_label_new("SJF -- Premptive");
    gtk_widget_set_hexpand(heading_label, TRUE);
    gtk_label_set_xalign(GTK_LABEL(heading_label), 0.5);
    gtk_grid_attach(GTK_GRID(grid), heading_label, 0, 1, 3, 1);

    GtkWidget *pid_label = gtk_label_new("Process ID");
    GtkWidget *arrival_label = gtk_label_new("Arrival Time");
    GtkWidget *burst_label = gtk_label_new("Burst Time");

    gtk_grid_attach(GTK_GRID(grid), pid_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), arrival_label, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), burst_label, 2, 2, 1, 1);

    gtk_widget_show(pid_label);
    gtk_widget_show(arrival_label);
    gtk_widget_show(burst_label);

    pid_entry = gtk_entry_new();
    arrival_entry = gtk_entry_new();
    burst_entry = gtk_entry_new();

    add_button = gtk_button_new_with_label("Add Process");
    gtk_grid_attach(GTK_GRID(grid), pid_entry, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), arrival_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), burst_entry, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), add_button, 0, 4, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), back_button, 0, 4, 3, 1);

    g_signal_connect(add_button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    gtk_widget_show(pid_entry);
    gtk_widget_show(arrival_entry);
    gtk_widget_show(burst_entry);
    gtk_widget_show(add_button);

    process_table = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(process_table), 5);
    gtk_grid_set_row_spacing(GTK_GRID(process_table), 5);
    gtk_grid_attach(GTK_GRID(grid), process_table, 0, 3, 3, process_count + 1);

    start_button = gtk_button_new_with_label("Start SJF");
    clear_button = gtk_button_new_with_label("Clear");
    result_label = gtk_label_new("");
    starvation_label = gtk_label_new("");
    back_button = gtk_button_new_with_label("Back");

    gtk_grid_attach(GTK_GRID(grid), start_button, 0, process_count + 20, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), clear_button, 0, process_count + 21, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), result_label, 0, process_count + 22, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), starvation_label, 0, process_count + 23, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), back_button, 0, process_count + 24, 3, 1);
    

    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_button_clicked), NULL);
    g_signal_connect(clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), NULL);
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), NULL);

    gtk_widget_show(start_button);
    gtk_widget_show(clear_button);
    gtk_widget_show(result_label);
    gtk_widget_show(starvation_label);
    gtk_widget_show(back_button);
    gtk_widget_show(grid);
    gtk_widget_show(window);

    gtk_main();

    return 0;
}
