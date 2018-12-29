import java.util.List;
import java.util.PriorityQueue;
import java.util.Queue;


class ComparableSmall implements Comparable<ComparableSmall> {
    private int id;
    private String name;
    private int time;

    public ComparableSmall(int id, String name, int time) {
        this.id = id;
        this.name = name;
        this.time = time;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getTime() {
        return time;
    }

    public void setTime(int time) {
        this.id = time;
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof ComparableSmall)) {
            return false;
        }
        ComparableSmall p = (ComparableSmall) o;
        if (this.id == p.getId()) {
            return true;
        }

        return false;
    }

    @Override
    public int hashCode() {
        return this.id;
    }

    @Override
    public String toString() {
        return name + '\t' + id + '\t' + time;
    }

    @Override
    public int compareTo(ComparableSmall cp) {

        int cpTime = cp.getTime();
        String cpName = cp.getName();

        if (this.getTime() < cpTime) {
            return -1;
        }

        if (this.getTime() > cpTime) {
            return 1;
        }

        if (this.getTime() == cpTime) {
            return this.getName().compareTo(cpName);
        }

        // Should not reach here
        return 0;
    }
}


public class SJF implements Algorithm{
    private List<Task> queue;
    public SJF(List<Task> q){
        queue = q;
    }
    @Override
    public void schedule(){
        Queue<ComparableSmall> sjfQueue = new PriorityQueue<>();
        while(!queue.isEmpty()){
            Task tmp = pickNextTask();
            sjfQueue.add(new ComparableSmall(tmp.getPriority(), tmp.getName(),tmp.getBurst()));
            queue.remove(0);
        }
        System.out.println("Shortest-job-first");
        while (sjfQueue.peek() != null) {
            ComparableSmall tmp = sjfQueue.peek();
            System.out.println(tmp.getName()+"\t"+tmp.getId()+"\t"+tmp.getTime());
            sjfQueue.remove();
        }
    }
    @Override
    public Task pickNextTask(){
        return this.queue.get(0);
    }
}
