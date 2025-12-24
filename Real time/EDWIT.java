import java.util.PriorityQueue;
import java.util.Comparator;

class Task {
    int id;                
    long deadline;         
    long period;           
    long wcet;             
    long nextReleaseTime;  

    
    public Task(int id, long period, long wcet) {
        this.id = id;
        this.period = period;
        this.wcet = wcet;
        this.deadline = period; 
        this.nextReleaseTime = 0;
    }

    @Override
    public String toString() {
        return "Task ID: " + id + ", Deadline: " + deadline + ", WCET: " + wcet;
    }
}

class EDFScheduler {
    private PriorityQueue<Task> readyQueue;

    private long currentTime = 0;

    public EDFScheduler() {
        readyQueue = new PriorityQueue<>(Comparator.comparingLong(task -> task.deadline));
    }

    public void addTask(Task task) {
        if (currentTime >= task.nextReleaseTime) {
            task.deadline = currentTime + task.period;
            readyQueue.offer(task);
        }
    }

    public void runScheduler(long simulationDuration) {
        while (currentTime < simulationDuration) {
            if (!readyQueue.isEmpty()) {
                Task currentTask = readyQueue.poll();

                System.out.println("Time " + currentTime + ": Executing " + currentTask);
                currentTime += currentTask.wcet;

                if (currentTime > currentTask.deadline) {
                    System.out.println("Task " + currentTask.id + " missed its deadline.");
                }

                currentTask.nextReleaseTime = currentTask.deadline;
                currentTask.deadline += currentTask.period;
                addTask(currentTask);
            } else {
                System.out.println("Time " + currentTime + ": CPU idle.");
                long nextReleaseTime = findNextReleaseTime();
                currentTime = nextReleaseTime; 
            }
        }
    }

    private long findNextReleaseTime() {
        return readyQueue.stream()
                .mapToLong(task -> task.nextReleaseTime)
                .min()
                .orElse(currentTime + 1); 
    }
}

public class EDWIT {
    public static void main(String[] args) {
        EDFScheduler scheduler = new EDFScheduler();

        Task task1 = new Task(1, 10, 3); 
        Task task2 = new Task(2, 15, 5); 
        Task task3 = new Task(3, 20, 7); 

        scheduler.addTask(task1);
        scheduler.addTask(task2);
        scheduler.addTask(task3);

        scheduler.runScheduler(50);
    }
}
