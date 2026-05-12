
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;

class Task {
private:
    int id;
    string title;
    bool done;
    vector<int> dependencies; // sorted ascending

public:
    Task(int id, const string& title);

    int getId() const;
    const string& getTitle() const;
    bool isDone() const;
    const vector<int>& getDependencies() const;

    void addDependency(int depId);
    void markDone();
    bool hasDependency(int depId) const;
};

Task::Task(int id, const string& title)
    : id(id), title(title), done(false) {}

int Task::getId() const { return id; }
const string& Task::getTitle() const { return title; }
bool Task::isDone() const { return done; }
const vector<int>& Task::getDependencies() const { return dependencies; }

void Task::addDependency(int depId) {
    dependencies.push_back(depId);
    sort(dependencies.begin(), dependencies.end());
}

void Task::markDone() { done = true; }

bool Task::hasDependency(int depId) const {
    return find(dependencies.begin(), dependencies.end(), depId) != dependencies.end();
}


class TodoList {
private:
    string name;
    vector<Task> tasks; // kept sorted by id

    Task* findTask(int id);
    const Task* findTask(int id) const;

    // Returns true if any task has depId in its dependency list
    bool hasDependents(int id) const;

public:
    TodoList(const string& name);

    void setName(const string& newName);

    // Commands
    void addTask(int id, const string& title);
    void addTaskWithDeps(int id, const string& title, const vector<int>& deps);
    void markDone(int id);
    void printStatus(int id) const;
    void printAll() const;
    void printPending() const;
    void printReady() const;
    void removeTask(int id);
};

Task* TodoList::findTask(int id) {
    for (Task& t : tasks)
        if (t.getId() == id) return &t;
    return nullptr;
}

const Task* TodoList::findTask(int id) const {
    for (const Task& t : tasks)
        if (t.getId() == id) return &t;
    return nullptr;
}

bool TodoList::hasDependents(int id) const {
    for (const Task& t : tasks)
        if (t.hasDependency(id)) return true;
    return false;
}

TodoList::TodoList(const string& name) : name(name) {}

void TodoList::setName(const string& newName) {
    name = newName;
    tasks.clear();
}

void TodoList::addTask(int id, const string& title) {
    if (findTask(id)) {
        cout << "Error: task " << id << " already exists.\n";
        return;
    }
    tasks.emplace_back(id, title);
    // Keep tasks sorted by id for deterministic output
    sort(tasks.begin(), tasks.end(),
              [](const Task& a, const Task& b){ return a.getId() < b.getId(); });
    cout << "Task " << id << " added.\n";
}

void TodoList::addTaskWithDeps(int id, const string& title,
                               const vector<int>& deps) {
    if (findTask(id)) {
        cout << "Error: task " << id << " already exists.\n";
        return;
    }
    // Validate all deps first
    for (int dep : deps) {
        if (!findTask(dep)) {
            cout << "Error: dependency " << dep << " not found.\n";
            return;
        }
    }
    tasks.emplace_back(id, title);
    Task* t = findTask(id); // now it exists
    for (int dep : deps)
        t->addDependency(dep);

    sort(tasks.begin(), tasks.end(),
              [](const Task& a, const Task& b){ return a.getId() < b.getId(); });

    cout << "Task " << id << " added with " << deps.size() << " deps.\n";
}

void TodoList::markDone(int id) {
    Task* t = findTask(id);
    if (!t) {
        cout << "Error: task " << id << " not found.\n";
        return;
    }
    // Check all prerequisites are done
    for (int depId : t->getDependencies()) {
        const Task* dep = findTask(depId);
        if (!dep || !dep->isDone()) {
            cout << "Cannot complete task " << id
                      << ": prerequisites not met.\n";
            return;
        }
    }
    t->markDone();
    cout << "Task " << id << " marked as done.\n";
}

