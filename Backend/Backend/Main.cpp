#include <iostream>
#include <vector>
#include <string>
#include "httplib.h"

int main() {
    // 1. Initialize the server object that will listen for network traffic
    httplib::Server svr;
    std::vector<std::string> tasks;

    // 3. DEFINE THE GET ROUTE: Triggered when someone requests http://localhost:8080/tasks
    svr.Get("/tasks", [&tasks](const httplib::Request& req, httplib::Response& res) {
        
        // Combine all tasks in our vector into a single string, separated by newlines
        std::string all_tasks = "";
        for (const auto& task : tasks) {
            all_tasks += task + "\n";
        }

        // CORS Header: Tells the browser it's okay to accept this response
        res.set_header("Access-Control-Allow-Origin", "*");
        
        // Send the compiled string back to the browser as plain text
        res.set_content(all_tasks, "text/plain");
    });

    // 4. DEFINE THE POST ROUTE: Triggered when the frontend sends a new task here
    svr.Post("/tasks", [&tasks](const httplib::Request& req, httplib::Response& res) {
        
        // req.body holds whatever text payload the frontend sent over
        std::string new_task = req.body;

        // If the task isn't empty, store it in our vector
        if (!new_task.empty()) {
            tasks.push_back(new_task);
        }

        // CORS Header: Allows the browser to process the response back
        res.set_header("Access-Control-Allow-Origin", "*");

        // Send a confirmation message back to the frontend
        res.set_content("Task added successfully", "text/plain");
    });

    // 5. START THE SERVER: Bind it to port 8080 and start listening
    std::cout << "C++ Backend running at http://localhost:8080...\n";
    svr.listen("localhost", 8080);

    return 0;
}