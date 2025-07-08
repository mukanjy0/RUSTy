fn main() {
    let mut a: i64 = 10;
    let b: i64 = 5;
    let c = a + b; // mixing i64 and i32
    println!("{}", c);
    let d = b + 3; // constant + variable
    println!("{}", d);
}