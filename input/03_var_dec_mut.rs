fn main() {
    let x: i32;
    // x = 10;
    let mut y: i32 = 10;
    x = 20;
    let mut z: i32 = x+y;
    y = 30;
    z += 5;
    z *= 4;
    z /= 2;
    z -= y;
    println!("{}",z);
}
