fn main(){
    let mut arr: [i32; 5] = [1, 2, 3, 4, 5];
    let a2: [i32; 5] = [100, 200, 300, 400, 500];
    for i in 0..5 {
        arr[i] = a2[4 - i];
    }
    for i in 0..5 {
        println!("{}", arr[i]);
    }
}