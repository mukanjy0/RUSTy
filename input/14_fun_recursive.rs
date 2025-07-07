fn fib(n: i32) -> i32 {
    if n < 2 {
        return 1;
    }
    return fib(n - 1) + fib(n - 2);
}

fn main(){
    let n: i32 = 5;
    let result = fib(n);
    println!("The fibonacci of {} is: {}", n, result);
}