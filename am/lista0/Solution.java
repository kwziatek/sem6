import java.util.*;
import java.io.PrintWriter;
import java.io.File;


public class Solution {

    public static Integer calculatePathDistance(Integer n, List<List<Double>> twoDemPoints, List<Integer> permutation) {
        int result = 0;
        for(int i = 1; i < n; i++) {
            double x1 = twoDemPoints.get(permutation.get(i - 1)).get(0);
            double y1 = twoDemPoints.get(permutation.get(i - 1)).get(1);
            double x2 = twoDemPoints.get(permutation.get(i)).get(0);
            double y2 = twoDemPoints.get(permutation.get(i)).get(1);
            result += Utils.calculateTwoPointsDistance(x1, y1, x2, y2);
        }
        return result;
    }

    // 1000 tries
    public static void solve(Integer n, List<List<Double>> twoDemPoints) {
        List<Integer> solution = findSolutionForKTries(n, twoDemPoints, 1000);
        try {
            PrintWriter writer = new PrintWriter(new File("solution.txt"));
            Double meanForTens = findMinimum(solution, 1000, 10);
            Double meanForFifties = findMinimum(solution, 1000, 50);
            Double meanForThousand = findMinimum(solution, 1000, 1000);
            writer.println(meanForTens);
            writer.println(meanForFifties);
            writer.println(meanForThousand);
            writer.close();
            System.out.println("mean for 10: " + meanForTens + ", mean for 50: " + meanForFifties + ", mean for 1000: " + meanForThousand);
            List<List<Integer>> MST = findMST(n, twoDemPoints);
            TSPSolution tspSolution = new TSPSolution();
            int[][] graph = createMatrix(n, twoDemPoints);
            tspSolution.findTSPSolution(MST, n, graph);
        } catch (Exception e) {
            System.out.println("unexpected writing to file exception " + e.getMessage());

        }
    }

    public static List<Integer> findSolutionForKTries(Integer n, List<List<Double>> twoDemPoints, Integer k) {
        List<Integer> result = new ArrayList<>();
        for (int i = 1; i <= k; i++) {
            List<Integer> permutation = Utils.generatePermutation(n);
            Integer singlePathDistance = calculatePathDistance(n, twoDemPoints, permutation);
            result.add(singlePathDistance);
        }
        return result;
    }

    public static Double findMinimum(List<Integer> pathDistances, int n, int divisor) {
        double result = 0;
        int it = 0;
        int minValue = Integer.MAX_VALUE;
        for(Integer i: pathDistances) {
            minValue = Math.min(minValue, i);
            it++;
            if(it == divisor) {
                it = 0;
                result += minValue;
                minValue = Integer.MAX_VALUE;
            }
        }
        return result/(n / divisor);
    }

    private static List<List<Integer>> findMST(Integer n, List<List<Double>> twoDemPoints) {
        int[][] graph = createMatrix(n, twoDemPoints);
        int[] parent = new int[n+1];
        int[] key = new int[n+1];
        Boolean[] mstSet = new Boolean[n+1];
        initialize(key, mstSet);

        key[1] = 0;
        parent[1] = -1;

        for(int i = 1; i < n; i++) {
            int u = minKey(key, mstSet);

            mstSet[u] = true;

            for (int j = 1; j <= n; j++) {

                if (i != j && mstSet[j] == false && graph[u][j] < key[j]) {
                    parent[j] = u;
                    key[j] = graph[u][j];
                }
            }

        }

        return printMST(n, parent, graph);

    }

    private static int[][] createMatrix(Integer n, List<List<Double>> twoDemPoints) {
        int[][] result = new int[n + 1][n + 1];
        for(int i = 1; i <= n; i++) {
            for(int j = i; j <= n; j++) {
                if(i == j) {
                    result[i][j] = -1;
                } else {
                    result[i][j] = Utils.calculateTwoPointsDistance(twoDemPoints.get(i).get(0), twoDemPoints.get(i).get(1), twoDemPoints.get(j).get(0), twoDemPoints.get(j).get(1));
                    result[j][i] = result[i][j];
                }
            }
        }
        return result;
    }

    private static void initialize(int[] table, Boolean[] boolTable) {
        Arrays.fill(table, Integer.MAX_VALUE);
        Arrays.fill(boolTable, false);
    }

    private static int minKey(int[] key, Boolean[] mstSet) {
        int min = Integer.MAX_VALUE, index = -1;
        for(int i = 1; i < key.length; i++) {
            if(!mstSet[i] && key[i] < min) {
                min = key[i];
                index = i;
            }
        }
        return index;
    }

    private static List<List<Integer>> printMST(int n, int[] parent, int[][]graph) {
        int length = 0;
        List<List<Integer>> result = new ArrayList<>();
        System.out.println("Edge \t Weight");
        for(int i = 2; i <= n; i++) {
            System.out.println(parent[i] + " - " + i + "  " + graph[parent[i]][i]);
            result.add(List.of(i, parent[i], graph[parent[i]][i]));
            length += graph[parent[i]][i];
        }
        System.out.println("length of mst: " + length);
        return result;
    }

}