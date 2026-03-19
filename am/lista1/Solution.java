import java.util.List;
import java.util.ArrayList;
import java.io.PrintWriter;
import java.io.File;


public class Solution {

    public static Integer calculatePathDistance(Integer n, List<List<Double>> twoDemPoints, List<Integer> permutation) {
        Integer result = 0;
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
            Double meanForTens = findMinimum(solution, 1000, 10, writer);
            Double meanForFifties = findMinimum(solution, 1000, 50, writer);
            Double meanForThousand = findMinimum(solution, 1000, 1000, writer);
            writer.println(meanForTens);
            writer.println(meanForFifties);
            writer.println(meanForThousand);
            writer.close();
            System.out.println("mean for 10: " + meanForTens + ", mean for 50: " + meanForFifties + ", mean for 1000: " + meanForThousand);
        } catch (Exception e) {
            System.out.println("unexpected writing to file exception");
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

    public static Double findMinimum(List<Integer> pathDistances, int n, int divisor, PrintWriter printWriter) {
        double result = 0;
        int it = 0;
        Integer minValue = Integer.MAX_VALUE;
        for(Integer i: pathDistances) {
            minValue = Math.min(minValue, i);
            it++;
            if(it == divisor) {
                it = 0;
                result += minValue;
                // printWriter.println(minValue);
                minValue = Integer.MAX_VALUE;
            }
        }
        return result/(n / divisor);
    }
}