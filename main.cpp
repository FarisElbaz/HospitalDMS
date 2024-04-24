#include "mainstuff.h"

#include <QApplication>
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Create an instance of the Function class
    Functions dbc;

    // Attempt to open the Function connection
    if (!dbc.connOpen()) {
        QMessageBox::critical(nullptr, "Error", "Failed to connect to the server!");
        return 1; // Return an error code
    }
    else{
        QMessageBox::information(nullptr,"Success","Connected to SQL server");
    }

    // If the connection is successful, create the main window
    mainstuff w(&dbc);
    w.show();

    // Start the application event loop
    return a.exec();
}
