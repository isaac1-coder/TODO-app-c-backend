#include <iostream>     // Includes standard input/output stream library for printing to the console
#include <vector>       // Includes the std::vector dynamic array container for storing tasks in RAM
#include <string>       // Includes the std::string class for handling text data
#include "httplib.h"    // Includes the single-header HTTP library that handles low-level server networking

// Helper function to handle GET requests for retrieving all tasks
void getTasksHandler(const auto& req, auto& res, const std::vector<std::string>& tasks) {
    std::string all_tasks = "";                   // Initialize an empty string to accumulate all tasks
    for (const auto& task : tasks) {              // Range-based for loop to iterate through every task in the vector efficiently
        all_tasks += task + "\n";                 // Append each task followed by a newline character to separate them
    }

    res.set_header("Access-Control-Allow-Origin", "*"); // Set CORS header so browser security allows frontend access
    res.set_content(all_tasks, "text/plain");     // Package the combined task string and content type into the HTTP response
}

// Helper function to handle POST requests for adding a new task
void postTasksHandler(const auto& req, auto& res, std::vector<std::string>& tasks) {
    std::string new_task = req.body;              // Extract the raw text payload sent from the browser's fetch body

    if (!new_task.empty()) {                      // Check if the incoming task string is not empty before storing
        tasks.push_back(new_task);                // Push the new task into our in-memory vector container
    }

    res.set_header("Access-Control-Allow-Origin", "*"); // Set CORS header to allow response back to the browser
    res.set_content("Task added successfully", "text/plain"); // Send a confirmation message back as plain text
}

int main() {                                      // Entry point where execution of the C++ program begins
    httplib::Server svr;                          // Instantiate the HTTP server object that will listen for network requests
    std::vector<std::string> tasks;               // Declare a local vector container in RAM to live-store our todo tasks

    svr.set_mount_point("/", "../");              // Tell the server to serve static files (like index.html) from the parent directory

    // Register a GET route for "/tasks" using an inline lambda function
    svr.Get("/tasks", [&tasks](auto& req, auto& res) { // [&tasks] captures the vector by reference; (auto& req, auto& res) accept packets
        getTasksHandler(req, res, tasks);         // Forward the request, response, and tasks vector to our clean handler function
    });

    // Register a POST route for "/tasks" using an inline lambda function
    svr.Post("/tasks", [&tasks](auto& req, auto& res) { // [&tasks] captures vector by reference so we can write into it
        postTasksHandler(req, res, tasks);        // Forward the request, response, and tasks vector to our clean handler function
    });

    std::cout << "C++ Backend running at http://localhost:8080...\n"; // Print server startup notification to the terminal
    svr.listen("localhost", 8080);                // Start blocking server loop, listening on localhost port 8080 for incoming traffic

    return 0;                                     // Return 0 to the OS (though svr.listen keeps the program alive infinitely)
}