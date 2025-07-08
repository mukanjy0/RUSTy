fn main(){
    let mut arr: [i32; 5] = [1, 2, 3, 4, 5];
    arr[3] = 100;
//     let f3 = arr[0] + arr[1] + arr[2];
//     println!("{}", f3);
    for i in 0..5 {
        println!("{}", arr[i]);
    }
}