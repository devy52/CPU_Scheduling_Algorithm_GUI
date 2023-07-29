#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Process {
    int pid; // process ID
    int at;  // arrival Time
    int bt;  // burst Time
};

int comparison(const void *a, const void *b);
void findAverageTime(struct Process proc[], int n);
void findWaitingTime(struct Process proc[], int n, int wt[]);
void findTurnAroundTime(struct Process proc[], int n, int wt[], int tat[]);

GtkWidget *window;
GtkWidget *grid;
GtkWidget *process_table;
GtkWidget *result_label;
GtkWidget *warning_label;
GtkWidget *process_id_entry;
GtkWidget *arrival_time_entry; // New entry for arrival time
GtkWidget *burst_time_entry;
GtkWidget *add_button;
GtkWidget *start_button;
GtkWidget *clear_button;
GtkWidget *back_button;

struct Process processes[100];
int process_count = 0;

void clear_input_fields() {
    gtk_entry_set_text(GTK_ENTRY(process_id_entry), "");
    gtk_entry_set_text(GTK_ENTRY(arrival_time_entry), ""); // Clear the arrival time entry
    gtk_entry_set_text(GTK_ENTRY(burst_time_entry), "");
}

void update_process_table() {
    gtk_widget_destroy(process_table);
    process_table = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(process_table), 5);
    gtk_grid_set_row_spacing(GTK_GRID(process_table), 5);
    gtk_grid_attach(GTK_GRID(grid), process_table, 0, 6, 3, process_count + 1); // Adjust row number

    GtkWidget *pid_header_label = gtk_label_new("Process ID");
    GtkWidget *arrival_header_label = gtk_label_new("Arrival Time"); // New header for arrival time
    GtkWidget *burst_header_label = gtk_label_new("Burst Time");

    gtk_grid_attach(GTK_GRID(process_table), pid_header_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(process_table), arrival_header_label, 1, 0, 1, 1); // Attach arrival header
    gtk_grid_attach(GTK_GRID(process_table), burst_header_label, 2, 0, 1, 1); // Adjust column number

    gtk_widget_show(pid_header_label);
    gtk_widget_show(arrival_header_label); // Show the arrival header
    gtk_widget_show(burst_header_label);

    for (int i = 0; i < process_count; i++) {
        gchar pid_str[10], arrival_str[10], burst_str[10];
        sprintf(pid_str, "%d", processes[i].pid);
        sprintf(arrival_str, "%d", processes[i].at); // Get arrival time as string
        sprintf(burst_str, "%d", processes[i].bt);

        GtkWidget *pid_label = gtk_label_new(pid_str);
        GtkWidget *arrival_label = gtk_label_new(arrival_str); // Create a label for arrival time
        GtkWidget *burst_label = gtk_label_new(burst_str);

        gtk_grid_attach(GTK_GRID(process_table), pid_label, 0, i + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(process_table), arrival_label, 1, i + 1, 1, 1); // Attach arrival label
        gtk_grid_attach(GTK_GRID(process_table), burst_label, 2, i + 1, 1, 1); // Adjust column number

        gtk_widget_show(pid_label);
        gtk_widget_show(arrival_label); // Show the arrival label
        gtk_widget_show(burst_label);
    }

    gtk_widget_show(process_table);
}

void on_add_button_clicked(GtkWidget *widget, gpointer data) {
    int pid = atoi(gtk_entry_get_text(GTK_ENTRY(process_id_entry)));
    int arrival_time = atoi(gtk_entry_get_text(GTK_ENTRY(arrival_time_entry))); // Get arrival time value
    int burst_time = atoi(gtk_entry_get_text(GTK_ENTRY(burst_time_entry)));

    processes[process_count].pid = pid;
    processes[process_count].at = arrival_time; // Set arrival time for the new process
    processes[process_count].bt = burst_time;
    process_count++;

    clear_input_fields();
    update_process_table();
}

// ... (other functions remain unchanged)

