import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class Main {
    public static void main(String[] args) {
        Main driver = new Main();
        List<String> input = driver.getFileLines("input.txt");
        List<Integer> leftList = new ArrayList<>();
        List<Integer> rightList = new ArrayList<>();

        for(String line : input){
            String[] tokens = line.split("\\s+");
            leftList.add(Integer.valueOf(tokens[0]));
            rightList.add(Integer.valueOf(tokens[1]));
        }

        int distance = 0;

        Collections.sort(leftList);
        Collections.sort(rightList);

        for(int i = 0; i < leftList.size(); ++i){
            distance += Math.abs(leftList.get(i) - rightList.get(i));
        }

        System.out.printf("Distance P1 = %d\n", distance);

        Map<Integer, Integer> rightListFreq = new HashMap<>();

        for(int num : rightList){
            if(rightListFreq.containsKey(num)){
                rightListFreq.put(num, rightListFreq.get(num) + 1);
            }
            else{
                rightListFreq.put(num, 1);
            }
        }

        int score = 0;
        for(int num : leftList){
            score += num * rightListFreq.getOrDefault(num, 0);
        }

        System.out.printf("P2 score: %d\n", score);
    }

    public List<String> getFileLines(String path){
        List<String> lines = new ArrayList<>();
        String line;

        try(BufferedReader bufferedReader = new BufferedReader(new FileReader(path))){
            while((line = bufferedReader.readLine()) != null){
                lines.add(line);
            }
        }
        catch(IOException e){
            System.out.println("Error reading file");
        }

        return lines;
    }
}