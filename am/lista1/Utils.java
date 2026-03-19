import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import java.util.HashSet;
import java.util.Random;
import java.awt.geom.Point2D;
import java.math.BigDecimal;
import java.math.RoundingMode;

public class Utils {

    public static List<Integer> generatePermutation(int n) {
        List<Integer> result = new ArrayList<>();
        int k = n;
        Set<Integer> toBeUsed = fillSetWithValues(n);
        Random random = new Random();
        while(k > 0) {
            int gen = random.nextInt(k);
            int it = gen;
            for(int i: toBeUsed) {
                if(it == 0) {
                    toBeUsed.remove(i);
                    result.add(i);
                    k--;
                    break;
                } else {
                    it--;
                }
            }

        }
        return result;
    }

    private static Set<Integer> fillSetWithValues(int n) {
        Set<Integer> result = new HashSet<>();
        for(int i = 1; i <= n; i++) {
            result.add(i);
        }
        return result;
    }

    public static Integer calculateTwoPointsDistance(double x1, double y1, double x2, double y2) {
        double distance = Point2D.distance(x1, y1, x2, y2);
        return BigDecimal.valueOf(distance).setScale(0, RoundingMode.HALF_EVEN).intValue();
    }
}