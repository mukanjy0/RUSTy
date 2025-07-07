fn main() {
    let x: i32;
    // x = 10;
    let mut y: i32 = 10;
    x = 30;
    let mut z: i32 = x-y;
    y = 30;
    z += 5;
//     z *= y;
//     z /= 2;
    println!("{}",y);
//     z -= y;
    println!("{}",z);
}
