class Computer {
    public static void main(String[] args) {
        int[] arr = new int[]{-1, 0, 2, -3, 6};

        int sum = new Computer().filterSum(arr);
        System.out.println("Sum is = " + sum);
    }

    int filterSum(int[] arr) {
        int sum = 0;
        for(int i = 0; i < arr.length; i=i+1) {
            if(arr[i] > 0) {
                sum = sum + arr[i];
            }
        }
        return sum;
    }
}