import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;

public class TSPSolution {

    private Boolean[] visited;
    private List<HashMap<Integer, Integer>> adjacencyList;
    private int prev;
    private int[][] graph;

    public void findTSPSolution(List<List<Integer>> edges, int n, int[][] graph) {
        initialize(n, graph);
        for(List<Integer> edge: edges) {
            // System.out.println(edge.getFirst() + " " + edge.get(1) + " " + edge.get(2));
            adjacencyList.get(edge.getFirst()).put(edge.get(1), edge.get(2));
            adjacencyList.get(edge.get(1)).put(edge.getFirst(), edge.get(2));
        }

        System.out.println(dfs(1, 0));

    }

    private void initialize(int n, int[][] graph) {
        this.adjacencyList = new ArrayList<>();
        this.visited = new Boolean[n + 1];
        this.prev = -1;
        this.graph = graph;
        for(int i = 0; i <= n; i++) {
            adjacencyList.add(new HashMap<>());
            visited[i] = false;
        }
    }

    private int dfs(int v, int sum) {
        if(!visited[v]) {
            visited[v] = true;
            prev = v;
            System.out.println(v);
            Map<Integer, Integer> vEdges = adjacencyList.get(v);
            for(Integer key: vEdges.keySet()) {
                // System.out.println(v + " " + key + " " + vEdges.get(key));
                if(!visited[key]) {
                    sum += graph[prev][key];
                    sum += dfs(key, 0);
                }
            }
        }
        // if(v == 1) {
        //     System.out.println(sum);
        // }
        return sum;

    }
}