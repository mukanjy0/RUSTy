fn main(){
    let a1 : [i32; 2] = [2, 4];
    let mut a2 : [i32; 2] = a1;
    println!("{}", a2[1]);
    a2[1]=100;
    println!("{}", a1[1]);
    println!("{}", a2[1]);
}