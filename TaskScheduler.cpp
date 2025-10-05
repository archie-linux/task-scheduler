#include <queue>
#include <map>
#include <set>
#include <memory>
#include <string>
#include <optional>
#include <algorithm>
#include <fstream>
#include <iostream>

struct Task {
    int id;
    std::string description;
    int priority;
    std::optional<std::string> deadline;
    Task(int i, std::string desc, int prio, std::optional<std::string> dl = std::nullopt)
        : id(i), description(desc), priority(prio), deadline(dl) {}
};

class TaskScheduler {
    std::map<int, std::unique_ptr<Task>> tasks;
    std::queue<std::unique_ptr<Task>> task_queue;
    std::map<int, std::set<int>> dependencies;
    std::vector<std::unique_ptr<Task>> completed_tasks;
    int next_id = 1;

public:
    void add_task(const std::string& desc, int priority, std::optional<std::string> deadline = std::nullopt) {
        auto task = std::make_unique<Task>(next_id, desc, priority, deadline);
        tasks[next_id] = std::move(task);
        task_queue.push(std::make_unique<Task>(next_id, desc, priority, deadline));
        next_id++;
    }

    void add_dependency(int task_id, int dep_id) {
        if (tasks.find(task_id) != tasks.end() && tasks.find(dep_id) != tasks.end()) {
            dependencies[task_id].insert(dep_id);
        }
    }

    bool can_execute(int task_id) const {
        for (int dep_id : dependencies.at(task_id)) {
            auto it = std::find_if(completed_tasks.begin(), completed_tasks.end(),
                [dep_id](const auto& t) { return t->id == dep_id; });
            if (it == completed_tasks.end()) return false; // Dependency not completed
        }
        return true;
    }

    void execute_next() {
        if (task_queue.empty()) {
            std::cout << "No tasks to execute.\n";
            return;
        }
        auto task = std::move(task_queue.front());
        task_queue.pop();

        if (dependencies.find(task->id) != dependencies.end() && !can_execute(task->id)) {
            std::cout << "Cannot execute task " << task->id << ": dependencies not met.\n";
            task_queue.push(std::move(task)); // Requeue if dependencies not met
            return;
        }

        std::cout << "Executing: " << task->description << " (ID: " << task->id << ")\n";
        completed_tasks.push_back(std::move(task));
    }

    void list_tasks() const {
        std::vector<std::unique_ptr<Task>> sorted_tasks;
        for (const auto& pair : tasks) {
            sorted_tasks.push_back(std::make_unique<Task>(*pair.second));
        }
        std::sort(sorted_tasks.begin(), sorted_tasks.end(),
            [](const auto& a, const auto& b) { return a->priority > b->priority; });

        for (const auto& task : sorted_tasks) {
            std::cout << "ID: " << task->id << ", Description: " << task->description
                      << ", Priority: " << task->priority
                      << (task->deadline ? ", Deadline: " + *task->deadline : "") << "\n";
        }
    }

    void save_to_file(const std::string& filename) const {
        std::ofstream file(filename);
        for (const auto& pair : tasks) {
            const auto& task = pair.second;
            file << task->id << "|" << task->description << "|" << task->priority
                 << "|" << (task->deadline ? *task->deadline : "none") << "\n";
        }
    }
};

int main() {
    TaskScheduler scheduler;
    scheduler.add_task("Write report", 2, "2025-06-10");
    scheduler.add_task("Review code", 1);
    scheduler.add_dependency(2, 1); // Review code depends on Write report
    scheduler.list_tasks();
    scheduler.execute_next();
    scheduler.execute_next();
    scheduler.save_to_file("tasks.txt");
}
