#include <gtk/gtk.h>
#include <stdlib.h>

GtkWidget* window;

// Callback function when Button1 is clicked
void on_button1_clicked(GtkWidget* button, gpointer data)
{
    execlp("./fcfs", "fcfs", NULL);
    gtk_main_quit();
    gtk_widget_destroy(window);
}

// Callback function when Button2 is clicked
void on_button2_clicked(GtkWidget* button, gpointer data)
{
    execlp("./sjf_npe", "sjf_npe", NULL);
    gtk_main_quit();
    gtk_widget_destroy(window);
}

void on_button3_clicked(GtkWidget* button, gpointer data)
{
    execlp("./sjf_pe", "sjf_pe", NULL);
    gtk_main_quit();
    gtk_widget_destroy(window);
    //executeProgram("sjf_pe");
}

void on_button4_clicked(GtkWidget* button, gpointer data)
{
    execlp("./rr", "rr", NULL);
    gtk_main_quit();
    gtk_widget_destroy(window);
}

void on_button5_clicked(GtkWidget* button, gpointer data)
{
    execlp("./pri_sch", "pri_sch", NULL);
    gtk_main_quit();
    gtk_widget_destroy(window);
}

// Main function
int main(int argc, char** argv)
{
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "CPU SCHEDULING ALGORITHMS");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 200);

    // Create the main vertical box container
    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create the top horizontal box container
    GtkWidget* hbox_top = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_top, FALSE, FALSE, 0);
    
    // Create the logo box
    GtkWidget* logo_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(hbox_top), logo_box, FALSE, FALSE, 0);

    // Create the logo
    GtkWidget* logo = gtk_image_new_from_file("vnrlogo2.jpeg"); // Replace "logo.png" with your logo file path
    gtk_box_pack_start(GTK_BOX(logo_box), logo, FALSE, FALSE, 0);
    
     gtk_widget_set_size_request(logo, 5, 5);

    // Create the name label
    GtkWidget* name_label = gtk_label_new("CPU SCHEDULING ALGORITHMS"); // Replace with your program name
    gtk_box_pack_start(GTK_BOX(hbox_top), name_label, FALSE, FALSE, 0);

    // Create the bottom horizontal box container
    GtkWidget* hbox_bottom = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_end(GTK_BOX(vbox), hbox_bottom, FALSE, FALSE, 0);

    // Create the team column
    GtkWidget* team_column = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(hbox_bottom), team_column, FALSE, FALSE, 0);

    // Create the team label
    GtkWidget* team_label = gtk_label_new("Team Members:"); // Replace with your team name
    gtk_box_pack_start(GTK_BOX(team_column), team_label, FALSE, FALSE, 0);

    // Create team member labels
    GtkWidget* member_label1 = gtk_label_new("Devvrat Yadav - 21071A6683");
    GtkWidget* member_label2 = gtk_label_new("Manaswini - 21071A6672");
    GtkWidget* member_label3 = gtk_label_new("A.Siddeshwari - 21071A6666");
    GtkWidget* member_label4 = gtk_label_new("G.Chandrasekhar - 21071A6688");
    GtkWidget* member_label5 = gtk_label_new("Hima Poojitha - 21071A66A0");
    gtk_box_pack_start(GTK_BOX(team_column), member_label1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(team_column), member_label2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(team_column), member_label3, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(team_column), member_label4, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(team_column), member_label5, FALSE, FALSE, 0);

    // Create the faculty column
    GtkWidget* faculty_column = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_end(GTK_BOX(hbox_bottom), faculty_column, FALSE, FALSE, 0);

    // Create the faculty label
    GtkWidget* faculty_label = gtk_label_new("Faculty Name:"); // Replace with your faculty name
    gtk_box_pack_start(GTK_BOX(faculty_column), faculty_label, FALSE, FALSE, 0);
    GtkWidget* faculty_name = gtk_label_new("M.Ajay Dilip Kumar"); // Replace with your faculty name
    gtk_box_pack_start(GTK_BOX(faculty_column), faculty_name, FALSE, FALSE, 0);
    GtkWidget* faculty_post = gtk_label_new("Assistant proffessor"); // Replace with your faculty name
    gtk_box_pack_start(GTK_BOX(faculty_column), faculty_post, FALSE, FALSE, 0);

    // Create Button1
    GtkWidget* button1 = gtk_button_new_with_label("FCFS");
    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button1, TRUE, TRUE, 0);

    // Create Button2
    GtkWidget* button2 = gtk_button_new_with_label("SJF - Non Preemptive");
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button2, TRUE, TRUE, 0);
    
    // Create Button3
    GtkWidget* button3 = gtk_button_new_with_label("SJF - Preemptive");
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button3_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button3, TRUE, TRUE, 0);
    
    // Create Button4
    GtkWidget* button4 = gtk_button_new_with_label("Round Robin");
    g_signal_connect(button4, "clicked", G_CALLBACK(on_button4_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button4, TRUE, TRUE, 0);
    
    // Create Button5
    GtkWidget* button5 = gtk_button_new_with_label("Priority Scheduling - Non - Preemptive");
    g_signal_connect(button5, "clicked", G_CALLBACK(on_button5_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button5, TRUE, TRUE, 0);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
