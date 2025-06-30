fn main() {
    let mut x: i32 = 1;
    let mut y: i32 = 2;
    let z: i32 = 3;

    if x + y > z {
        println!("{}", x + y);
        x = x * 2;
    }
    if x - y < z {
        println!("{}", x - y);
        y = y + 5;
    }
    if y / x > 0 {
        println!("{}", y/x);
    }
}