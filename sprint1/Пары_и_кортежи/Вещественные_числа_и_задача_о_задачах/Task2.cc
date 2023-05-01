#include <map>
#include <string>
#include <tuple>
#include <iostream>
#include <vector>

using namespace std::string_literals;

enum class TaskStatus {
	NEW,
	IN_PROGRESS,
	TESTING,
	DONE,
};

using TasksInfo = std::map<TaskStatus, int>;

class TeamTasks {
	public:
		const TasksInfo& GetPersonTasksInfo(const std::string& person) const {
			return person_to_tasksInfo_.at(person);
		}
	
		void AddNewTask(const std::string& person) {
			++person_to_tasksInfo_[person][TaskStatus::NEW];
		}
		
		std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(
							const std::string& person, int task_count) {
			TasksInfo updated, untouched;

			auto& task_info = person_to_tasksInfo_[person];
			for (int i = 0; i < static_cast<int>(TaskStatus::DONE); ++i) {
				TaskStatus status = static_cast<TaskStatus>(i);
				TaskStatus next_status = static_cast<TaskStatus>(i + 1);

				auto diff = std::min(task_count, task_info[status]);
				if (diff > 0) {
					updated[next_status] = diff;
					task_count -= diff;
				}
				if ((task_info[status] - diff > 0)) {
					untouched[status] = task_info[status] - diff;
				}
			}
			for (int i = 0; i < static_cast<int>(TaskStatus::DONE); ++i) {
				TaskStatus status = static_cast<TaskStatus>(i);
				int countUpdated = (updated.count(status) > 0) ? updated[status] : 0;
				int countUntouched = (untouched.count(status) > 0) ? untouched[status] : 0;
	   			task_info[status] = countUpdated + countUntouched;
 			}
			task_info[TaskStatus::DONE] += (updated.count(TaskStatus::DONE) > 0) ? updated.at(TaskStatus::DONE) : 0;
			return {updated, untouched};
		}
	
	private:
		std::map<std::string, TasksInfo> person_to_tasksInfo_;
};

void PrintTasksInfo(TasksInfo tasks_info) {
	std::cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
		", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
		", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
		", " << tasks_info[TaskStatus::DONE] << " tasks are done" << std::endl;
}

int main() {
	TeamTasks tasks;
	tasks.AddNewTask("Ilia");
	for (int i = 0; i < 3; ++i) {
		tasks.AddNewTask("Ivan");
	}
	std::cout << "Ilia's tasks: ";
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
	std::cout << "Ivan's tasks: ";
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));
  
	TasksInfo updated_tasks, untouched_tasks;
  
	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
	std::cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);
  
	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
	std::cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	std::cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);
}