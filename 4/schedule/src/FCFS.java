import java.util.List;

public class FCFS implements Algorithm{
    private List<Task> queue;
    public FCFS(List<Task> q){
        queue = q;
    }
    @Override
    public void schedule(){
        System.out.println("FCFS");
        while(!queue.isEmpty()){
            Task tmp = pickNextTask();
            System.out.println(tmp.getName()+"\t"+tmp.getPriority()+"\t"+tmp.getBurst());
            queue.remove(0);
        }
    }
    @Override
    public Task pickNextTask(){
        return this.queue.get(0);
    }
}
