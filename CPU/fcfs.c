#include <gtk/gtk.h>
#include <stdio.h>

struct Process {
    int pid;          // Process ID
    int burst_time;   // Burst time of the process
    int arrival_time; // Arrival time of the process
};

GtkWidget *window;
GtkWidget *grid;
GtkWidget *pid_entry;
GtkWidget *arrival_entry;
GtkWidget *burst_entry;
GtkWidget *add_button;
GtkWidget *start_button;
GtkWidget *clear_button;
GtkWidget *back_button;
GtkWidget *process_table;
GtkWidget *result_label;
GtkWidget *convoy_label;

struct Process processes[100];
int process_count = 0;

void clear_input_fields() {
    gtk_entry_set_text(GTK_ENTRY(pid_entry), "");
    gtk_entry_set_text(GTK_ENTRY(arrival_entry), "");
    gtk_entry_set_text(GTK_ENTRY(burst_entry), "");
}

void update_process_table() {
    gtk_widget_destroy(process_table);
    process_table = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(process_table), 5);
    gtk_grid_set_row_spacing(GTK_GRID(process_table), 5);
    gtk_grid_attach(GTK_GRID(grid), process_table, 0, 4, 3, process_count + 1);

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

    gtk_widget_show(process_table);
}

void on_add_button_clicked(GtkWidget *widget, gpointer data) {
    int pid = atoi(gtk_entry_get_text(GTK_ENTRY(pid_entry)));
    int arrival_time = atoi(gtk_entry_get_text(GTK_ENTRY(arrival_entry)));
    int burst_time = atoi(gtk_entry_get_text(GTK_ENTRY(burst_entry)));

    processes[process_count].pid = pid;
    processes[process_count].arrival_time = arrival_time;
    processes[process_count].burst_time = burst_time;
    process_count++;

    clear_input_fields();
    update_process_table();
}

void fcfsScheduling(struct Process processes[], int n) {
    int total_time = 0;
    int i;

    // Calculate waiting time, turnaround time, and average waiting time
    float total_wait_time = 0, average_wait_time = 0;
    int turnaround_time[n];

    for (i = 0; i < n; i++) {
        if (processes[i].arrival_time > total_time) {
            total_time = processes[i].arrival_time;
        }

        turnaround_time[i] = total_time + processes[i].burst_time - processes[i].arrival_time;
        total_wait_time += total_time - processes[i].arrival_time;
        total_time += processes[i].burst_time;
    }

    average_wait_time = total_wait_time / n;

    gchar result_str[100];
    sprintf(result_str, "Average Waiting Time: %.2f", average_wait_time);
    gtk_label_set_text(GTK_LABEL(result_label), result_str);

    int convoy_flag = 0;
    for (i = 1; i < n; i++) {
        if ((turnaround_time[i] - processes[i].burst_time) - (turnaround_time[i - 1] - processes[i - 1].burst_time) > 5) {
            convoy_flag = 1;
            break;
        }
    }

    if (convoy_flag) {
        gtk_label_set_text(GTK_LABEL(convoy_label), "Convoy effect is occurring.");
    } else {
        gtk_label_set_text(GTK_LABEL(convoy_label), "No convoy effect is occurring.");
    }
}

void on_start_button_clicked(GtkWidget *widget, gpointer data) {
    fcfsScheduling(processes, process_count);
}

void on_clear_button_clicked(GtkWidget *widget, gpointer data) {
    process_count = 0;
    update_process_table();
    clear_input_fields();
    gtk_label_set_text(GTK_LABEL(result_label), "");
    gtk_label_set_text(GTK_LABEL(convoy_label), "");
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
    gtk_window_set_title(GTK_WINDOW(window), "FCFS Scheduling");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *heading_label = gtk_label_new("FCFS Scheduling");
    gtk_widget_set_hexpand(heading_label, TRUE);
    gtk_label_set_xalign(GTK_LABEL(heading_label), 0.5);
    gtk_grid_attach(GTK_GRID(grid), heading_label, 0, 0, 3, 1);

    GtkWidget *pid_label = gtk_label_new("Process ID");
    GtkWidget *arrival_label = gtk_label_new("Arrival Time");
    GtkWidget *burst_label = gtk_label_new("Burst Time");

    gtk_grid_attach(GTK_GRID(grid), pid_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), arrival_label, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), burst_label, 2, 1, 1, 1);

    pid_entry = gtk_entry_new();
    arrival_entry = gtk_entry_new();
    burst_entry = gtk_entry_new();

    add_button = gtk_button_new_with_label("Add Process");
    gtk_grid_attach(GTK_GRID(grid), pid_entry, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), arrival_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), burst_entry, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), add_button, 0, 3, 3, 1);

    g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_button_clicked), NULL);

    process_table = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(process_table), 5);
    gtk_grid_set_row_spacing(GTK_GRID(process_table), 5);
    gtk_grid_attach(GTK_GRID(grid), process_table, 0, 4, 3, process_count + 1);

    start_button = gtk_button_new_with_label("Start FCFS");
    clear_button = gtk_button_new_with_label("Clear");
    back_button = gtk_button_new_with_label("Back");
    result_label = gtk_label_new("");
    convoy_label = gtk_label_new("");

    gtk_grid_attach(GTK_GRID(grid), start_button, 0, process_count + 20, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), clear_button, 0, process_count + 21, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), result_label, 0, process_count + 22, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), convoy_label, 0, process_count + 23, 3, 1);
    gtk_grid_attach(GTK_GRID(grid), back_button, 0, process_count + 24, 3, 1);

    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start_button_clicked), NULL);
    g_signal_connect(clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), NULL);
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
