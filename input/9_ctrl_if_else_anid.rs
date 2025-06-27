fn main(){
    let x:bool = true;
    let y:bool = false;
    // if and else statements with boolean expressions
    if x {
        if y {
            println!("{}", x||y);
        } else {
            println!("{}", x&&y);
        }
    } else {
        if y {
            println!("{}", x||y);
        } else {
            println!("{}", x&&y);
        }
    }
}