#include <iostream>
#include <vector>
#include <string>
#include "httplib.h"

int main() {
    httplib::Server svr;

    // In-memory task list
    std::vector<std::string> tasks;

    // 1. GET Route: Send tasks to the browser
    svr.Get("/tasks", [&tasks](auto& req, auto& res) {
        std::string all_tasks = "";
        for (const auto& task : tasks) {
            all_tasks += task + "\n";
        }

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(all_tasks, "text/plain");
    });

    // 2. POST Route: Receive a new task from the browser
    svr.Post("/tasks", [&tasks](auto& req, auto& res) {
        std::string new_task = req.body;

        if (!new_task.empty()) {
            tasks.push_back(new_task);
        }

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("Task added successfully", "text/plain");
    });

    std::cout << "C++ Backend running at http://localhost:8080...\n";
    svr.listen("localhost", 8080);

    return 0;
}