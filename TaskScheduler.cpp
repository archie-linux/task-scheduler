#include <queue>
#include <map>
#include <set>
#include <memory>
#include <string>
#include <optional>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

// Task definition
struct Task {
    int id;
    std::string description;
    int priority;
    std::optional<std::string> deadline;
    Task(int i, const std::string& desc, int prio, std::optional<std::string> dl = std::nullopt)
        : id(i), description(desc), priority(prio), deadline(dl) {}
};

// Comparator for priority queue (higher priority first)
struct TaskPtrComparator {
    bool operator()(const Task* a, const Task* b) const {
        return a->priority < b->priority; // higher priority first
    }
};

class TaskScheduler {
    std::map<int, std::unique_ptr<Task>> tasks;
    std::priority_queue<Task*, std::vector<Task*>, TaskPtrComparator> task_queue;
    std::map<int, std::set<int>> dependencies;
    std::set<int> completed_tasks;
    int next_id = 1;

    // Helper for cycle detection
    bool has_cycle(int start, int target, std::set<int>& visited) const {
        if (start == target) return true;
        if (visited.count(start)) return false;
        visited.insert(start);
        auto it = dependencies.find(start);
        if (it != dependencies.end()) {
            for (int dep : it->second) {
                if (has_cycle(dep, target, visited)) return true;
            }
        }
        return false;
    }

public:
    void add_task(const std::string& desc, int priority, std::optional<std::string> deadline = std::nullopt) {
        auto task = std::make_unique<Task>(next_id, desc, priority, deadline);
        task_queue.push(task.get());
        tasks[next_id] = std::move(task);
        next_id++;
    }

    bool add_dependency(int task_id, int dep_id) {
        if (tasks.find(task_id) == tasks.end() || tasks.find(dep_id) == tasks.end()) {
            std::cerr << "Invalid task IDs for dependency.\n";
            return false;
        }
        dependencies[task_id].insert(dep_id);
        std::set<int> visited;
        if (has_cycle(dep_id, task_id, visited)) {
            dependencies[task_id].erase(dep_id);
            std::cerr << "Dependency would create a cycle. Not added.\n";
            return false;
        }
        return true;
    }

    bool can_execute(int task_id) const {
        auto it = dependencies.find(task_id);
        if (it == dependencies.end()) return true;
        for (int dep_id : it->second) {
            if (completed_tasks.find(dep_id) == completed_tasks.end())
                return false;
        }
        return true;
    }

    void execute_next() {
        if (task_queue.empty()) {
            std::cout << "No tasks to execute.\n";
            return;
        }
        std::vector<Task*> deferred;
        bool executed = false;
        while (!task_queue.empty()) {
            Task* task = task_queue.top();
            task_queue.pop();
            if (can_execute(task->id)) {
                std::cout << "Executing: " << task->description << " (ID: " << task->id << ")\n";
                completed_tasks.insert(task->id);
                executed = true;
                break;
            } else {
                deferred.push_back(task);
            }
        }
        for (Task* t : deferred) {
            task_queue.push(t);
        }
        if (!executed) {
            std::cout << "No executable tasks (dependencies not met).\n";
        }
    }

    void list_tasks() const {
        std::vector<const Task*> sorted_tasks;
        for (const auto& pair : tasks) {
            sorted_tasks.push_back(pair.second.get());
        }
        std::sort(sorted_tasks.begin(), sorted_tasks.end(),
            [](const Task* a, const Task* b) { return a->priority > b->priority; });

        for (const Task* task : sorted_tasks) {
            std::cout << "ID: " << task->id << ", Description: " << task->description
                      << ", Priority: " << task->priority
                      << (task->deadline ? ", Deadline: " + *task->deadline : "")
                      << (completed_tasks.count(task->id) ? " [COMPLETED]" : "") << "\n";
        }
    }

    void save_to_file(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Failed to open file for writing.\n";
            return;
        }
        for (const auto& pair : tasks) {
            const auto& task = pair.second;
            file << task->id << "|" << task->description << "|" << task->priority
                 << "|" << (task->deadline ? *task->deadline : "none")
                 << "|" << (completed_tasks.count(task->id) ? "completed" : "pending") << "\n";
        }
        file << "#DEPENDENCIES\n";
        for (const auto& dep : dependencies) {
            for (int d : dep.second) {
                file << dep.first << "->" << d << "\n";
            }
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
