fn main() {
    let x: bool;
    x = true;
    println!("{}", x);
    let y: bool = false;
    println!("{}", y);
    let mut z: bool;
    z = x && y;
    println!("{}", z);
    z = x || y;
    println!("{}", z);
    z = !x;
    println!("{}", z);
    z = !y;
    println!("{}", z);
    z = x == y;
    println!("{}", z);
    z = x != y;
    println!("{}", z);
}