void on_start_button_clicked(GtkWidget *widget, gpointer data) {
    qsort(processes, process_count, sizeof(struct Process), comparison);
    findAverageTime(processes, process_count);
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

int comparison(const void *a, const void *b) {
    struct Process *p1 = (struct Process *)a;
    struct Process *p2 = (struct Process *)b;
    return (p1->bt - p2->bt);
}

void findWaitingTime(struct Process proc[], int n, int wt[]) {
    wt[0] = 0;
    for (int i = 1; i < n; i++) {
        wt[i] = proc[i - 1].bt + wt[i - 1];
    }
}

void findTurnAroundTime(struct Process proc[], int n, int wt[], int tat[]) {
    for (int i = 0; i < n; i++) {
        tat[i] = proc[i].bt + wt[i];
    }
}

void findAverageTime(struct Process proc[], int n) {
    int wt[n], tat[n], total_wt = 0, total_tat = 0;
    int starvationThreshold = 10; // Adjust this threshold as needed

    findWaitingTime(proc, n, wt);
    findTurnAroundTime(proc, n, wt, tat);

    gchar result_str[200] = ""; // String to store the result

    sprintf(result_str, "Processes\tBurst time\tWaiting time\tTurn around time\n");
    for (int i = 0; i < n; i++) {
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
        gchar temp_str[100];
        sprintf(temp_str, "%d\t\t\t\t%d\t\t\t%d\t\t\t\t%d\n", proc[i].pid, proc[i].bt, wt[i], tat[i]);
        strcat(result_str, temp_str);
    }

    gchar avg_wt_str[100], avg_tat_str[100];
    sprintf(avg_wt_str, "Average waiting time = %.2f\n", (float)total_wt / (float)n);
    sprintf(avg_tat_str, "Average turn around time = %.2f\n", (float)total_tat / (float)n);

    strcat(result_str, avg_wt_str);
    strcat(result_str, avg_tat_str);

    gtk_label_set_text(GTK_LABEL(result_label), result_str); // Update the result_label widget

    for (int i = 0; i < n; i++) {
        if (wt[i] > starvationThreshold) {
            gchar warning_str[100];
            sprintf(warning_str, "Starvation occurred for process %d\n", proc[i].pid);
            gtk_label_set_text(GTK_LABEL(warning_label), warning_str); // Update the warning_label widget
            break;
        }
        gtk_label_set_text(GTK_LABEL(warning_label), ""); // Clear the warning_label widget if no starvation occurs
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // ... (other parts of the main function remain unchanged)
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "SJF (Non-Preemptive) Scheduling");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *heading_label = gtk_label_new("SJF (Non-Preemptive) Scheduling");
    gtk_widget_set_hexpand(heading_label, TRUE);
    gtk_label_set_xalign(GTK_LABEL(heading_label), 0.5);
    gtk_grid_attach(GTK_GRID(grid), heading_label, 0, 0, 3, 1);

    GtkWidget *process_id_label = gtk_label_new("Process ID");
    GtkWidget *arrival_time_label = gtk_label_new("Arrival Time"); // New label for arrival time
    GtkWidget *burst_time_label = gtk_label_new("Burst Time");

    gtk_grid_attach(GTK_GRID(grid), process_id_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), arrival_time_label, 1, 1, 1, 1); // Attach arrival label
    gtk_grid_attach(GTK_GRID(grid), burst_time_label, 2, 1, 1, 1); // Adjust column number

    process_id_entry = gtk_entry_new();
    arrival_time_entry = gtk_entry_new(); // Create the entry for arrival time
    burst_time_entry = gtk_entry_new();

    add_button = gtk_button_new_with_label("Add Process");
    gtk_grid_attach(GTK_GRID(grid), process_id_entry, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), arrival_time_entry, 1, 2, 1, 1); // Attach arrival entry
    gtk_grid_attach(GTK_GRID(grid), burst_time_entry, 2, 2, 1, 1); // Adjust column number
    gtk_grid_attach(GTK_GRID(grid), add_button, 0, 3, 3, 1); // Adjust column span

    // ... (connecting signal handlers and other parts remain unchanged)
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

    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_button_clicked), NULL);
    g_signal_connect(clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), NULL);
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
