fn fib(n: i32) -> i32 {
    if n < 2 {
        println!("The fibonacci of {} is: {}", n, 1);
        return 1;
    }
    println!("calling fibonacci of {}", n);
    let res = fib(n - 1) + fib(n - 2);
    println!("The fibonacci of {} is: {}", n, res);
    return res;
}

fn main(){
    let n: i32 = 3;
    let result = fib(n);
    println!("The fibonacci of {} is: {}", n, result);
}