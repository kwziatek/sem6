import java.util.List;
import java.util.Random;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.Arrays;
import java.io.File;

public class Main{
    public static void main(String[] args) {
            for(String arg: args) {
                try {
                    List<List<Double>> twoDemPoints = new ArrayList<>();
                    twoDemPoints.add(new ArrayList<>()); // needed to keep 1-st point at 1-st index
                    Scanner scanner = new Scanner(new File(arg));
                    int n = scanner.nextInt();
                    for (int i = 1; i <= n; i++) {
                        scanner.next();
                        double x = scanner.nextDouble();
                        double y = scanner.nextDouble();
                        List<Double> toAdd = Arrays.asList(x, y);
                        twoDemPoints.add(toAdd);
                    }
                    Solution.solve(n, twoDemPoints);
                } catch (Exception e) {
                    System.out.println("Problem reading from a file - most likley bad file name, should be input/'nameOfRegion'");
                }
            }
    }
}