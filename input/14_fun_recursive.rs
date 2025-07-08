fn fib(n: i32) -> i32 {
    let mut res : i32;
    if n < 2 {
        res = 1;
    }
    else {
        res = fib(n-1) + fib(n-2);
    }
    println!("The fibonacci of {} is: {}", n, res);
    return res;
}

fn main(){
    let n: i32 = 5;
    let result = fib(n);
    println!("The fibonacci of {} is: {}", n, result);
}