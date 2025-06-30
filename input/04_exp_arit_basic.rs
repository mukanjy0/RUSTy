fn main() {
    let mut x: i32;
    x = 10*2;
    println!("{}", x);
    x = 20 + x;
    println!("{}", x);
    let y: i32 = 30 - x;
    println!("{}", y);
    let z: i32;
    z = x / y;
    println!("{}", z);
}