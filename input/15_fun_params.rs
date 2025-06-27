fn fun(a: i32, b: i32) -> i32 {
    return a + b;
}

fn main(){
    let x: i32 = 10;
    let y: i32 = 20;
    let result = fun(x, y);
    println!("The result is: {}", result);
}