
public class ComparableBig implements Comparable<ComparableBig> {
    private int id;
    private String name;
    private int time;

    public ComparableBig(int id, String name, int time) {
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
        if (!(o instanceof ComparableBig)) {
            return false;
        }
        ComparableBig p = (ComparableBig) o;
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
    public int compareTo(ComparableBig cp) {

        int cpId = cp.getId();
        String cpName = cp.getName();

        if (this.getId() > cpId) {
            return -1;
        }

        if (this.getId() < cpId) {
            return 1;
        }

        if (this.getId() == cpId) {
            return this.getName().compareTo(cpName);
        }

        // Should not reach here
        return 0;
    }
}