void TodoList::printStatus(int id) const {
    const Task* t = findTask(id);
    if (!t) {
        cout << "Error: task " << id << " not found.\n";
        return;
    }
    cout << "Task " << t->getId() << ": " << t->getTitle() << "\n";
    cout << "Status: " << (t->isDone() ? "done" : "pending") << "\n";

    const vector<int>& deps = t->getDependencies();
    if (!deps.empty()) {
        cout << "Dependencies:";
        for (int i = 0; i < (int)deps.size(); ++i) {
            const Task* d = findTask(deps[i]);
            cout << " " << deps[i]
                      << " (" << (d ? d->getTitle() : "?") << ")";
            if (i + 1 < (int)deps.size()) cout << ",";
        }
        cout << "\n";
    }
}

void TodoList::printAll() const {
    // tasks already sorted by id
    for (const Task& t : tasks) {
        if (t.isDone())
            cout << "[done]    " << t.getId() << ": " << t.getTitle() << "\n";
        else
            cout << "[pending] " << t.getId() << ": " << t.getTitle() << "\n";
    }
}

void TodoList::printPending() const {
    vector<const Task*> pending;
    for (const Task& t : tasks)
        if (!t.isDone()) pending.push_back(&t);

    if (pending.empty()) {
        // Per PDF spec: empty line (no output)
        return;
    }
    cout << "Pending tasks:";
    for (int i = 0; i < (int)pending.size(); ++i) {
        cout << " " << pending[i]->getId()
                  << " (" << pending[i]->getTitle() << ")";
        if (i + 1 < (int)pending.size()) cout << ",";
    }
    cout << "\n";
}

void TodoList::printReady() const {
    vector<const Task*> ready;
    for (const Task& t : tasks) {
        if (t.isDone()) continue; // already done, not "ready to do"
        bool allDepsDone = true;
        for (int depId : t.getDependencies()) {
            const Task* dep = findTask(depId);
            if (!dep || !dep->isDone()) { allDepsDone = false; break; }
        }
        if (allDepsDone) ready.push_back(&t);
    }

    if (ready.empty()) {
        // Per PDF spec: empty line (no output)
        return;
    }
    cout << "Ready tasks:";
    for (const Task* t : ready)
        cout << " " << t->getId();
    cout << "\n";
}

void TodoList::removeTask(int id) {
    Task* t = findTask(id);
    if (!t) {
        cout << "Error: task " << id << " not found.\n";
        return;
    }
    if (hasDependents(id)) {
        cout << "Cannot remove task " << id
                  << ": other tasks depend on it.\n";
        return;
    }
    tasks.erase(remove_if(tasks.begin(), tasks.end(),
                               [id](const Task& t){ return t.getId() == id; }),
                tasks.end());
    cout << "Task " << id << " removed.\n";
}

int main() {
    unique_ptr<TodoList> list;
    string line;

    while (getline(cin, line)) {
        if (line.empty()) continue;

        istringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "NEW_LIST") {
            string name;
            ss >> name;
            if (!list) {
                list = make_unique<TodoList>(name);
            } else {
                list->setName(name);
            }
            cout << "List '" << name << "' created.\n";

        } else if (cmd == "ADD") {
            int id;
            string title;
            ss >> id >> title;
            list->addTask(id, title);

        } else if (cmd == "ADD_DEP") {
            int id;
            string title;
            ss >> id >> title;
            vector<int> deps;
            int dep;
            while (ss >> dep) deps.push_back(dep);
            list->addTaskWithDeps(id, title, deps);

        } else if (cmd == "DONE") {
            int id;
            ss >> id;
            list->markDone(id);

        } else if (cmd == "STATUS") {
            int id;
            ss >> id;
            list->printStatus(id);

        } else if (cmd == "PRINT_ALL") {
            list->printAll();

        } else if (cmd == "PENDING") {
            list->printPending();

        } else if (cmd == "READY") {
            list->printReady();

        } else if (cmd == "REMOVE") {
            int id;
            ss >> id;
            list->removeTask(id);

        } else if (cmd == "EXIT") {
            break;
        }
    }

    return 0;
}